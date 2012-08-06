This is Doom 3 Android repository. Based on Oliver McFadden's work on GLES 2 port of Doom 3.


WARNING! This will work only on Linux 64-bit machines!!
WARNING2! It is highly experimental.
It might kill your kitten, destroy your house and start the mayan apocalypse.
Use it at your own risk. Have Fun! :)

If you already cloned this branch, make sure youre in 'android' dir of dante's repo. (ex. ~/Projekty/gry/dante/android in my case)

Then I prepared handy script for your pleasure that will prepare NDK and all required stuff related to NDK and toolchain...

So just run:

chmod +x prepare_toolchain.sh
./prepare_toolchain.sh

After that you should have correct NDK and prepared standalone toolchain that next script will use.

Now run:

./build_android_doom.sh

And pray, pray then pray again. It should compile Doom 3 libraries... if something fails there... well then you have a bad time, right? :)
You can create new issue after that or something but I assume everything will go smooth and straight and you will see this line:
    
scons: done building targets.

It means that propably you're lucky enough and everything went successfuly :)

Assuming youre still in 'dante/android/' directory, you need to did it this way:

./android-sdk-linux/tools/android

New window should shows up. Then you need to install couple of things from it... things that you need:

    Tools/Android SDK Platform-tools
    Android 2.3.3 (API 10)/SDK Platform

If you installed it then you can close this window and back to your terminal again.

Now connect your device through USB cable to your PC and let the magic happpen...

Please run script:

./buildit.sh

To build your Doom3 apk and install it on your phone (if your phone is connected through USB in developer mode)

Sadly... thats not all. Of course your game needs assets on your sdcard, so you need to put whole pak00*.pk4 files on your SD card.

I did it this way:

./android-sdk-linux/platform-tools/adb shell

It will open shell on your phone, now:

mkdir -p /sdcard/Android/data/com.kkszysiu.doom3/base/
exit

It will prepare required dirs on your SD card. Type in your terminal:

./android-sdk-linux/platform-tools/adb push /path/to/your/pak00* /sdcard/Android/data/com.kkszysiu.doom3/base/

Oh you should clone gl2progs repo from https://github.com/omcfadde/gl2progs and put that to /sdcard/Android/data/com.kkszysiu.doom3/base/, for example this way:
    
git clone https://github.com/omcfadde/gl2progs.git
./android-sdk-linux/platform-tools/adb push gl2progs /sdcard/Android/data/com.kkszysiu.doom3/base/gl2progs/

After that you should open:

./android-sdk-linux/platform-tools/adb logcat for example from your another terminal window to watch on logs from your phone and try to run Doom3Activity on your phone.

Your Doom 3 should load after that. (But be patient, it's pretty slow to load everything)
Sometimes you will get "Application not responding" warnings. Please touch on "Wait" then... this is happening because Doom 3 loading screen is blocking whole main thread and Android thinks that application is freezed...

You must take care of your phone screen, you need to prevent it from goin black because saving/restoring application state is not implemented yet! So touch your screen couple of times when loading a game... you will be happy and screen will be happy too :)

If something not working please create new issue with description and attached logcat results if possible.

If you have any questions, please contact me via github, gtalk, email or whatever.

Enjoy!

FAQ:
    TODO: Write why we're using R7C
    TODO: Questions?
