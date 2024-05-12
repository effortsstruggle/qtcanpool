/**
 * Copyright (C) 2020-2022 maminjie <canpool@163.com>
 * SPDX-License-Identifier: MulanPSL-2.0
**/
#include "ribboncontainers.h"

#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QAction>
#include <QToolButton>
#include <QToolBar>

QCANPOOL_BEGIN_NAMESPACE

/* RibbonContainer */
RibbonContainer::RibbonContainer(QWidget *parent)
    : QWidget(parent)
{
}

RibbonContainer::~RibbonContainer()
{
}

/* RibbonGridLayout */
class RibbonGridLayout : public QGridLayout
{
public:
    RibbonGridLayout(QWidget *parent = nullptr);
public:
    void addWidget(QWidget *widget);
};

RibbonGridLayout::RibbonGridLayout(QWidget *parent)
    : QGridLayout(parent)
{

}

void RibbonGridLayout::addWidget(QWidget *widget)
{
    int cnt = count();
    int column = cnt / 3;
    int row = column ? (cnt % 3) : cnt;
    QGridLayout::addWidget(widget, row, column, Qt::AlignLeft);
}

/* RibbonGridContainerPrivate */
class RibbonGridContainerPrivate
{
public:
    RibbonGridContainerPrivate();

    void init();

public:
    RibbonGridContainer *q;
    RibbonGridLayout *m_gridLayout;
};

RibbonGridContainerPrivate::RibbonGridContainerPrivate()
{}

void RibbonGridContainerPrivate::init()
{
    m_gridLayout = new RibbonGridLayout(q);
    m_gridLayout->setSpacing(1);
    m_gridLayout->setContentsMargins(2, 1, 2, 1);
    q->setLayout(m_gridLayout);
}

/* RibbonGridContainer */
RibbonGridContainer::RibbonGridContainer(QWidget *parent)
    : RibbonContainer(parent)
    , d(new RibbonGridContainerPrivate)
{
    d->q = this;
    d->init();
}

RibbonGridContainer::~RibbonGridContainer()
{
    delete d;
}

void RibbonGridContainer::addWidget(QWidget *widget)
{
    d->m_gridLayout->addWidget(widget);
}

/* RibbonActionContainerPrivate */
class RibbonActionContainerPrivate
{
public:
    RibbonActionContainerPrivate();

    void init();

    QAction *addAction(const QIcon &icon, const QString &text, RibbonGroup::GroupSize size);
    void addAction(QAction *action, RibbonGroup::GroupSize size); // for action that has menu

public:
    RibbonActionContainer *q;
    QHBoxLayout *m_largeLayout;
    RibbonGridLayout *m_mediumLayout;
    RibbonGridLayout *m_smallLayout;
};

RibbonActionContainerPrivate::RibbonActionContainerPrivate()
{}

void RibbonActionContainerPrivate::init()
{
    m_largeLayout = new QHBoxLayout();
    m_largeLayout->setSpacing(0);
    m_largeLayout->setContentsMargins(0, 0, 0, 0);

    m_mediumLayout = new RibbonGridLayout();
    m_mediumLayout->setSpacing(0);
    m_mediumLayout->setContentsMargins(0, 0, 0, 0);

    m_smallLayout = new RibbonGridLayout();
    m_smallLayout->setSpacing(0);
    m_smallLayout->setContentsMargins(0, 0, 0, 0);

    QHBoxLayout *mainLayout = new QHBoxLayout();
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(1, 0, 1, 0);
    mainLayout->addLayout(m_largeLayout);
    mainLayout->addLayout(m_mediumLayout);
    mainLayout->addLayout(m_smallLayout);
    q->setLayout(mainLayout);
}

QAction *RibbonActionContainerPrivate::addAction(const QIcon &icon, const QString &text,
                                                      RibbonGroup::GroupSize size)
{
    QAction *action = new QAction(icon, text, q);
    addAction(action, size);
    return action;
}

void RibbonActionContainerPrivate::addAction(QAction *action, RibbonGroup::GroupSize size)
{
    QToolButton *button = new QToolButton(q);
    button->setAutoRaise(true);
    button->setDefaultAction(action);

    switch (size) {
        case RibbonGroup::GroupLarge: {
            button->setIconSize(QSize(32, 32));
            button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
            m_largeLayout->addWidget(button);
            m_largeLayout->setAlignment(button, Qt::AlignLeft);
        }
        break;
        case RibbonGroup::GroupMedium: {
            button->setIconSize(QSize(18, 18));
            button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
            m_mediumLayout->addWidget(button);
            m_mediumLayout->setAlignment(button, Qt::AlignLeft);
        }
        break;
        default: {
            button->setIconSize(QSize(18, 18));
            button->setToolButtonStyle(Qt::ToolButtonIconOnly);
            m_smallLayout->addWidget(button);
            m_smallLayout->setAlignment(button, Qt::AlignLeft);
        }
        break;
    }
    button->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
}

/* RibbonActionContainer */
RibbonActionContainer::RibbonActionContainer(QWidget *parent)
    : RibbonContainer(parent)
    , d(new RibbonActionContainerPrivate)
{
    d->q = this;
    d->init();
}

RibbonActionContainer::~RibbonActionContainer()
{
    delete d;
}

QAction *RibbonActionContainer::addAction(const QIcon &icon, const QString &text,
                                               RibbonGroup::GroupSize size)
{
    return d->addAction(icon, text, size);
}

void RibbonActionContainer::addAction(QAction *action, RibbonGroup::GroupSize size)
{
    return d->addAction(action, size);
}

/* RibbonLoftContainerPrivate */
class RibbonLoftContainerPrivate
{
public:
    RibbonLoftContainerPrivate();

    void init();

public:
    RibbonLoftContainer *q;
    QToolBar *m_topBar;
    QToolBar *m_bottomBar;
};

RibbonLoftContainerPrivate::RibbonLoftContainerPrivate()
{

}

void RibbonLoftContainerPrivate::init()
{
    m_topBar = new QToolBar();
    m_topBar->setIconSize(QSize(20, 20));
    m_topBar->setToolButtonStyle(Qt::ToolButtonIconOnly);

    m_bottomBar = new QToolBar();
    m_bottomBar->setIconSize(QSize(20, 20));
    m_bottomBar->setToolButtonStyle(Qt::ToolButtonIconOnly);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->setSpacing(5);
    mainLayout->setContentsMargins(1, 0, 1, 0);
    mainLayout->addWidget(m_topBar);
    mainLayout->addWidget(m_bottomBar);
    mainLayout->addStretch();
    q->setLayout(mainLayout);
}

RibbonLoftContainer::RibbonLoftContainer(QWidget *parent)
    : RibbonContainer(parent)
    , d(new RibbonLoftContainerPrivate)
{
    d->q = this;
    d->init();
}

RibbonLoftContainer::~RibbonLoftContainer()
{
    delete d;
}

QToolBar *RibbonLoftContainer::toolBar(RibbonLoftContainer::Position pos)
{
    if (pos == RibbonLoftContainer::Top) {
        return d->m_topBar;
    } else {
        return d->m_bottomBar;
    }
}

QCANPOOL_END_NAMESPACE
