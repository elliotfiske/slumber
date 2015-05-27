cmake_minimum_required(VERSION 2.8)
project(slumber)

# Determine if C++11 is supported
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)

# Compiler-specific C++11 activation.
if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
    execute_process(
    COMMAND ${CMAKE_CXX_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
    if (NOT (GCC_VERSION VERSION_GREATER 4.7 OR GCC_VERSION VERSION_EQUAL 4.7))
        message(STATUS "****NOTE: C++11 compiler not found.  Disabling Networking!!!")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
        #        add_definitions(-DTHREADS)
    else()
        message(STATUS "I see you have GCC 4.8 or above! nice. Enabling networking.")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
        add_definitions(-DTHREADS)
    endif ()

elseif ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
    message(STATUS "I see you're on a mac!  Or using Clang otherwise.  Networking enabled!")
    add_definitions(-DTHREADS)
else ()
    message(STATUS "****NOTE: C++11 compiler not found.  Disabling Networking!!!")
endif ()

set(SRC_DIR "${PROJECT_SOURCE_DIR}/source")
set(DEP_DIR "${PROJECT_SOURCE_DIR}/dependencies")

# Source / header files
include("sources.txt")

# GLFW
set(GLFW_DIR "${DEP_DIR}/glfw")
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Build the GLFW example programs")
set(GLFW_BUILD_TESTS OFF CACHE BOOL "Build the GLFW test programs")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "Build the GLFW documentation")
set(GLFW_INSTALL OFF CACHE BOOL "Generate installation target")
add_subdirectory(${GLFW_DIR})
list(APPEND INCLUDES
   ${GLFW_DIR}/include
)
list(APPEND LIBS
   glfw ${GLFW_LIBRARIES}
)

# glad
set(GLAD_DIR "${DEP_DIR}/glad")
list(APPEND LIB_SOURCES
   ${GLAD_DIR}/src/glad.c
)
list(APPEND INCLUDES
   ${GLAD_DIR}/include
)

# dl libs
list(APPEND LIBS
   ${CMAKE_DL_LIBS}
)

# SFML
if (${CMAKE_GENERATOR} MATCHES "Xcode" )
    set(SFML_DIR "${DEP_DIR}/SFML-2.3-osx")
    list(APPEND INCLUDES
       ${SFML_DIR}/include
    )
    list(APPEND LIBS
       ${SFML_DIR}/lib/libsfml-system.dylib
    )
    list(APPEND LIBS
        ${SFML_DIR}/lib/libsfml-audio.dylib
    )
else()
    set(SFML_DIR "${DEP_DIR}/SFML-2.0")
    list(APPEND INCLUDES
        ${SFML_DIR}/include
    )
    list(APPEND LIBS
        ${SFML_DIR}/lib/libsfml-system.so
    )
    list(APPEND LIBS
        ${SFML_DIR}/lib/libsfml-audio.so
    )
endif (${CMAKE_GENERATOR} MATCHES "Xcode" )


# pthread libs
find_package (Threads)
list(APPEND LIBS
   ${CMAKE_THREAD_LIBS_INIT}
)

foreach(HEADER ${HEADERS})
   list(APPEND ABS_HEADERS
      "${SRC_DIR}/${HEADER}"
   )
endforeach(HEADER)

foreach(SOURCE ${SOURCES})
   list(APPEND ABS_SOURCES
      "${SRC_DIR}/${SOURCE}"
   )
endforeach(SOURCE)

include_directories(${INCLUDES})
add_executable(${PROJECT_NAME} ${ABS_SOURCES} ${ABS_HEADERS} ${LIB_SOURCES}
	
)
target_link_libraries(${PROJECT_NAME} ${LIBS})

# Any stuff that's XCode specific. Currently has stuff specific to Elliot's machine.
if (${CMAKE_GENERATOR} MATCHES "Xcode" )
    add_definitions(-DXCODE_IS_TERRIBLE)
endif (${CMAKE_GENERATOR} MATCHES "Xcode" )
