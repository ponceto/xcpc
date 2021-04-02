/*
 * libxcpc-priv.h - Copyright (c) 2001-2021 - Olivier Poncet
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
#ifndef __XCPC_LIBXCPC_PRIV_H__
#define __XCPC_LIBXCPC_PRIV_H__

#include <xcpc/libxcpc.h>

#ifdef __cplusplus
extern "C" {
#endif

#define XCPC_DEFAULT_INPUT_STREAM stdin
#define XCPC_DEFAULT_PRINT_STREAM stdout
#define XCPC_DEFAULT_ERROR_STREAM stderr

typedef struct _XcpcLibrary    XcpcLibrary;
typedef struct _XcpcColorEntry XcpcColorEntry;

struct _XcpcLibrary
{
    int   initialized;
    int   major_version;
    int   minor_version;
    int   micro_version;
    int   loglevel;
    FILE* input_stream;
    FILE* print_stream;
    FILE* error_stream;
};

struct _XcpcColorEntry
{
    XcpcColor      color;
    const char*    label;
    unsigned short red;
    unsigned short green;
    unsigned short blue;
    unsigned short luminance;
};

#ifdef __cplusplus
}
#endif

#endif /* __XCPC_LIBXCPC_PRIV_H__ */
