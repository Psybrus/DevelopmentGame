cd Build\gmake-android-gcc-arm\project
call ant debug

call adb install -r bin\DevelopmentGame-debug.apk
call adb shell logcat

pause