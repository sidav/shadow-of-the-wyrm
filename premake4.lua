#!lua
solution "ShadowOfTheWyrm"
  configurations { "Debug", "Release" }

project "ShadowOfTheWyrm"
  kind "ConsoleApp"
  language "C++"
  files { "**.hpp", "**.cpp" }

  -- TODO: Once premake supports per-configuration exclude lists,
  -- create MapTester/SaveConverter configurations that exclude the appropriate
  -- files.
  excludes { "source/SaveConverter.cpp", "source/MapTester.cpp" }

  includedirs { "include",
                "controller/include",
                "display/include",
                "display/animation/include",
                "engine/include",
                "engine/actions/include",
                "engine/actions/read/include",
                "engine/calculators/include",
                "engine/combat/include",
                "engine/commands/include",
                "engine/commands/equipment/include",
                "engine/commands/help/include",
                "engine/commands/inventory/include",
                "engine/commands/magic/include",
                "engine/commands/screens/include",
                "engine/commands/select_tile/include",
                "engine/commands/skills/include",
                "engine/conversion/include",
                "engine/creatures/include",
                "engine/decision_strategies/deity/include",
                "engine/decision_strategies/include",
                "engine/decision_strategies/magic/include",
                "engine/decision_strategies/search/include",
                "engine/description/include",
                "engine/effects/include",
                "engine/field_of_view/include",
                "engine/generators/include",
                "engine/generators/crypts/include",
                "engine/generators/fortresses/include",
                "engine/generators/gardens/include",
                "engine/generators/rooms/include",
                "engine/generators/settlements/include",
                "engine/generators/tile_config/include",
                "engine/generators/worship_sites/include",
                "engine/items/include",
                "engine/magic/include",
                "engine/maps/include",
                "engine/maps/tiles/include",
                "engine/maps/tiles/features/include",
                "engine/screens/include",
                "engine/scripting/include",
                "engine/skills/include",
                "engine/serialization/include",
                "engine/strings/include",
                "engine/time/include",
                "engine/translators/include",
                "engine/XML/include",
                "engine/XML/custom_maps/include",
                "engine/XML/custom_maps/features/include",
                "engine/XML/custom_maps/world/include",
                "world/include",
                "world/magic/include",
                "world/materials/include",
                "world/religion/include",
                "world/seasons/include",
                "world/tiles/include",
                "world/tiles/features/include" }
  excludes { "**_test.cpp" }
  links { "z", "lua", "xerces-c", "ncurses", "gtest" }
  flags { "ExtraWarnings" }

  -- Ignore SaveConverter, MapTester configs.
  configuration "Debug"
    defines { "_DEBUG", "DEBUG", "UNIT_TESTS" }
    flags { "Symbols" }
    excludes { "source/MapTester.cpp", "source/SaveConverter.cpp" }

  configuration "Release"
    defines { "NDEBUG" }
    flags { "Optimize" }

