#!/bin/sh

#SDK_TOOLS="/home/kkszysiu/Pobrane/android-sdk-linux/tools/"
PROJECT_PATH="doom3_android_project"

mkdir -p ./$PROJECT_PATH/libs/armeabi/

#cp ../neo/android/libjpeg/libs/armeabi/libjpeg.so ./$PROJECT_PATH/libs/armeabi/
cp ../neo/doom.x86_64 ./$PROJECT_PATH/libs/armeabi/libdoom3.so
cp ../neo/gamex86_64-base.so ./$PROJECT_PATH/libs/armeabi/libgame-base.so

cd ./doom3_android_project/
ant debug
cd ..
android-sdk-linux/platform-tools/adb install -r ./doom3_android_project/bin/Doom3-debug.apk
