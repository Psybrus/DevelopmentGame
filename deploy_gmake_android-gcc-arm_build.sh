cd Dist
./DevelopmentGameImporter-gmake-osx-clang-Debug -c ../Psybrus/Dist/Platforms/android.json
cd ..

cd Build/gmake-android-gcc-arm/project
python ../../../Psybrus/Scripts/CopyPackedContent.py
ant debug install

cd ../..
