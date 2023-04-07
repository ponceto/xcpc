#!/bin/sh
#
# ci-prepare.sh - Copyright (c) 2001-2023 - Olivier Poncet
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

# ----------------------------------------------------------------------------
# variables
# ----------------------------------------------------------------------------

arg_prefix="/opt/xcpc"

# ----------------------------------------------------------------------------
# debug
# ----------------------------------------------------------------------------

set -x

# ----------------------------------------------------------------------------
# autoreconf
# ----------------------------------------------------------------------------

autoreconf -v -i -f                                                  || exit 1

# ----------------------------------------------------------------------------
# configure the build system
# ----------------------------------------------------------------------------

./configure --prefix="${arg_prefix}"                                 || exit 1

# ----------------------------------------------------------------------------
# create the tarball
# ----------------------------------------------------------------------------

make dist                                                            || exit 1

# ----------------------------------------------------------------------------
# End-Of-File
# ----------------------------------------------------------------------------
