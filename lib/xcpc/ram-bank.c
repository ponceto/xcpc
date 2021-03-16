/*
 * ram-bank.c - Copyright (c) 2001-2021 - Olivier Poncet
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

#ifndef CHECK_INSTANCE
#define CHECK_INSTANCE(pointer) (pointer != NULL ? XCPC_RAM_BANK_STATUS_SUCCESS : XCPC_RAM_BANK_STATUS_FAILURE)
#endif

XcpcRamBank* xcpc_ram_bank_alloc(void)
{
    /* debug */ {
        g_log ( XCPC_LOG_DOMAIN, G_LOG_LEVEL_DEBUG
              , "%s::alloc()"
              , "XcpcRamBank" );
    }
    return xcpc_new(XcpcRamBank);
}

XcpcRamBank* xcpc_ram_bank_free(XcpcRamBank* self)
{
    /* debug */ {
        g_log ( XCPC_LOG_DOMAIN, G_LOG_LEVEL_DEBUG
              , "%s::free()"
              , "XcpcRamBank" );
    }
    return xcpc_delete(XcpcRamBank, self);
}

XcpcRamBank* xcpc_ram_bank_construct(XcpcRamBank* self)
{
    /* debug */ {
        g_log ( XCPC_LOG_DOMAIN, G_LOG_LEVEL_DEBUG
              , "%s::construct()"
              , "XcpcRamBank" );
    }
    return xcpc_ram_bank_clear(self);
}

XcpcRamBank* xcpc_ram_bank_destruct(XcpcRamBank* self)
{
    /* debug */ {
        g_log ( XCPC_LOG_DOMAIN, G_LOG_LEVEL_DEBUG
              , "%s::destruct()"
              , "XcpcRamBank" );
    }
    return self;
}

XcpcRamBank* xcpc_ram_bank_new(void)
{
    /* debug */ {
        g_log ( XCPC_LOG_DOMAIN, G_LOG_LEVEL_DEBUG
              , "%s::new()"
              , "XcpcRamBank" );
    }
    return xcpc_ram_bank_construct(xcpc_ram_bank_alloc());
}

XcpcRamBank* xcpc_ram_bank_delete(XcpcRamBank* self)
{
    /* debug */ {
        g_log ( XCPC_LOG_DOMAIN, G_LOG_LEVEL_DEBUG
              , "%s::delete()"
              , "XcpcRamBank" );
    }
    return xcpc_ram_bank_free(xcpc_ram_bank_destruct(self));
}

XcpcRamBank* xcpc_ram_bank_reset(XcpcRamBank* self)
{
    /* debug */ {
        g_log ( XCPC_LOG_DOMAIN, G_LOG_LEVEL_DEBUG
              , "%s::reset()"
              , "XcpcRamBank" );
    }
    if(self != NULL) {
        (void) memset(self->data, 0, sizeof(self->data));
    }
    return self;
}

XcpcRamBank* xcpc_ram_bank_clear(XcpcRamBank* self)
{
    /* debug */ {
        g_log ( XCPC_LOG_DOMAIN, G_LOG_LEVEL_DEBUG
              , "%s::clear()"
              , "XcpcRamBank" );
    }
    if(self != NULL) {
        (void) memset(self->data, 0, sizeof(self->data));
    }
    return self;
}

XcpcRamBankStatus xcpc_ram_bank_load(XcpcRamBank* self, const char* filename, size_t offset)
{
    XcpcRamBankStatus status = CHECK_INSTANCE(self);
    FILE*             file   = NULL;

    /* debug */ {
        g_log ( XCPC_LOG_DOMAIN, G_LOG_LEVEL_DEBUG
              , "%s::load()"
              , "XcpcRamBank" );
    }
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
            void*  ram_data = &self->data;
            size_t ram_size = sizeof(self->data);
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
    XcpcRamBankStatus status = CHECK_INSTANCE(self);

    /* debug */ {
        g_log ( XCPC_LOG_DOMAIN, G_LOG_LEVEL_DEBUG
              , "%s::copy()"
              , "XcpcRamBank" );
    }
    /* check data and size */ {
        if(status == XCPC_RAM_BANK_STATUS_SUCCESS) {
            if((data == NULL) || (size > sizeof(self->data))) {
                status = XCPC_RAM_BANK_STATUS_FAILURE;
            }
        }
    }
    /* copy data */ {
        if(status == XCPC_RAM_BANK_STATUS_SUCCESS) {
            (void) memcpy(self->data, data, size);
        }
    }
    return status;
}
