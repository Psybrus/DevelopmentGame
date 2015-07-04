#!/bin/bash

python Psybrus/reflection_parse.py DevelopmentGame

Psybrus/Tools/genie/genie-linux --platform=x64 --toolchain=windows-mingw-gcc --boostpath=$BOOST_ROOT gmake
