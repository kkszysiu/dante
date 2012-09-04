echo "test!!!!!!!!!!!!!!!!"

#wget http://www.lolcats.com/images/u/12/24/lolcatsdotcompromdate.jpg
wget http://dl.google.com/android/ndk/android-ndk-r8b-linux-x86.tar.bz2

tar -jxvf android-ndk-r8b-linux-x86.tar.bz2

scons

echo "This will go into the body of the mail." | mail -s "Hello world" kkszysiu@gmail.com
