/*
 * psg-8910-impl.h - Copyright (c) 2001-2021 - Olivier Poncet
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
#ifndef __XCPC_PSG_8910_IMPL_H__
#define __XCPC_PSG_8910_IMPL_H__

#include <xcpc/libxcpc.h>

#ifdef __cplusplus
extern "C" {
#endif

#define XCPC_PSG_8910_IFACE(instance) (&(instance)->iface)
#define XCPC_PSG_8910_SETUP(instance) (&(instance)->setup)
#define XCPC_PSG_8910_STATE(instance) (&(instance)->state)

typedef struct _XcpcPsg8910Iface XcpcPsg8910Iface;
typedef struct _XcpcPsg8910Setup XcpcPsg8910Setup;
typedef struct _XcpcPsg8910State XcpcPsg8910State;
typedef struct _XcpcPsg8910      XcpcPsg8910;

struct _XcpcPsg8910Iface
{
    void* user_data;
    uint8_t (*rd_port_a)(XcpcPsg8910* psg_8910, uint8_t data);
    uint8_t (*wr_port_a)(XcpcPsg8910* psg_8910, uint8_t data);
    uint8_t (*rd_port_b)(XcpcPsg8910* psg_8910, uint8_t data);
    uint8_t (*wr_port_b)(XcpcPsg8910* psg_8910, uint8_t data);
};

struct _XcpcPsg8910Setup
{
    int reserved;
};

struct _XcpcPsg8910State
{
    union
    {
        struct
        {
            uint8_t addr;
            uint8_t data[16];
        } array;
        struct
        {
            uint8_t address_register;
            uint8_t channel_a_fine_tune;
            uint8_t channel_a_coarse_tune;
            uint8_t channel_b_fine_tune;
            uint8_t channel_b_coarse_tune;
            uint8_t channel_c_fine_tune;
            uint8_t channel_c_coarse_tune;
            uint8_t noise_period;
            uint8_t mixer_and_io_control;
            uint8_t channel_a_volume;
            uint8_t channel_b_volume;
            uint8_t channel_c_volume;
            uint8_t envelope_fine_tune;
            uint8_t envelope_coarse_tune;
            uint8_t envelope_shape;
            uint8_t io_port_a;
            uint8_t io_port_b;
        } named;
    } regs;
};

struct _XcpcPsg8910
{
    XcpcPsg8910Iface iface;
    XcpcPsg8910Setup setup;
    XcpcPsg8910State state;
};

#ifdef __cplusplus
}
#endif

#endif /* __XCPC_PSG_8910_IMPL_H__ */
