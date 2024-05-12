/**
 * Copyright (C) 2023 maminjie <canpool@163.com>
 * SPDX-License-Identifier: MulanPSL-2.0
**/
#pragma once

#include "qcanpool.h"
#include <QObject>
#include <QMap>

class QAction;
class QActionGroup;
class QToolButton;

QCANPOOL_BEGIN_NAMESPACE

class TinyTabBar;

class TinyTabBarPrivate : public QObject
{
    Q_OBJECT
public:
    QCP_DECLARE_PUBLIC(TinyTabBar)
public:
    TinyTabBarPrivate();
    virtual ~TinyTabBarPrivate();

    void init();
    bool validIndex(int index) const
    { return index >= 0 && index < m_tabs.count(); }

    int indexOf(QAction *action);

    void layoutActions();

private Q_SLOTS:
    void onTriggered(QAction *action);
    void onOrientationChanged(Qt::Orientation orientation);
public:
    QList<QAction *> m_tabs;
    QActionGroup *m_group;
    int m_currentIndex;
    bool m_togglable;
};

QCANPOOL_END_NAMESPACE
