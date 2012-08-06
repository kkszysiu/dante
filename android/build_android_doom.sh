#!/bin/sh

# NDK
ANDROID_NDK=`pwd`/android-ndk-r7c/

mkdir -p ./../neo/android/libjpeg/include/
cp ./../neo/android/libjpeg/jpeg-8c/*.h ./../neo/android/libjpeg/include/

ABI_BIN=`pwd`/android_standalone_toolchain_r7c/bin/
CC=$ABI_BIN/arm-linux-androideabi-gcc
CXX=$ABI_BIN/arm-linux-androideabi-g++
RANLIB=$ABI_BIN/arm-linux-androideabi-ranlib
AR=$ABI_BIN/arm-linux-androideabi-ar
LD=$ABI_BIN/arm-linux-androideabi-ld

export PATH=$ABI_BIN:$PATH

export CC=$ABI_BIN/arm-linux-androideabi-gcc
export CXX=$ABI_BIN/arm-linux-androideabi-g++
export RANLIB=$ABI_BIN/arm-linux-androideabi-ranlib
export AR=$ABI_BIN/arm-linux-androideabi-ar
export LD=$ABI_BIN/arm-linux-androideabi-ld

CPPFLAGS="-DANDROID"

#TOP=$TOP CC=$CC CXX=$CXX RANLIB=$RANLIB AR=$AR scons os=android arch=arm $@
#CC=$CC CXX=$CXX RANLIB=$RANLIB AR=$AR scons os=android arch=arm $@
cd ../neo/

CC=$CC CXX=$CXX RANLIB=$RANLIB AR=$AR LD=$LD scons -j7 arch=arm CC=$CC CXX=$CXX RANLIB=$RANLIB AR=$AR BASEFLAGS=$CPPFLAGS NOCURL=1 ALSA=0 BUILD_GAMEPAK=1 $@

cd ../android/
