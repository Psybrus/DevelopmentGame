Psybrus Development Game
========================

Development game I use to much around with Psybrus development.
Currently it just has some 3D animated lit characters walking round,
but it will evolve into a better set of examples I can use for testing.


To build, setup a folder structure like so:

PsybrusDev/Psybrus 			- should be a clone of https://github.com/neilogd/Psybrus
PsybrusDev/DevelopmentGame	- should be a clone of https://github.com/neilogd/Psybrus_DevelopmentGame

Then to generate the VS project run make_vs2012_build.bat in DevelopmentGame. If you wish to use another IDE or earlier version of VS, duplicate and modify the batch file.

When the project has been built (goes to Build/vs2012), load up in VS and compile. Set the DevelopmentGame project working directory to "../../Dist" before running.
