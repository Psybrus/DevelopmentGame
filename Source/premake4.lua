project "DevelopmentGame"
	kind "WindowedApp"
	language "C++"
	files { "./**.h", "./**.inl", "./**.cpp" }
   debugdir "../Dist"
   includedirs { 
      "./", 
      psybrusSDK .. "/Engine/Source/Shared/", 
      psybrusSDK .. "/Engine/Source/Platforms/Windows/"
   }
	-- External includes.
	includedirs { 
      psybrusSDK .. "/External/jsoncpp/include", 
      boostInclude
   }

	configuration "windows"
         libdirs {
            openal32LibPath,
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
            "OpenAL32",

            -- Engine libs.
            "Engine"
         }
