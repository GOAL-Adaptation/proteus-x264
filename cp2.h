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

#include <stdint.h>
#include "x264.h"

#ifdef __cplusplus
extern "C" {
#endif

// cp2 knobs

typedef struct cp2_knobs {
  int me;
  int subme;
  int reframes;
  int qp;
} cp2_knobs;

void cp2_update_knob_settings(cp2_knobs initialKnobSettings);
void cp2_update_x264_param();

// cp2 meausre and features

double x264_cp2_get_quality();
double x264_cp2_get_bitrate();
double x264_cp2_get_8x8_inter();
double x264_cp2_get_8x8_intra();

// x264 encode functions

void x264_cp2_init(const char *inputPath, const char *outputPath, int wdth, int hght, cp2_knobs knobSettings);
  // FIXME: switch above line to version below when we add error checking
  //int x264_cp2_init(const char *inputPath, const char *outputPath, int wdth, int hght, cp2_knobs knobSettings);
int x264_cp2_setup();
int x264_cp2_encode_frame();
int x264_cp2_teardown();

#ifdef __cplusplus
}
#endif

#endif
