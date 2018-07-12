/*****************************************************************************
 * cp2_main.c: proteus cp2 test main
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
#include <cp2.h>

#define FAIL_IF_ERROR( cond, ... )\
do\
{\
    if( cond )\
    {\
        fprintf( stderr, __VA_ARGS__ );\
        goto fail;\
    }\
} while( 0 )

int main( int argc, char **argv )
{
  int w, h;
  char *inPath = argv[1];
  char *outPath = argv[2];
  int processed_frames = 0;

#ifdef _WIN32
    _setmode( _fileno( stdin ),  _O_BINARY );
    _setmode( _fileno( stdout ), _O_BINARY );
    _setmode( _fileno( stderr ), _O_BINARY );
#endif

    FAIL_IF_ERROR( !(argc > 1), "Example usage: cp2 input.yuv output.h264 352x288\n" );
    FAIL_IF_ERROR( 2 != sscanf( argv[3], "%dx%d", &w, &h ), "resolution not specified or incorrect\n" );

    cp2_knobs init_knobs = {
      .me = 12,
      .subme = 7,
      .reframes = 1,
      .qp = 0
    };
    x264_cp2_init(inPath, outPath, w, h, init_knobs);
    /* FIXME: switch above line to this when we add error checking
    if (x264_cp2_init(inPath, outPath, w, h, init_knobs) < 0) {
      goto fail;
    }
    */
    if (x264_cp2_setup() < 0) {
      goto fail;
    }

    while (1) {
      if (processed_frames == 100) {
        cp2_knobs updated_knobs = {
          .me = 12,
          .subme = 7,
          .reframes = 1,
          .qp = 40
        };
        cp2_update_knob_settings(updated_knobs);
      }
      int enc_ret = x264_cp2_encode_frame();
      if (enc_ret > 0) {
        break;
      } else if (enc_ret < 0) {
        goto fail;
      }

      double quality = x264_cp2_get_quality();
      fprintf(stderr, "[cp2] quality for frame %d: %f.\n", processed_frames, quality);
      double bitrate = x264_cp2_get_bitrate();
      fprintf(stderr, "[cp2] bitrate for frame %d: %f.\n", processed_frames, bitrate);
      double inter = x264_cp2_get_8x8_inter();
      fprintf(stderr, "[cp2] inter for frame %d: %f%%.\n", processed_frames, inter);
      double intra = x264_cp2_get_8x8_intra();
      fprintf(stderr, "[cp2] intra for frame %d: %f%%.\n", processed_frames, intra);

      processed_frames++;
    }

    return x264_cp2_teardown();

fail:
    fprintf(stderr, "failed from cp2_main.");
    return -1;
}
