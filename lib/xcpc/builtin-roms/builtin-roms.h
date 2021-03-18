/*
 * builtin-roms.h - Copyright (c) 2001-2021 - Olivier Poncet
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
#ifndef __XCPC_BUILTIN_ROMS_H__
#define __XCPC_BUILTIN_ROMS_H__

#include <xcpc/builtin-roms/builtin-roms-impl.h>

#ifdef __cplusplus
extern "C" {
#endif

extern const XcpcSystemRom xcpc_cpc464_rom;
extern const XcpcSystemRom xcpc_cpc664_rom;
extern const XcpcSystemRom xcpc_cpc6128_rom;
extern const XcpcAmsdosRom xcpc_amsdos_rom;

#ifdef __cplusplus
}
#endif

#endif /* __XCPC_BUILTIN_ROMS_H__ */
