/*
 * ram-bank.c - Copyright (c) 2001-2023 - Olivier Poncet
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
#include "ram-bank-priv.h"

static void log_trace(const char* function)
{
    xcpc_log_trace("XcpcRamBank::%s()", function);
}

XcpcRamBank* xcpc_ram_bank_alloc(void)
{
    log_trace("alloc");

    return xcpc_new(XcpcRamBank);
}

XcpcRamBank* xcpc_ram_bank_free(XcpcRamBank* self)
{
    log_trace("free");

    return xcpc_delete(XcpcRamBank, self);
}

XcpcRamBank* xcpc_ram_bank_construct(XcpcRamBank* self, const XcpcRamBankIface* iface)
{
    log_trace("construct");

    /* clear all */ {
        (void) memset(&self->iface, 0, sizeof(XcpcRamBankIface));
        (void) memset(&self->setup, 0, sizeof(XcpcRamBankSetup));
        (void) memset(&self->state, 0, sizeof(XcpcRamBankState));
    }
    /* initialize iface */ {
        if(iface != NULL) {
            *(&self->iface) = *(iface);
        }
        else {
            self->iface.user_data = NULL;
        }
    }
    /* reset */ {
        (void) xcpc_ram_bank_reset(self);
    }
    return self;
}

XcpcRamBank* xcpc_ram_bank_destruct(XcpcRamBank* self)
{
    log_trace("destruct");

    return self;
}

XcpcRamBank* xcpc_ram_bank_new(const XcpcRamBankIface* iface)
{
    log_trace("new");

    return xcpc_ram_bank_construct(xcpc_ram_bank_alloc(), iface);
}

XcpcRamBank* xcpc_ram_bank_delete(XcpcRamBank* self)
{
    log_trace("delete");

    return xcpc_ram_bank_free(xcpc_ram_bank_destruct(self));
}

XcpcRamBank* xcpc_ram_bank_reset(XcpcRamBank* self)
{
    log_trace("reset");

    /* reset state */ {
        unsigned int index = 0;
        unsigned int count = countof(self->state.data);
        do {
            self->state.data[index] &= 0;
        } while(++index < count);
    }
    return self;
}

XcpcRamBankStatus xcpc_ram_bank_load(XcpcRamBank* self, const char* filename, size_t offset)
{
    XcpcRamBankStatus status = XCPC_RAM_BANK_STATUS_SUCCESS;
    FILE*             file   = NULL;

    log_trace("load");
    /* check filename */ {
        if(status == XCPC_RAM_BANK_STATUS_SUCCESS) {
            if((filename == NULL) || (*filename == '\0')) {
                status = XCPC_RAM_BANK_STATUS_FAILURE;
            }
        }
    }
    /* open file */ {
        if(status == XCPC_RAM_BANK_STATUS_SUCCESS) {
            file = fopen(filename, "r");
            if(file == NULL) {
                status = XCPC_RAM_BANK_STATUS_FAILURE;
            }
        }
    }
    /* seek file */ {
        if(status == XCPC_RAM_BANK_STATUS_SUCCESS) {
            if(fseek(file, offset, SEEK_SET) != 0) {
                status = XCPC_RAM_BANK_STATUS_FAILURE;
            }
        }
    }
    /* load data */ {
        if(status == XCPC_RAM_BANK_STATUS_SUCCESS) {
            void*  ram_data = &self->state.data;
            size_t ram_size = sizeof(self->state.data);
            size_t byte_count = fread(ram_data, 1, ram_size, file);
            if(byte_count != ram_size) {
                status = XCPC_RAM_BANK_STATUS_FAILURE;
            }
        }
    }
    /* close file */ {
        if(file != NULL) {
            file = ((void) fclose(file), NULL);
        }
    }
    return status;
}

XcpcRamBankStatus xcpc_ram_bank_copy(XcpcRamBank* self, const uint8_t* data, size_t size)
{
    XcpcRamBankStatus status = XCPC_RAM_BANK_STATUS_SUCCESS;

    log_trace("copy");
    /* check data and size */ {
        if(status == XCPC_RAM_BANK_STATUS_SUCCESS) {
            if((data == NULL) || (size > sizeof(self->state.data))) {
                status = XCPC_RAM_BANK_STATUS_FAILURE;
            }
        }
    }
    /* copy data */ {
        if(status == XCPC_RAM_BANK_STATUS_SUCCESS) {
            (void) memcpy(self->state.data, data, size);
        }
    }
    return status;
}
