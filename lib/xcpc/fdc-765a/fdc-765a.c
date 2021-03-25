/*
 * fdc-765a.c - Copyright (c) 2001-2021 - Olivier Poncet
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
#include "fdc-765a-priv.h"

static void xcpc_fdc_765a_trace(const char* function)
{
    xcpc_log_trace("XcpcFdc765a::%s()", function);
}

XcpcFdc765a* xcpc_fdc_765a_alloc(void)
{
    xcpc_fdc_765a_trace("alloc");

    return xcpc_new(XcpcFdc765a);
}

XcpcFdc765a* xcpc_fdc_765a_free(XcpcFdc765a* self)
{
    xcpc_fdc_765a_trace("free");

    return xcpc_delete(XcpcFdc765a, self);
}

XcpcFdc765a* xcpc_fdc_765a_construct(XcpcFdc765a* self)
{
    xcpc_fdc_765a_trace("construct");

    /* clear iface */ {
        (void) memset(&self->iface, 0, sizeof(XcpcFdc765aIface));
    }
    /* clear state */ {
        (void) memset(&self->state, 0, sizeof(XcpcFdc765aState));
    }
    /* initialize state */ {
        self->state.fdc_impl = xcpc_fdc_impl_new();
        self->state.fd0_impl = xcpc_fdd_impl_new();
        self->state.fd1_impl = xcpc_fdd_impl_new();
        self->state.fd2_impl = xcpc_fdd_impl_new();
        self->state.fd3_impl = xcpc_fdd_impl_new();
    }
    return xcpc_fdc_765a_reset(self);
}

XcpcFdc765a* xcpc_fdc_765a_destruct(XcpcFdc765a* self)
{
    xcpc_fdc_765a_trace("destruct");

    /* detach drives */ {
        (void) xcpc_fdc_765a_detach(self, 3);
        (void) xcpc_fdc_765a_detach(self, 2);
        (void) xcpc_fdc_765a_detach(self, 1);
        (void) xcpc_fdc_765a_detach(self, 0);
    }
    /* finalize state */ {
        self->state.fd3_impl = xcpc_fdd_impl_delete(self->state.fd3_impl);
        self->state.fd2_impl = xcpc_fdd_impl_delete(self->state.fd2_impl);
        self->state.fd1_impl = xcpc_fdd_impl_delete(self->state.fd1_impl);
        self->state.fd0_impl = xcpc_fdd_impl_delete(self->state.fd0_impl);
        self->state.fdc_impl = xcpc_fdc_impl_delete(self->state.fdc_impl);
    }
    return self;
}

XcpcFdc765a* xcpc_fdc_765a_new(void)
{
    xcpc_fdc_765a_trace("new");

    return xcpc_fdc_765a_construct(xcpc_fdc_765a_alloc());
}

XcpcFdc765a* xcpc_fdc_765a_delete(XcpcFdc765a* self)
{
    xcpc_fdc_765a_trace("delete");

    return xcpc_fdc_765a_free(xcpc_fdc_765a_destruct(self));
}

XcpcFdc765a* xcpc_fdc_765a_reset(XcpcFdc765a* self)
{
    xcpc_fdc_765a_trace("reset");

    /* reset state */ {
        (void) xcpc_fdc_impl_reset(self->state.fdc_impl);
        (void) xcpc_fdd_impl_reset(self->state.fd0_impl);
        (void) xcpc_fdd_impl_reset(self->state.fd1_impl);
        (void) xcpc_fdd_impl_reset(self->state.fd2_impl);
        (void) xcpc_fdd_impl_reset(self->state.fd3_impl);
    }
    return self;
}

XcpcFdc765a* xcpc_fdc_765a_clock(XcpcFdc765a* self)
{
    /* clock state */ {
        (void) xcpc_fdc_impl_clock(self->state.fdc_impl);
    }
    return self;
}

XcpcFdc765a* xcpc_fdc_765a_attach(XcpcFdc765a* self, int drive)
{
    switch(drive) {
        case 0:
            (void) xcpc_fdc_impl_set_drive(self->state.fdc_impl, self->state.fd0_impl, drive);
            break;
        case 1:
            (void) xcpc_fdc_impl_set_drive(self->state.fdc_impl, self->state.fd1_impl, drive);
            break;
        case 2:
            (void) xcpc_fdc_impl_set_drive(self->state.fdc_impl, self->state.fd2_impl, drive);
            break;
        case 3:
            (void) xcpc_fdc_impl_set_drive(self->state.fdc_impl, self->state.fd3_impl, drive);
            break;
        default:
            break;
    }
    return self;
}

XcpcFdc765a* xcpc_fdc_765a_detach(XcpcFdc765a* self, int drive)
{
    switch(drive) {
        case 0:
            (void) xcpc_fdc_impl_set_drive(self->state.fdc_impl, NULL, drive);
            break;
        case 1:
            (void) xcpc_fdc_impl_set_drive(self->state.fdc_impl, NULL, drive);
            break;
        case 2:
            (void) xcpc_fdc_impl_set_drive(self->state.fdc_impl, NULL, drive);
            break;
        case 3:
            (void) xcpc_fdc_impl_set_drive(self->state.fdc_impl, NULL, drive);
            break;
        default:
            break;
    }
    return self;
}

XcpcFdc765a* xcpc_fdc_765a_insert(XcpcFdc765a* self, int drive, const char* filename)
{
    switch(drive) {
        case 0:
            (void) xcpc_fdd_impl_insert(self->state.fd0_impl, filename);
            break;
        case 1:
            (void) xcpc_fdd_impl_insert(self->state.fd1_impl, filename);
            break;
        case 2:
            (void) xcpc_fdd_impl_insert(self->state.fd2_impl, filename);
            break;
        case 3:
            (void) xcpc_fdd_impl_insert(self->state.fd3_impl, filename);
            break;
        default:
            break;
    }
    return self;
}

XcpcFdc765a* xcpc_fdc_765a_remove(XcpcFdc765a* self, int drive)
{
    switch(drive) {
        case 0:
            (void) xcpc_fdd_impl_remove(self->state.fd0_impl);
            break;
        case 1:
            (void) xcpc_fdd_impl_remove(self->state.fd1_impl);
            break;
        case 2:
            (void) xcpc_fdd_impl_remove(self->state.fd2_impl);
            break;
        case 3:
            (void) xcpc_fdd_impl_remove(self->state.fd3_impl);
            break;
        default:
            break;
    }
    return self;
}

XcpcFdc765a* xcpc_fdc_765a_set_motor(XcpcFdc765a* self, uint8_t motor)
{
    return ((void) xcpc_fdc_impl_set_motor(self->state.fdc_impl, motor), self);
}

XcpcFdc765a* xcpc_fdc_765a_rd_stat(XcpcFdc765a* self, uint8_t* data)
{
    return ((void) xcpc_fdc_impl_rd_stat(self->state.fdc_impl, data), self);
}

XcpcFdc765a* xcpc_fdc_765a_wr_stat(XcpcFdc765a* self, uint8_t* data)
{
    return ((void) xcpc_fdc_impl_wr_stat(self->state.fdc_impl, data), self);
}

XcpcFdc765a* xcpc_fdc_765a_rd_data(XcpcFdc765a* self, uint8_t* data)
{
    return ((void) xcpc_fdc_impl_rd_data(self->state.fdc_impl, data), self);
}

XcpcFdc765a* xcpc_fdc_765a_wr_data(XcpcFdc765a* self, uint8_t* data)
{
    return ((void) xcpc_fdc_impl_wr_data(self->state.fdc_impl, data), self);
}

XcpcFdcImpl* xcpc_fdc_impl_new(void)
{
    return fdc_new();
}

XcpcFdcImpl* xcpc_fdc_impl_delete(XcpcFdcImpl* fdc_impl)
{
    if(fdc_impl != NULL) {
        fdc_impl = (fdc_destroy(&fdc_impl), NULL);
    }
    return fdc_impl;
}

XcpcFdcImpl* xcpc_fdc_impl_reset(XcpcFdcImpl* fdc_impl)
{
    if(fdc_impl != NULL) {
        XcpcFddImpl* fd0 = fdc_getdrive(fdc_impl, 0);
        XcpcFddImpl* fd1 = fdc_getdrive(fdc_impl, 1);
        XcpcFddImpl* fd2 = fdc_getdrive(fdc_impl, 2);
        XcpcFddImpl* fd3 = fdc_getdrive(fdc_impl, 3);
        fdc_reset(fdc_impl);
        fdc_setdrive(fdc_impl, 0, fd0);
        fdc_setdrive(fdc_impl, 1, fd1);
        fdc_setdrive(fdc_impl, 2, fd2);
        fdc_setdrive(fdc_impl, 3, fd3);
    }
    return fdc_impl;
}

XcpcFdcImpl* xcpc_fdc_impl_clock(XcpcFdcImpl* fdc_impl)
{
    if(fdc_impl != NULL) {
        fdc_tick(fdc_impl);
    }
    return fdc_impl;
}

XcpcFdcImpl* xcpc_fdc_impl_set_drive(XcpcFdcImpl* fdc_impl, XcpcFddImpl* fdd_impl, int drive)
{
    if(fdc_impl != NULL) {
        fdc_setdrive(fdc_impl, drive, fdd_impl);
    }
    return fdc_impl;
}

XcpcFdcImpl* xcpc_fdc_impl_set_motor(XcpcFdcImpl* fdc_impl, uint8_t motor)
{
    if(fdc_impl != NULL) {
        fdc_set_motor(fdc_impl, motor);
    }
    return fdc_impl;
}

XcpcFdcImpl* xcpc_fdc_impl_rd_stat(XcpcFdcImpl* fdc_impl, uint8_t* data)
{
    if(fdc_impl != NULL) {
        *data = fdc_read_ctrl(fdc_impl);
    }
    return fdc_impl;
}

XcpcFdcImpl* xcpc_fdc_impl_wr_stat(XcpcFdcImpl* fdc_impl, uint8_t* data)
{
    if(fdc_impl != NULL) {
        /* do nothing */
    }
    return fdc_impl;
}

XcpcFdcImpl* xcpc_fdc_impl_rd_data(XcpcFdcImpl* fdc_impl, uint8_t* data)
{
    if(fdc_impl != NULL) {
        *data = fdc_read_data(fdc_impl);
    }
    return fdc_impl;
}

XcpcFdcImpl* xcpc_fdc_impl_wr_data(XcpcFdcImpl* fdc_impl, uint8_t* data)
{
    if(fdc_impl != NULL) {
        fdc_write_data(fdc_impl, *data);
    }
    return fdc_impl;
}

XcpcFddImpl* xcpc_fdd_impl_new(void)
{
    return fd_newldsk();
}

XcpcFddImpl* xcpc_fdd_impl_delete(XcpcFddImpl* fdd_impl)
{
    if(fdd_impl != NULL) {
        fdd_impl = (fd_destroy(&fdd_impl), NULL);
    }
    return fdd_impl;
}

XcpcFddImpl* xcpc_fdd_impl_reset(XcpcFddImpl* fdd_impl)
{
    if(fdd_impl != NULL) {
        fd_reset(fdd_impl);
    }
    return fdd_impl;
}

XcpcFddImpl* xcpc_fdd_impl_insert(XcpcFddImpl* fdd_impl, const char* filename)
{
    if(fdd_impl != NULL) {
        if((filename != NULL) && (*filename != '\0')) {
            fdl_setfilename(fdd_impl, filename);
        }
        else {
            fd_eject(fdd_impl);
        }
    }
    return fdd_impl;
}

XcpcFddImpl* xcpc_fdd_impl_remove(XcpcFddImpl* fdd_impl)
{
    if(fdd_impl != NULL) {
        fd_eject(fdd_impl);
    }
    return fdd_impl;
}
