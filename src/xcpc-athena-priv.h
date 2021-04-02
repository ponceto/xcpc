/*
 * xcpc-athena-priv.h - Copyright (c) 2001-2021 - Olivier Poncet
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
#ifndef __XCPC_ATHENA_PRIV_H__
#define __XCPC_ATHENA_PRIV_H__

#include <xcpc/machine/machine.h>
#include "xcpc-athena.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _
#define _(string) (string)
#endif

typedef struct _XcpcResourcesRec
{
    Boolean quiet_flag;
    Boolean trace_flag;
    Boolean debug_flag;
} XcpcResourcesRec, *XcpcResources;

typedef struct _XcpcApplicationRec
{
    Widget main_wnd;
    Widget menu_bar;
    Widget file_menu;
    Widget file_pldn;
    Widget load_snapshot;
    Widget save_snapshot;
    Widget separator1;
    Widget drivea_insert;
    Widget drivea_eject;
    Widget separator2;
    Widget driveb_insert;
    Widget driveb_eject;
    Widget separator3;
    Widget exit_emulator;
    Widget ctrl_menu;
    Widget ctrl_pldn;
    Widget pause_emu;
    Widget reset_emu;
    Widget help_menu;
    Widget help_pldn;
    Widget legal_info;
    Widget separator4;
    Widget about_xcpc;
    Widget emulator;
} XcpcApplicationRec;

#ifdef __cplusplus
}
#endif

#endif /* __XCPC_ATHENA_PRIV_H__ */
