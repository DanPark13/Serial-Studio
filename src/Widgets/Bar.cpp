/*
 * Copyright (c) 2020-2021 Alex Spataru <https://github.com/alex-spataru>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "Bar.h"
#include "UI/Dashboard.h"
#include "Misc/ThemeManager.h"

#include <QResizeEvent>

namespace Widgets
{

/**
 * Constructor function, configures widget style & signal/slot connections.
 */
Bar::Bar(const int index)
    : m_index(index)
{
    // Get pointers to serial studio modules
    const auto dash = UI::Dashboard::getInstance();
    const auto theme = Misc::ThemeManager::getInstance();

    // Invalid index, abort initialization
    if (m_index < 0 || m_index >= dash->barCount())
        return;

    // Set thermo palette
    QPalette palette;
    palette.setColor(QPalette::Base, theme->base());
    palette.setColor(QPalette::Highlight, QColor("#f00"));
    palette.setColor(QPalette::Text, theme->widgetIndicator());
    palette.setColor(QPalette::Dark, theme->widgetIndicator());
    palette.setColor(QPalette::Light, theme->widgetIndicator());
    palette.setColor(QPalette::ButtonText, theme->widgetIndicator());
    palette.setColor(QPalette::WindowText, theme->widgetIndicator());
    m_thermo.setPalette(palette);

    // Get thermo color
    QString color;
    const auto colors = theme->widgetColors();
    if (colors.count() > m_index)
        color = colors.at(m_index);
    else
        color = colors.at(colors.count() % m_index);

    // Configure thermo style
    m_thermo.setPipeWidth(64);
    m_thermo.setBorderWidth(1);
    m_thermo.setFillBrush(QBrush(QColor(color)));

    // Lazy widgets, get initial properties from dataset
#ifdef LAZY_WIDGETS
    const auto dataset = UI::Dashboard::getInstance()->getBar(m_index);
    if (dataset)
    {
        m_thermo.setAlarmLevel(dataset->alarm());
        m_thermo.setAlarmEnabled(m_thermo.alarmLevel() > 0);
        m_thermo.setScale(dataset->min(), dataset->max());
    }
#endif

    // Set widget pointer & disable auto resize
    setWidget(&m_thermo, Qt::AlignHCenter, false);

    // React to dashboard events
    connect(dash, SIGNAL(updated()), this, SLOT(updateData()));
    connect(this, SIGNAL(resized()), this, SLOT(onResized()));
}

/**
 * Checks if the widget is enabled, if so, the widget shall be updated
 * to display the latest data frame.
 *
 * If the widget is disabled (e.g. the user hides it, or the external
 * window is hidden), then the widget shall ignore the update request.
 */
void Bar::updateData()
{
    // Widget not enabled, do nothing
    if (!isEnabled())
        return;

    // Update bar level
    const auto dataset = UI::Dashboard::getInstance()->getBar(m_index);
    if (dataset)
    {
#ifndef LAZY_WIDGETS
        m_thermo.setAlarmLevel(dataset->alarm());
        m_thermo.setAlarmEnabled(m_thermo.alarmLevel() > 0);
        m_thermo.setScale(dataset->min(), dataset->max());
#endif
        const auto value = dataset->value().toDouble();
        m_thermo.setValue(value);
        setValue(QString("%1 %2").arg(
            QString::number(value, 'f', UI::Dashboard::getInstance()->precision()),
            dataset->units()));
    }
}

/**
 * Resizes the thermo to fit the size of the parent window.
 */
void Bar::onResized()
{
    m_thermo.setPipeWidth(width() * 0.25);
}
}
