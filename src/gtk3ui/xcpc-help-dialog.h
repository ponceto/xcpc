/*
 * xcpc-help-dialog.h - Copyright (c) 2001-2025 - Olivier Poncet
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
#ifndef __XCPC_GTK3UI_HELP_DIALOG_H__
#define __XCPC_GTK3UI_HELP_DIALOG_H__

#include "xcpc-application.h"

// ---------------------------------------------------------------------------
// xcpc::HelpDialog
// ---------------------------------------------------------------------------

namespace xcpc {

class HelpDialog final
    : public base::HelpDialog
{
public: // public interface
    HelpDialog(Application&);

    virtual ~HelpDialog() = default;

    virtual void run() override;
};

}

// ---------------------------------------------------------------------------
// End-Of-File
// ---------------------------------------------------------------------------

#endif /* __XCPC_GTK3UI_HELP_DIALOG_H__ */
