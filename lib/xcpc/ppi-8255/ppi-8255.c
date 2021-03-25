/*
 * ppi-8255.c - Copyright (c) 2001-2021 - Olivier Poncet
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ppi-8255-priv.h"

static void xcpc_ppi_8255_trace(const char* function)
{
    xcpc_log_trace("XcpcPpi8255::%s()", function);
}

XcpcPpi8255* xcpc_ppi_8255_alloc(void)
{
    xcpc_ppi_8255_trace("alloc");

    return xcpc_new(XcpcPpi8255);
}

XcpcPpi8255* xcpc_ppi_8255_free(XcpcPpi8255* self)
{
    xcpc_ppi_8255_trace("free");

    return xcpc_delete(XcpcPpi8255, self);
}

XcpcPpi8255* xcpc_ppi_8255_construct(XcpcPpi8255* self)
{
    xcpc_ppi_8255_trace("construct");

    /* clear iface */ {
        (void) memset(&self->iface, 0, sizeof(XcpcPpi8255Iface));
    }
    /* clear state */ {
        (void) memset(&self->state, 0, sizeof(XcpcPpi8255State));
    }
    return xcpc_ppi_8255_reset(self);
}

XcpcPpi8255* xcpc_ppi_8255_destruct(XcpcPpi8255* self)
{
    xcpc_ppi_8255_trace("destruct");

    return self;
}

XcpcPpi8255* xcpc_ppi_8255_new(void)
{
    xcpc_ppi_8255_trace("new");

    return xcpc_ppi_8255_construct(xcpc_ppi_8255_alloc());
}

XcpcPpi8255* xcpc_ppi_8255_delete(XcpcPpi8255* self)
{
    xcpc_ppi_8255_trace("delete");

    return xcpc_ppi_8255_free(xcpc_ppi_8255_destruct(self));
}

XcpcPpi8255* xcpc_ppi_8255_reset(XcpcPpi8255* self)
{
    xcpc_ppi_8255_trace("reset");

    /* reset state */ {
        self->state.port_a = DEFAULT_VALUE_OF_PORT_A;
        self->state.port_b = DEFAULT_VALUE_OF_PORT_B;
        self->state.port_c = DEFAULT_VALUE_OF_PORT_C;
        self->state.ctrl_p = DEFAULT_VALUE_OF_CTRL_P;
    }
    return self;
}

XcpcPpi8255* xcpc_ppi_8255_clock(XcpcPpi8255* self)
{
    return self;
}
