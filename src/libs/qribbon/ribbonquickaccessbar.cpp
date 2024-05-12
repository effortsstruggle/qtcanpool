/**
 * Copyleft (C) 2023 maminjie <canpool@163.com>
 **/
#include "ribbonquickaccessbar.h"
#include "ribbon_def.h"
#include "ribbonbar.h"
#include "ribbonbar_p.h"
#include "ribboncustomizemanager.h"
#include "ribboncustomizemanager_p.h"
#include "ribbonstyle.h"
#include "stylehelpers.h"
#include <QApplication>
#include <QMenu>
#include <QPainter>
#include <QStyleOption>
#include <QToolButton>
#include <qevent.h>

QRIBBON_USE_NAMESPACE

QRIBBON_BEGIN_NAMESPACE

class RibbonQuickAccessButton : public QToolButton
{
public:
    RibbonQuickAccessButton(QWidget *parent = Q_NULL);
    virtual ~RibbonQuickAccessButton();
public:
    virtual QSize sizeHint() const;
protected:
    virtual void paintEvent(QPaintEvent *);
private:
    Q_DISABLE_COPY(RibbonQuickAccessButton)
};

class QuickAccessAction : public QAction
{
public:
    QuickAccessAction(QObject *p, QAction *srcAction) : QAction(srcAction->text(), p), m_srcAction(srcAction)
    {
        setCheckable(true);
        QToolBar *toolBar = qobject_cast<QToolBar *>(parent());
        Q_ASSERT(toolBar);
        setChecked(toolBar->widgetForAction(srcAction));
    }
    void update()
    {
        QToolBar *toolBar = qobject_cast<QToolBar *>(parent());
        Q_ASSERT(toolBar);
        setChecked(toolBar->widgetForAction(m_srcAction));
        setText(m_srcAction->text());
    }
public:
    QAction *m_srcAction;
private:
    Q_DISABLE_COPY(QuickAccessAction)
};

class QuickAccessActionInvisible : public QAction
{
public:
    QuickAccessActionInvisible(QObject *p, QActionGroup *data) : QAction(p)
    {
        m_data = data;
        setProperty(__qrn_Action_Invisible, QLatin1String("__qrn_Action_Invisible"));
        setVisible(false);
    }
public:
    QActionGroup *m_data;
private:
    Q_DISABLE_COPY(QuickAccessActionInvisible)
};

class RibbonQuickAccessBarPrivate : public QObject
{
public:
    QRN_DECLARE_PUBLIC(RibbonQuickAccessBar)
public:
    explicit RibbonQuickAccessBarPrivate();
    virtual ~RibbonQuickAccessBarPrivate();
public:
    void init();
    QuickAccessAction *findQuickAccessAction(QAction *action) const;
    QAction *findBeforeAction(QAction *action) const;
    void updateAction(QAction *action);
    void updateParentLayout() const;
    void setActionVisible(QAction *action, bool visible);
public:
    RibbonQuickAccessButton *m_accessPopup;
    QMenu *m_menu;
    QAction *m_actionAccessPopup;
    QActionGroup *m_customizeGroupAction;
    QList<QAction *> m_actionList;
    bool m_removeAction : 1;
    bool m_customizeAction : 1;
private:
    Q_DISABLE_COPY(RibbonQuickAccessBarPrivate)
};

QRIBBON_END_NAMESPACE

/* RibbonQuickAccessButton */
RibbonQuickAccessButton::RibbonQuickAccessButton(QWidget *parent) : QToolButton(parent)
{
    setObjectName("RibbonQuickAccessButton");
}

RibbonQuickAccessButton::~RibbonQuickAccessButton() {}

QSize RibbonQuickAccessButton::sizeHint() const
{
    QSize sz = QToolButton::sizeHint();
    if (sz.isNull())
        sz = QSize(16, 16);
    return QSize(sz.height() * 13 / sz.width() + 1, sz.height());
}

void RibbonQuickAccessButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter p(this);
    QStyleOptionToolButton opt;
    initStyleOption(&opt);
    style()->drawPrimitive((QStyle::PrimitiveElement)RibbonStyle::PE_RibbonQuickAccessButton, &opt, &p, this);
}

/* RibbonQuickAccessBarPrivate */
RibbonQuickAccessBarPrivate::RibbonQuickAccessBarPrivate()
{
    m_accessPopup = Q_NULL;
    m_menu = Q_NULL;
    m_actionAccessPopup = Q_NULL;
    m_customizeGroupAction = Q_NULL;
    m_removeAction = false;
    m_customizeAction = false;
}

RibbonQuickAccessBarPrivate::~RibbonQuickAccessBarPrivate() {}

void RibbonQuickAccessBarPrivate::init()
{
    Q_Q(RibbonQuickAccessBar);
    q->setIconSize(QSize(16, 16));
    m_accessPopup = new RibbonQuickAccessButton(q);
    m_accessPopup->setPopupMode(QToolButton::InstantPopup);
    m_menu = new QMenu(q);
    m_accessPopup->setMenu(m_menu);

    m_actionAccessPopup = q->addWidget(m_accessPopup);
    m_actionAccessPopup->setProperty(__qrn_Quick_Access_Button, QLatin1String("__qrn_Quick_Access_Button"));

    m_customizeGroupAction = new QActionGroup(q);
    m_customizeGroupAction->setExclusive(false);
    QObject::connect(m_customizeGroupAction, SIGNAL(triggered(QAction *)), q, SLOT(customizeAction(QAction *)));

    QObject::connect(m_menu, SIGNAL(aboutToShow()), q, SLOT(aboutToShowCustomizeMenu()));
    QObject::connect(m_menu, SIGNAL(aboutToHide()), q, SLOT(aboutToHideCustomizeMenu()));

    q->addAction(new QuickAccessActionInvisible(q, m_customizeGroupAction));
}

QuickAccessAction *RibbonQuickAccessBarPrivate::findQuickAccessAction(QAction *action) const
{
    if (m_customizeGroupAction == Q_NULL)
        return Q_NULL;

    QList<QAction *> list = m_customizeGroupAction->actions();
    for (int i = 0; i < list.count(); ++i) {
        QuickAccessAction *act = dynamic_cast<QuickAccessAction *>(list[i]);
        if (act && action == act->m_srcAction)
            return act;
    }
    return Q_NULL;
}

QAction *RibbonQuickAccessBarPrivate::findBeforeAction(QAction *action) const
{
    QList<QAction *> list = m_customizeGroupAction->actions();
    bool find = false;
    for (int i = 0, count = list.count(); i < count; ++i) {
        if (find) {
            if (QuickAccessAction *beforeAct = dynamic_cast<QuickAccessAction *>(list[i])) {
                if (beforeAct->isChecked())
                    return beforeAct->m_srcAction;
            }
        }
        if (!find && action == list[i])
            find = true;
    }
    return Q_NULL;
}

void RibbonQuickAccessBarPrivate::updateAction(QAction *action)
{
    if (QuickAccessAction *wrapper = findQuickAccessAction(action))
        wrapper->update();
}

void RibbonQuickAccessBarPrivate::updateParentLayout() const
{
    Q_Q(const RibbonQuickAccessBar);
    if (!q->isVisible())
        return;

    if (RibbonBar *ribbonBar = qobject_cast<RibbonBar *>(q->parentWidget())) {
        if (ribbonBar->quickAccessBarPosition() == RibbonBar::QATopPosition)
            QApplication::postEvent(ribbonBar, new QResizeEvent(ribbonBar->size(), QSize(0, 0)));
    }
}

void RibbonQuickAccessBarPrivate::setActionVisible(QAction *action, bool visible)
{
    Q_Q(RibbonQuickAccessBar);
    if (QuickAccessAction *wrapper = findQuickAccessAction(action)) {
        if (visible) {
            if (m_customizeAction) {
                QAction *beforeAct = findBeforeAction(wrapper);
                q->insertAction(beforeAct, action);
            } else
                q->addAction(action);
        } else {
            q->removeAction(action);
            m_removeAction = false;
        }
        wrapper->update();
        q->adjustSize();
    }
}

/* RibbonQuickAccessBar */
RibbonQuickAccessBar::RibbonQuickAccessBar(QWidget *parent) : QToolBar(parent)
{
    QRN_INIT_PRIVATE(RibbonQuickAccessBar);
    Q_D(RibbonQuickAccessBar);
    d->init();
}

RibbonQuickAccessBar::~RibbonQuickAccessBar() { QRN_FINI_PRIVATE(); }

/*!
\brief Returns a pointer to the QAction that is associated with the customize button of Ribbon's Quick Access Bar.
*/
QAction *RibbonQuickAccessBar::actionCustomizeButton() const
{
    Q_D(const RibbonQuickAccessBar);
    return d->m_actionAccessPopup;
}

/*!
\brief Sets the visibility of the visual representation of action on the quick access toolbar. Parameter \a action is
action, parameter \a visible is a visibility.
*/
void RibbonQuickAccessBar::setActionVisible(QAction *action, bool visible)
{
    Q_D(RibbonQuickAccessBar);
    d->setActionVisible(action, visible);
}

/*!
\brief Returns the visibility of the visual representation for the given \a action.
*/
bool RibbonQuickAccessBar::isActionVisible(QAction *action) const
{
    Q_D(const RibbonQuickAccessBar);
    if (QuickAccessAction *wrapper = d->findQuickAccessAction(action))
        return wrapper->isChecked();
    return false;
}

/*!
\brief Returns the count of the elements visible on Ribbon's Quick Access Bar.
*/
int RibbonQuickAccessBar::visibleCount() const
{
    Q_D(const RibbonQuickAccessBar);
    int visibleCount = 0;
    QList<QAction *> list = d->m_customizeGroupAction->actions();
    for (int i = 0, count = list.count(); i < count; ++i) {
        if (QuickAccessAction *beforeAct = dynamic_cast<QuickAccessAction *>(list[i]))
            if (beforeAct->isChecked())
                visibleCount++;
    }
    return visibleCount;
}

/*!
\brief Returns the recommended size for Ribbon's Quick Access Bar.
\reimp
*/
QSize RibbonQuickAccessBar::sizeHint() const
{
    RibbonBar *ribbonBar = qobject_cast<RibbonBar *>(parentWidget());
    if (ribbonBar == Q_NULL)
        return QToolBar::sizeHint();

    int heightTabs = ribbonBar->d_func()->heightTabs();
    return QSize(QToolBar::sizeHint().width(), heightTabs + 1).expandedTo(QApplication::globalStrut());
}

/*! \internal */
void RibbonQuickAccessBar::customizeAction(QAction *action)
{
    Q_D(RibbonQuickAccessBar);
    d->m_customizeAction = true;
    if (QuickAccessAction *act = dynamic_cast<QuickAccessAction *>(action)) {
        setActionVisible(act->m_srcAction, !widgetForAction(act->m_srcAction));
        if (RibbonBar *ribbonBar = qobject_cast<RibbonBar *>(parentWidget())) {
            if (RibbonCustomizeManager *manager = ribbonBar->customizeManager()) {
                manager->setEditMode();
                manager->d_func()->m_ribbonManager->setQuickAccessBar();
                manager->setEditMode(false);
            }
        }
    }
    d->m_customizeAction = false;
}

/*! \internal */
void RibbonQuickAccessBar::aboutToShowCustomizeMenu()
{
    Q_D(RibbonQuickAccessBar);
    d->m_menu->clear();

    d->m_menu->setSeparatorsCollapsible(false);
    d->m_menu->addSeparator()->setText(RibbonBar::tr_compatible(RibbonCustomizeQuickAccessToolBarString));

    QListIterator<QAction *> itAction(d->m_actionList);
    while (itAction.hasNext()) {
        QAction *action = itAction.next();
        d->m_menu->addAction(action);
    }

    emit showCustomizeMenu(d->m_menu);
}

/*! \internal */
void RibbonQuickAccessBar::aboutToHideCustomizeMenu()
{
    Q_D(RibbonQuickAccessBar);
    d->m_menu->clear();
}

/*! \reimp */
bool RibbonQuickAccessBar::event(QEvent *event)
{
    if ((QEvent::Hide == event->type() || QEvent::Show == event->type()) && parentWidget()) {
        adjustSize();
        if (RibbonBar *ribbonBar = qobject_cast<RibbonBar *>(parentWidget()))
            ribbonBar->updateLayout();
    }
    return QToolBar::event(event);
}

/*! \reimp */
void RibbonQuickAccessBar::actionEvent(QActionEvent *event)
{
    Q_D(RibbonQuickAccessBar);

    QToolBar::actionEvent(event);

    if (d->m_actionAccessPopup) {
        if (event->type() == QEvent::ActionAdded) {
            if (!d->m_removeAction)
                removeAction(d->m_actionAccessPopup);

            QuickAccessAction *quickAccessAction = d->findQuickAccessAction(event->action());
            if (event->action() != d->m_actionAccessPopup && !quickAccessAction &&
                !dynamic_cast<QuickAccessActionInvisible *>(event->action())) {
                QuickAccessAction *act = new QuickAccessAction(this, event->action());

                bool addActionToMenu = true;

                if (RibbonBar *ribbonBar = qobject_cast<RibbonBar *>(parentWidget())) {
                    if (RibbonCustomizeManager *manager = ribbonBar->customizeManager()) {
                        if (manager->isEditMode()) {
                            QList<QAction *> defaultActions =
                                manager->d_func()->m_ribbonManager->defaultToolBars().value(this);
                            if (!defaultActions.contains(event->action()))
                                addActionToMenu = false;
                        }
                    }
                }

                if (addActionToMenu)
                    d->m_actionList.append(act);

                d->m_customizeGroupAction->addAction(act);
                adjustSize();
            } else if (quickAccessAction)
                quickAccessAction->update();
        } else if (event->type() == QEvent::ActionRemoved) {
            if (event->action() == d->m_actionAccessPopup) {
                d->m_removeAction = true;
                addAction(d->m_actionAccessPopup);
                d->m_removeAction = false;
            } else if (QuickAccessActionInvisible *actInvisible =
                           dynamic_cast<QuickAccessActionInvisible *>(event->action())) {
                d->m_actionList.clear();
                QList<QAction *> actList = actInvisible->m_data->actions();
                for (int i = actList.size() - 1; i >= 0; i--) {
                    QAction *actionWrapper = actList[i];
                    d->m_customizeGroupAction->removeAction(actionWrapper);
                    delete actionWrapper;
                }
                delete actInvisible;
                addAction(new QuickAccessActionInvisible(this, d->m_customizeGroupAction));
            } else
                d->updateAction(event->action());
        } else if (event->type() == QEvent::ActionChanged) {
            if (event->action() == d->m_actionAccessPopup)
                d->m_accessPopup->setDefaultAction(d->m_actionAccessPopup);
            else if (!dynamic_cast<QuickAccessAction *>(event->action()))
                d->updateAction(event->action());
        }
    }
}

/*! \reimp */
void RibbonQuickAccessBar::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    if (RibbonBar *ribbonBar = qobject_cast<RibbonBar *>(parentWidget())) {
        QPainter p(this);
        StyleRibbonQuickAccessBar opt;
        opt.init(this);
        opt.quickAccessBarPosition = ribbonBar->quickAccessBarPosition();
        style()->drawControl(QStyle::CE_ToolBar, &opt, &p, this);
    }
}

/*! \reimp */
void RibbonQuickAccessBar::resizeEvent(QResizeEvent *event)
{
    QToolBar::resizeEvent(event);
    Q_D(RibbonQuickAccessBar);
    d->updateParentLayout();
}
