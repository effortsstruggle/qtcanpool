/**
 * Copyright (C) 2023 maminjie <canpool@163.com>
 * SPDX-License-Identifier: MulanPSL-2.0
 **/
#include "tinytabwidget.h"
#include "tinytabbar.h"

#include <QStackedWidget>
#include <QToolButton>
#include <QBoxLayout>
#include <QIcon>

QCANPOOL_BEGIN_NAMESPACE

class TinyTabWidgetPrivate
{
    QCP_DECLARE_PUBLIC(TinyTabWidget)
public:
    TinyTabWidgetPrivate();

    void init();
    void updateTabBarPosition();
public:
    TinyTabBar *m_tabBar;
    QStackedWidget *m_stack;
    QBoxLayout *m_layout;
    QBoxLayout *m_tabLayout;
    TinyTabWidget::TabPosition m_pos;
};

TinyTabWidgetPrivate::TinyTabWidgetPrivate()
    : m_pos(TinyTabWidget::North)
{
}

void TinyTabWidgetPrivate::init()
{
    Q_Q(TinyTabWidget);
    m_tabBar = new TinyTabBar(q);
    m_tabBar->setTogglable(false);
    m_tabBar->setObjectName(QLatin1String("qx_TinyTabWidget_tabbar"));

    m_stack = new QStackedWidget(q);
    m_stack->setObjectName(QLatin1String("qx_TinyTabWidget_stackedwidget"));
    m_stack->setLineWidth(0);

    m_tabLayout = new QBoxLayout(QBoxLayout::LeftToRight);
    m_tabLayout->setContentsMargins(0, 0, 0, 0);
    m_tabLayout->setSpacing(0);
    m_tabLayout->addWidget(m_tabBar);
    m_tabLayout->addStretch();

    m_layout = new QBoxLayout(QBoxLayout::TopToBottom);
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->setSpacing(0);
    m_layout->addLayout(m_tabLayout);
    m_layout->addWidget(m_stack);
    q->setLayout(m_layout);

    QObject::connect(m_tabBar, SIGNAL(currentChanged(int)), m_stack, SLOT(setCurrentIndex(int)));
    QObject::connect(m_stack, SIGNAL(currentChanged(int)), q, SIGNAL(currentChanged(int)));

    updateTabBarPosition();
}

void TinyTabWidgetPrivate::updateTabBarPosition()
{
    switch (m_pos) {
    case TinyTabWidget::North: {
        m_layout->setDirection(QBoxLayout::TopToBottom);
        m_tabLayout->setDirection(QBoxLayout::LeftToRight);
        m_tabBar->setOrientation(Qt::Horizontal);
        break;
    }
    case TinyTabWidget::South: {
        m_layout->setDirection(QBoxLayout::BottomToTop);
        m_tabLayout->setDirection(QBoxLayout::LeftToRight);
        m_tabBar->setOrientation(Qt::Horizontal);
        break;
    }
    case TinyTabWidget::West: {
        m_layout->setDirection(QBoxLayout::LeftToRight);
        m_tabLayout->setDirection(QBoxLayout::TopToBottom);
        m_tabBar->setOrientation(Qt::Vertical);
        break;
    }
    case TinyTabWidget::East: {
        m_layout->setDirection(QBoxLayout::RightToLeft);
        m_tabLayout->setDirection(QBoxLayout::TopToBottom);
        m_tabBar->setOrientation(Qt::Vertical);
        break;
    }
    }
}

TinyTabWidget::TinyTabWidget(QWidget *parent)
    : QWidget(parent)
{
    QCP_INIT_PRIVATE(TinyTabWidget)
    Q_D(TinyTabWidget);
    d->init();

    setAttribute(Qt::WA_StyledBackground, true);
}

TinyTabWidget::~TinyTabWidget()
{
    QCP_FINI_PRIVATE()
}

int TinyTabWidget::addTab(QWidget *widget, const QString &label)
{
    return insertTab(-1, widget, label);
}

int TinyTabWidget::addTab(QWidget *widget, const QIcon &icon, const QString &label)
{
    return insertTab(-1, widget, icon, label);
}

int TinyTabWidget::insertTab(int index, QWidget *widget, const QString &label)
{
    return insertTab(index, widget, QIcon(), label);
}

int TinyTabWidget::insertTab(int index, QWidget *widget, const QIcon &icon, const QString &label)
{
    Q_D(TinyTabWidget);
    if (!widget) {
        return -1;
    }
    index = d->m_stack->insertWidget(index, widget);
    d->m_tabBar->insertTab(index, icon, label);

    return index;
}

void TinyTabWidget::removeTab(int index)
{
    Q_D(TinyTabWidget);
    if (QWidget *w = d->m_stack->widget(index)) {
        d->m_stack->removeWidget(w);
        d->m_tabBar->removeTab(index);
    }
}

int TinyTabWidget::currentIndex() const
{
    Q_D(const TinyTabWidget);
    return d->m_stack->currentIndex();
}

QWidget *TinyTabWidget::currentWidget() const
{
    Q_D(const TinyTabWidget);
    return d->m_stack->currentWidget();
}

QWidget *TinyTabWidget::widget(int index) const
{
    Q_D(const TinyTabWidget);
    return d->m_stack->widget(index);
}

int TinyTabWidget::indexOf(QWidget *widget) const
{
    Q_D(const TinyTabWidget);
    return d->m_stack->indexOf(widget);
}

int TinyTabWidget::count() const
{
    Q_D(const TinyTabWidget);
    return d->m_tabBar->count();
}

void TinyTabWidget::setTabEnabled(int index, bool enable)
{
    Q_D(TinyTabWidget);
    d->m_tabBar->setTabEnabled(index, enable);
}

void TinyTabWidget::setTabVisible(int index, bool visible)
{
    Q_D(TinyTabWidget);
    d->m_tabBar->setTabVisible(index, visible);
}

TinyTabBar *TinyTabWidget::tabBar() const
{
    Q_D(const TinyTabWidget);
    return d->m_tabBar;
}

TinyTabWidget::TabPosition TinyTabWidget::tabPosition() const
{
    Q_D(const TinyTabWidget);
    return d->m_pos;
}

void TinyTabWidget::setTabPosition(TinyTabWidget::TabPosition pos)
{
    Q_D(TinyTabWidget);
    if (d->m_pos == pos) {
        return;
    }
    d->m_pos = pos;
    d->updateTabBarPosition();
}

QToolButton *TinyTabWidget::addButton(const QString &text)
{
    return addButton(QIcon(), text);
}

QToolButton *TinyTabWidget::addButton(const QIcon &icon, const QString &text)
{
    Q_D(TinyTabWidget);
    QToolButton *button = new QToolButton(this);
    button->setText(text);
    button->setToolTip(text);
    button->setIcon(icon);
    button->setIconSize(d->m_tabBar->iconSize());
    button->setAutoRaise(true);
    button->setFocusPolicy(Qt::NoFocus);
    button->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    d->m_tabLayout->addWidget(button);
    return button;
}

void TinyTabWidget::removeButton(QToolButton *button)
{
    if (button == Q_NULLPTR) {
        return;
    }
    Q_D(TinyTabWidget);
    d->m_tabLayout->removeWidget(button);
    delete button;
}

void TinyTabWidget::setCurrentIndex(int index)
{
    Q_D(TinyTabWidget);
    d->m_tabBar->setCurrentIndex(index);
}

void TinyTabWidget::setCurrentWidget(QWidget *widget)
{
    Q_D(TinyTabWidget);
    d->m_tabBar->setCurrentIndex(indexOf(widget));
}

QCANPOOL_END_NAMESPACE
