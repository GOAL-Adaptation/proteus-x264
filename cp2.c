/*****************************************************************************
 * cp2.c: proteus cp2 functions
 *****************************************************************************
 * Copyright (C) 2017 Rice University
 *
 * Derived from sample.c under GPLv2.
 *****************************************************************************
 * Copyright (C) 2014-2017 x264 project
 *
 * Authors: Anton Mitrofanov <BugMaster@narod.ru>
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

#ifdef _WIN32
#include <io.h>       /* _setmode() */
#include <fcntl.h>    /* _O_BINARY */
#endif

#include <stdint.h>
#include <stdio.h>
#include <x264.h>
#include <common/common.h>
#include <cp2.h>

// global variables

static cp2_knobs knobs;

int width, height;
x264_param_t param;
x264_picture_t pic;
x264_picture_t pic_out;
x264_t *h;
int i_frame = 0;
int i_frame_size;
x264_nal_t *nal;
int i_nal;
FILE *fin;
FILE *fout;

void cp2_update_knob_settings(cp2_knobs knobs_settings) {
  knobs = knobs_settings;
  cp2_update_x264_param();
}

void cp2_update_x264_param() {
  for (int i = 0; i < h->param.i_threads; i++) {
    h->thread[i]->param.analyse.i_me_range = knobs.me;
    h->thread[i]->param.analyse.i_subpel_refine = knobs.subme;
    h->thread[i]->param.i_frame_reference = knobs.reframes;
  }
}

int x264_cp2_setup(const char *inputPath, const char *outputPath, int wdth, int hght, cp2_knobs initialKnobSettings) {
  fin = fopen(inputPath, "rb");
  fout = fopen(outputPath, "wb");
  width = wdth;
  height = hght;

  if( x264_param_default_preset( &param, "medium", NULL ) < 0 )
      return -1;

  /* Configure non-default params */
  param.i_width  = width;
  param.i_height = height;
  param.analyse.b_psnr = 1;
  param.rc.i_qp_constant = initialKnobSettings.qp;
  param.rc.i_qp_min = initialKnobSettings.qp;
  param.rc.i_qp_max = initialKnobSettings.qp;

  /* Apply profile restrictions. */
  if( x264_param_apply_profile( &param, "high" ) < 0 )
      return -2;

  if( x264_picture_alloc( &pic, param.i_csp, param.i_width, param.i_height ) < 0 )
      return -3;

  h = x264_encoder_open( &param );
  if( !h )
    return -4;

  cp2_update_knob_settings(initialKnobSettings);

  return 0;
}

int x264_cp2_encode_frame() {
  int luma_size = width * height;
  int chroma_size = luma_size / 4;

  if( fread( pic.img.plane[0], 1, luma_size, fin ) != luma_size )
      return 1;
  if( fread( pic.img.plane[1], 1, chroma_size, fin ) != chroma_size )
      return 1;
  if( fread( pic.img.plane[2], 1, chroma_size, fin ) != chroma_size )
      return 1;

  pic.i_pts = i_frame;
  i_frame_size = x264_encoder_encode( h, &nal, &i_nal, &pic, &pic_out );
  if( i_frame_size < 0 )
      return -1;
  else if( i_frame_size )
  {
      if( !fwrite( nal->p_payload, i_frame_size, 1, fout ) )
          return -2;
  }

  i_frame++;

  return 0;

  // 0   ::= continue the loop
  // > 0 ::= terminate the loop
  // < 0 ::= error
}

int x264_cp2_teardown() {
  while( x264_encoder_delayed_frames( h ) )
  {
      i_frame_size = x264_encoder_encode( h, &nal, &i_nal, NULL, &pic_out );
      if( i_frame_size < 0 )
          return -1;
      else if( i_frame_size )
      {
          if( !fwrite( nal->p_payload, i_frame_size, 1, fout ) )
              return -2;
      }
  }

  fclose(fin);
  fclose(fout);
  x264_encoder_close( h );
  x264_picture_clean( &pic );

  return 0;
}

double x264_cp2_get_quality() {
  return h->stat.f_psnr_average[h->sh.i_type] * 1000.0;
}
