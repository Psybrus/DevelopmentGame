cd Dist
call adb push PackedContent /sdcard/PackedContent
cd ..

cd Build\gmake-android-gcc-arm\project
call python ../../../Psybrus/Scripts/CopyPackedContent.py
call ant debug install

cd ..\..

pause
