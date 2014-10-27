
project( solution().name )
	kind "WindowedApp"
	language "C++"
	files { "./AutoGenRegisterReflection.cpp", "./**.h", "./**.inl", "./**.cpp" }
   debugdir "../Dist"
   includedirs { 
      "./", 
      "../../Psybrus/Engine/Source/Shared/", 
   }
	-- External includes.
	includedirs { 
      "../../Psybrus/External/jsoncpp/include", 
   }

  configuration "linux"
      includedirs {
         "../../Psybrus/Engine/Source/Platforms/Linux/",
      }

      prebuildcommands {
            -- "python ../../../Psybrus/reflection_parse.py " .. solution().name
      }

      links {
         -- Engine libs.
         "Engine",

         "Engine_System_Sound",
         "Engine_System_Scene",
         "Engine_System_Renderer",
         "Engine_System_Os",
         "Engine_System_Network",
         "Engine_System_Debug",
         "Engine_System_Content",
         "Engine_System_File",
         "Engine_System",
         "Engine_Serialisation",
         "Engine_Reflection",
         "Engine_Math",
         "Engine_Import",
         "Engine_Events",
         "Engine_Base",


         -- External libs.
         "External_assimp",
         "External_assimp_contrib",
         "External_BulletPhysics",
         "External_enet",
         "External_freetype",
         "External_glew",
         "External_HLSLCrossCompiler",
         "External_jsoncpp",
         "External_libb64",
         "External_pcre",
         "External_png",
         "External_SoLoud",
         "External_squish",
         "External_webby",
         "External_zlib",

         -- Boost libs.
         "boost_regex",
         "boost_filesystem",
         "boost_system",
         "boost_wave",

         -- Linux libs.
         "X11",
         "GL",
         "pthread",
         "SDL2"
      }

	configuration "windows"
      includedirs {
         "../../Psybrus/Engine/Source/Platforms/Windows/",
         boostInclude
      }

      prebuildcommands {
            "C:\\Python27\\python.exe $(PSYBRUS_SDK)/reflection_parse.py " .. solution().name
      }

      libdirs {
           boostLib
      }

      links {
         -- Windows libs.
         "user32",
         "gdi32",
         "opengl32",
         "winmm",
         "ws2_32",
         "IPHlpApi",

         -- Engine libs.
         "Engine",

         "Engine_System_Sound",
         "Engine_System_Scene",
         "Engine_System_Renderer",
         "Engine_System_Os",
         "Engine_System_Network",
         "Engine_System_Debug",
         "Engine_System_Content",
         "Engine_System_File",
         "Engine_System",
         "Engine_Serialisation",
         "Engine_Reflection",
         "Engine_Math",
         "Engine_Import",
         "Engine_Events",
         "Engine_Base",


         -- External libs.
         "External_assimp",
         "External_assimp_contrib",
         "External_BulletPhysics",
         "External_enet",
         "External_freetype",
         "External_glew",
         "External_HLSLCrossCompiler",
         "External_jsoncpp",
         "External_libb64",
         "External_pcre",
         "External_png",
         "External_SoLoud",
         "External_squish",
         "External_webby",
         "External_zlib",
      }

