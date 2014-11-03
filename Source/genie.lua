
PsyProjectGameExe( solution().name )
	files { "./AutoGenRegisterReflection.cpp", "./**.h", "./**.inl", "./**.cpp" }
   debugdir "../Dist"
   includedirs { 
      "./", 
      "../Psybrus/Engine/Source/Shared/", 
   }
	-- External includes.
	includedirs { 
      "../Psybrus/External/jsoncpp/include", 
   }

   PsyAddEngineLinks {
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
   }
   
   PsyAddExternalLinks {
      -- External libs.
      "External_assimp",
      "External_assimp_contrib",
      "External_BulletPhysics",
      "External_freetype",
      "External_HLSLCrossCompiler",
      "External_jsoncpp",
      "External_libb64",
      "External_pcre",
      "External_png",
      "External_SoLoud",
      "External_squish",
      "External_zlib",
   }

   PsyAddBoostLibs {
      "boost_regex",
      "boost_filesystem",
      "boost_system",
      "boost_wave",
   }

   configuration { "windows or linux-gcc or linux-clang" }
   PsyAddExternalLinks {
      "External_enet",
      "External_glew",
      "External_webby",
   }

   configuration "linux"
      includedirs {
         "../Psybrus/Engine/Source/Platforms/Linux/",
      }

      prebuildcommands {
            -- "python ../../../Psybrus/reflection_parse.py " .. solution().name
      }


   configuration "windows"
      includedirs {
         "../../Psybrus/Engine/Source/Platforms/Windows/",
         BOOST_INCLUDE_PATH
      }

      prebuildcommands {
            "C:\\Python27\\python.exe $(PSYBRUS_SDK)/reflection_parse.py " .. solution().name
      }

      libdirs {
           BOOST_LIB_PATH
      }

