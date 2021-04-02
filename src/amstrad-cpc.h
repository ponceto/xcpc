/*
 * amstrad-cpc.h - Copyright (c) 2001-2021 - Olivier Poncet
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
#ifndef __AMSTRAD_CPC_H__
#define __AMSTRAD_CPC_H__

#include <xcpc.h>
#include <sys/time.h>
#include <xcpc/options/options.h>
#include <xcpc/monitor/monitor.h>
#include <xcpc/keyboard/keyboard.h>
#include <xcpc/joystick/joystick.h>
#include <xcpc/cpu-z80a/cpu-z80a.h>
#include <xcpc/vga-core/vga-core.h>
#include <xcpc/vdc-6845/vdc-6845.h>
#include <xcpc/ppi-8255/ppi-8255.h>
#include <xcpc/psg-8910/psg-8910.h>
#include <xcpc/fdc-765a/fdc-765a.h>
#include <xcpc/ram-bank/ram-bank.h>
#include <xcpc/rom-bank/rom-bank.h>
#include <xcpc/snapshot/snapshot.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _AMSTRAD_CPC_EMULATOR AMSTRAD_CPC_EMULATOR;

typedef void (*PaintFunc)(AMSTRAD_CPC_EMULATOR* self);
typedef void (*KeybdFunc)(AMSTRAD_CPC_EMULATOR* self, XEvent* xevent);
typedef void (*MouseFunc)(AMSTRAD_CPC_EMULATOR* self, XEvent* xevent);

struct _AMSTRAD_CPC_EMULATOR
{
    XcpcOptions* options;
    struct _setup {
        XcpcCompanyName  company_name;
        XcpcMachineType  machine_type;
        XcpcMonitorType  monitor_type;
        XcpcRefreshRate  refresh_rate;
        XcpcKeyboardType keyboard_type;
        XcpcMemorySize   memory_size;
        int              turbo;
        int              xshm;
        int              fps;
    } setup;
    struct _state {
        uint8_t hsync;
        uint8_t vsync;
        uint8_t refresh;
        uint8_t company;
        uint8_t expansion;
        uint8_t parallel;
        uint8_t cassette;
    } state;
    struct _board {
        XcpcMonitor*  monitor;
        XcpcKeyboard* keyboard;
        XcpcJoystick* joystick;
        XcpcCpuZ80a*  cpu_z80a;
        XcpcVgaCore*  vga_core;
        XcpcVdc6845*  vdc_6845;
        XcpcPpi8255*  ppi_8255;
        XcpcPsg8910*  psg_8910;
        XcpcFdc765a*  fdc_765a;
        XcpcRamBank*  ram_bank[8];
        XcpcRomBank*  rom_bank[2];
        XcpcRomBank*  exp_bank[256];
    } board;
    struct _pager {
        struct {
            uint8_t* rd[4];
            uint8_t* wr[4];
        } bank;
        struct {
            uint8_t ram;
            uint8_t rom;
        } conf;
    } pager;
    struct _frame {
        XcpcScanline array[312];
        int          index;
    } frame;
    struct _stats {
        unsigned int rate;
        unsigned int time;
        unsigned int count;
        unsigned int drawn;
        char         buffer[256];
    } stats;
    struct _timer {
        struct timeval deadline;
        struct timeval profiler;
    } timer;
    struct _funcs {
        PaintFunc paint_func;
        KeybdFunc keybd_func;
        MouseFunc mouse_func;
    } funcs;
    int cpu_period;
};

extern AMSTRAD_CPC_EMULATOR amstrad_cpc;

extern void          amstrad_cpc_new           (int* argc, char*** argv);
extern void          amstrad_cpc_delete        (void);
extern void          amstrad_cpc_start         (AMSTRAD_CPC_EMULATOR* amstrad_cpc);
extern void          amstrad_cpc_close         (AMSTRAD_CPC_EMULATOR* amstrad_cpc);
extern void          amstrad_cpc_reset         (AMSTRAD_CPC_EMULATOR* amstrad_cpc);

extern void          amstrad_cpc_load_snapshot (AMSTRAD_CPC_EMULATOR* amstrad_cpc, const char* filename);
extern void          amstrad_cpc_save_snapshot (AMSTRAD_CPC_EMULATOR* amstrad_cpc, const char* filename);

extern void          amstrad_cpc_insert_drive0 (AMSTRAD_CPC_EMULATOR* amstrad_cpc, const char* filename);
extern void          amstrad_cpc_remove_drive0 (AMSTRAD_CPC_EMULATOR* amstrad_cpc);
extern void          amstrad_cpc_insert_drive1 (AMSTRAD_CPC_EMULATOR* amstrad_cpc, const char* filename);
extern void          amstrad_cpc_remove_drive1 (AMSTRAD_CPC_EMULATOR* amstrad_cpc);

extern unsigned long amstrad_cpc_create_proc   (AMSTRAD_CPC_EMULATOR* amstrad_cpc, XEvent* event);
extern unsigned long amstrad_cpc_destroy_proc  (AMSTRAD_CPC_EMULATOR* amstrad_cpc, XEvent* event);
extern unsigned long amstrad_cpc_realize_proc  (AMSTRAD_CPC_EMULATOR* amstrad_cpc, XEvent* event);
extern unsigned long amstrad_cpc_resize_proc   (AMSTRAD_CPC_EMULATOR* amstrad_cpc, XEvent* event);
extern unsigned long amstrad_cpc_expose_proc   (AMSTRAD_CPC_EMULATOR* amstrad_cpc, XEvent* event);
extern unsigned long amstrad_cpc_timer_proc    (AMSTRAD_CPC_EMULATOR* amstrad_cpc, XEvent* event);
extern unsigned long amstrad_cpc_input_proc    (AMSTRAD_CPC_EMULATOR* amstrad_cpc, XEvent* event);

#ifdef __cplusplus
}
#endif

#endif /* __AMSTRAD_CPC_H__ */
