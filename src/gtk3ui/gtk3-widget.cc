/*
 * gtk3-widget.cc - Copyright (c) 2001-2024 - Olivier Poncet
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <climits>
#include <cassert>
#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include <stdexcept>
#include "gtk3-widget.h"

// ---------------------------------------------------------------------------
// gtk3::WidgetTraits
// ---------------------------------------------------------------------------

namespace gtk3 {

struct WidgetTraits
    : BasicTraits
{
    static GtkWidget* create_widget()
    {
        return nullptr;
    }

    static void destroy(GtkWidget*& instance)
    {
        if(instance != nullptr) {
            instance = (::gtk_widget_destroy(instance), nullptr);
        }
    }

    static void show_all(Widget& widget)
    {
        if(widget) {
            return ::gtk_widget_show_all(widget);
        }
    }

    static void show(Widget& widget)
    {
        if(widget) {
            return ::gtk_widget_show(widget);
        }
    }

    static void hide(Widget& widget)
    {
        if(widget) {
            return ::gtk_widget_hide(widget);
        }
    }

    static void grab_focus(Widget& widget)
    {
        if(widget) {
            return ::gtk_widget_grab_focus(widget);
        }
    }

    static void set_sensitive(Widget& widget, bool sensitive)
    {
        if(widget) {
            return ::gtk_widget_set_sensitive(widget, sensitive);
        }
    }

    static bool is_sensitive(Widget& widget)
    {
        if(widget) {
            return ::gtk_widget_is_sensitive(widget) != FALSE;
        }
        return false;
    }

    static void drag_dest_set(Widget& widget, GtkDestDefaults flags, const GtkTargetEntry* targets, int num_targets, GdkDragAction actions)
    {
        if(widget) {
            return ::gtk_drag_dest_set(widget, flags, targets, num_targets, actions);
        }
    }
};

}

// ---------------------------------------------------------------------------
// <anonymous>::traits
// ---------------------------------------------------------------------------

namespace {

using traits = gtk3::WidgetTraits;

}

// ---------------------------------------------------------------------------
// gtk3::Widget
// ---------------------------------------------------------------------------

namespace gtk3 {

Widget::Widget()
    : Widget(traits::create_widget())
{
}

Widget::Widget(GtkWidget* instance)
    : _instance(instance)
{
    traits::register_widget_instance(_instance);
}

Widget::~Widget()
{
    traits::destroy(_instance);
}

void Widget::destroy()
{
    return traits::destroy(_instance);
}

void Widget::show_all()
{
    return traits::show_all(*this);
}

void Widget::show()
{
    return traits::show(*this);
}

void Widget::hide()
{
    return traits::hide(*this);
}

void Widget::grab_focus()
{
    return traits::grab_focus(*this);
}

void Widget::set_sensitive(bool sensitive)
{
    return traits::set_sensitive(*this, sensitive);
}

bool Widget::is_sensitive()
{
    return traits::is_sensitive(*this);
}

void Widget::drag_dest_set(GtkDestDefaults flags, const GtkTargetEntry* targets, int num_targets, GdkDragAction actions)
{
    return traits::drag_dest_set(*this, flags, targets, num_targets, actions);
}

void Widget::signal_connect(const char* signal, GCallback callback, void* data)
{
    return traits::signal_connect(_instance, signal, callback, data);
}

void Widget::add_drag_data_received_callback(GCallback callback, void* data)
{
    return signal_connect(sig_drag_data_received, callback, data);
}

}

// ---------------------------------------------------------------------------
// End-Of-File
// ---------------------------------------------------------------------------
