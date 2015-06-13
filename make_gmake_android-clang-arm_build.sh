#!/bin/bash

python Psybrus/reflection_parse.py DevelopmentGame

Psybrus/Tools/genie/genie-linux --toolchain=android-clang-arm --boostpath=$BOOST_ROOT gmake
