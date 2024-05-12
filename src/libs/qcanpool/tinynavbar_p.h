/**
 * Copyright (C) 2023 maminjie <canpool@163.com>
 * SPDX-License-Identifier: MulanPSL-2.0
**/
#pragma once

#include "qcanpool.h"
#include "tinytabbar_p.h"
#include "tinynavbar.h" // must, for Q_DECLARE_PUBLIC -> static_cast
#include <QToolButton>
#include <QMap>

class QMenu;

QCANPOOL_BEGIN_NAMESPACE

class MenuAccessButton;
class TinyNavBar;

class TinyNavBarPrivate : public TinyTabBarPrivate
{
    Q_OBJECT
public:
    Q_DECLARE_PUBLIC(TinyNavBar)
public:
    TinyNavBarPrivate();
public:
    void init();
private Q_SLOTS:
    void customizeAction(QAction *action);
    void aboutToShowCustomizeMenu();
    void aboutToHideCustomizeMenu();
public:
    QMenu *m_menu;
    QAction *m_actionAccessPopup;
    QActionGroup *m_customizeGroup;
    MenuAccessButton *m_accessPopup;
    QList<QAction *> m_actionList;
    QMap<QAction *, QAction *> m_actionMap; // lowAction,checkAction
    bool m_removingAction : 1;
};

QCANPOOL_END_NAMESPACE
