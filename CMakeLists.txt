cmake_minimum_required (VERSION 3.12)

# Set options for this project.
project(graph)
#project (${PROJECT_NAME})
set (PROJECT_SOURCE_DECLARATION_DIRECTORY ${PROJECT_SOURCE_DIR}/include)
set (PROJECT_SOURCE_DEFINITION_DIRECTORY ${PROJECT_SOURCE_DIR})
set (MAIN_FILE ${PROJECT_SOURCE_DEFINITION_DIRECTORY}/main.cpp)

# Set options used by "CMake".
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)

# Set options used by "FindBoost.cmake".



# find_package (Boost 1.67.0 REQUIRED COMPONENTS) #system filesystem)
# if (Boost_FOUND)
#     include_directories (SYSTEM ${Boost_INCLUDE_DIR})
#     add_definitions(-DBOOST_ERROR_CODE_HEADER_ONLY)
#     set (Boost_NO_SYSTEM_PATHS ON)
#     set (Boost_USE_MULTITHREADED ON)
#     set (Boost_USE_STATIC_LIBS ON)
#     set (Boost_USE_STATIC_RUNTIME ON)
#     set (BOOST_ALL_DYN_LINK ON)
# endif ()

include_directories (${PROJECT_SOURCE_DECLARATION_DIRECTORY})

# "Add an executable to the project using the specified source files."
add_executable (${PROJECT_NAME} ${MAIN_FILE})
# SET(CMAKE_BUILD_TOOL C:/MinGW/bin/mingw32-make.exe)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set_target_properties(${PROJECT_NAME}  PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    COMPILE_OPTIONS "-Wpedantic;-Wall;-Wextra"
    #COMPILE_OPTIONS "-Wstrict"
)
# "Link a target to given libraries."
# target_link_libraries (${PROJECT_NAME} ${Boost_LIBRARIES})
