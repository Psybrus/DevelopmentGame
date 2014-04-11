openal32LibPath = psybrusSDK .. "/External/openal/libs/Win32/"

project "DevelopmentGame"
	kind "WindowedApp"
	language "C++"
	files { "./**.hh", "./**.h", "./**.c", "./**.cpp" }
   debugdir "../Dist"
	includedirs { "./", psybrusSDK .. "/Engine/Source/Shared/", psybrusSDK .. "/Engine/Source/Platforms/Windows/" }

	-- External includes.
	includedirs { 
      psybrusSDK .. "/External/jsoncpp/include", 
      psybrusSDK .. "/External/portaudio/include"
   }

	configuration "windows"
         libdirs {
            openal32LibPath
         }
   		links {
   			-- Windows libs.
   			"user32",
   			"gdi32",
   			"opengl32",
   			"winmm",
            "ws2_32",
            "IPHlpApi",
            "OpenAL32",

   			-- External libs.
            "External_BulletPhysics",
   			"External_freetype",
   			"External_glew",
   			"External_jsoncpp",
   			"External_libb64",
            "External_mongoose",
   			"External_ogg",
            "External_pcre",
   			"External_png",
            "External_portaudio",
   			"External_squish",
   			"External_tremor",
   			"External_zlib",

   			-- Engine libs.
   			"Engine_Shared",
   			"Engine_Windows",
   		}
