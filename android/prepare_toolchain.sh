#!/bin/sh

LOCAL_DIR=`dirname $0`

#echo "LOCAL_DIR: ./$LOCAL_DIR/"

cd "./$LOCAL_DIR/"

echo "Step 0.2: Downloading SDK."

IS_DOWNLOADED=`ls -l | grep "android-sdk_r20.0.1-linux.tgz"`
if [ -z "$IS_DOWNLOADED" ]; then
    wget "http://dl.google.com/android/android-sdk_r20.0.1-linux.tgz"
else
    echo "SDK is already downloaded..."
fi

echo "Step 0.5: Unpacking SDK"

ALREADY_UNPACKED=`ls -l | grep "^d" | grep "android-sdk-linux"`
if [ -z "$ALREADY_UNPACKED" ]; then
    gunzip -c android-sdk_r20.0.1-linux.tgz | tar -xvf -
else
    echo "SDK seems to be already unpacked..."
fi

echo "Step 1: Downloading NDK, r7c version."

IS_DOWNLOADED=`ls -l | grep "android-ndk-r7c-linux-x86.tar.bz2"`
if [ -z "$IS_DOWNLOADED" ]; then
    wget "http://dl.google.com/android/ndk/android-ndk-r7c-linux-x86.tar.bz2"
else
    echo "NDK is already downloaded..."
fi

echo "Step 2: Unpacking NDK."

ALREADY_UNPACKED=`ls -l | grep "^d" | grep "android-ndk-r7c"`
if [ -z "$ALREADY_UNPACKED" ]; then
    tar -jxvf android-ndk-r7c-linux-x86.tar.bz2
else
    echo "NDK seems to be already unpacked..."
fi

echo "Step 3: Creating standalone toolchain."

./android-ndk-r7c/build/tools/make-standalone-toolchain.sh --platform=android-9 --install-dir=`pwd`/android_standalone_toolchain_r7c/

echo "Congratulations! Work of this script is already done! Go run ./build_android_doom.sh now :)"