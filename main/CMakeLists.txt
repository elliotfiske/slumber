cmake_minimum_required(VERSION 2.8)
project(slumber)

# Determine if C++11 is supported
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG(“-std=c++11” COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG(“-std=c++0x” COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Oh well!  Networking is disabled.“)
endif()

MESSAGE( STATUS “Cmake cxx flags: ${CMAKE_CXX_FLAGS}" )

if (NOT ${CMAKE_GENERATOR} MATCHES "Xcode" )
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
endif (NOT ${CMAKE_GENERATOR} MATCHES "Xcode" )

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

# Copy over .obj's, .mtl's, etc.'
if (${CMAKE_GENERATOR} MATCHES "Xcode" )

add_custom_target(
    Resources ALL
    ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/resources ${PROJECT_BINARY_DIR}/Debug
    SOURCES ${Resources}
)

else()

add_custom_target(
    Resources ALL
    ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/resources ${PROJECT_BINARY_DIR}/bin
    SOURCES ${Resources}
)

endif (${CMAKE_GENERATOR} MATCHES "Xcode" )

add_dependencies(slumber Resources)