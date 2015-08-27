cd Dist
call DevelopmentGameImporter-vs2013-windows-vs-v120-Debug.exe -c ../Psybrus/Dist/Platforms/android.json
cd ..

cd Build\gmake-android-gcc-arm\project
call python ../../../Psybrus/Scripts/CopyPackedContent.py
adb uninstall com.psybrus.DevelopmentGame
call ant debug install

cd ..\..\..

pause
