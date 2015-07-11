cd Projects\gmake-android-gcc-arm
set OLD_PATH=%PATH%
PATH=C:\Windows\System32
%ANDROID_NDK%\prebuilt\windows-x86_64\bin\make -j 5
PATH=%OLD_PATH%

cd ..\..

pause