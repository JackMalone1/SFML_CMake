#Require a minimum version of cmake.
cmake_minimum_required(VERSION 3.15)

#Set the name of the project.
project(GGJ2020
    VERSION 1.0
    DESCRIPTION "GGJ2020"
    LANGUAGES CXX)

# Global flag to cause add_library() to create shared libraries if on.
set(BUILD_SHARED_LIBS FALSE)

#Adds a subdirectory to the build.
add_subdirectory(libs/sfml)
add_subdirectory(libs/entt)
add_subdirectory(libs/lua)
include_directories(libs/LuaBridge)

# File manipulation command. (Automatically checks and finds all source files at build time)
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS "src/*.cpp")

# Add an executable to the project using the specified source files.
add_executable(${PROJECT_NAME} ${SRC_FILES} "src/Components.h" "src/Entity.h"   "src/Globals.h")

if(WIN32)
    add_custom_command(TARGET ${PROJECT_NAME}
                        PRE_BUILD
                        COMMAND cmd copy_assets.bat)
endif()

# Add expected compiler features to a target.
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)


# Add include directories to a target.
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/src)

# Specify libraries or flags to use when linking a given target. (Linking SFML)
target_link_libraries(${PROJECT_NAME} PRIVATE sfml-system sfml-audio sfml-graphics sfml-network sfml-window lua::lib)


