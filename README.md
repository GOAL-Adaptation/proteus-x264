# proteus-x264

## Install dependencies

### macOS

- `brew install nasm`

### Linux

`nasm` with minimal version of 2.13 from http://www.nasm.us is suggested by `x264`.
Pre-built packages for `nasm` are available for RPM-based Linux distributions,
otherwise, a local build and installation is needed.
Detailed instructions can be found at `nasm` website, but here are instructions that confirmed to work on Ubuntu 16.04:
```
mkdir nasm_build
cd nasm_build
wget http://www.nasm.us/pub/nasm/releasebuilds/2.13.01/nasm-2.13.01.tar.bz2
tar xjvf nasm-2.13.01.tar.bz2
cd nasm-2.13.01
./autogen.sh
PATH="$HOME/bin:$PATH" ./configure --prefix="$HOME/ffmpeg_build" --bindir="$HOME/bin"
PATH="$HOME/bin:$PATH" make
make install
```

## Configure

`./configure --enable-pic`

## Build

`make cp2`

## Installation

`make install-cp2`

## Uninstallation

`make uninstall`
