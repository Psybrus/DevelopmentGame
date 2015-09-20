PsyProjectGameLib( solution().name )
	files { "./**.h", "./**.inl", "./**.cpp" }
	files { "./AutoGenRegisterReflection.cpp" }


PsyProjectGameExe( solution().name )

PsyProjectImporterExe( solution().name )
