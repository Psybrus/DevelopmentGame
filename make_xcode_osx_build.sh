#!/bin/bash

python Psybrus/reflection_parse.py DevelopmentGame

Psybrus/Tools/genie/genie-osx --platform=x64 --toolchain=osx-clang --boostpath=$BOOST_ROOT xcode4
