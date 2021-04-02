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
#include <limits.h>
#include <sys/time.h>
#include "machine-priv.h"

static void log_trace(const char* function)
{
    xcpc_log_trace("XcpcMachine::%s()", function);
}

static int is_set(const char* value)
{
    if(value == NULL) {
        return 0;
    }
    if(strcmp(value, "{not-set}") == 0) {
        return 0;
    }
    return 1;
}

static char* build_filename(const char* directory, const char* filename)
{
    char buffer[PATH_MAX + 1];

    (void) snprintf(buffer, sizeof(buffer), "%s/%s/%s", XCPC_RESDIR, directory, filename);

    return strdup(buffer);
}

static void compute_stats(XcpcMachine* self)
{
    struct timeval prev_time = self->timer.profiler;
    struct timeval curr_time = self->timer.profiler;
    unsigned long elapsed_us = 0;

    /* get the current time */ {
        if(gettimeofday(&curr_time, NULL) != 0) {
            xcpc_log_error("gettimeofday() has failed");
        }
    }
    /* compute the elapsed time in us */ {
        const long long t1 = (((long long) prev_time.tv_sec) * 1000000LL) + ((long long) prev_time.tv_usec);
        const long long t2 = (((long long) curr_time.tv_sec) * 1000000LL) + ((long long) curr_time.tv_usec);
        if(t2 >= t1) {
            elapsed_us = ((unsigned long)(t2 - t1));
        }
        else {
            elapsed_us = 0UL;
        }
    }
    /* compute and build the statistics */ {
        if(elapsed_us != 0) {
            const double stats_frames  = (double) (self->stats.drawn * 1000000UL);
            const double stats_elapsed = (double) elapsed_us;
            const double stats_fps     = (stats_frames / stats_elapsed);
            (void) snprintf(self->stats.buffer, sizeof(self->stats.buffer), "refresh = %2d Hz, framerate = %.2f fps", self->stats.rate, stats_fps);
        }
        else {
            (void) snprintf(self->stats.buffer, sizeof(self->stats.buffer), "refresh = %2d Hz", self->stats.rate);
        }
    }
    /* print statistics */ {
        if(self->setup.fps != 0) {
            xcpc_log_print(self->stats.buffer);
        }
    }
    /* set the new reference */ {
        self->timer.profiler = curr_time;
        self->stats.count    = 0;
        self->stats.drawn    = 0;
    }
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
                        scanline->color[index].value  = ink;
                        scanline->color[index].pixel1 = monitor->state.palette1[ink].pixel;
                        scanline->color[index].pixel2 = monitor->state.palette2[ink].pixel;
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
    return data;
}

static uint8_t ppi_wr_port_a(XcpcPpi8255* ppi_8255, uint8_t data)
{
    XcpcMachine* self = ((XcpcMachine*)(ppi_8255->iface.user_data));

    if(self != NULL) {
        log_trace("ppi_wr_port_a");
    }
    return data;
}

static uint8_t ppi_rd_port_b(XcpcPpi8255* ppi_8255, uint8_t data)
{
    XcpcMachine* self = ((XcpcMachine*)(ppi_8255->iface.user_data));

    if(self != NULL) {
        log_trace("ppi_rd_port_b");
    }
    return data;
}

static uint8_t ppi_wr_port_b(XcpcPpi8255* ppi_8255, uint8_t data)
{
    XcpcMachine* self = ((XcpcMachine*)(ppi_8255->iface.user_data));

    if(self != NULL) {
        log_trace("ppi_wr_port_b");
    }
    return data;
}

static uint8_t ppi_rd_port_c(XcpcPpi8255* ppi_8255, uint8_t data)
{
    XcpcMachine* self = ((XcpcMachine*)(ppi_8255->iface.user_data));

    if(self != NULL) {
        log_trace("ppi_rd_port_c");
    }
    return data;
}

static uint8_t ppi_wr_port_c(XcpcPpi8255* ppi_8255, uint8_t data)
{
    XcpcMachine* self = ((XcpcMachine*)(ppi_8255->iface.user_data));

    if(self != NULL) {
        log_trace("ppi_wr_port_c");
    }
    return data;
}

static uint8_t psg_rd_port_a(XcpcPsg8910* psg_8910, uint8_t data)
{
    XcpcMachine* self = ((XcpcMachine*)(psg_8910->iface.user_data));

    if(self != NULL) {
        log_trace("psg_rd_port_a");
    }
    return data;
}

static uint8_t psg_wr_port_a(XcpcPsg8910* psg_8910, uint8_t data)
{
    XcpcMachine* self = ((XcpcMachine*)(psg_8910->iface.user_data));

    if(self != NULL) {
        log_trace("psg_wr_port_a");
    }
    return data;
}

static uint8_t psg_rd_port_b(XcpcPsg8910* psg_8910, uint8_t data)
{
    XcpcMachine* self = ((XcpcMachine*)(psg_8910->iface.user_data));

    if(self != NULL) {
        log_trace("psg_rd_port_b");
    }
    return data;
}

static uint8_t psg_wr_port_b(XcpcPsg8910* psg_8910, uint8_t data)
{
    XcpcMachine* self = ((XcpcMachine*)(psg_8910->iface.user_data));

    if(self != NULL) {
        log_trace("psg_wr_port_b");
    }
    return data;
}

static void paint_default(XcpcMachine* self)
{
}

static void paint_08bpp(XcpcMachine* self)
{
    XcpcMonitor* monitor = self->board.monitor;
    XcpcVdc6845* vdc_6845 = self->board.vdc_6845;
    XcpcVgaCore* vga_core = self->board.vga_core;
    unsigned int sa = ((vdc_6845->state.regs.named.start_address_high << 8) | vdc_6845->state.regs.named.start_address_low);
    unsigned int hd = (vdc_6845->state.regs.named.horizontal_displayed < 48 ? vdc_6845->state.regs.named.horizontal_displayed : 48);
    unsigned int hp = ((XCPC_MONITOR_WIDTH >> 0) - (hd << 4)) >> 1;
    unsigned int mr = vdc_6845->state.regs.named.maximum_scanline_address + 1;
    unsigned int vt = vdc_6845->state.regs.named.vertical_total + 1;
    unsigned int vd = (vdc_6845->state.regs.named.vertical_displayed < 39 ? vdc_6845->state.regs.named.vertical_displayed : 39);
    unsigned int vp = ((XCPC_MONITOR_HEIGHT >> 1) - (vd * mr)) >> 1;
    XcpcScanline* scanline = self->frame.array;
    uint8_t* dst = (uint8_t*) monitor->state.image->data;
    uint8_t* nxt = dst;
    uint8_t pixel1, pixel2;
    unsigned int cx, cy, ra;
    uint16_t addr;
    uint16_t bank;
    uint16_t disp;
    uint8_t data;

    scanline = &self->frame.array[(vt * mr) - (1 * vp)];
    for(cy = 0; cy < vp; cy++) {
        nxt += XCPC_MONITOR_WIDTH;
        pixel1 = scanline->color[16].pixel1;
        pixel2 = scanline->color[16].pixel2;
        for(cx = 0; cx < XCPC_MONITOR_WIDTH; cx++) {
            *dst++ = pixel1;
            *nxt++ = pixel2;
        }
        dst = nxt; scanline++;
    }
    scanline = &self->frame.array[6];
    for(cy = 0; cy < vd; cy++) {
        for(ra = 0; ra < mr; ra++) {
            nxt += XCPC_MONITOR_WIDTH;
            switch(scanline->mode) {
                case 0x00: /* mode 0 */
                    {
                        /* left border */ {
                            pixel1 = scanline->color[16].pixel1;
                            pixel2 = scanline->color[16].pixel2;
                            for(cx = 0; cx < hp; cx++) {
                                *dst++ = pixel1;
                                *nxt++ = pixel2;
                            }
                        }
                        /* active display */ {
                            for(cx = 0; cx < hd; cx++) {
                                /* decode */ {
                                    addr = ((sa & 0x3000) << 2) | ((ra & 0x0007) << 11) | (((sa + cx) & 0x03ff) << 1);
                                    bank = ((addr >> 14) & 0x0003);
                                    disp = ((addr >>  0) & 0x3fff);
                                }
                                /* fetch 1st byte */ {
                                    data = self->board.ram_bank[bank]->state.data[disp | 0];
                                    data = vga_core->state.mode0[data];
                                }
                                /* pixel 0 */ {
                                    pixel1 = scanline->color[data & 0x0f].pixel1;
                                    pixel2 = scanline->color[data & 0x0f].pixel2;
                                    *dst++ = pixel1; *dst++ = pixel1; *dst++ = pixel1; *dst++ = pixel1;
                                    *nxt++ = pixel2; *nxt++ = pixel2; *nxt++ = pixel2; *nxt++ = pixel2;
                                    data >>= 4;
                                }
                                /* pixel 1 */ {
                                    pixel1 = scanline->color[data & 0x0f].pixel1;
                                    pixel2 = scanline->color[data & 0x0f].pixel2;
                                    *dst++ = pixel1; *dst++ = pixel1; *dst++ = pixel1; *dst++ = pixel1;
                                    *nxt++ = pixel2; *nxt++ = pixel2; *nxt++ = pixel2; *nxt++ = pixel2;
                                    data >>= 4;
                                }
                                /* fetch 2nd byte */ {
                                    data = self->board.ram_bank[bank]->state.data[disp | 1];
                                    data = vga_core->state.mode0[data];
                                }
                                /* pixel 0 */ {
                                    pixel1 = scanline->color[data & 0x0f].pixel1;
                                    pixel2 = scanline->color[data & 0x0f].pixel2;
                                    *dst++ = pixel1; *dst++ = pixel1; *dst++ = pixel1; *dst++ = pixel1;
                                    *nxt++ = pixel2; *nxt++ = pixel2; *nxt++ = pixel2; *nxt++ = pixel2;
                                    data >>= 4;
                                }
                                /* pixel 1 */ {
                                    pixel1 = scanline->color[data & 0x0f].pixel1;
                                    pixel2 = scanline->color[data & 0x0f].pixel2;
                                    *dst++ = pixel1; *dst++ = pixel1; *dst++ = pixel1; *dst++ = pixel1;
                                    *nxt++ = pixel2; *nxt++ = pixel2; *nxt++ = pixel2; *nxt++ = pixel2;
                                    data >>= 4;
                                }
                            }
                        }
                        /* right border */ {
                            pixel1 = scanline->color[16].pixel1;
                            pixel2 = scanline->color[16].pixel2;
                            for(cx = 0; cx < hp; cx++) {
                                *dst++ = pixel1;
                                *nxt++ = pixel2;
                            }
                        }
                    }
                    break;
                case 0x01: /* mode 1 */
                    {
                        /* left border */ {
                            pixel1 = scanline->color[16].pixel1;
                            pixel2 = scanline->color[16].pixel2;
                            for(cx = 0; cx < hp; cx++) {
                                *dst++ = pixel1;
                                *nxt++ = pixel2;
                            }
                        }
                        /* active display */ {
                            for(cx = 0; cx < hd; cx++) {
                                /* decode */ {
                                    addr = ((sa & 0x3000) << 2) | ((ra & 0x0007) << 11) | (((sa + cx) & 0x03ff) << 1);
                                    bank = ((addr >> 14) & 0x0003);
                                    disp = ((addr >>  0) & 0x3fff);
                                }
                                /* fetch 1st byte */ {
                                    data = self->board.ram_bank[bank]->state.data[disp | 0];
                                    data = vga_core->state.mode1[data];
                                }
                                /* pixel 0 */ {
                                    pixel1 = scanline->color[data & 0x03].pixel1;
                                    pixel2 = scanline->color[data & 0x03].pixel2;
                                    *dst++ = pixel1; *dst++ = pixel1;
                                    *nxt++ = pixel2; *nxt++ = pixel2;
                                    data >>= 2;
                                }
                                /* pixel 1 */ {
                                    pixel1 = scanline->color[data & 0x03].pixel1;
                                    pixel2 = scanline->color[data & 0x03].pixel2;
                                    *dst++ = pixel1; *dst++ = pixel1;
                                    *nxt++ = pixel2; *nxt++ = pixel2;
                                    data >>= 2;
                                }
                                /* pixel 2 */ {
                                    pixel1 = scanline->color[data & 0x03].pixel1;
                                    pixel2 = scanline->color[data & 0x03].pixel2;
                                    *dst++ = pixel1; *dst++ = pixel1;
                                    *nxt++ = pixel2; *nxt++ = pixel2;
                                    data >>= 2;
                                }
                                /* pixel 3 */ {
                                    pixel1 = scanline->color[data & 0x03].pixel1;
                                    pixel2 = scanline->color[data & 0x03].pixel2;
                                    *dst++ = pixel1; *dst++ = pixel1;
                                    *nxt++ = pixel2; *nxt++ = pixel2;
                                    data >>= 2;
                                }
                                /* fetch 2nd byte */ {
                                    data = self->board.ram_bank[bank]->state.data[disp | 1];
                                    data = vga_core->state.mode1[data];
                                }
                                /* pixel 0 */ {
                                    pixel1 = scanline->color[data & 0x03].pixel1;
                                    pixel2 = scanline->color[data & 0x03].pixel2;
                                    *dst++ = pixel1; *dst++ = pixel1;
                                    *nxt++ = pixel2; *nxt++ = pixel2;
                                    data >>= 2;
                                }
                                /* pixel 1 */ {
                                    pixel1 = scanline->color[data & 0x03].pixel1;
                                    pixel2 = scanline->color[data & 0x03].pixel2;
                                    *dst++ = pixel1; *dst++ = pixel1;
                                    *nxt++ = pixel2; *nxt++ = pixel2;
                                    data >>= 2;
                                }
                                /* pixel 2 */ {
                                    pixel1 = scanline->color[data & 0x03].pixel1;
                                    pixel2 = scanline->color[data & 0x03].pixel2;
                                    *dst++ = pixel1; *dst++ = pixel1;
                                    *nxt++ = pixel2; *nxt++ = pixel2;
                                    data >>= 2;
                                }
                                /* pixel 3 */ {
                                    pixel1 = scanline->color[data & 0x03].pixel1;
                                    pixel2 = scanline->color[data & 0x03].pixel2;
                                    *dst++ = pixel1; *dst++ = pixel1;
                                    *nxt++ = pixel2; *nxt++ = pixel2;
                                    data >>= 2;
                                }
                            }
                        }
                        /* right border */ {
                            pixel1 = scanline->color[16].pixel1;
                            pixel2 = scanline->color[16].pixel2;
                            for(cx = 0; cx < hp; cx++) {
                                *dst++ = pixel1;
                                *nxt++ = pixel2;
                            }
                        }
                    }
                    break;
                case 0x02: /* mode 2 */
                    {
                        /* left border */ {
                            pixel1 = scanline->color[16].pixel1;
                            pixel2 = scanline->color[16].pixel2;
                            for(cx = 0; cx < hp; cx++) {
                                *dst++ = pixel1;
                                *nxt++ = pixel2;
                            }
                        }
                        /* active display */ {
                            for(cx = 0; cx < hd; cx++) {
                                /* decode */ {
                                    addr = ((sa & 0x3000) << 2) | ((ra & 0x0007) << 11) | (((sa + cx) & 0x03ff) << 1);
                                    bank = ((addr >> 14) & 0x0003);
                                    disp = ((addr >>  0) & 0x3fff);
                                }
                                /* fetch 1st byte */ {
                                    data = self->board.ram_bank[bank]->state.data[disp | 0];
                                    data = vga_core->state.mode2[data];
                                }
                                /* pixel 0 */ {
                                    pixel1 = scanline->color[data & 0x01].pixel1;
                                    pixel2 = scanline->color[data & 0x01].pixel2;
                                    *dst++ = pixel1;
                                    *nxt++ = pixel2;
                                    data >>= 1;
                                }
                                /* pixel 1 */ {
                                    pixel1 = scanline->color[data & 0x01].pixel1;
                                    pixel2 = scanline->color[data & 0x01].pixel2;
                                    *dst++ = pixel1;
                                    *nxt++ = pixel2;
                                    data >>= 1;
                                }
                                /* pixel 2 */ {
                                    pixel1 = scanline->color[data & 0x01].pixel1;
                                    pixel2 = scanline->color[data & 0x01].pixel2;
                                    *dst++ = pixel1;
                                    *nxt++ = pixel2;
                                    data >>= 1;
                                }
                                /* pixel 3 */ {
                                    pixel1 = scanline->color[data & 0x01].pixel1;
                                    pixel2 = scanline->color[data & 0x01].pixel2;
                                    *dst++ = pixel1;
                                    *nxt++ = pixel2;
                                    data >>= 1;
                                }
                                /* pixel 4 */ {
                                    pixel1 = scanline->color[data & 0x01].pixel1;
                                    pixel2 = scanline->color[data & 0x01].pixel2;
                                    *dst++ = pixel1;
                                    *nxt++ = pixel2;
                                    data >>= 1;
                                }
                                /* pixel 5 */ {
                                    pixel1 = scanline->color[data & 0x01].pixel1;
                                    pixel2 = scanline->color[data & 0x01].pixel2;
                                    *dst++ = pixel1;
                                    *nxt++ = pixel2;
                                    data >>= 1;
                                }
                                /* pixel 6 */ {
                                    pixel1 = scanline->color[data & 0x01].pixel1;
                                    pixel2 = scanline->color[data & 0x01].pixel2;
                                    *dst++ = pixel1;
                                    *nxt++ = pixel2;
                                    data >>= 1;
                                }
                                /* pixel 7 */ {
                                    pixel1 = scanline->color[data & 0x01].pixel1;
                                    pixel2 = scanline->color[data & 0x01].pixel2;
                                    *dst++ = pixel1;
                                    *nxt++ = pixel2;
                                    data >>= 1;
                                }
                                /* fetch 2nd byte */ {
                                    data = self->board.ram_bank[bank]->state.data[disp | 1];
                                    data = vga_core->state.mode2[data];
                                }
                                /* pixel 0 */ {
                                    pixel1 = scanline->color[data & 0x01].pixel1;
                                    pixel2 = scanline->color[data & 0x01].pixel2;
                                    *dst++ = pixel1;
                                    *nxt++ = pixel2;
                                    data >>= 1;
                                }
                                /* pixel 1 */ {
                                    pixel1 = scanline->color[data & 0x01].pixel1;
                                    pixel2 = scanline->color[data & 0x01].pixel2;
                                    *dst++ = pixel1;
                                    *nxt++ = pixel2;
                                    data >>= 1;
                                }
                                /* pixel 2 */ {
                                    pixel1 = scanline->color[data & 0x01].pixel1;
                                    pixel2 = scanline->color[data & 0x01].pixel2;
                                    *dst++ = pixel1;
                                    *nxt++ = pixel2;
                                    data >>= 1;
                                }
                                /* pixel 3 */ {
                                    pixel1 = scanline->color[data & 0x01].pixel1;
                                    pixel2 = scanline->color[data & 0x01].pixel2;
                                    *dst++ = pixel1;
                                    *nxt++ = pixel2;
                                    data >>= 1;
                                }
                                /* pixel 4 */ {
                                    pixel1 = scanline->color[data & 0x01].pixel1;
                                    pixel2 = scanline->color[data & 0x01].pixel2;
                                    *dst++ = pixel1;
                                    *nxt++ = pixel2;
                                    data >>= 1;
                                }
                                /* pixel 5 */ {
                                    pixel1 = scanline->color[data & 0x01].pixel1;
                                    pixel2 = scanline->color[data & 0x01].pixel2;
                                    *dst++ = pixel1;
                                    *nxt++ = pixel2;
                                    data >>= 1;
                                }
                                /* pixel 6 */ {
                                    pixel1 = scanline->color[data & 0x01].pixel1;
                                    pixel2 = scanline->color[data & 0x01].pixel2;
                                    *dst++ = pixel1;
                                    *nxt++ = pixel2;
                                    data >>= 1;
                                }
                                /* pixel 7 */ {
                                    pixel1 = scanline->color[data & 0x01].pixel1;
                                    pixel2 = scanline->color[data & 0x01].pixel2;
                                    *dst++ = pixel1;
                                    *nxt++ = pixel2;
                                    data >>= 1;
                                }
                            }
                        }
                        /* right border */ {
                            pixel1 = scanline->color[16].pixel1;
                            pixel2 = scanline->color[16].pixel2;
                            for(cx = 0; cx < hp; cx++) {
                                *dst++ = pixel1;
                                *nxt++ = pixel2;
                            }
                        }
                    }
                    break;
            }
            dst = nxt; scanline++;
        }
        sa += hd;
    }
    scanline = &self->frame.array[(vd * mr) + (0 * vp)];
    for(cy = 0; cy < vp; cy++) {
        nxt += XCPC_MONITOR_WIDTH;
        pixel1 = scanline->color[16].pixel1;
        pixel2 = scanline->color[16].pixel2;
        for(cx = 0; cx < XCPC_MONITOR_WIDTH; cx++) {
            *dst++ = pixel1;
            *nxt++ = pixel2;
        }
        dst = nxt; scanline++;
    }
    (void) xcpc_monitor_put_image(self->board.monitor);
}

static void paint_16bpp(XcpcMachine* self)
{
    XcpcMonitor* monitor = self->board.monitor;
    XcpcVdc6845* vdc_6845 = self->board.vdc_6845;
    XcpcVgaCore* vga_core = self->board.vga_core;
    unsigned int sa = ((vdc_6845->state.regs.named.start_address_high << 8) | vdc_6845->state.regs.named.start_address_low);
    unsigned int hd = (vdc_6845->state.regs.named.horizontal_displayed < 48 ? vdc_6845->state.regs.named.horizontal_displayed : 48);
    unsigned int hp = ((XCPC_MONITOR_WIDTH >> 0) - (hd << 4)) >> 1;
    unsigned int mr = vdc_6845->state.regs.named.maximum_scanline_address + 1;
    unsigned int vt = vdc_6845->state.regs.named.vertical_total + 1;
    unsigned int vd = (vdc_6845->state.regs.named.vertical_displayed < 39 ? vdc_6845->state.regs.named.vertical_displayed : 39);
    unsigned int vp = ((XCPC_MONITOR_HEIGHT >> 1) - (vd * mr)) >> 1;
    XcpcScanline* scanline = self->frame.array;
    uint16_t* dst = (uint16_t*) monitor->state.image->data;
    uint16_t* nxt = dst;
    uint16_t pixel1, pixel2;
    unsigned int cx, cy, ra;
    uint16_t addr;
    uint16_t bank;
    uint16_t disp;
    uint8_t data;

    scanline = &self->frame.array[(vt * mr) - (1 * vp)];
    for(cy = 0; cy < vp; cy++) {
        nxt += XCPC_MONITOR_WIDTH;
        pixel1 = scanline->color[16].pixel1;
        pixel2 = scanline->color[16].pixel2;
        for(cx = 0; cx < XCPC_MONITOR_WIDTH; cx++) {
            *dst++ = pixel1;
            *nxt++ = pixel2;
        }
        dst = nxt; scanline++;
    }
    scanline = &self->frame.array[6];
    for(cy = 0; cy < vd; cy++) {
        for(ra = 0; ra < mr; ra++) {
            nxt += XCPC_MONITOR_WIDTH;
            switch(scanline->mode) {
                case 0x00: /* mode 0 */
                    {
                        /* left border */ {
                            pixel1 = scanline->color[16].pixel1;
                            pixel2 = scanline->color[16].pixel2;
                            for(cx = 0; cx < hp; cx++) {
                                *dst++ = pixel1;
                                *nxt++ = pixel2;
                            }
                        }
                        /* active display */ {
                            for(cx = 0; cx < hd; cx++) {
                                /* decode */ {
                                    addr = ((sa & 0x3000) << 2) | ((ra & 0x0007) << 11) | (((sa + cx) & 0x03ff) << 1);
                                    bank = ((addr >> 14) & 0x0003);
                                    disp = ((addr >>  0) & 0x3fff);
                                }
                                /* fetch 1st byte */ {
                                    data = self->board.ram_bank[bank]->state.data[disp | 0];
                                    data = vga_core->state.mode0[data];
                                }
                                /* pixel 0 */ {
                                    pixel1 = scanline->color[data & 0x0f].pixel1;
                                    pixel2 = scanline->color[data & 0x0f].pixel2;
                                    *dst++ = pixel1; *dst++ = pixel1; *dst++ = pixel1; *dst++ = pixel1;
                                    *nxt++ = pixel2; *nxt++ = pixel2; *nxt++ = pixel2; *nxt++ = pixel2;
                                    data >>= 4;
                                }
                                /* pixel 1 */ {
                                    pixel1 = scanline->color[data & 0x0f].pixel1;
                                    pixel2 = scanline->color[data & 0x0f].pixel2;
                                    *dst++ = pixel1; *dst++ = pixel1; *dst++ = pixel1; *dst++ = pixel1;
                                    *nxt++ = pixel2; *nxt++ = pixel2; *nxt++ = pixel2; *nxt++ = pixel2;
                                    data >>= 4;
                                }
                                /* fetch 2nd byte */ {
                                    data = self->board.ram_bank[bank]->state.data[disp | 1];
                                    data = vga_core->state.mode0[data];
                                }
                                /* pixel 0 */ {
                                    pixel1 = scanline->color[data & 0x0f].pixel1;
                                    pixel2 = scanline->color[data & 0x0f].pixel2;
                                    *dst++ = pixel1; *dst++ = pixel1; *dst++ = pixel1; *dst++ = pixel1;
                                    *nxt++ = pixel2; *nxt++ = pixel2; *nxt++ = pixel2; *nxt++ = pixel2;
                                    data >>= 4;
                                }
                                /* pixel 1 */ {
                                    pixel1 = scanline->color[data & 0x0f].pixel1;
                                    pixel2 = scanline->color[data & 0x0f].pixel2;
                                    *dst++ = pixel1; *dst++ = pixel1; *dst++ = pixel1; *dst++ = pixel1;
                                    *nxt++ = pixel2; *nxt++ = pixel2; *nxt++ = pixel2; *nxt++ = pixel2;
                                    data >>= 4;
                                }
                            }
                        }
                        /* right border */ {
                            pixel1 = scanline->color[16].pixel1;
                            pixel2 = scanline->color[16].pixel2;
                            for(cx = 0; cx < hp; cx++) {
                                *dst++ = pixel1;
                                *nxt++ = pixel2;
                            }
                        }
                    }
                    break;
                case 0x01: /* mode 1 */
                    {
                        /* left border */ {
                            pixel1 = scanline->color[16].pixel1;
                            pixel2 = scanline->color[16].pixel2;
                            for(cx = 0; cx < hp; cx++) {
                                *dst++ = pixel1;
                                *nxt++ = pixel2;
                            }
                        }
                        /* active display */ {
                            for(cx = 0; cx < hd; cx++) {
                                /* decode */ {
                                    addr = ((sa & 0x3000) << 2) | ((ra & 0x0007) << 11) | (((sa + cx) & 0x03ff) << 1);
                                    bank = ((addr >> 14) & 0x0003);
                                    disp = ((addr >>  0) & 0x3fff);
                                }
                                /* fetch 1st byte */ {
                                    data = self->board.ram_bank[bank]->state.data[disp | 0];
                                    data = vga_core->state.mode1[data];
                                }
                                /* pixel 0 */ {
                                    pixel1 = scanline->color[data & 0x03].pixel1;
                                    pixel2 = scanline->color[data & 0x03].pixel2;
                                    *dst++ = pixel1; *dst++ = pixel1;
                                    *nxt++ = pixel2; *nxt++ = pixel2;
                                    data >>= 2;
                                }
                                /* pixel 1 */ {
                                    pixel1 = scanline->color[data & 0x03].pixel1;
                                    pixel2 = scanline->color[data & 0x03].pixel2;
                                    *dst++ = pixel1; *dst++ = pixel1;
                                    *nxt++ = pixel2; *nxt++ = pixel2;
                                    data >>= 2;
                                }
                                /* pixel 2 */ {
                                    pixel1 = scanline->color[data & 0x03].pixel1;
                                    pixel2 = scanline->color[data & 0x03].pixel2;
                                    *dst++ = pixel1; *dst++ = pixel1;
                                    *nxt++ = pixel2; *nxt++ = pixel2;
                                    data >>= 2;
                                }
                                /* pixel 3 */ {
                                    pixel1 = scanline->color[data & 0x03].pixel1;
                                    pixel2 = scanline->color[data & 0x03].pixel2;
                                    *dst++ = pixel1; *dst++ = pixel1;
                                    *nxt++ = pixel2; *nxt++ = pixel2;
                                    data >>= 2;
                                }
                                /* fetch 2nd byte */ {
                                    data = self->board.ram_bank[bank]->state.data[disp | 1];
                                    data = vga_core->state.mode1[data];
                                }
                                /* pixel 0 */ {
                                    pixel1 = scanline->color[data & 0x03].pixel1;
                                    pixel2 = scanline->color[data & 0x03].pixel2;
                                    *dst++ = pixel1; *dst++ = pixel1;
                                    *nxt++ = pixel2; *nxt++ = pixel2;
                                    data >>= 2;
                                }
                                /* pixel 1 */ {
                                    pixel1 = scanline->color[data & 0x03].pixel1;
                                    pixel2 = scanline->color[data & 0x03].pixel2;
                                    *dst++ = pixel1; *dst++ = pixel1;
                                    *nxt++ = pixel2; *nxt++ = pixel2;
                                    data >>= 2;
                                }
                                /* pixel 2 */ {
                                    pixel1 = scanline->color[data & 0x03].pixel1;
                                    pixel2 = scanline->color[data & 0x03].pixel2;
                                    *dst++ = pixel1; *dst++ = pixel1;
                                    *nxt++ = pixel2; *nxt++ = pixel2;
                                    data >>= 2;
                                }
                                /* pixel 3 */ {
                                    pixel1 = scanline->color[data & 0x03].pixel1;
                                    pixel2 = scanline->color[data & 0x03].pixel2;
                                    *dst++ = pixel1; *dst++ = pixel1;
                                    *nxt++ = pixel2; *nxt++ = pixel2;
                                    data >>= 2;
                                }
                            }
                        }
                        /* right border */ {
                            pixel1 = scanline->color[16].pixel1;
                            pixel2 = scanline->color[16].pixel2;
                            for(cx = 0; cx < hp; cx++) {
                                *dst++ = pixel1;
                                *nxt++ = pixel2;
                            }
                        }
                    }
                    break;
                case 0x02: /* mode 2 */
                    {
                        /* left border */ {
                            pixel1 = scanline->color[16].pixel1;
                            pixel2 = scanline->color[16].pixel2;
                            for(cx = 0; cx < hp; cx++) {
                                *dst++ = pixel1;
                                *nxt++ = pixel2;
                            }
                        }
                        /* active display */ {
                            for(cx = 0; cx < hd; cx++) {
                                /* decode */ {
                                    addr = ((sa & 0x3000) << 2) | ((ra & 0x0007) << 11) | (((sa + cx) & 0x03ff) << 1);
                                    bank = ((addr >> 14) & 0x0003);
                                    disp = ((addr >>  0) & 0x3fff);
                                }
                                /* fetch 1st byte */ {
                                    data = self->board.ram_bank[bank]->state.data[disp | 0];
                                    data = vga_core->state.mode2[data];
                                }
                                /* pixel 0 */ {
                                    pixel1 = scanline->color[data & 0x01].pixel1;
                                    pixel2 = scanline->color[data & 0x01].pixel2;
                                    *dst++ = pixel1;
                                    *nxt++ = pixel2;
                                    data >>= 1;
                                }
                                /* pixel 1 */ {
                                    pixel1 = scanline->color[data & 0x01].pixel1;
                                    pixel2 = scanline->color[data & 0x01].pixel2;
                                    *dst++ = pixel1;
                                    *nxt++ = pixel2;
                                    data >>= 1;
                                }
                                /* pixel 2 */ {
                                    pixel1 = scanline->color[data & 0x01].pixel1;
                                    pixel2 = scanline->color[data & 0x01].pixel2;
                                    *dst++ = pixel1;
                                    *nxt++ = pixel2;
                                    data >>= 1;
                                }
                                /* pixel 3 */ {
                                    pixel1 = scanline->color[data & 0x01].pixel1;
                                    pixel2 = scanline->color[data & 0x01].pixel2;
                                    *dst++ = pixel1;
                                    *nxt++ = pixel2;
                                    data >>= 1;
                                }
                                /* pixel 4 */ {
                                    pixel1 = scanline->color[data & 0x01].pixel1;
                                    pixel2 = scanline->color[data & 0x01].pixel2;
                                    *dst++ = pixel1;
                                    *nxt++ = pixel2;
                                    data >>= 1;
                                }
                                /* pixel 5 */ {
                                    pixel1 = scanline->color[data & 0x01].pixel1;
                                    pixel2 = scanline->color[data & 0x01].pixel2;
                                    *dst++ = pixel1;
                                    *nxt++ = pixel2;
                                    data >>= 1;
                                }
                                /* pixel 6 */ {
                                    pixel1 = scanline->color[data & 0x01].pixel1;
                                    pixel2 = scanline->color[data & 0x01].pixel2;
                                    *dst++ = pixel1;
                                    *nxt++ = pixel2;
                                    data >>= 1;
                                }
                                /* pixel 7 */ {
                                    pixel1 = scanline->color[data & 0x01].pixel1;
                                    pixel2 = scanline->color[data & 0x01].pixel2;
                                    *dst++ = pixel1;
                                    *nxt++ = pixel2;
                                    data >>= 1;
                                }
                                /* fetch 2nd byte */ {
                                    data = self->board.ram_bank[bank]->state.data[disp | 1];
                                    data = vga_core->state.mode2[data];
                                }
                                /* pixel 0 */ {
                                    pixel1 = scanline->color[data & 0x01].pixel1;
                                    pixel2 = scanline->color[data & 0x01].pixel2;
                                    *dst++ = pixel1;
                                    *nxt++ = pixel2;
                                    data >>= 1;
                                }
                                /* pixel 1 */ {
                                    pixel1 = scanline->color[data & 0x01].pixel1;
                                    pixel2 = scanline->color[data & 0x01].pixel2;
                                    *dst++ = pixel1;
                                    *nxt++ = pixel2;
                                    data >>= 1;
                                }
                                /* pixel 2 */ {
                                    pixel1 = scanline->color[data & 0x01].pixel1;
                                    pixel2 = scanline->color[data & 0x01].pixel2;
                                    *dst++ = pixel1;
                                    *nxt++ = pixel2;
                                    data >>= 1;
                                }
                                /* pixel 3 */ {
                                    pixel1 = scanline->color[data & 0x01].pixel1;
                                    pixel2 = scanline->color[data & 0x01].pixel2;
                                    *dst++ = pixel1;
                                    *nxt++ = pixel2;
                                    data >>= 1;
                                }
                                /* pixel 4 */ {
                                    pixel1 = scanline->color[data & 0x01].pixel1;
                                    pixel2 = scanline->color[data & 0x01].pixel2;
                                    *dst++ = pixel1;
                                    *nxt++ = pixel2;
                                    data >>= 1;
                                }
                                /* pixel 5 */ {
                                    pixel1 = scanline->color[data & 0x01].pixel1;
                                    pixel2 = scanline->color[data & 0x01].pixel2;
                                    *dst++ = pixel1;
                                    *nxt++ = pixel2;
                                    data >>= 1;
                                }
                                /* pixel 6 */ {
                                    pixel1 = scanline->color[data & 0x01].pixel1;
                                    pixel2 = scanline->color[data & 0x01].pixel2;
                                    *dst++ = pixel1;
                                    *nxt++ = pixel2;
                                    data >>= 1;
                                }
                                /* pixel 7 */ {
                                    pixel1 = scanline->color[data & 0x01].pixel1;
                                    pixel2 = scanline->color[data & 0x01].pixel2;
                                    *dst++ = pixel1;
                                    *nxt++ = pixel2;
                                    data >>= 1;
                                }
                            }
                        }
                        /* right border */ {
                            pixel1 = scanline->color[16].pixel1;
                            pixel2 = scanline->color[16].pixel2;
                            for(cx = 0; cx < hp; cx++) {
                                *dst++ = pixel1;
                                *nxt++ = pixel2;
                            }
                        }
                    }
                    break;
            }
            dst = nxt; scanline++;
        }
        sa += hd;
    }
    scanline = &self->frame.array[(vd * mr) + (0 * vp)];
    for(cy = 0; cy < vp; cy++) {
        nxt += XCPC_MONITOR_WIDTH;
        pixel1 = scanline->color[16].pixel1;
        pixel2 = scanline->color[16].pixel2;
        for(cx = 0; cx < XCPC_MONITOR_WIDTH; cx++) {
            *dst++ = pixel1;
            *nxt++ = pixel2;
        }
        dst = nxt; scanline++;
    }
    (void) xcpc_monitor_put_image(self->board.monitor);
}

static void paint_32bpp(XcpcMachine* self)
{
    XcpcMonitor* monitor = self->board.monitor;
    XcpcVdc6845* vdc_6845 = self->board.vdc_6845;
    XcpcVgaCore* vga_core = self->board.vga_core;
    unsigned int sa = ((vdc_6845->state.regs.named.start_address_high << 8) | vdc_6845->state.regs.named.start_address_low);
    unsigned int hd = (vdc_6845->state.regs.named.horizontal_displayed < 48 ? vdc_6845->state.regs.named.horizontal_displayed : 48);
    unsigned int hp = ((XCPC_MONITOR_WIDTH >> 0) - (hd << 4)) >> 1;
    unsigned int mr = vdc_6845->state.regs.named.maximum_scanline_address + 1;
    unsigned int vt = vdc_6845->state.regs.named.vertical_total + 1;
    unsigned int vd = (vdc_6845->state.regs.named.vertical_displayed < 39 ? vdc_6845->state.regs.named.vertical_displayed : 39);
    unsigned int vp = ((XCPC_MONITOR_HEIGHT >> 1) - (vd * mr)) >> 1;
    XcpcScanline* scanline = self->frame.array;
    uint32_t* dst = (uint32_t*) monitor->state.image->data;
    uint32_t* nxt = dst;
    uint32_t pixel1, pixel2;
    unsigned int cx, cy, ra;
    uint16_t addr;
    uint16_t bank;
    uint16_t disp;
    uint8_t data;

    scanline = &self->frame.array[(vt * mr) - (1 * vp)];
    for(cy = 0; cy < vp; cy++) {
        nxt += XCPC_MONITOR_WIDTH;
        pixel1 = scanline->color[16].pixel1;
        pixel2 = scanline->color[16].pixel2;
        for(cx = 0; cx < XCPC_MONITOR_WIDTH; cx++) {
            *dst++ = pixel1;
            *nxt++ = pixel2;
        }
        dst = nxt; scanline++;
    }
    scanline = &self->frame.array[6];
    for(cy = 0; cy < vd; cy++) {
        for(ra = 0; ra < mr; ra++) {
            nxt += XCPC_MONITOR_WIDTH;
            switch(scanline->mode) {
                case 0x00: /* mode 0 */
                    {
                        /* left border */ {
                            pixel1 = scanline->color[16].pixel1;
                            pixel2 = scanline->color[16].pixel2;
                            for(cx = 0; cx < hp; cx++) {
                                *dst++ = pixel1;
                                *nxt++ = pixel2;
                            }
                        }
                        /* active display */ {
                            for(cx = 0; cx < hd; cx++) {
                                /* decode */ {
                                    addr = ((sa & 0x3000) << 2) | ((ra & 0x0007) << 11) | (((sa + cx) & 0x03ff) << 1);
                                    bank = ((addr >> 14) & 0x0003);
                                    disp = ((addr >>  0) & 0x3fff);
                                }
                                /* fetch 1st byte */ {
                                    data = self->board.ram_bank[bank]->state.data[disp | 0];
                                    data = vga_core->state.mode0[data];
                                }
                                /* pixel 0 */ {
                                    pixel1 = scanline->color[data & 0x0f].pixel1;
                                    pixel2 = scanline->color[data & 0x0f].pixel2;
                                    *dst++ = pixel1; *dst++ = pixel1; *dst++ = pixel1; *dst++ = pixel1;
                                    *nxt++ = pixel2; *nxt++ = pixel2; *nxt++ = pixel2; *nxt++ = pixel2;
                                    data >>= 4;
                                }
                                /* pixel 1 */ {
                                    pixel1 = scanline->color[data & 0x0f].pixel1;
                                    pixel2 = scanline->color[data & 0x0f].pixel2;
                                    *dst++ = pixel1; *dst++ = pixel1; *dst++ = pixel1; *dst++ = pixel1;
                                    *nxt++ = pixel2; *nxt++ = pixel2; *nxt++ = pixel2; *nxt++ = pixel2;
                                    data >>= 4;
                                }
                                /* fetch 2nd byte */ {
                                    data = self->board.ram_bank[bank]->state.data[disp | 1];
                                    data = vga_core->state.mode0[data];
                                }
                                /* pixel 0 */ {
                                    pixel1 = scanline->color[data & 0x0f].pixel1;
                                    pixel2 = scanline->color[data & 0x0f].pixel2;
                                    *dst++ = pixel1; *dst++ = pixel1; *dst++ = pixel1; *dst++ = pixel1;
                                    *nxt++ = pixel2; *nxt++ = pixel2; *nxt++ = pixel2; *nxt++ = pixel2;
                                    data >>= 4;
                                }
                                /* pixel 1 */ {
                                    pixel1 = scanline->color[data & 0x0f].pixel1;
                                    pixel2 = scanline->color[data & 0x0f].pixel2;
                                    *dst++ = pixel1; *dst++ = pixel1; *dst++ = pixel1; *dst++ = pixel1;
                                    *nxt++ = pixel2; *nxt++ = pixel2; *nxt++ = pixel2; *nxt++ = pixel2;
                                    data >>= 4;
                                }
                            }
                        }
                        /* right border */ {
                            pixel1 = scanline->color[16].pixel1;
                            pixel2 = scanline->color[16].pixel2;
                            for(cx = 0; cx < hp; cx++) {
                                *dst++ = pixel1;
                                *nxt++ = pixel2;
                            }
                        }
                    }
                    break;
                case 0x01: /* mode 1 */
                    {
                        /* left border */ {
                            pixel1 = scanline->color[16].pixel1;
                            pixel2 = scanline->color[16].pixel2;
                            for(cx = 0; cx < hp; cx++) {
                                *dst++ = pixel1;
                                *nxt++ = pixel2;
                            }
                        }
                        /* active display */ {
                            for(cx = 0; cx < hd; cx++) {
                                /* decode */ {
                                    addr = ((sa & 0x3000) << 2) | ((ra & 0x0007) << 11) | (((sa + cx) & 0x03ff) << 1);
                                    bank = ((addr >> 14) & 0x0003);
                                    disp = ((addr >>  0) & 0x3fff);
                                }
                                /* fetch 1st byte */ {
                                    data = self->board.ram_bank[bank]->state.data[disp | 0];
                                    data = vga_core->state.mode1[data];
                                }
                                /* pixel 0 */ {
                                    pixel1 = scanline->color[data & 0x03].pixel1;
                                    pixel2 = scanline->color[data & 0x03].pixel2;
                                    *dst++ = pixel1; *dst++ = pixel1;
                                    *nxt++ = pixel2; *nxt++ = pixel2;
                                    data >>= 2;
                                }
                                /* pixel 1 */ {
                                    pixel1 = scanline->color[data & 0x03].pixel1;
                                    pixel2 = scanline->color[data & 0x03].pixel2;
                                    *dst++ = pixel1; *dst++ = pixel1;
                                    *nxt++ = pixel2; *nxt++ = pixel2;
                                    data >>= 2;
                                }
                                /* pixel 2 */ {
                                    pixel1 = scanline->color[data & 0x03].pixel1;
                                    pixel2 = scanline->color[data & 0x03].pixel2;
                                    *dst++ = pixel1; *dst++ = pixel1;
                                    *nxt++ = pixel2; *nxt++ = pixel2;
                                    data >>= 2;
                                }
                                /* pixel 3 */ {
                                    pixel1 = scanline->color[data & 0x03].pixel1;
                                    pixel2 = scanline->color[data & 0x03].pixel2;
                                    *dst++ = pixel1; *dst++ = pixel1;
                                    *nxt++ = pixel2; *nxt++ = pixel2;
                                    data >>= 2;
                                }
                                /* fetch 2nd byte */ {
                                    data = self->board.ram_bank[bank]->state.data[disp | 1];
                                    data = vga_core->state.mode1[data];
                                }
                                /* pixel 0 */ {
                                    pixel1 = scanline->color[data & 0x03].pixel1;
                                    pixel2 = scanline->color[data & 0x03].pixel2;
                                    *dst++ = pixel1; *dst++ = pixel1;
                                    *nxt++ = pixel2; *nxt++ = pixel2;
                                    data >>= 2;
                                }
                                /* pixel 1 */ {
                                    pixel1 = scanline->color[data & 0x03].pixel1;
                                    pixel2 = scanline->color[data & 0x03].pixel2;
                                    *dst++ = pixel1; *dst++ = pixel1;
                                    *nxt++ = pixel2; *nxt++ = pixel2;
                                    data >>= 2;
                                }
                                /* pixel 2 */ {
                                    pixel1 = scanline->color[data & 0x03].pixel1;
                                    pixel2 = scanline->color[data & 0x03].pixel2;
                                    *dst++ = pixel1; *dst++ = pixel1;
                                    *nxt++ = pixel2; *nxt++ = pixel2;
                                    data >>= 2;
                                }
                                /* pixel 3 */ {
                                    pixel1 = scanline->color[data & 0x03].pixel1;
                                    pixel2 = scanline->color[data & 0x03].pixel2;
                                    *dst++ = pixel1; *dst++ = pixel1;
                                    *nxt++ = pixel2; *nxt++ = pixel2;
                                    data >>= 2;
                                }
                            }
                        }
                        /* right border */ {
                            pixel1 = scanline->color[16].pixel1;
                            pixel2 = scanline->color[16].pixel2;
                            for(cx = 0; cx < hp; cx++) {
                                *dst++ = pixel1;
                                *nxt++ = pixel2;
                            }
                        }
                    }
                    break;
                case 0x02: /* mode 2 */
                    {
                        /* left border */ {
                            pixel1 = scanline->color[16].pixel1;
                            pixel2 = scanline->color[16].pixel2;
                            for(cx = 0; cx < hp; cx++) {
                                *dst++ = pixel1;
                                *nxt++ = pixel2;
                            }
                        }
                        /* active display */ {
                            for(cx = 0; cx < hd; cx++) {
                                /* decode */ {
                                    addr = ((sa & 0x3000) << 2) | ((ra & 0x0007) << 11) | (((sa + cx) & 0x03ff) << 1);
                                    bank = ((addr >> 14) & 0x0003);
                                    disp = ((addr >>  0) & 0x3fff);
                                }
                                /* fetch 1st byte */ {
                                    data = self->board.ram_bank[bank]->state.data[disp | 0];
                                    data = vga_core->state.mode2[data];
                                }
                                /* pixel 0 */ {
                                    pixel1 = scanline->color[data & 0x01].pixel1;
                                    pixel2 = scanline->color[data & 0x01].pixel2;
                                    *dst++ = pixel1;
                                    *nxt++ = pixel2;
                                    data >>= 1;
                                }
                                /* pixel 1 */ {
                                    pixel1 = scanline->color[data & 0x01].pixel1;
                                    pixel2 = scanline->color[data & 0x01].pixel2;
                                    *dst++ = pixel1;
                                    *nxt++ = pixel2;
                                    data >>= 1;
                                }
                                /* pixel 2 */ {
                                    pixel1 = scanline->color[data & 0x01].pixel1;
                                    pixel2 = scanline->color[data & 0x01].pixel2;
                                    *dst++ = pixel1;
                                    *nxt++ = pixel2;
                                    data >>= 1;
                                }
                                /* pixel 3 */ {
                                    pixel1 = scanline->color[data & 0x01].pixel1;
                                    pixel2 = scanline->color[data & 0x01].pixel2;
                                    *dst++ = pixel1;
                                    *nxt++ = pixel2;
                                    data >>= 1;
                                }
                                /* pixel 4 */ {
                                    pixel1 = scanline->color[data & 0x01].pixel1;
                                    pixel2 = scanline->color[data & 0x01].pixel2;
                                    *dst++ = pixel1;
                                    *nxt++ = pixel2;
                                    data >>= 1;
                                }
                                /* pixel 5 */ {
                                    pixel1 = scanline->color[data & 0x01].pixel1;
                                    pixel2 = scanline->color[data & 0x01].pixel2;
                                    *dst++ = pixel1;
                                    *nxt++ = pixel2;
                                    data >>= 1;
                                }
                                /* pixel 6 */ {
                                    pixel1 = scanline->color[data & 0x01].pixel1;
                                    pixel2 = scanline->color[data & 0x01].pixel2;
                                    *dst++ = pixel1;
                                    *nxt++ = pixel2;
                                    data >>= 1;
                                }
                                /* pixel 7 */ {
                                    pixel1 = scanline->color[data & 0x01].pixel1;
                                    pixel2 = scanline->color[data & 0x01].pixel2;
                                    *dst++ = pixel1;
                                    *nxt++ = pixel2;
                                    data >>= 1;
                                }
                                /* fetch 2nd byte */ {
                                    data = self->board.ram_bank[bank]->state.data[disp | 1];
                                    data = vga_core->state.mode2[data];
                                }
                                /* pixel 0 */ {
                                    pixel1 = scanline->color[data & 0x01].pixel1;
                                    pixel2 = scanline->color[data & 0x01].pixel2;
                                    *dst++ = pixel1;
                                    *nxt++ = pixel2;
                                    data >>= 1;
                                }
                                /* pixel 1 */ {
                                    pixel1 = scanline->color[data & 0x01].pixel1;
                                    pixel2 = scanline->color[data & 0x01].pixel2;
                                    *dst++ = pixel1;
                                    *nxt++ = pixel2;
                                    data >>= 1;
                                }
                                /* pixel 2 */ {
                                    pixel1 = scanline->color[data & 0x01].pixel1;
                                    pixel2 = scanline->color[data & 0x01].pixel2;
                                    *dst++ = pixel1;
                                    *nxt++ = pixel2;
                                    data >>= 1;
                                }
                                /* pixel 3 */ {
                                    pixel1 = scanline->color[data & 0x01].pixel1;
                                    pixel2 = scanline->color[data & 0x01].pixel2;
                                    *dst++ = pixel1;
                                    *nxt++ = pixel2;
                                    data >>= 1;
                                }
                                /* pixel 4 */ {
                                    pixel1 = scanline->color[data & 0x01].pixel1;
                                    pixel2 = scanline->color[data & 0x01].pixel2;
                                    *dst++ = pixel1;
                                    *nxt++ = pixel2;
                                    data >>= 1;
                                }
                                /* pixel 5 */ {
                                    pixel1 = scanline->color[data & 0x01].pixel1;
                                    pixel2 = scanline->color[data & 0x01].pixel2;
                                    *dst++ = pixel1;
                                    *nxt++ = pixel2;
                                    data >>= 1;
                                }
                                /* pixel 6 */ {
                                    pixel1 = scanline->color[data & 0x01].pixel1;
                                    pixel2 = scanline->color[data & 0x01].pixel2;
                                    *dst++ = pixel1;
                                    *nxt++ = pixel2;
                                    data >>= 1;
                                }
                                /* pixel 7 */ {
                                    pixel1 = scanline->color[data & 0x01].pixel1;
                                    pixel2 = scanline->color[data & 0x01].pixel2;
                                    *dst++ = pixel1;
                                    *nxt++ = pixel2;
                                    data >>= 1;
                                }
                            }
                        }
                        /* right border */ {
                            pixel1 = scanline->color[16].pixel1;
                            pixel2 = scanline->color[16].pixel2;
                            for(cx = 0; cx < hp; cx++) {
                                *dst++ = pixel1;
                                *nxt++ = pixel2;
                            }
                        }
                    }
                    break;
            }
            dst = nxt; scanline++;
        }
        sa += hd;
    }
    scanline = &self->frame.array[(vd * mr) + (0 * vp)];
    for(cy = 0; cy < vp; cy++) {
        nxt += XCPC_MONITOR_WIDTH;
        pixel1 = scanline->color[16].pixel1;
        pixel2 = scanline->color[16].pixel2;
        for(cx = 0; cx < XCPC_MONITOR_WIDTH; cx++) {
            *dst++ = pixel1;
            *nxt++ = pixel2;
        }
        dst = nxt; scanline++;
    }
    (void) xcpc_monitor_put_image(self->board.monitor);
}

static void keybd_default(XcpcMachine* self, XEvent* event)
{
}

static void keybd_qwerty(XcpcMachine* self, XEvent* event)
{
    (void) xcpc_keyboard_qwerty(self->board.keyboard, &event->xkey);
}

static void keybd_azerty(XcpcMachine* self, XEvent* event)
{
    (void) xcpc_keyboard_azerty(self->board.keyboard, &event->xkey);
}

static void mouse_default(XcpcMachine* self, XEvent* event)
{
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
    self->setup.options       = xcpc_options_new();
    self->setup.company_name  = XCPC_COMPANY_NAME_DEFAULT;
    self->setup.machine_type  = XCPC_MACHINE_TYPE_DEFAULT;
    self->setup.monitor_type  = XCPC_MONITOR_TYPE_DEFAULT;
    self->setup.refresh_rate  = XCPC_REFRESH_RATE_DEFAULT;
    self->setup.keyboard_type = XCPC_KEYBOARD_TYPE_DEFAULT;
    self->setup.memory_size   = XCPC_MEMORY_SIZE_DEFAULT;
    self->setup.turbo         = 0;
    self->setup.xshm          = 0;
    self->setup.fps           = 0;
}

static void destruct_setup(XcpcMachine* self)
{
    self->setup.options = xcpc_options_delete(self->setup.options);
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
        unsigned int ram_index = 0;
        unsigned int ram_count = countof(self->board.ram_bank);
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
        unsigned int rom_index = 0;
        unsigned int rom_count = countof(self->board.rom_bank);
        do {
            if(self->board.rom_bank[rom_index] == NULL) {
                self->board.rom_bank[rom_index] = xcpc_rom_bank_new();
                self->board.rom_bank[rom_index] = xcpc_rom_bank_set_iface(self->board.rom_bank[rom_index], &rom_bank_iface);
            }
        } while(++rom_index < rom_count);
    }
#if 0
    /* exp_bank */ {
        const XcpcRomBankIface exp_bank_iface = {
            self /* user_data */
        };
        unsigned int exp_index = 0;
        unsigned int exp_count = countof(self->board.exp_bank);
        do {
            if(self->board.exp_bank[exp_index] == NULL) {
                self->board.exp_bank[exp_index] = xcpc_rom_bank_new();
                self->board.exp_bank[exp_index] = xcpc_rom_bank_set_iface(self->board.exp_bank[exp_index], &exp_bank_iface);
            }
        } while(++exp_index < exp_count);
    }
#endif
}

static void destruct_board(XcpcMachine* self)
{
    /* exp_bank */ {
        unsigned int exp_index = 0;
        unsigned int exp_count = countof(self->board.exp_bank);
        do {
            if(self->board.exp_bank[exp_index] != NULL) {
                self->board.exp_bank[exp_index] = xcpc_rom_bank_delete(self->board.exp_bank[exp_index]);
            }
        } while(++exp_index < exp_count);
    }
    /* rom_bank */ {
        unsigned int rom_index = 0;
        unsigned int rom_count = countof(self->board.rom_bank);
        do {
            if(self->board.rom_bank[rom_index] != NULL) {
                self->board.rom_bank[rom_index] = xcpc_rom_bank_delete(self->board.rom_bank[rom_index]);
            }
        } while(++rom_index < rom_count);
    }
    /* ram_bank */ {
        unsigned int ram_index = 0;
        unsigned int ram_count = countof(self->board.ram_bank);
        do {
            if(self->board.ram_bank[ram_index] != NULL) {
                self->board.ram_bank[ram_index] = xcpc_ram_bank_delete(self->board.ram_bank[ram_index]);
            }
        } while(++ram_index < ram_count);
    }
    /* fdc_765a */ {
        if(self->board.fdc_765a != NULL) {
            self->board.fdc_765a = xcpc_fdc_765a_delete(self->board.fdc_765a);
        }
    }
    /* psg_8910 */ {
        if(self->board.psg_8910 != NULL) {
            self->board.psg_8910 = xcpc_psg_8910_delete(self->board.psg_8910);
        }
    }
    /* ppi_8255 */ {
        if(self->board.ppi_8255 != NULL) {
            self->board.ppi_8255 = xcpc_ppi_8255_delete(self->board.ppi_8255);
        }
    }
    /* vdc_6845 */ {
        if(self->board.vdc_6845 != NULL) {
            self->board.vdc_6845 = xcpc_vdc_6845_delete(self->board.vdc_6845);
        }
    }
    /* vga_core */ {
        if(self->board.vga_core != NULL) {
            self->board.vga_core = xcpc_vga_core_delete(self->board.vga_core);
        }
    }
    /* cpu_z80a */ {
        if(self->board.cpu_z80a != NULL) {
            self->board.cpu_z80a = xcpc_cpu_z80a_delete(self->board.cpu_z80a);
        }
    }
    /* joystick */ {
        if(self->board.joystick != NULL) {
            self->board.joystick = xcpc_joystick_delete(self->board.joystick);
        }
    }
    /* keyboard */ {
        if(self->board.keyboard != NULL) {
            self->board.keyboard = xcpc_keyboard_delete(self->board.keyboard);
        }
    }
    /* monitor */ {
        if(self->board.monitor != NULL) {
            self->board.monitor = xcpc_monitor_delete(self->board.monitor);
        }
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
        unsigned int ram_index = 0;
        unsigned int ram_count = countof(self->board.ram_bank);
        do {
            if(self->board.ram_bank[ram_index] != NULL) {
                (void) xcpc_ram_bank_reset(self->board.ram_bank[ram_index]);
            }
        } while(++ram_index < ram_count);
    }
    /* rom_bank */ {
        unsigned int rom_index = 0;
        unsigned int rom_count = countof(self->board.rom_bank);
        do {
            if(self->board.rom_bank[rom_index] != NULL) {
                (void) xcpc_rom_bank_reset(self->board.rom_bank[rom_index]);
            }
        } while(++rom_index < rom_count);
    }
    /* exp_bank */ {
        unsigned int exp_index = 0;
        unsigned int exp_count = countof(self->board.exp_bank);
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

static void construct_stats(XcpcMachine* self)
{
    self->stats.rate       = 0;
    self->stats.time       = 0;
    self->stats.count      = 0;
    self->stats.drawn      = 0;
    self->stats.buffer[0]  = '\0';
    self->stats.cpu_period = 0;
}

static void destruct_stats(XcpcMachine* self)
{
}

static void reset_stats(XcpcMachine* self)
{
    self->stats.rate       |= 0; /* dont'modify */
    self->stats.time       |= 0; /* dont'modify */
    self->stats.count      &= 0; /* clear value */
    self->stats.drawn      &= 0; /* clear value */
    self->stats.buffer[0]  &= 0; /* clear value */
    self->stats.cpu_period |= 0; /* dont'modify */
}

static void construct_timer(XcpcMachine* self)
{
    self->timer.deadline.tv_sec  = 0;
    self->timer.deadline.tv_usec = 0;
    self->timer.profiler.tv_sec  = 0;
    self->timer.profiler.tv_usec = 0;
}

static void destruct_timer(XcpcMachine* self)
{
}

static void reset_timer(XcpcMachine* self)
{
    if(gettimeofday(&self->timer.deadline, NULL) != 0) {
        xcpc_log_error("gettimeofday() has failed");
    }
    if(gettimeofday(&self->timer.profiler, NULL) != 0) {
        xcpc_log_error("gettimeofday() has failed");
    }
}

static void construct_funcs(XcpcMachine* self)
{
    self->funcs.paint_func = &paint_default;
    self->funcs.keybd_func = &keybd_default;
    self->funcs.mouse_func = &mouse_default;
}

static void destruct_funcs(XcpcMachine* self)
{
}

static void reset_funcs(XcpcMachine* self)
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
        (void) memset(&self->stats, 0, sizeof(XcpcMachineStats));
        (void) memset(&self->timer, 0, sizeof(XcpcMachineTimer));
        (void) memset(&self->funcs, 0, sizeof(XcpcMachineFuncs));
    }
    /* construct all subsystems */ {
        construct_iface(self);
        construct_setup(self);
        construct_state(self);
        construct_board(self);
        construct_pager(self);
        construct_frame(self);
        construct_stats(self);
        construct_timer(self);
        construct_funcs(self);
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
        destruct_funcs(self);
        destruct_timer(self);
        destruct_stats(self);
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
        reset_stats(self);
        reset_timer(self);
        reset_funcs(self);
    }
    return self;
}

XcpcMachine* xcpc_machine_clock(XcpcMachine* self)
{
    XcpcCpuZ80a* cpu_z80a = self->board.cpu_z80a;
    XcpcVdc6845* vdc_6845 = self->board.vdc_6845;
    XcpcFdc765a* fdc_765a = self->board.fdc_765a;

    /* process each scanline */ {
        self->frame.index = 0;
        do {
            int cpu_tick;
            for(cpu_tick = 0; cpu_tick < self->stats.cpu_period; cpu_tick += 4) {
                xcpc_vdc_6845_clock(vdc_6845);
                if((cpu_z80a->state.ctrs.i_period += 4) > 0) {
                    int32_t i_period = cpu_z80a->state.ctrs.i_period;
                    xcpc_cpu_z80a_clock(self->board.cpu_z80a);
                    cpu_z80a->state.ctrs.i_period = i_period - (((i_period - cpu_z80a->state.ctrs.i_period) + 3) & (~3));
                }
            }
        } while(++self->frame.index < 312);
    }
    /* clock the fdc */ {
        xcpc_fdc_765a_clock(fdc_765a);
    }
    return self;
}

XcpcMachine* xcpc_machine_parse(XcpcMachine* self, int* argc, char*** argv)
{
    /* parse command-line */ {
        (void) xcpc_options_parse(self->setup.options, argc, argv);
    }
    return self;
}

XcpcMachine* xcpc_machine_start(XcpcMachine* self)
{
    char* system_rom = NULL;
    char* amsdos_rom = NULL;
    const char* opt_company  = XCPC_OPTIONS_STATE(self->setup.options)->company;
    const char* opt_machine  = XCPC_OPTIONS_STATE(self->setup.options)->machine;
    const char* opt_monitor  = XCPC_OPTIONS_STATE(self->setup.options)->monitor;
    const char* opt_refresh  = XCPC_OPTIONS_STATE(self->setup.options)->refresh;
    const char* opt_keyboard = XCPC_OPTIONS_STATE(self->setup.options)->keyboard;
    const char* opt_system   = XCPC_OPTIONS_STATE(self->setup.options)->sysrom;
    const char* opt_amsdos   = XCPC_OPTIONS_STATE(self->setup.options)->rom007;
    const char* opt_drive0   = XCPC_OPTIONS_STATE(self->setup.options)->drive0;
    const char* opt_drive1   = XCPC_OPTIONS_STATE(self->setup.options)->drive1;
    const char* opt_snapshot = XCPC_OPTIONS_STATE(self->setup.options)->snapshot;
    const int   opt_turbo    = XCPC_OPTIONS_STATE(self->setup.options)->turbo;
    const int   opt_xshm     = XCPC_OPTIONS_STATE(self->setup.options)->xshm;
    const int   opt_fps      = XCPC_OPTIONS_STATE(self->setup.options)->fps;

    /* initialize setup */ {
        self->setup.company_name  = xcpc_company_name(opt_company, XCPC_COMPANY_NAME_DEFAULT);
        self->setup.machine_type  = xcpc_machine_type(opt_machine, XCPC_MACHINE_TYPE_DEFAULT);
        self->setup.monitor_type  = xcpc_monitor_type(opt_monitor, XCPC_MONITOR_TYPE_DEFAULT);
        self->setup.refresh_rate  = xcpc_refresh_rate(opt_refresh, XCPC_REFRESH_RATE_DEFAULT);
        self->setup.keyboard_type = xcpc_keyboard_type(opt_keyboard, XCPC_KEYBOARD_TYPE_DEFAULT);
        self->setup.memory_size   = XCPC_MEMORY_SIZE_DEFAULT;
        self->setup.turbo         = opt_turbo;
        self->setup.xshm          = opt_xshm;
        self->setup.fps           = opt_fps;
    }
    /* adjust company name */ {
        if(self->setup.company_name == XCPC_COMPANY_NAME_DEFAULT) {
            self->setup.company_name = XCPC_COMPANY_NAME_AMSTRAD;
        }
    }
    /* adjust machine type */ {
        if(self->setup.machine_type == XCPC_MACHINE_TYPE_DEFAULT) {
            self->setup.machine_type = XCPC_MACHINE_TYPE_CPC6128;
        }
    }
    /* adjust monitor type */ {
        if(self->setup.monitor_type == XCPC_MONITOR_TYPE_DEFAULT) {
            self->setup.monitor_type = XCPC_MONITOR_TYPE_COLOR;
        }
    }
    /* adjust refresh rate */ {
        if(self->setup.refresh_rate == XCPC_REFRESH_RATE_DEFAULT) {
            self->setup.refresh_rate = XCPC_REFRESH_RATE_50HZ;
        }
    }
    /* adjust keyboard type */ {
        if(self->setup.keyboard_type == XCPC_KEYBOARD_TYPE_DEFAULT) {
            self->setup.keyboard_type = XCPC_KEYBOARD_TYPE_QWERTY;
        }
    }
    /* adjust memory size */ {
        if(self->setup.memory_size == XCPC_MEMORY_SIZE_DEFAULT) {
            if(self->setup.machine_type == XCPC_MACHINE_TYPE_CPC6128) {
                self->setup.memory_size = XCPC_MEMORY_SIZE_128K;
            }
            else {
                self->setup.memory_size = XCPC_MEMORY_SIZE_64K;
            }
        }
    }
    /* prepare roms */ {
        switch(self->setup.machine_type) {
            case XCPC_MACHINE_TYPE_CPC464:
                {
                    system_rom = (is_set(opt_system) ? strdup(opt_system) : build_filename("roms", "cpc464.rom"));
                    amsdos_rom = (is_set(opt_amsdos) ? strdup(opt_amsdos) : NULL                                );
                }
                break;
            case XCPC_MACHINE_TYPE_CPC664:
                {
                    system_rom = (is_set(opt_system) ? strdup(opt_system) : build_filename("roms", "cpc664.rom"));
                    amsdos_rom = (is_set(opt_amsdos) ? strdup(opt_amsdos) : build_filename("roms", "amsdos.rom"));
                }
                break;
            case XCPC_MACHINE_TYPE_CPC6128:
                {
                    system_rom = (is_set(opt_system) ? strdup(opt_system) : build_filename("roms", "cpc6128.rom"));
                    amsdos_rom = (is_set(opt_amsdos) ? strdup(opt_amsdos) : build_filename("roms", "amsdos.rom" ));
                }
                break;
            default:
                xcpc_log_error("unknown machine type");
                break;
        }
    }
    /* initialize state */ {
        self->state.hsync     = 0; /* no hsync      */
        self->state.vsync     = 0; /* no vsync      */
        self->state.refresh   = 1; /* 50Hz          */
        self->state.company   = 7; /* amstrad       */
        self->state.expansion = 1; /* present       */
        self->state.parallel  = 1; /* not connected */
        self->state.cassette  = 0; /* no data       */
    }
    /* initialize board */ {
        //  construct_board(self, system_rom, amsdos_rom);
    }
    /* load lower rom */ {
        XcpcRomBankStatus status = xcpc_rom_bank_load(self->board.rom_bank[0], system_rom, 0x0000);
        if(status != XCPC_ROM_BANK_STATUS_SUCCESS) {
            xcpc_log_error("lower-rom: loading error (%s)", system_rom);
        }
    }
    /* load upper rom */ {
        XcpcRomBankStatus status = xcpc_rom_bank_load(self->board.rom_bank[1], system_rom, 0x4000);
        if(status != XCPC_ROM_BANK_STATUS_SUCCESS) {
            xcpc_log_error("upper-rom: loading error (%s)", system_rom);
        }
    }
    /* load expansion roms */ {
        const char* cpc_expansions[16] = {
            XCPC_OPTIONS_STATE(self->setup.options)->rom000,
            XCPC_OPTIONS_STATE(self->setup.options)->rom001,
            XCPC_OPTIONS_STATE(self->setup.options)->rom002,
            XCPC_OPTIONS_STATE(self->setup.options)->rom003,
            XCPC_OPTIONS_STATE(self->setup.options)->rom004,
            XCPC_OPTIONS_STATE(self->setup.options)->rom005,
            XCPC_OPTIONS_STATE(self->setup.options)->rom006,
            XCPC_OPTIONS_STATE(self->setup.options)->rom007,
            XCPC_OPTIONS_STATE(self->setup.options)->rom008,
            XCPC_OPTIONS_STATE(self->setup.options)->rom009,
            XCPC_OPTIONS_STATE(self->setup.options)->rom010,
            XCPC_OPTIONS_STATE(self->setup.options)->rom011,
            XCPC_OPTIONS_STATE(self->setup.options)->rom012,
            XCPC_OPTIONS_STATE(self->setup.options)->rom013,
            XCPC_OPTIONS_STATE(self->setup.options)->rom014,
            XCPC_OPTIONS_STATE(self->setup.options)->rom015,
        };
        /* create expansion roms banks */ {
            const XcpcRomBankIface exp_bank_iface = {
                self /* user_data */
            };
            unsigned int exp_index = 0;
            unsigned int exp_count = countof(self->board.exp_bank);
            for(exp_index = 0; exp_index < exp_count; ++exp_index) {
                const char* filename = NULL;
                if(exp_index < countof(cpc_expansions)) {
                    filename = cpc_expansions[exp_index];
                    if((exp_index == 7) && (is_set(amsdos_rom))) {
                        filename = amsdos_rom;
                    }
                }
                if(is_set(filename)) {
                    if(self->board.exp_bank[exp_index] == NULL) {
                        self->board.exp_bank[exp_index] = xcpc_rom_bank_new();
                        self->board.exp_bank[exp_index] = xcpc_rom_bank_set_iface(self->board.exp_bank[exp_index], &exp_bank_iface);
                    }
                }
                else {
                    if(self->board.exp_bank[exp_index] != NULL) {
                        self->board.exp_bank[exp_index] = xcpc_rom_bank_delete(self->board.exp_bank[exp_index]);
                    }
                }
                if(is_set(filename)) {
                    XcpcRomBankStatus status = xcpc_rom_bank_load(self->board.exp_bank[exp_index], filename, 0x0000);
                    if(status != XCPC_ROM_BANK_STATUS_SUCCESS) {
                        xcpc_log_error("expansion-rom: loading error (%s)", filename);
                    }
                }
            }
        }
    }
    /* compute frame rate/time and cpu period */ {
        switch(self->setup.refresh_rate) {
            case XCPC_REFRESH_RATE_50HZ:
                self->state.company    = ((self->setup.company_name - 1) & 7);
                self->state.refresh    = 1;
                self->stats.rate       = 50;
                self->stats.time       = 20000;
                self->stats.cpu_period = (int) (4000000.0 / (50.0 * 312.5));
                break;
            case XCPC_REFRESH_RATE_60HZ:
                self->state.company    = ((self->setup.company_name - 1) & 7);
                self->state.refresh    = 0;
                self->stats.rate       = 60;
                self->stats.time       = 16667;
                self->stats.cpu_period = (int) (4000000.0 / (60.0 * 262.5));
                break;
            default:
                xcpc_log_error("unsupported refresh rate %d", self->setup.refresh_rate);
                break;
        }
        if(self->setup.turbo != 0) {
            self->stats.time = 1000;
        }
    }
    /* reset instance */ {
        (void) xcpc_machine_reset(self);
    }
    /* load initial drive0 */ {
        if(is_set(opt_drive0)) {
            xcpc_machine_insert_drive0(self, opt_drive0);
        }
    }
    /* load initial drive1 */ {
        if(is_set(opt_drive1)) {
            xcpc_machine_insert_drive1(self, opt_drive1);
        }
    }
    /* load initial snapshot */ {
        if(is_set(opt_snapshot)) {
            xcpc_machine_load_snapshot(self, opt_snapshot);
        }
    }
    /* cleanup */ {
        if(system_rom != NULL) {
            system_rom = (free(system_rom), NULL);
        }
        if(amsdos_rom != NULL) {
            amsdos_rom = (free(amsdos_rom), NULL);
        }
    }
    return self;
}

XcpcMachine* xcpc_machine_close(XcpcMachine* self)
{
    /* unrealize */ {
        (void) xcpc_monitor_unrealize(self->board.monitor);
    }
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
    XcpcSnapshot*      snapshot = xcpc_snapshot_new();
    XcpcSnapshotStatus status   = XCPC_SNAPSHOT_STATUS_SUCCESS;
    uint32_t           ram_size = self->setup.memory_size;

    log_trace("xcpc_machine_load_snapshot");
    /* load snapshot */ {
        if(status == XCPC_SNAPSHOT_STATUS_SUCCESS) {
            status = xcpc_snapshot_load(snapshot, filename);
        }
    }
    /* fetch devices */ {
        if(status == XCPC_SNAPSHOT_STATUS_SUCCESS) {
            (void) xcpc_snapshot_fetch_cpu_z80a(snapshot, self->board.cpu_z80a);
            (void) xcpc_snapshot_fetch_vga_core(snapshot, self->board.vga_core);
            (void) xcpc_snapshot_fetch_vdc_6845(snapshot, self->board.vdc_6845);
            (void) xcpc_snapshot_fetch_ppi_8255(snapshot, self->board.ppi_8255);
            (void) xcpc_snapshot_fetch_psg_8910(snapshot, self->board.psg_8910);
            (void) xcpc_snapshot_fetch_fdc_765a(snapshot, self->board.fdc_765a);
        }
    }
    /* fetch ram/rom */ {
        if(status == XCPC_SNAPSHOT_STATUS_SUCCESS) {
            (void) xcpc_snapshot_fetch_ram_conf(snapshot, &self->pager.conf.ram);
            (void) xcpc_snapshot_fetch_rom_conf(snapshot, &self->pager.conf.rom);
            (void) xcpc_snapshot_fetch_ram_size(snapshot, &ram_size);
        }
    }
    /* fetch ram */ {
        if(status == XCPC_SNAPSHOT_STATUS_SUCCESS) {
            uint32_t     snap_size  = self->setup.memory_size;
            uint32_t     bank_size  = 16384;
            unsigned int bank_index = 0;
            while(snap_size >= bank_size) {
                (void) xcpc_snapshot_fetch_ram_bank(snapshot, self->board.ram_bank[bank_index++]);
                snap_size -= bank_size;
            }
            if(snap_size != 0) {
                status = XCPC_SNAPSHOT_STATUS_MEMORY_ERROR;
            }
        }
    }
    /* cleanup */ {
        snapshot = xcpc_snapshot_delete(snapshot);
    }
    /* check for error */ {
        if(status != XCPC_SNAPSHOT_STATUS_SUCCESS) {
            xcpc_log_error("load snapshot : %s", xcpc_snapshot_strerror(status));
        }
    }
    /* perform memory mapping or reset */ {
        if(status == XCPC_SNAPSHOT_STATUS_SUCCESS) {
            cpc_mem_select(self);
        }
        else {
            (void) xcpc_machine_reset(self);
        }
    }
    return self;
}

XcpcMachine* xcpc_machine_save_snapshot(XcpcMachine* self, const char* filename)
{
    XcpcSnapshot*      snapshot = xcpc_snapshot_new();
    XcpcSnapshotStatus status   = XCPC_SNAPSHOT_STATUS_SUCCESS;
    uint32_t           ram_size = self->setup.memory_size;

    log_trace("xcpc_machine_save_snapshot");
    /* store devices */ {
        if(status == XCPC_SNAPSHOT_STATUS_SUCCESS) {
            (void) xcpc_snapshot_store_cpu_z80a(snapshot, self->board.cpu_z80a);
            (void) xcpc_snapshot_store_vga_core(snapshot, self->board.vga_core);
            (void) xcpc_snapshot_store_vdc_6845(snapshot, self->board.vdc_6845);
            (void) xcpc_snapshot_store_ppi_8255(snapshot, self->board.ppi_8255);
            (void) xcpc_snapshot_store_psg_8910(snapshot, self->board.psg_8910);
            (void) xcpc_snapshot_store_fdc_765a(snapshot, self->board.fdc_765a);
        }
    }
    /* store ram/rom */ {
        if(status == XCPC_SNAPSHOT_STATUS_SUCCESS) {
            (void) xcpc_snapshot_store_ram_conf(snapshot, &self->pager.conf.ram);
            (void) xcpc_snapshot_store_rom_conf(snapshot, &self->pager.conf.rom);
            (void) xcpc_snapshot_store_ram_size(snapshot, &ram_size);
        }
    }
    /* store ram */ {
        if(status == XCPC_SNAPSHOT_STATUS_SUCCESS) {
            uint32_t     snap_size  = self->setup.memory_size;
            uint32_t     bank_size  = 16384;
            unsigned int bank_index = 0;
            while(snap_size >= bank_size) {
                (void) xcpc_snapshot_store_ram_bank(snapshot, self->board.ram_bank[bank_index++]);
                snap_size -= bank_size;
            }
            if(snap_size != 0) {
                status = XCPC_SNAPSHOT_STATUS_MEMORY_ERROR;
            }
        }
    }
    /* save snapshot */ {
        if(status == XCPC_SNAPSHOT_STATUS_SUCCESS) {
            status = xcpc_snapshot_save(snapshot, filename);
        }
    }
    /* cleanup */ {
        snapshot = xcpc_snapshot_delete(snapshot);
    }
    /* check for error */ {
        if(status != XCPC_SNAPSHOT_STATUS_SUCCESS) {
            xcpc_log_error("save snapshot : %s", xcpc_snapshot_strerror(status));
        }
    }
    return self;
}

unsigned long xcpc_machine_create_proc(XcpcMachine* self, XEvent* event)
{
    /* start */ {
        (void) xcpc_machine_start(self);
    }
    return 0UL;
}

unsigned long xcpc_machine_destroy_proc(XcpcMachine* self, XEvent* event)
{
    /* close */ {
        (void) xcpc_machine_close(self);
    }
    return 0UL;
}

unsigned long xcpc_machine_realize_proc(XcpcMachine* self, XEvent* event)
{
    if(self != NULL) {
        /* realize */ {
            (void) xcpc_monitor_realize ( self->board.monitor
                                        , self->setup.monitor_type
                                        , event->xany.display
                                        , event->xany.window
                                        , (self->setup.xshm != 0 ? True : False) );
        }
        /* init paint handler */ {
            switch(self->board.monitor->state.image->bits_per_pixel) {
                case 8:
                    self->funcs.paint_func = &paint_08bpp;
                    break;
                case 16:
                    self->funcs.paint_func = &paint_16bpp;
                    break;
                case 32:
                    self->funcs.paint_func = &paint_32bpp;
                    break;
                default:
                    self->funcs.paint_func = &paint_default;
                    break;
            }
        }
        /* init keybd handler */ {
            switch(self->setup.keyboard_type) {
                case XCPC_KEYBOARD_TYPE_QWERTY:
                    self->funcs.keybd_func = &keybd_qwerty;
                    break;
                case XCPC_KEYBOARD_TYPE_AZERTY:
                    self->funcs.keybd_func = &keybd_azerty;
                    break;
                default:
                    self->funcs.keybd_func = &keybd_default;
                    break;
            }
        }
    }
    return 0UL;
}

unsigned long xcpc_machine_resize_proc(XcpcMachine* self, XEvent* event)
{
    if((self != NULL) && (event != NULL) && (event->type == ConfigureNotify)) {
        (void) xcpc_monitor_resize(self->board.monitor, &event->xconfigure);
    }
    return 0UL;
}

unsigned long xcpc_machine_expose_proc(XcpcMachine* self, XEvent* event)
{
    if((self != NULL) && (event != NULL) && (event->type == Expose)) {
        (void) xcpc_monitor_expose(self->board.monitor, &event->xexpose);
    }
    return 0UL;
}

unsigned long xcpc_machine_timer_proc(XcpcMachine* self, XEvent* event)
{
    unsigned long elapsed = 0;
    unsigned long timeout = 0;

    /* clock the machine */ {
        xcpc_machine_clock(self);
    }
    /* compute the elapsed time in us */ {
        struct timeval prev_time = self->timer.deadline;
        struct timeval curr_time;
        if(gettimeofday(&curr_time, NULL) == 0) {
            const long long t1 = (((long long) prev_time.tv_sec) * 1000000LL) + ((long long) prev_time.tv_usec);
            const long long t2 = (((long long) curr_time.tv_sec) * 1000000LL) + ((long long) curr_time.tv_usec);
            if(t2 >= t1) {
                elapsed = ((unsigned long)(t2 - t1));
            }
            else {
                elapsed = 0UL;
            }
            if(elapsed >= 1000000UL) {
                self->timer.deadline = curr_time;
                elapsed              = 0UL;
            }
        }
    }
    /* draw the frame and compute stats if needed */ {
        if((self->stats.count == 0) || (elapsed <= self->stats.time)) {
            (*self->funcs.paint_func)(self);
            ++self->stats.drawn;
        }
        if(++self->stats.count == self->stats.rate) {
            compute_stats(self);
        }
    }
    /* compute the next frame absolute time */ {
        if((self->timer.deadline.tv_usec += self->stats.time) >= 1000000) {
            self->timer.deadline.tv_usec -= 1000000;
            self->timer.deadline.tv_sec  += 1;
        }
    }
    /* compute the deadline timeout in us */ {
        struct timeval next_time = self->timer.deadline;
        struct timeval curr_time;
        if(gettimeofday(&curr_time, NULL) == 0) {
            const long long t1 = (((long long) curr_time.tv_sec) * 1000000LL) + ((long long) curr_time.tv_usec);
            const long long t2 = (((long long) next_time.tv_sec) * 1000000LL) + ((long long) next_time.tv_usec);
            if(t2 >= t1) {
                timeout = ((unsigned long)(t2 - t1));
            }
            else {
                timeout = 0UL;
            }
        }
    }
    /* schedule the next frame in ms */ {
        if((timeout /= 1000UL) == 0UL) {
            timeout = 1UL;
        }
    }
    return timeout;
}

unsigned long xcpc_machine_input_proc(XcpcMachine* self, XEvent* event)
{
    switch(event->type) {
        case KeyPress:
            (*self->funcs.keybd_func)(self, event);
            break;
        case KeyRelease:
            (*self->funcs.keybd_func)(self, event);
            break;
        case ButtonPress:
            (*self->funcs.mouse_func)(self, event);
            break;
        case ButtonRelease:
            (*self->funcs.mouse_func)(self, event);
            break;
        case MotionNotify:
            (*self->funcs.mouse_func)(self, event);
            break;
        default:
            break;
    }
    return 0UL;
}
