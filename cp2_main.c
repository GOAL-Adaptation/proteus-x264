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

#ifdef _WIN32
    _setmode( _fileno( stdin ),  _O_BINARY );
    _setmode( _fileno( stdout ), _O_BINARY );
    _setmode( _fileno( stderr ), _O_BINARY );
#endif

    FAIL_IF_ERROR( !(argc > 1), "Example usage: cp2 352x288 <input.yuv >output.h264\n" );
    FAIL_IF_ERROR( 2 != sscanf( argv[1], "%dx%d", &w, &h ), "resolution not specified or incorrect\n" );

    if (x264_cp2_setup(w, h) < 0) {
      goto fail;
    }

    while (1) {
      int enc_ret = x264_cp2_encode_frame();
      if (enc_ret > 0) {
        break;
      } else if (enc_ret < 0) {
        goto fail;
      }
    }

    return x264_cp2_teardown();

fail:
    return -1;
}
