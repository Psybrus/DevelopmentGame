cd Dist
call adb push PackedContent /sdcard/PackedContent
cd ..

cd Build\gmake-android-gcc-arm\project
call ant debug
call adb uninstall com.psybrus.DevelopmentGame
call adb install -r bin\DevelopmentGame-debug.apk

cd ..\..

pause