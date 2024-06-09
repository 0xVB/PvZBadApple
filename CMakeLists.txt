# CMakeList.txt : CMake project for PvZBadApple, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("PvZBadApple")

# Add source to this project's executable.
add_library (PvZBadApple SHARED "Source/PvZBadApple.cpp" "Source/PvZBadApple.h" "Source/Include/LawnApp.h" "Source/Include/Listeners.h" "Source/Include/Definitions.h" "Source/Include/Image.h" "Source/Include/Lawn.h" "Source/Include/UIElement.h" "Source/Include/Zombie.h" "Source/Include/Plant.h" "Source/Include/Pickup.h" "Source/Include/PottedPlant.h" "Source/Include/LawnMower.h" "Source/Include/GridItem.h" "Source/Include/DataArray.h" "Source/Include/Projectile.h" "Source/Consts.h" "Source/Hook.h" "Source/Utils.h" "Source/MainFuncs.h" "Source/Include/Compression.h" "Source/Include/SeedBank.h" "Source/Include/SeedPacket.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET PvZBadApple PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
