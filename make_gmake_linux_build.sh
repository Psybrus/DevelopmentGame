#!/bin/bash

python Psybrus/reflection_parse.py DevelopmentGame

Psybrus/Tools/genie/genie-linux --os=linux --platform=x64 --toolchain=linux-clang gmake
Psybrus/Tools/genie/genie-linux --os=linux --platform=x64 --toolchain=linux-gcc gmake
Psybrus/Tools/genie/genie-linux --os=linux --toolchain=asmjs gmake

