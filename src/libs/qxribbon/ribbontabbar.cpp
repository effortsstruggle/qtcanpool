﻿/**
 * Copyleft (C) 2023 maminjie <canpool@163.com>
 * SPDX-License-Identifier: MIT
**/
#include "ribbontabbar.h"

QX_RIBBON_BEGIN_NAMESPACE

RibbonTabBar::RibbonTabBar(QWidget *parent)
    : QTabBar(parent)
    , m_tabMargin(6, 0, 0, 0)
{
    setExpanding(false);
}

const QMargins &RibbonTabBar::tabMargin() const
{
    return m_tabMargin;
}

void RibbonTabBar::setTabMargin(const QMargins &margin)
{
    m_tabMargin = margin;
}

QX_RIBBON_END_NAMESPACE
