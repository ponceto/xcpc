/*
 * libxcpc.h - Copyright (c) 2001-2024 - Olivier Poncet
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
#ifndef __XCPC_LIBXCPC_H__
#define __XCPC_LIBXCPC_H__

#include <xcpc/xlib/xlib.h>

#ifdef __cplusplus
extern "C" {
#endif

enum _XcpcLogLevel
{
    XCPC_LOGLEVEL_UNKNOWN = -1,
    XCPC_LOGLEVEL_QUIET   =  0,
    XCPC_LOGLEVEL_ERROR   =  1,
    XCPC_LOGLEVEL_ALERT   =  2,
    XCPC_LOGLEVEL_PRINT   =  3,
    XCPC_LOGLEVEL_TRACE   =  4,
    XCPC_LOGLEVEL_DEBUG   =  5,
};

enum _XcpcCompanyName
{
    XCPC_COMPANY_NAME_UNKNOWN   = -1,
    XCPC_COMPANY_NAME_DEFAULT   =  0,
    XCPC_COMPANY_NAME_ISP       =  1,
    XCPC_COMPANY_NAME_TRIUMPH   =  2,
    XCPC_COMPANY_NAME_SAISHO    =  3,
    XCPC_COMPANY_NAME_SOLAVOX   =  4,
    XCPC_COMPANY_NAME_AWA       =  5,
    XCPC_COMPANY_NAME_SCHNEIDER =  6,
    XCPC_COMPANY_NAME_ORION     =  7,
    XCPC_COMPANY_NAME_AMSTRAD   =  8,
};

enum _XcpcMachineType
{
    XCPC_MACHINE_TYPE_UNKNOWN = -1,
    XCPC_MACHINE_TYPE_DEFAULT =  0,
    XCPC_MACHINE_TYPE_CPC464  =  1,
    XCPC_MACHINE_TYPE_CPC664  =  2,
    XCPC_MACHINE_TYPE_CPC6128 =  3,
};

enum _XcpcMonitorType
{
    XCPC_MONITOR_TYPE_UNKNOWN = -1,
    XCPC_MONITOR_TYPE_DEFAULT =  0,
    XCPC_MONITOR_TYPE_COLOR   =  1,
    XCPC_MONITOR_TYPE_GREEN   =  2,
    XCPC_MONITOR_TYPE_GRAY    =  3,
    XCPC_MONITOR_TYPE_CTM640  =  4,
    XCPC_MONITOR_TYPE_CTM644  =  5,
    XCPC_MONITOR_TYPE_GT64    =  6,
    XCPC_MONITOR_TYPE_GT65    =  7,
    XCPC_MONITOR_TYPE_CM14    =  8,
    XCPC_MONITOR_TYPE_MM12    =  9,
};

enum _XcpcRefreshRate
{
    XCPC_REFRESH_RATE_UNKNOWN = -1,
    XCPC_REFRESH_RATE_DEFAULT =  0,
    XCPC_REFRESH_RATE_50HZ    =  1,
    XCPC_REFRESH_RATE_60HZ    =  2,
};

enum _XcpcKeyboardType
{
    XCPC_KEYBOARD_TYPE_UNKNOWN = -1,
    XCPC_KEYBOARD_TYPE_DEFAULT =  0,
    XCPC_KEYBOARD_TYPE_ENGLISH =  1,
    XCPC_KEYBOARD_TYPE_FRENCH  =  2,
    XCPC_KEYBOARD_TYPE_GERMAN  =  3,
    XCPC_KEYBOARD_TYPE_SPANISH =  4,
    XCPC_KEYBOARD_TYPE_DANISH  =  5,
};

enum _XcpcMemorySize
{
    XCPC_MEMORY_SIZE_UNKNOWN = -1,
    XCPC_MEMORY_SIZE_DEFAULT = (  0 * 1024),
    XCPC_MEMORY_SIZE_64K     = ( 64 * 1024),
    XCPC_MEMORY_SIZE_128K    = (128 * 1024),
    XCPC_MEMORY_SIZE_192K    = (192 * 1024),
    XCPC_MEMORY_SIZE_256K    = (256 * 1024),
    XCPC_MEMORY_SIZE_320K    = (320 * 1024),
    XCPC_MEMORY_SIZE_384K    = (384 * 1024),
    XCPC_MEMORY_SIZE_448K    = (448 * 1024),
    XCPC_MEMORY_SIZE_512K    = (512 * 1024),
};

struct _XcpcBackendAnyEvent
{
    XEvent* event;
};

struct _XcpcBackendIdleEvent
{
    XEvent* event;
};

struct _XcpcBackendResetEvent
{
    XEvent* event;
};

struct _XcpcBackendClockEvent
{
    XEvent* event;
};

struct _XcpcBackendCreateWindowEvent
{
    XEvent* event;
};

struct _XcpcBackendDeleteWindowEvent
{
    XEvent* event;
};

struct _XcpcBackendResizeWindowEvent
{
    XEvent* event;
};

struct _XcpcBackendExposeWindowEvent
{
    XEvent* event;
};

struct _XcpcBackendKeyPressEvent
{
    XEvent* event;
};

struct _XcpcBackendKeyReleaseEvent
{
    XEvent* event;
};

struct _XcpcBackendButtonPressEvent
{
    XEvent* event;
};

struct _XcpcBackendButtonReleaseEvent
{
    XEvent* event;
};

struct _XcpcBackendMotionNotifyEvent
{
    XEvent* event;
};

typedef enum   _XcpcLogLevel                  XcpcLogLevel;
typedef enum   _XcpcCompanyName               XcpcCompanyName;
typedef enum   _XcpcMachineType               XcpcMachineType;
typedef enum   _XcpcMonitorType               XcpcMonitorType;
typedef enum   _XcpcRefreshRate               XcpcRefreshRate;
typedef enum   _XcpcKeyboardType              XcpcKeyboardType;
typedef enum   _XcpcMemorySize                XcpcMemorySize;
typedef struct _XcpcBackend                   XcpcBackend;
typedef struct _XcpcBackendClosure            XcpcBackendClosure;
typedef struct _XcpcBackendAnyEvent           XcpcBackendAnyEvent;
typedef struct _XcpcBackendIdleEvent          XcpcBackendIdleEvent;
typedef struct _XcpcBackendResetEvent         XcpcBackendResetEvent;
typedef struct _XcpcBackendClockEvent         XcpcBackendClockEvent;
typedef struct _XcpcBackendCreateWindowEvent  XcpcBackendCreateWindowEvent;
typedef struct _XcpcBackendDeleteWindowEvent  XcpcBackendDeleteWindowEvent;
typedef struct _XcpcBackendResizeWindowEvent  XcpcBackendResizeWindowEvent;
typedef struct _XcpcBackendExposeWindowEvent  XcpcBackendExposeWindowEvent;
typedef struct _XcpcBackendKeyPressEvent      XcpcBackendKeyPressEvent;
typedef struct _XcpcBackendKeyReleaseEvent    XcpcBackendKeyReleaseEvent;
typedef struct _XcpcBackendButtonPressEvent   XcpcBackendButtonPressEvent;
typedef struct _XcpcBackendButtonReleaseEvent XcpcBackendButtonReleaseEvent;
typedef struct _XcpcBackendMotionNotifyEvent  XcpcBackendMotionNotifyEvent;

struct _XcpcBackendClosure
{
    union {
        XcpcBackendAnyEvent           any;
        XcpcBackendIdleEvent          idle;
        XcpcBackendResetEvent         reset;
        XcpcBackendClockEvent         clock;
        XcpcBackendCreateWindowEvent  create_window;
        XcpcBackendDeleteWindowEvent  delete_window;
        XcpcBackendResizeWindowEvent  resize_window;
        XcpcBackendExposeWindowEvent  expose_window;
        XcpcBackendKeyPressEvent      key_press;
        XcpcBackendKeyReleaseEvent    key_release;
        XcpcBackendButtonPressEvent   button_press;
        XcpcBackendButtonReleaseEvent button_release;
        XcpcBackendMotionNotifyEvent  motion_notify;
    } u;
};

struct _XcpcBackend
{
    void* instance;
    unsigned long (*idle_func)           (void* instance, XcpcBackendClosure* closure);
    unsigned long (*reset_func)          (void* instance, XcpcBackendClosure* closure);
    unsigned long (*clock_func)          (void* instance, XcpcBackendClosure* closure);
    unsigned long (*create_window_func)  (void* instance, XcpcBackendClosure* closure);
    unsigned long (*delete_window_func)  (void* instance, XcpcBackendClosure* closure);
    unsigned long (*resize_window_func)  (void* instance, XcpcBackendClosure* closure);
    unsigned long (*expose_window_func)  (void* instance, XcpcBackendClosure* closure);
    unsigned long (*key_press_func)      (void* instance, XcpcBackendClosure* closure);
    unsigned long (*key_release_func)    (void* instance, XcpcBackendClosure* closure);
    unsigned long (*button_press_func)   (void* instance, XcpcBackendClosure* closure);
    unsigned long (*button_release_func) (void* instance, XcpcBackendClosure* closure);
    unsigned long (*motion_notify_func)  (void* instance, XcpcBackendClosure* closure);
};

typedef unsigned long (*XcpcBackendHandler)(void* instance, XcpcBackendClosure* closure);

extern void        xcpc_begin             (void);
extern int         xcpc_main              (int* argc, char*** argv);
extern void        xcpc_end               (void);
extern int         xcpc_get_major_version (void);
extern int         xcpc_get_minor_version (void);
extern int         xcpc_get_micro_version (void);
extern const char* xcpc_get_version       (void);
extern const char* xcpc_get_copyright     (void);
extern const char* xcpc_get_comments      (void);
extern const char* xcpc_get_website       (void);
extern const char* xcpc_get_license       (void);
extern const char* xcpc_get_bindir        (void);
extern const char* xcpc_get_libdir        (void);
extern const char* xcpc_get_datdir        (void);
extern const char* xcpc_get_docdir        (void);
extern const char* xcpc_get_resdir        (void);
extern const char* xcpc_get_romdir        (void);
extern const char* xcpc_get_dskdir        (void);
extern const char* xcpc_get_snadir        (void);
extern const char* xcpc_get_joystick0     (void);
extern const char* xcpc_get_joystick1     (void);
extern int         xcpc_get_loglevel      (void);
extern int         xcpc_set_loglevel      (const int loglevel);
extern void        xcpc_println           (const char* format, ...);
extern void        xcpc_errorln           (const char* format, ...);
extern void        xcpc_log_error         (const char* format, ...);
extern void        xcpc_log_alert         (const char* format, ...);
extern void        xcpc_log_print         (const char* format, ...);
extern void        xcpc_log_trace         (const char* format, ...);
extern void        xcpc_log_debug         (const char* format, ...);

extern XcpcCompanyName  xcpc_company_name_from_string  (const char* label);
extern XcpcMachineType  xcpc_machine_type_from_string  (const char* label);
extern XcpcMonitorType  xcpc_monitor_type_from_string  (const char* label);
extern XcpcRefreshRate  xcpc_refresh_rate_from_string  (const char* label);
extern XcpcKeyboardType xcpc_keyboard_type_from_string (const char* label);
extern XcpcMemorySize   xcpc_memory_size_from_string   (const char* label);

extern const char*      xcpc_company_name_to_string    (XcpcCompanyName  value);
extern const char*      xcpc_machine_type_to_string    (XcpcMachineType  value);
extern const char*      xcpc_monitor_type_to_string    (XcpcMonitorType  value);
extern const char*      xcpc_refresh_rate_to_string    (XcpcRefreshRate  value);
extern const char*      xcpc_keyboard_type_to_string   (XcpcKeyboardType value);
extern const char*      xcpc_memory_size_to_string     (XcpcMemorySize   value);

#ifdef __cplusplus
}
#endif

#endif /* __XCPC_LIBXCPC_H__ */
