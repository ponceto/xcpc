/*
 * device.h - Copyright (c) 2001-2021 - Olivier Poncet
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
#ifndef __GDEV_DEVICE_H__
#define __GDEV_DEVICE_H__

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/keysym.h>
#include <glib-object.h>

G_BEGIN_DECLS

#define GDEV_TYPE_DEVICE            (gdev_device_get_type())
#define GDEV_DEVICE(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), GDEV_TYPE_DEVICE, GdevDevice))
#define GDEV_DEVICE_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass), GDEV_TYPE_DEVICE, GdevDeviceClass))
#define GDEV_IS_DEVICE(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), GDEV_TYPE_DEVICE))
#define GDEV_IS_DEVICE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), GDEV_TYPE_DEVICE))
#define GDEV_DEVICE_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj), GDEV_TYPE_DEVICE, GdevDeviceClass))

typedef struct _GdevDevice      GdevDevice;
typedef struct _GdevDeviceClass GdevDeviceClass;

typedef void (*GdevDeviceProc)(GdevDevice *device);

struct _GdevDevice {
  GObject parent_instance;
};

struct _GdevDeviceClass {
  GObjectClass parent_class;
  void (*reset)(GdevDevice *device);
  void (*clock)(GdevDevice *device);
};

extern GType       gdev_device_get_type (void);
extern GdevDevice *gdev_device_new      (void);
extern void        gdev_device_reset    (GdevDevice *device);
extern void        gdev_device_clock    (GdevDevice *device);

G_END_DECLS

#endif /* __GDEV_DEVICE_H__ */
