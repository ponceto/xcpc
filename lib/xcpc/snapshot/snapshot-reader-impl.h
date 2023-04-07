/*
 * snapshot-reader-impl.h - Copyright (c) 2001-2023 - Olivier Poncet
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
#ifndef __XCPC_SNAPSHOT_READER_IMPL_H__
#define __XCPC_SNAPSHOT_READER_IMPL_H__

#include <xcpc/libxcpc.h>
#include <xcpc/snapshot/snapshot.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _XcpcSnapshotReader XcpcSnapshotReader;

struct _XcpcSnapshotReader
{
    XcpcSnapshotStatus status;
    XcpcSnapshot*      snapshot;
    FILE*              file;
};

#ifdef __cplusplus
}
#endif

#endif /* __XCPC_SNAPSHOT_READER_IMPL_H__ */
