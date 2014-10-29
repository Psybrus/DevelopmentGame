
-- Run psybrus scripts.
psybrusSDK = "Psybrus"
dofile( "Psybrus/Scripts/Psybrus.lua" )

-- Check boost path on windows.
if os.is("windows") then
	boostRoot = os.getenv("BOOST_ROOT")
	if boostRoot == nil then
		print "Missing BOOST_ROOT: Defaulting to C:/Boost"
		boostRoot = "C:/Boost"
	else
		print ( "BOOST_ROOT: " .. boostRoot )
	end
end

psybrusSDK = os.getenv("PSYBRUS_SDK") or "../Psybrus"
boostRoot = os.getenv("BOOST_ROOT") or "C:/Boost"
boostInclude = boostRoot .. "/include/boost-1_56"
boostLib = boostRoot .. "/lib"

-- Solution.
PsySolutionGame( "DevelopmentGame" )

-- Build externals.
dofile ("Psybrus/External/genie.lua")

-- Build engine.
dofile ("Psybrus/Engine/genie.lua")

-- Build game source.
dofile ("./Source/genie.lua")

