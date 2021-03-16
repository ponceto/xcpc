/*
 * AppShell.h - Copyright (c) 2001-2021 - Olivier Poncet
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
#ifndef __XemAppShell_h__
#define __XemAppShell_h__

#include <X11/Shell.h>

#ifdef __cplusplus
extern "C" {
#endif

externalref WidgetClass xemAppShellWidgetClass;

typedef struct _XemAppShellClassRec* XemAppShellWidgetClass;
typedef struct _XemAppShellRec*      XemAppShellWidget;

#ifndef XemIsAppShell
#define XemIsAppShell(w) XtIsSubclass(w, xemAppShellWidgetClass)
#endif

extern Widget XemCreateAppShell(Widget parent, String name, ArgList args, Cardinal num_args);

#ifdef __cplusplus
}
#endif

#endif /* __XemAppShell_h__ */
