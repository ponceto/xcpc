/*
 * machine.c - Copyright (c) 2001-2021 - Olivier Poncet
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
#include "machine-priv.h"

static void log_trace(const char* function)
{
    xcpc_log_trace("XcpcMachine::%s()", function);
}

static void cpc_mem_select(XcpcMachine* self)
{
    if(self->setup.memory_size >= XCPC_MEMORY_SIZE_128K) {
        switch(self->pager.conf.ram) {
            case 0x00:
                {
                    self->pager.bank.rd[0] = self->pager.bank.wr[0] = self->board.ram_bank[0]->state.data;
                    self->pager.bank.rd[1] = self->pager.bank.wr[1] = self->board.ram_bank[1]->state.data;
                    self->pager.bank.rd[2] = self->pager.bank.wr[2] = self->board.ram_bank[2]->state.data;
                    self->pager.bank.rd[3] = self->pager.bank.wr[3] = self->board.ram_bank[3]->state.data;
                }
                break;
            case 0x01:
                {
                    self->pager.bank.rd[0] = self->pager.bank.wr[0] = self->board.ram_bank[0]->state.data;
                    self->pager.bank.rd[1] = self->pager.bank.wr[1] = self->board.ram_bank[1]->state.data;
                    self->pager.bank.rd[2] = self->pager.bank.wr[2] = self->board.ram_bank[2]->state.data;
                    self->pager.bank.rd[3] = self->pager.bank.wr[3] = self->board.ram_bank[7]->state.data;
                }
                break;
            case 0x02:
                {
                    self->pager.bank.rd[0] = self->pager.bank.wr[0] = self->board.ram_bank[4]->state.data;
                    self->pager.bank.rd[1] = self->pager.bank.wr[1] = self->board.ram_bank[5]->state.data;
                    self->pager.bank.rd[2] = self->pager.bank.wr[2] = self->board.ram_bank[6]->state.data;
                    self->pager.bank.rd[3] = self->pager.bank.wr[3] = self->board.ram_bank[7]->state.data;
                }
                break;
            case 0x03:
                {
                    self->pager.bank.rd[0] = self->pager.bank.wr[0] = self->board.ram_bank[0]->state.data;
                    self->pager.bank.rd[1] = self->pager.bank.wr[1] = self->board.ram_bank[3]->state.data;
                    self->pager.bank.rd[2] = self->pager.bank.wr[2] = self->board.ram_bank[2]->state.data;
                    self->pager.bank.rd[3] = self->pager.bank.wr[3] = self->board.ram_bank[7]->state.data;
                }
                break;
            case 0x04:
                {
                    self->pager.bank.rd[0] = self->pager.bank.wr[0] = self->board.ram_bank[0]->state.data;
                    self->pager.bank.rd[1] = self->pager.bank.wr[1] = self->board.ram_bank[4]->state.data;
                    self->pager.bank.rd[2] = self->pager.bank.wr[2] = self->board.ram_bank[2]->state.data;
                    self->pager.bank.rd[3] = self->pager.bank.wr[3] = self->board.ram_bank[3]->state.data;
                }
                break;
            case 0x05:
                {
                    self->pager.bank.rd[0] = self->pager.bank.wr[0] = self->board.ram_bank[0]->state.data;
                    self->pager.bank.rd[1] = self->pager.bank.wr[1] = self->board.ram_bank[5]->state.data;
                    self->pager.bank.rd[2] = self->pager.bank.wr[2] = self->board.ram_bank[2]->state.data;
                    self->pager.bank.rd[3] = self->pager.bank.wr[3] = self->board.ram_bank[3]->state.data;
                }
                break;
            case 0x06:
                {
                    self->pager.bank.rd[0] = self->pager.bank.wr[0] = self->board.ram_bank[0]->state.data;
                    self->pager.bank.rd[1] = self->pager.bank.wr[1] = self->board.ram_bank[6]->state.data;
                    self->pager.bank.rd[2] = self->pager.bank.wr[2] = self->board.ram_bank[2]->state.data;
                    self->pager.bank.rd[3] = self->pager.bank.wr[3] = self->board.ram_bank[3]->state.data;
                }
                break;
            case 0x07:
                {
                    self->pager.bank.rd[0] = self->pager.bank.wr[0] = self->board.ram_bank[0]->state.data;
                    self->pager.bank.rd[1] = self->pager.bank.wr[1] = self->board.ram_bank[7]->state.data;
                    self->pager.bank.rd[2] = self->pager.bank.wr[2] = self->board.ram_bank[2]->state.data;
                    self->pager.bank.rd[3] = self->pager.bank.wr[3] = self->board.ram_bank[3]->state.data;
                }
                break;
            default:
                xcpc_log_alert("cpc_mem_select() : unsupported ram configuration (%02x)", self->pager.conf.ram);
                break;
        }
    }
    else {
        self->pager.bank.rd[0] = self->pager.bank.wr[0] = self->board.ram_bank[0]->state.data;
        self->pager.bank.rd[1] = self->pager.bank.wr[1] = self->board.ram_bank[1]->state.data;
        self->pager.bank.rd[2] = self->pager.bank.wr[2] = self->board.ram_bank[2]->state.data;
        self->pager.bank.rd[3] = self->pager.bank.wr[3] = self->board.ram_bank[3]->state.data;
    }
    if((self->board.vga_core->state.rmr & 0x04) == 0) {
        if(self->board.rom_bank[0] != NULL) {
            self->pager.bank.rd[0] = self->board.rom_bank[0]->state.data;
        }
    }
    if((self->board.vga_core->state.rmr & 0x08) == 0) {
        if(self->board.rom_bank[1] != NULL) {
            self->pager.bank.rd[3] = self->board.rom_bank[1]->state.data;
        }
        if(self->board.exp_bank[self->pager.conf.rom] != NULL) {
            self->pager.bank.rd[3] = self->board.exp_bank[self->pager.conf.rom]->state.data;
        }
    }
}

static uint8_t cpu_mreq_m1(XcpcCpuZ80a* cpu_z80a, uint16_t addr, uint8_t data)
{
    XcpcMachine* self = ((XcpcMachine*)(cpu_z80a->iface.user_data));
    const uint16_t index  = ((addr >> 14) & 0x0003);
    const uint16_t offset = ((addr >>  0) & 0x3fff);

    /* mreq m1 */ {
        data = self->pager.bank.rd[index][offset];
    }
    return data;
}

static uint8_t cpu_mreq_rd(XcpcCpuZ80a* cpu_z80a, uint16_t addr, uint8_t data)
{
    XcpcMachine* self = ((XcpcMachine*)(cpu_z80a->iface.user_data));
    const uint16_t index  = ((addr >> 14) & 0x0003);
    const uint16_t offset = ((addr >>  0) & 0x3fff);

    /* mreq rd */ {
        data = self->pager.bank.rd[index][offset];
    }
    return data;
}

static uint8_t cpu_mreq_wr(XcpcCpuZ80a* cpu_z80a, uint16_t addr, uint8_t data)
{
    XcpcMachine* self = ((XcpcMachine*)(cpu_z80a->iface.user_data));
    const uint16_t index  = ((addr >> 14) & 0x0003);
    const uint16_t offset = ((addr >>  0) & 0x3fff);

    /* mreq wr */ {
        self->pager.bank.wr[index][offset] = data;
    }
    return data;
}

static uint8_t cpu_iorq_m1(XcpcCpuZ80a* cpu_z80a, uint16_t port, uint8_t data)
{
    XcpcMachine* self = ((XcpcMachine*)(cpu_z80a->iface.user_data));

    /* clear data */ {
        data = 0x00;
    }
    /* iorq m1 */ {
        self->board.vga_core->state.counter &= 0x1f;
    }
    return data;
}

static uint8_t cpu_iorq_rd(XcpcCpuZ80a* cpu_z80a, uint16_t port, uint8_t data)
{
    XcpcMachine* self = ((XcpcMachine*)(cpu_z80a->iface.user_data));

    /* clear data */ {
        data = 0x00;
    }
    /* vga-core [0-------xxxxxxxx] [0x7fxx] */ {
        if((port & 0x8000) == 0) {
            xcpc_log_alert("cpu_iorq_rd(0x%04x) : vga-core [---- illegal ----]", port);
        }
    }
    /* vdc-6845 [-0------xxxxxxxx] [0xbfxx] */ {
        if((port & 0x4000) == 0) {
            switch((port >> 8) & 3) {
                case 0:  /* [-0----00xxxxxxxx] [0xbcxx] */
                    xcpc_log_alert("cpu_iorq_rd(0x%04x) : vdc-6845 [---- illegal ----]", port);
                    break;
                case 1:  /* [-0----01xxxxxxxx] [0xbdxx] */
                    xcpc_log_alert("cpu_iorq_rd(0x%04x) : vdc-6845 [---- illegal ----]", port);
                    break;
                case 2:  /* [-0----10xxxxxxxx] [0xbexx] */
                    xcpc_log_alert("cpu_iorq_rd(0x%04x) : vdc-6845 [- not supported -]", port);
                    break;
                case 3:  /* [-0----11xxxxxxxx] [0xbfxx] */
                    data = xcpc_vdc_6845_rd(self->board.vdc_6845, 0xff);
                    break;
            }
        }
    }
    /* rom-conf [--0-----xxxxxxxx] [0xdfxx] */ {
        if((port & 0x2000) == 0) {
            xcpc_log_alert("cpu_iorq_rd(0x%04x) : rom-conf [---- illegal ----]", port);
        }
    }
    /* prt-port [---0----xxxxxxxx] [0xefxx] */ {
        if((port & 0x1000) == 0) {
            xcpc_log_alert("cpu_iorq_rd(0x%04x) : prt-port [---- illegal ----]", port);
        }
    }
    /* ppi-8255 [----0---xxxxxxxx] [0xf7xx] */ {
        if((port & 0x0800) == 0) {
            switch((port >> 8) & 3) {
                case 0:  /* [----0-00xxxxxxxx] [0xf4xx] */
                    self->board.ppi_8255->state.port_a = self->board.keyboard->state.keys[self->board.keyboard->state.line];
                    data = self->board.ppi_8255->state.port_a;
                    break;
                case 1:  /* [----0-01xxxxxxxx] [0xf5xx] */
                    self->board.ppi_8255->state.port_b = ((self->state.cassette  & 0x01) << 7)
                                                       | ((self->state.parallel  & 0x01) << 6)
                                                       | ((self->state.expansion & 0x01) << 5)
                                                       | ((self->state.refresh   & 0x01) << 4)
                                                       | ((self->state.company   & 0x07) << 1)
                                                       | ((self->state.vsync     & 0x01) << 0);
                    data = self->board.ppi_8255->state.port_b;
                    break;
                case 2:  /* [----0-10xxxxxxxx] [0xf6xx] */
                    data = self->board.ppi_8255->state.port_c;
                    break;
                case 3:  /* [----0-11xxxxxxxx] [0xf7xx] */
                    xcpc_log_alert("cpu_iorq_rd(0x%04x) : ppi-8255 [---- illegal ----]", port);
                    break;
            }
        }
    }
    /* fdc-765a [-----0--0xxxxxxx] [0xfb7f] */ {
        if((port & 0x0480) == 0) {
            switch(((port >> 7) & 2) | (port & 1)) {
                case 0:  /* [-----0-00xxxxxx0] [0xfa7e] */
                    xcpc_log_alert("cpu_iorq_rd(0x%04x) : fdc-765a [---- illegal ----]", port);
                    break;
                case 1:  /* [-----0-00xxxxxx1] [0xfa7f] */
                    xcpc_log_alert("cpu_iorq_rd(0x%04x) : fdc-765a [---- illegal ----]", port);
                    break;
                case 2:  /* [-----0-10xxxxxx0] [0xfb7e] */
                    xcpc_fdc_765a_rd_stat(self->board.fdc_765a, &data);
                    break;
                case 3:  /* [-----0-10xxxxxx1] [0xfb7f] */
                    xcpc_fdc_765a_rd_data(self->board.fdc_765a, &data);
                    break;
            }
        }
    }
    return data;
}

static uint8_t cpu_iorq_wr(XcpcCpuZ80a* cpu_z80a, uint16_t port, uint8_t data)
{
    XcpcMachine* self = ((XcpcMachine*)(cpu_z80a->iface.user_data));

    /* vga-core [0-------xxxxxxxx] [0x7fxx] */ {
        if((port & 0x8000) == 0) {
            switch((data >> 6) & 3) {
                case 0: /* Select pen */
                    self->board.vga_core->state.pen = (data & 0x10 ? 0x10 : data & 0x0f);
                    break;
                case 1: /* Select color */
                    self->board.vga_core->state.ink[self->board.vga_core->state.pen] = data & 0x1f;
                    break;
                case 2: /* Interrupt control, ROM configuration and screen mode */
                    if((data & 0x10) != 0) {
                        self->board.vga_core->state.counter = 0;
                    }
                    self->board.vga_core->state.rmr = data & 0x1f;
                    cpc_mem_select(self);
                    break;
                case 3: /* RAM memory management */
                    self->pager.conf.ram = data & 0x3f;
                    cpc_mem_select(self);
                    break;
            }
        }
    }
    /* vdc-6845 [-0------xxxxxxxx] [0xbfxx] */ {
        if((port & 0x4000) == 0) {
            switch((port >> 8) & 3) {
                case 0:  /* [-0----00xxxxxxxx] [0xbcxx] */
                    xcpc_vdc_6845_rs(self->board.vdc_6845, data);
                    break;
                case 1:  /* [-0----01xxxxxxxx] [0xbdxx] */
                    xcpc_vdc_6845_wr(self->board.vdc_6845, data);
                    break;
                case 2:  /* [-0----10xxxxxxxx] [0xbexx] */
                    xcpc_log_alert("cpu_iorq_wr(0x%04x) : vdc-6845 [- not supported -]", port);
                    break;
                case 3:  /* [-0----11xxxxxxxx] [0xbfxx] */
                    xcpc_log_alert("cpu_iorq_wr(0x%04x) : vdc-6845 [---- illegal ----]", port);
                    break;
            }
        }
    }
    /* rom-conf [--0-----xxxxxxxx] [0xdfxx] */ {
        if((port & 0x2000) == 0) {
            self->pager.conf.rom = data;
            cpc_mem_select(self);
        }
    }
    /* prt-port [---0----xxxxxxxx] [0xefxx] */ {
        if((port & 0x1000) == 0) {
            /* xxx */
        }
    }
    /* ppi-8255 [----0---xxxxxxxx] [0xf7xx] */ {
        if((port & 0x0800) == 0) {
            switch((port >> 8) & 3) {
                case 0:  /* [----0-00xxxxxxxx] [0xf4xx] */
                    self->board.ppi_8255->state.port_a = data;
                    break;
                case 1:  /* [----0-01xxxxxxxx] [0xf5xx] */
                /*  self->board.ppi_8255->state.port_b = data; */
                    break;
                case 2:  /* [----0-10xxxxxxxx] [0xf6xx] */
                    self->board.ppi_8255->state.port_c = data;
                    self->board.keyboard->state.line = data & 0x0F;
                    break;
                case 3:  /* [----0-11xxxxxxxx] [0xf7xx] */
                    self->board.ppi_8255->state.ctrl_p = data;
                    break;
            }
        }
    }
    /* fdc-765a [-----0--0xxxxxxx] [0xfb7f] */ {
        if((port & 0x0480) == 0) {
            switch(((port >> 7) & 2) | ((port >> 0) & 1)) {
                case 0:  /* [-----0-00xxxxxx0] [0xfa7e] */
                    xcpc_fdc_765a_set_motor(self->board.fdc_765a, ((data & 1) << 1) | ((data & 1) << 0));
                    break;
                case 1:  /* [-----0-00xxxxxx1] [0xfa7f] */
                    xcpc_fdc_765a_set_motor(self->board.fdc_765a, ((data & 1) << 1) | ((data & 1) << 0));
                    break;
                case 2:  /* [-----0-10xxxxxx0] [0xfb7e] */
                    xcpc_fdc_765a_wr_stat(self->board.fdc_765a, &data);
                    break;
                case 3:  /* [-----0-10xxxxxx1] [0xfb7f] */
                    xcpc_fdc_765a_wr_data(self->board.fdc_765a, &data);
                    break;
            }
        }
    }
    return data;
}

static uint8_t vdc_hsync(XcpcVdc6845* vdc_6845, int hsync)
{
    XcpcMachine* self = ((XcpcMachine*)(vdc_6845->iface.user_data));
    XcpcMonitor* monitor  = self->board.monitor;
    XcpcCpuZ80a* cpu_z80a = self->board.cpu_z80a;
    XcpcVgaCore* vga_core = self->board.vga_core;

    if((self->state.hsync = hsync) == 0) {
        /* falling edge */ {
            if(++vga_core->state.counter == 52) {
                xcpc_cpu_z80a_pulse_int(cpu_z80a);
                vga_core->state.counter = 0;
            }
            if(vga_core->state.delayed > 0) {
                if(--vga_core->state.delayed == 0) {
                    if(vga_core->state.counter >= 32) {
                        xcpc_cpu_z80a_pulse_int(cpu_z80a);
                    }
                    vga_core->state.counter = 0;
                }
            }
            /* update scanline */ {
                XcpcScanline* scanline = &self->frame.array[(self->frame.index + 1) % 312];
                /* update mode */ {
                    scanline->mode = vga_core->state.rmr & 0x03;
                }
                /* update colors */ {
                    int index = 0;
                    do {
                        const uint8_t ink = vga_core->state.ink[index];
                        scanline->ink[index].value = ink;
                        scanline->ink[index].pixel = monitor->state.palette[ink].pixel;
                    } while(++index < 17);
                }
            }
        }
    }
    else {
        /* rising edge */ {
            /* do nothing */
        }
    }
    return 0x00;
}

static uint8_t vdc_vsync(XcpcVdc6845* vdc_6845, int vsync)
{
    XcpcMachine* self = ((XcpcMachine*)(vdc_6845->iface.user_data));

    if((self->state.vsync = vsync) != 0) {
        /* rising edge */ {
            self->board.vga_core->state.delayed = 2;
        }
    }
    else {
        /* falling edge */ {
            /* do nothing */
        }
    }
    return 0x00;
}

static uint8_t ppi_rd_port_a(XcpcPpi8255* ppi_8255, uint8_t data)
{
    XcpcMachine* self = ((XcpcMachine*)(ppi_8255->iface.user_data));

    if(self != NULL) {
        log_trace("ppi_rd_port_a");
    }
    return 0x00;
}

static uint8_t ppi_wr_port_a(XcpcPpi8255* ppi_8255, uint8_t data)
{
    XcpcMachine* self = ((XcpcMachine*)(ppi_8255->iface.user_data));

    if(self != NULL) {
        log_trace("ppi_wr_port_a");
    }
    return 0x00;
}

static uint8_t ppi_rd_port_b(XcpcPpi8255* ppi_8255, uint8_t data)
{
    XcpcMachine* self = ((XcpcMachine*)(ppi_8255->iface.user_data));

    if(self != NULL) {
        log_trace("ppi_rd_port_b");
    }
    return 0x00;
}

static uint8_t ppi_wr_port_b(XcpcPpi8255* ppi_8255, uint8_t data)
{
    XcpcMachine* self = ((XcpcMachine*)(ppi_8255->iface.user_data));

    if(self != NULL) {
        log_trace("ppi_wr_port_b");
    }
    return 0x00;
}

static uint8_t ppi_rd_port_c(XcpcPpi8255* ppi_8255, uint8_t data)
{
    XcpcMachine* self = ((XcpcMachine*)(ppi_8255->iface.user_data));

    if(self != NULL) {
        log_trace("ppi_rd_port_c");
    }
    return 0x00;
}

static uint8_t ppi_wr_port_c(XcpcPpi8255* ppi_8255, uint8_t data)
{
    XcpcMachine* self = ((XcpcMachine*)(ppi_8255->iface.user_data));

    if(self != NULL) {
        log_trace("ppi_wr_port_c");
    }
    return 0x00;
}

static uint8_t psg_rd_port_a(XcpcPsg8910* psg_8910, uint8_t data)
{
    XcpcMachine* self = ((XcpcMachine*)(psg_8910->iface.user_data));

    if(self != NULL) {
        log_trace("psg_rd_port_a");
    }
    return 0x00;
}

static uint8_t psg_wr_port_a(XcpcPsg8910* psg_8910, uint8_t data)
{
    XcpcMachine* self = ((XcpcMachine*)(psg_8910->iface.user_data));

    if(self != NULL) {
        log_trace("psg_wr_port_a");
    }
    return 0x00;
}

static uint8_t psg_rd_port_b(XcpcPsg8910* psg_8910, uint8_t data)
{
    XcpcMachine* self = ((XcpcMachine*)(psg_8910->iface.user_data));

    if(self != NULL) {
        log_trace("psg_rd_port_b");
    }
    return 0x00;
}

static uint8_t psg_wr_port_b(XcpcPsg8910* psg_8910, uint8_t data)
{
    XcpcMachine* self = ((XcpcMachine*)(psg_8910->iface.user_data));

    if(self != NULL) {
        log_trace("psg_wr_port_b");
    }
    return 0x00;
}

static void construct_iface(XcpcMachine* self)
{
    (void) xcpc_machine_set_iface(self, NULL);
}

static void destruct_iface(XcpcMachine* self)
{
}

static void reset_iface(XcpcMachine* self)
{
}

static void construct_setup(XcpcMachine* self)
{
    self->setup.company_name  = XCPC_COMPANY_NAME_DEFAULT;
    self->setup.machine_type  = XCPC_MACHINE_TYPE_DEFAULT;
    self->setup.monitor_type  = XCPC_MONITOR_TYPE_DEFAULT;
    self->setup.refresh_rate  = XCPC_REFRESH_RATE_DEFAULT;
    self->setup.keyboard_type = XCPC_KEYBOARD_TYPE_DEFAULT;
    self->setup.memory_size   = XCPC_MEMORY_SIZE_DEFAULT;
}

static void destruct_setup(XcpcMachine* self)
{
}

static void reset_setup(XcpcMachine* self)
{
}

static void construct_state(XcpcMachine* self)
{
    self->state.hsync     = 0; /* no hsync      */
    self->state.vsync     = 0; /* no vsync      */
    self->state.refresh   = 1; /* 50Hz          */
    self->state.company   = 7; /* amstrad       */
    self->state.expansion = 1; /* present       */
    self->state.parallel  = 1; /* not connected */
    self->state.cassette  = 0; /* no data       */
}

static void destruct_state(XcpcMachine* self)
{
}

static void reset_state(XcpcMachine* self)
{
    self->state.hsync     &= 0; /* clear value */
    self->state.vsync     &= 0; /* clear value */
    self->state.refresh   |= 0; /* dont'modify */
    self->state.company   |= 0; /* dont'modify */
    self->state.expansion |= 0; /* dont'modify */
    self->state.parallel  |= 0; /* dont'modify */
    self->state.cassette  &= 0; /* clear value */
}

static void construct_board(XcpcMachine* self)
{
    /* monitor */ {
        const XcpcMonitorIface monitor_iface = {
            self /* user_data */
        };
        if(self->board.monitor == NULL) {
            self->board.monitor = xcpc_monitor_new();
            self->board.monitor = xcpc_monitor_set_iface(self->board.monitor, &monitor_iface);
        }
    }
    /* keyboard */ {
        const XcpcKeyboardIface keyboard_iface = {
            self /* user_data */
        };
        if(self->board.keyboard == NULL) {
            self->board.keyboard = xcpc_keyboard_new();
            self->board.keyboard = xcpc_keyboard_set_iface(self->board.keyboard, &keyboard_iface);
        }
    }
    /* joystick */ {
        const XcpcJoystickIface joystick_iface = {
            self /* user_data */
        };
        if(self->board.joystick == NULL) {
            self->board.joystick = xcpc_joystick_new();
            self->board.joystick = xcpc_joystick_set_iface(self->board.joystick, &joystick_iface);
        }
    }
    /* cpu_z80a */ {
        const XcpcCpuZ80aIface cpu_z80a_iface = {
            self,         /* user_data */
            &cpu_mreq_m1, /* mreq_m1   */
            &cpu_mreq_rd, /* mreq_rd   */
            &cpu_mreq_wr, /* mreq_wr   */
            &cpu_iorq_m1, /* iorq_m1   */
            &cpu_iorq_rd, /* iorq_rd   */
            &cpu_iorq_wr, /* iorq_wr   */
        };
        if(self->board.cpu_z80a == NULL) {
            self->board.cpu_z80a = xcpc_cpu_z80a_new();
            self->board.cpu_z80a = xcpc_cpu_z80a_set_iface(self->board.cpu_z80a, &cpu_z80a_iface);
        }
    }
    /* vga_core */ {
        const XcpcVgaCoreIface vga_core_iface = {
            self /* user_data */
        };
        if(self->board.vga_core == NULL) {
            self->board.vga_core = xcpc_vga_core_new();
            self->board.vga_core = xcpc_vga_core_set_iface(self->board.vga_core, &vga_core_iface);
        }
    }
    /* vdc_6845 */ {
        const XcpcVdc6845Iface vdc_6845_iface = {
            self,       /* user_data */
            &vdc_hsync, /* hsync     */
            &vdc_vsync, /* vsync     */
        };
        if(self->board.vdc_6845 == NULL) {
            self->board.vdc_6845 = xcpc_vdc_6845_new();
            self->board.vdc_6845 = xcpc_vdc_6845_set_iface(self->board.vdc_6845, &vdc_6845_iface);
        }
    }
    /* ppi_8255 */ {
        const XcpcPpi8255Iface ppi_8255_iface = {
            self,           /* user_data */
            &ppi_rd_port_a, /* rd_port_a */
            &ppi_wr_port_a, /* wr_port_a */
            &ppi_rd_port_b, /* rd_port_b */
            &ppi_wr_port_b, /* wr_port_b */
            &ppi_rd_port_c, /* rd_port_c */
            &ppi_wr_port_c, /* wr_port_c */
        };
        if(self->board.ppi_8255 == NULL) {
            self->board.ppi_8255 = xcpc_ppi_8255_new();
            self->board.ppi_8255 = xcpc_ppi_8255_set_iface(self->board.ppi_8255, &ppi_8255_iface);
        }
    }
    /* psg_8910 */ {
        const XcpcPsg8910Iface psg_8910_iface = {
            self,           /* user_data */
            &psg_rd_port_a, /* rd_port_a */
            &psg_wr_port_a, /* wr_port_a */
            &psg_rd_port_b, /* rd_port_b */
            &psg_wr_port_b, /* wr_port_b */
        };
        if(self->board.psg_8910 == NULL) {
            self->board.psg_8910 = xcpc_psg_8910_new();
            self->board.psg_8910 = xcpc_psg_8910_set_iface(self->board.psg_8910, &psg_8910_iface);
        }
    }
    /* fdc_765a */ {
        const XcpcFdc765aIface fdc_765a_iface = {
            self /* user_data */
        };
        if(self->board.fdc_765a == NULL) {
            self->board.fdc_765a = xcpc_fdc_765a_new();
            self->board.fdc_765a = xcpc_fdc_765a_set_iface(self->board.fdc_765a, &fdc_765a_iface);
            (void) xcpc_fdc_765a_attach(self->board.fdc_765a, XCPC_FDC_765A_DRIVE0);
            (void) xcpc_fdc_765a_attach(self->board.fdc_765a, XCPC_FDC_765A_DRIVE1);
        }
    }
    /* ram_bank */ {
        const XcpcRamBankIface ram_bank_iface = {
            self /* user_data */
        };
        int ram_index = 0;
        int ram_count = countof(self->board.ram_bank);
        do {
            if(self->board.ram_bank[ram_index] == NULL) {
                self->board.ram_bank[ram_index] = xcpc_ram_bank_new();
                self->board.ram_bank[ram_index] = xcpc_ram_bank_set_iface(self->board.ram_bank[ram_index], &ram_bank_iface);
            }
        } while(++ram_index < ram_count);
    }
    /* rom_bank */ {
        const XcpcRomBankIface rom_bank_iface = {
            self /* user_data */
        };
        int rom_index = 0;
        int rom_count = countof(self->board.rom_bank);
        do {
            if(self->board.rom_bank[rom_index] == NULL) {
                self->board.rom_bank[rom_index] = xcpc_rom_bank_new();
                self->board.rom_bank[rom_index] = xcpc_rom_bank_set_iface(self->board.rom_bank[rom_index], &rom_bank_iface);
            }
        } while(++rom_index < rom_count);
    }
    /* exp_bank */ {
        int exp_index = 0;
        int exp_count = countof(self->board.exp_bank);
        do {
            if(self->board.exp_bank[exp_index] == NULL) {
                self->board.exp_bank[exp_index] = xcpc_rom_bank_new();
            }
        } while(++exp_index < exp_count);
    }
}

static void destruct_board(XcpcMachine* self)
{
    /* monitor */ {
        if(self->board.monitor != NULL) {
            self->board.monitor = xcpc_monitor_delete(self->board.monitor);
        }
    }
    /* keyboard */ {
        if(self->board.keyboard != NULL) {
            self->board.keyboard = xcpc_keyboard_delete(self->board.keyboard);
        }
    }
    /* joystick */ {
        if(self->board.joystick != NULL) {
            self->board.joystick = xcpc_joystick_delete(self->board.joystick);
        }
    }
    /* cpu_z80a */ {
        if(self->board.cpu_z80a != NULL) {
            self->board.cpu_z80a = xcpc_cpu_z80a_delete(self->board.cpu_z80a);
        }
    }
    /* vga_core */ {
        if(self->board.vga_core != NULL) {
            self->board.vga_core = xcpc_vga_core_delete(self->board.vga_core);
        }
    }
    /* vdc_6845 */ {
        if(self->board.vdc_6845 != NULL) {
            self->board.vdc_6845 = xcpc_vdc_6845_delete(self->board.vdc_6845);
        }
    }
    /* ppi_8255 */ {
        if(self->board.ppi_8255 != NULL) {
            self->board.ppi_8255 = xcpc_ppi_8255_delete(self->board.ppi_8255);
        }
    }
    /* psg_8910 */ {
        if(self->board.psg_8910 != NULL) {
            self->board.psg_8910 = xcpc_psg_8910_delete(self->board.psg_8910);
        }
    }
    /* fdc_765a */ {
        if(self->board.fdc_765a != NULL) {
            self->board.fdc_765a = xcpc_fdc_765a_delete(self->board.fdc_765a);
        }
    }
    /* ram_bank */ {
        int ram_index = 0;
        int ram_count = countof(self->board.ram_bank);
        do {
            if(self->board.ram_bank[ram_index] != NULL) {
                self->board.ram_bank[ram_index] = xcpc_ram_bank_delete(self->board.ram_bank[ram_index]);
            }
        } while(++ram_index < ram_count);
    }
    /* rom_bank */ {
        int rom_index = 0;
        int rom_count = countof(self->board.rom_bank);
        do {
            if(self->board.rom_bank[rom_index] != NULL) {
                self->board.rom_bank[rom_index] = xcpc_rom_bank_delete(self->board.rom_bank[rom_index]);
            }
        } while(++rom_index < rom_count);
    }
    /* exp_bank */ {
        int exp_index = 0;
        int exp_count = countof(self->board.exp_bank);
        do {
            if(self->board.exp_bank[exp_index] != NULL) {
                self->board.exp_bank[exp_index] = xcpc_rom_bank_delete(self->board.exp_bank[exp_index]);
            }
        } while(++exp_index < exp_count);
    }
}

static void reset_board(XcpcMachine* self)
{
    /* monitor */ {
        if(self->board.monitor != NULL) {
            (void) xcpc_monitor_reset(self->board.monitor);
        }
    }
    /* keyboard */ {
        if(self->board.keyboard != NULL) {
            (void) xcpc_keyboard_reset(self->board.keyboard);
        }
    }
    /* joystick */ {
        if(self->board.joystick != NULL) {
            (void) xcpc_joystick_reset(self->board.joystick);
        }
    }
    /* cpu_z80a */ {
        if(self->board.cpu_z80a != NULL) {
            (void) xcpc_cpu_z80a_reset(self->board.cpu_z80a);
        }
    }
    /* vga_core */ {
        if(self->board.vga_core != NULL) {
            (void) xcpc_vga_core_reset(self->board.vga_core);
        }
    }
    /* vdc_6845 */ {
        if(self->board.vdc_6845 != NULL) {
            (void) xcpc_vdc_6845_reset(self->board.vdc_6845);
        }
    }
    /* ppi_8255 */ {
        if(self->board.ppi_8255 != NULL) {
            (void) xcpc_ppi_8255_reset(self->board.ppi_8255);
        }
    }
    /* psg_8910 */ {
        if(self->board.psg_8910 != NULL) {
            (void) xcpc_psg_8910_reset(self->board.psg_8910);
        }
    }
    /* fdc_765a */ {
        if(self->board.fdc_765a != NULL) {
            (void) xcpc_fdc_765a_reset(self->board.fdc_765a);
        }
    }
    /* ram_bank */ {
        int ram_index = 0;
        int ram_count = countof(self->board.ram_bank);
        do {
            if(self->board.ram_bank[ram_index] != NULL) {
                (void) xcpc_ram_bank_reset(self->board.ram_bank[ram_index]);
            }
        } while(++ram_index < ram_count);
    }
    /* rom_bank */ {
        int rom_index = 0;
        int rom_count = countof(self->board.rom_bank);
        do {
            if(self->board.rom_bank[rom_index] != NULL) {
                (void) xcpc_rom_bank_reset(self->board.rom_bank[rom_index]);
            }
        } while(++rom_index < rom_count);
    }
    /* exp_bank */ {
        int exp_index = 0;
        int exp_count = countof(self->board.exp_bank);
        do {
            if(self->board.exp_bank[exp_index] != NULL) {
                (void) xcpc_rom_bank_reset(self->board.exp_bank[exp_index]);
            }
        } while(++exp_index < exp_count);
    }
}

static void construct_pager(XcpcMachine* self)
{
}

static void destruct_pager(XcpcMachine* self)
{
}

static void reset_pager(XcpcMachine* self)
{
    self->pager.bank.rd[0] = self->pager.bank.wr[0] = NULL;
    self->pager.bank.rd[1] = self->pager.bank.wr[1] = NULL;
    self->pager.bank.rd[2] = self->pager.bank.wr[2] = NULL;
    self->pager.bank.rd[3] = self->pager.bank.wr[3] = NULL;
    self->pager.conf.ram = 0x00;
    self->pager.conf.rom = 0x00;
    cpc_mem_select(self);
}

static void construct_frame(XcpcMachine* self)
{
}

static void destruct_frame(XcpcMachine* self)
{
}

static void reset_frame(XcpcMachine* self)
{
}

XcpcMachine* xcpc_machine_alloc(void)
{
    log_trace("alloc");

    return xcpc_new(XcpcMachine);
}

XcpcMachine* xcpc_machine_free(XcpcMachine* self)
{
    log_trace("free");

    return xcpc_delete(XcpcMachine, self);
}

XcpcMachine* xcpc_machine_construct(XcpcMachine* self)
{
    log_trace("construct");

    /* clear all */ {
        (void) memset(&self->iface, 0, sizeof(XcpcMachineIface));
        (void) memset(&self->setup, 0, sizeof(XcpcMachineSetup));
        (void) memset(&self->state, 0, sizeof(XcpcMachineState));
        (void) memset(&self->board, 0, sizeof(XcpcMachineBoard));
        (void) memset(&self->pager, 0, sizeof(XcpcMachinePager));
        (void) memset(&self->frame, 0, sizeof(XcpcMachineFrame));
    }
    /* construct all subsystems */ {
        construct_iface(self);
        construct_setup(self);
        construct_state(self);
        construct_board(self);
        construct_pager(self);
        construct_frame(self);
    }
    /* reset */ {
        (void) xcpc_machine_reset(self);
    }
    return self;
}

XcpcMachine* xcpc_machine_destruct(XcpcMachine* self)
{
    log_trace("destruct");

    /* destruct all subsystems */ {
        destruct_frame(self);
        destruct_pager(self);
        destruct_board(self);
        destruct_state(self);
        destruct_setup(self);
        destruct_iface(self);
    }
    return self;
}

XcpcMachine* xcpc_machine_new(void)
{
    log_trace("new");

    return xcpc_machine_construct(xcpc_machine_alloc());
}

XcpcMachine* xcpc_machine_delete(XcpcMachine* self)
{
    log_trace("delete");

    return xcpc_machine_free(xcpc_machine_destruct(self));
}

XcpcMachine* xcpc_machine_set_iface(XcpcMachine* self, const XcpcMachineIface* iface)
{
    log_trace("set_iface");

    if(iface != NULL) {
        *(&self->iface) = *(iface);
    }
    else {
        self->iface.user_data = self;
    }
    return self;
}

XcpcMachine* xcpc_machine_reset(XcpcMachine* self)
{
    log_trace("reset");

    /* reset all subsystems */ {
        reset_iface(self);
        reset_setup(self);
        reset_state(self);
        reset_board(self);
        reset_pager(self);
        reset_frame(self);
    }
    return self;
}

XcpcMachine* xcpc_machine_clock(XcpcMachine* self)
{
    return self;
}

XcpcMachine* xcpc_machine_insert_drive0(XcpcMachine* self, const char* filename)
{
    log_trace("xcpc_machine_insert_drive0");

    if(self->board.fdc_765a != NULL) {
        (void) xcpc_fdc_765a_insert(self->board.fdc_765a, XCPC_FDC_765A_DRIVE0, filename);
    }
    return self;
}

XcpcMachine* xcpc_machine_remove_drive0(XcpcMachine* self)
{
    log_trace("xcpc_machine_remove_drive0");

    if(self->board.fdc_765a != NULL) {
        (void) xcpc_fdc_765a_remove(self->board.fdc_765a, XCPC_FDC_765A_DRIVE0);
    }
    return self;
}

XcpcMachine* xcpc_machine_insert_drive1(XcpcMachine* self, const char* filename)
{
    log_trace("xcpc_machine_insert_drive1");

    if(self->board.fdc_765a != NULL) {
        (void) xcpc_fdc_765a_insert(self->board.fdc_765a, XCPC_FDC_765A_DRIVE1, filename);
    }
    return self;
}

XcpcMachine* xcpc_machine_remove_drive1(XcpcMachine* self)
{
    log_trace("xcpc_machine_remove_drive1");

    if(self->board.fdc_765a != NULL) {
        (void) xcpc_fdc_765a_remove(self->board.fdc_765a, XCPC_FDC_765A_DRIVE1);
    }
    return self;
}

XcpcMachine* xcpc_machine_load_snapshot(XcpcMachine* self, const char* filename)
{
    log_trace("xcpc_machine_load_snapshot");

    return self;
}

XcpcMachine* xcpc_machine_save_snapshot(XcpcMachine* self, const char* filename)
{
    log_trace("xcpc_machine_save_snapshot");

    return self;
}
