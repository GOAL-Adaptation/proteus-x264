/*****************************************************************************
 * cp2.h: proteus cp2 public header
 *****************************************************************************
 * Copyright (C) 2017 Rice University
 *
 * Derived from x264.h under GPLv2.
 *****************************************************************************
 * Copyright (C) 2003-2017 x264 project
 *
 * Authors: Laurent Aimar <fenrir@via.ecp.fr>
 *          Loren Merritt <lorenm@u.washington.edu>
 *          Fiona Glaser <fiona@x264.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02111, USA.
 *
 * This program is also available under a commercial proprietary license.
 * For more information, contact us at licensing@x264.com.
 *****************************************************************************/

#ifndef X264_CP2_H
#define X264_CP2_H

#ifdef __cplusplus
extern "C" {
#endif

int x264_cp2_setup();
int x264_cp2_encode_frame();
int x264_cp2_teardown();
int x264_cp2_get_quality();

#ifdef __cplusplus
}
#endif

#endif
