# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list

# Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = //home/efiske/slumber/lab1/Cmake

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = //home/efiske/slumber/lab1/Cmake

# Include any dependencies generated for this target.
include CMakeFiles/Lab1.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Lab1.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Lab1.dir/flags.make

CMakeFiles/Lab1.dir/source/main.cpp.o: CMakeFiles/Lab1.dir/flags.make
CMakeFiles/Lab1.dir/source/main.cpp.o: source/main.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report //home/efiske/slumber/lab1/Cmake/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/Lab1.dir/source/main.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/Lab1.dir/source/main.cpp.o -c //home/efiske/slumber/lab1/Cmake/source/main.cpp

CMakeFiles/Lab1.dir/source/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Lab1.dir/source/main.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E //home/efiske/slumber/lab1/Cmake/source/main.cpp > CMakeFiles/Lab1.dir/source/main.cpp.i

CMakeFiles/Lab1.dir/source/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Lab1.dir/source/main.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S //home/efiske/slumber/lab1/Cmake/source/main.cpp -o CMakeFiles/Lab1.dir/source/main.cpp.s

CMakeFiles/Lab1.dir/source/main.cpp.o.requires:
.PHONY : CMakeFiles/Lab1.dir/source/main.cpp.o.requires

CMakeFiles/Lab1.dir/source/main.cpp.o.provides: CMakeFiles/Lab1.dir/source/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/Lab1.dir/build.make CMakeFiles/Lab1.dir/source/main.cpp.o.provides.build
.PHONY : CMakeFiles/Lab1.dir/source/main.cpp.o.provides

CMakeFiles/Lab1.dir/source/main.cpp.o.provides.build: CMakeFiles/Lab1.dir/source/main.cpp.o

CMakeFiles/Lab1.dir/source/actor.cpp.o: CMakeFiles/Lab1.dir/flags.make
CMakeFiles/Lab1.dir/source/actor.cpp.o: source/actor.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report //home/efiske/slumber/lab1/Cmake/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/Lab1.dir/source/actor.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/Lab1.dir/source/actor.cpp.o -c //home/efiske/slumber/lab1/Cmake/source/actor.cpp

CMakeFiles/Lab1.dir/source/actor.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Lab1.dir/source/actor.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E //home/efiske/slumber/lab1/Cmake/source/actor.cpp > CMakeFiles/Lab1.dir/source/actor.cpp.i

CMakeFiles/Lab1.dir/source/actor.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Lab1.dir/source/actor.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S //home/efiske/slumber/lab1/Cmake/source/actor.cpp -o CMakeFiles/Lab1.dir/source/actor.cpp.s

CMakeFiles/Lab1.dir/source/actor.cpp.o.requires:
.PHONY : CMakeFiles/Lab1.dir/source/actor.cpp.o.requires

CMakeFiles/Lab1.dir/source/actor.cpp.o.provides: CMakeFiles/Lab1.dir/source/actor.cpp.o.requires
	$(MAKE) -f CMakeFiles/Lab1.dir/build.make CMakeFiles/Lab1.dir/source/actor.cpp.o.provides.build
.PHONY : CMakeFiles/Lab1.dir/source/actor.cpp.o.provides

CMakeFiles/Lab1.dir/source/actor.cpp.o.provides.build: CMakeFiles/Lab1.dir/source/actor.cpp.o

CMakeFiles/Lab1.dir/source/gamestate.cpp.o: CMakeFiles/Lab1.dir/flags.make
CMakeFiles/Lab1.dir/source/gamestate.cpp.o: source/gamestate.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report //home/efiske/slumber/lab1/Cmake/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/Lab1.dir/source/gamestate.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/Lab1.dir/source/gamestate.cpp.o -c //home/efiske/slumber/lab1/Cmake/source/gamestate.cpp

CMakeFiles/Lab1.dir/source/gamestate.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Lab1.dir/source/gamestate.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E //home/efiske/slumber/lab1/Cmake/source/gamestate.cpp > CMakeFiles/Lab1.dir/source/gamestate.cpp.i

CMakeFiles/Lab1.dir/source/gamestate.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Lab1.dir/source/gamestate.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S //home/efiske/slumber/lab1/Cmake/source/gamestate.cpp -o CMakeFiles/Lab1.dir/source/gamestate.cpp.s

CMakeFiles/Lab1.dir/source/gamestate.cpp.o.requires:
.PHONY : CMakeFiles/Lab1.dir/source/gamestate.cpp.o.requires

CMakeFiles/Lab1.dir/source/gamestate.cpp.o.provides: CMakeFiles/Lab1.dir/source/gamestate.cpp.o.requires
	$(MAKE) -f CMakeFiles/Lab1.dir/build.make CMakeFiles/Lab1.dir/source/gamestate.cpp.o.provides.build
.PHONY : CMakeFiles/Lab1.dir/source/gamestate.cpp.o.provides

CMakeFiles/Lab1.dir/source/gamestate.cpp.o.provides.build: CMakeFiles/Lab1.dir/source/gamestate.cpp.o

CMakeFiles/Lab1.dir/source/windowsetup.cpp.o: CMakeFiles/Lab1.dir/flags.make
CMakeFiles/Lab1.dir/source/windowsetup.cpp.o: source/windowsetup.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report //home/efiske/slumber/lab1/Cmake/CMakeFiles $(CMAKE_PROGRESS_4)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/Lab1.dir/source/windowsetup.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/Lab1.dir/source/windowsetup.cpp.o -c //home/efiske/slumber/lab1/Cmake/source/windowsetup.cpp

CMakeFiles/Lab1.dir/source/windowsetup.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Lab1.dir/source/windowsetup.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E //home/efiske/slumber/lab1/Cmake/source/windowsetup.cpp > CMakeFiles/Lab1.dir/source/windowsetup.cpp.i

CMakeFiles/Lab1.dir/source/windowsetup.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Lab1.dir/source/windowsetup.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S //home/efiske/slumber/lab1/Cmake/source/windowsetup.cpp -o CMakeFiles/Lab1.dir/source/windowsetup.cpp.s

CMakeFiles/Lab1.dir/source/windowsetup.cpp.o.requires:
.PHONY : CMakeFiles/Lab1.dir/source/windowsetup.cpp.o.requires

CMakeFiles/Lab1.dir/source/windowsetup.cpp.o.provides: CMakeFiles/Lab1.dir/source/windowsetup.cpp.o.requires
	$(MAKE) -f CMakeFiles/Lab1.dir/build.make CMakeFiles/Lab1.dir/source/windowsetup.cpp.o.provides.build
.PHONY : CMakeFiles/Lab1.dir/source/windowsetup.cpp.o.provides

CMakeFiles/Lab1.dir/source/windowsetup.cpp.o.provides.build: CMakeFiles/Lab1.dir/source/windowsetup.cpp.o

CMakeFiles/Lab1.dir/source/GLSL.cpp.o: CMakeFiles/Lab1.dir/flags.make
CMakeFiles/Lab1.dir/source/GLSL.cpp.o: source/GLSL.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report //home/efiske/slumber/lab1/Cmake/CMakeFiles $(CMAKE_PROGRESS_5)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/Lab1.dir/source/GLSL.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/Lab1.dir/source/GLSL.cpp.o -c //home/efiske/slumber/lab1/Cmake/source/GLSL.cpp

CMakeFiles/Lab1.dir/source/GLSL.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Lab1.dir/source/GLSL.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E //home/efiske/slumber/lab1/Cmake/source/GLSL.cpp > CMakeFiles/Lab1.dir/source/GLSL.cpp.i

CMakeFiles/Lab1.dir/source/GLSL.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Lab1.dir/source/GLSL.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S //home/efiske/slumber/lab1/Cmake/source/GLSL.cpp -o CMakeFiles/Lab1.dir/source/GLSL.cpp.s

CMakeFiles/Lab1.dir/source/GLSL.cpp.o.requires:
.PHONY : CMakeFiles/Lab1.dir/source/GLSL.cpp.o.requires

CMakeFiles/Lab1.dir/source/GLSL.cpp.o.provides: CMakeFiles/Lab1.dir/source/GLSL.cpp.o.requires
	$(MAKE) -f CMakeFiles/Lab1.dir/build.make CMakeFiles/Lab1.dir/source/GLSL.cpp.o.provides.build
.PHONY : CMakeFiles/Lab1.dir/source/GLSL.cpp.o.provides

CMakeFiles/Lab1.dir/source/GLSL.cpp.o.provides.build: CMakeFiles/Lab1.dir/source/GLSL.cpp.o

CMakeFiles/Lab1.dir/source/tiny_obj_loader.cc.o: CMakeFiles/Lab1.dir/flags.make
CMakeFiles/Lab1.dir/source/tiny_obj_loader.cc.o: source/tiny_obj_loader.cc
	$(CMAKE_COMMAND) -E cmake_progress_report //home/efiske/slumber/lab1/Cmake/CMakeFiles $(CMAKE_PROGRESS_6)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/Lab1.dir/source/tiny_obj_loader.cc.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/Lab1.dir/source/tiny_obj_loader.cc.o -c //home/efiske/slumber/lab1/Cmake/source/tiny_obj_loader.cc

CMakeFiles/Lab1.dir/source/tiny_obj_loader.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Lab1.dir/source/tiny_obj_loader.cc.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E //home/efiske/slumber/lab1/Cmake/source/tiny_obj_loader.cc > CMakeFiles/Lab1.dir/source/tiny_obj_loader.cc.i

CMakeFiles/Lab1.dir/source/tiny_obj_loader.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Lab1.dir/source/tiny_obj_loader.cc.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S //home/efiske/slumber/lab1/Cmake/source/tiny_obj_loader.cc -o CMakeFiles/Lab1.dir/source/tiny_obj_loader.cc.s

CMakeFiles/Lab1.dir/source/tiny_obj_loader.cc.o.requires:
.PHONY : CMakeFiles/Lab1.dir/source/tiny_obj_loader.cc.o.requires

CMakeFiles/Lab1.dir/source/tiny_obj_loader.cc.o.provides: CMakeFiles/Lab1.dir/source/tiny_obj_loader.cc.o.requires
	$(MAKE) -f CMakeFiles/Lab1.dir/build.make CMakeFiles/Lab1.dir/source/tiny_obj_loader.cc.o.provides.build
.PHONY : CMakeFiles/Lab1.dir/source/tiny_obj_loader.cc.o.provides

CMakeFiles/Lab1.dir/source/tiny_obj_loader.cc.o.provides.build: CMakeFiles/Lab1.dir/source/tiny_obj_loader.cc.o

CMakeFiles/Lab1.dir/source/RenderingHelper.cpp.o: CMakeFiles/Lab1.dir/flags.make
CMakeFiles/Lab1.dir/source/RenderingHelper.cpp.o: source/RenderingHelper.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report //home/efiske/slumber/lab1/Cmake/CMakeFiles $(CMAKE_PROGRESS_7)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/Lab1.dir/source/RenderingHelper.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/Lab1.dir/source/RenderingHelper.cpp.o -c //home/efiske/slumber/lab1/Cmake/source/RenderingHelper.cpp

CMakeFiles/Lab1.dir/source/RenderingHelper.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Lab1.dir/source/RenderingHelper.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E //home/efiske/slumber/lab1/Cmake/source/RenderingHelper.cpp > CMakeFiles/Lab1.dir/source/RenderingHelper.cpp.i

CMakeFiles/Lab1.dir/source/RenderingHelper.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Lab1.dir/source/RenderingHelper.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S //home/efiske/slumber/lab1/Cmake/source/RenderingHelper.cpp -o CMakeFiles/Lab1.dir/source/RenderingHelper.cpp.s

CMakeFiles/Lab1.dir/source/RenderingHelper.cpp.o.requires:
.PHONY : CMakeFiles/Lab1.dir/source/RenderingHelper.cpp.o.requires

CMakeFiles/Lab1.dir/source/RenderingHelper.cpp.o.provides: CMakeFiles/Lab1.dir/source/RenderingHelper.cpp.o.requires
	$(MAKE) -f CMakeFiles/Lab1.dir/build.make CMakeFiles/Lab1.dir/source/RenderingHelper.cpp.o.provides.build
.PHONY : CMakeFiles/Lab1.dir/source/RenderingHelper.cpp.o.provides

CMakeFiles/Lab1.dir/source/RenderingHelper.cpp.o.provides.build: CMakeFiles/Lab1.dir/source/RenderingHelper.cpp.o

CMakeFiles/Lab1.dir/source/camera.cpp.o: CMakeFiles/Lab1.dir/flags.make
CMakeFiles/Lab1.dir/source/camera.cpp.o: source/camera.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report //home/efiske/slumber/lab1/Cmake/CMakeFiles $(CMAKE_PROGRESS_8)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/Lab1.dir/source/camera.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/Lab1.dir/source/camera.cpp.o -c //home/efiske/slumber/lab1/Cmake/source/camera.cpp

CMakeFiles/Lab1.dir/source/camera.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Lab1.dir/source/camera.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E //home/efiske/slumber/lab1/Cmake/source/camera.cpp > CMakeFiles/Lab1.dir/source/camera.cpp.i

CMakeFiles/Lab1.dir/source/camera.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Lab1.dir/source/camera.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S //home/efiske/slumber/lab1/Cmake/source/camera.cpp -o CMakeFiles/Lab1.dir/source/camera.cpp.s

CMakeFiles/Lab1.dir/source/camera.cpp.o.requires:
.PHONY : CMakeFiles/Lab1.dir/source/camera.cpp.o.requires

CMakeFiles/Lab1.dir/source/camera.cpp.o.provides: CMakeFiles/Lab1.dir/source/camera.cpp.o.requires
	$(MAKE) -f CMakeFiles/Lab1.dir/build.make CMakeFiles/Lab1.dir/source/camera.cpp.o.provides.build
.PHONY : CMakeFiles/Lab1.dir/source/camera.cpp.o.provides

CMakeFiles/Lab1.dir/source/camera.cpp.o.provides.build: CMakeFiles/Lab1.dir/source/camera.cpp.o

CMakeFiles/Lab1.dir/source/control.cpp.o: CMakeFiles/Lab1.dir/flags.make
CMakeFiles/Lab1.dir/source/control.cpp.o: source/control.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report //home/efiske/slumber/lab1/Cmake/CMakeFiles $(CMAKE_PROGRESS_9)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/Lab1.dir/source/control.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/Lab1.dir/source/control.cpp.o -c //home/efiske/slumber/lab1/Cmake/source/control.cpp

CMakeFiles/Lab1.dir/source/control.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Lab1.dir/source/control.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E //home/efiske/slumber/lab1/Cmake/source/control.cpp > CMakeFiles/Lab1.dir/source/control.cpp.i

CMakeFiles/Lab1.dir/source/control.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Lab1.dir/source/control.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S //home/efiske/slumber/lab1/Cmake/source/control.cpp -o CMakeFiles/Lab1.dir/source/control.cpp.s

CMakeFiles/Lab1.dir/source/control.cpp.o.requires:
.PHONY : CMakeFiles/Lab1.dir/source/control.cpp.o.requires

CMakeFiles/Lab1.dir/source/control.cpp.o.provides: CMakeFiles/Lab1.dir/source/control.cpp.o.requires
	$(MAKE) -f CMakeFiles/Lab1.dir/build.make CMakeFiles/Lab1.dir/source/control.cpp.o.provides.build
.PHONY : CMakeFiles/Lab1.dir/source/control.cpp.o.provides

CMakeFiles/Lab1.dir/source/control.cpp.o.provides.build: CMakeFiles/Lab1.dir/source/control.cpp.o

CMakeFiles/Lab1.dir/source/assets.cpp.o: CMakeFiles/Lab1.dir/flags.make
CMakeFiles/Lab1.dir/source/assets.cpp.o: source/assets.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report //home/efiske/slumber/lab1/Cmake/CMakeFiles $(CMAKE_PROGRESS_10)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/Lab1.dir/source/assets.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/Lab1.dir/source/assets.cpp.o -c //home/efiske/slumber/lab1/Cmake/source/assets.cpp

CMakeFiles/Lab1.dir/source/assets.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Lab1.dir/source/assets.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E //home/efiske/slumber/lab1/Cmake/source/assets.cpp > CMakeFiles/Lab1.dir/source/assets.cpp.i

CMakeFiles/Lab1.dir/source/assets.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Lab1.dir/source/assets.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S //home/efiske/slumber/lab1/Cmake/source/assets.cpp -o CMakeFiles/Lab1.dir/source/assets.cpp.s

CMakeFiles/Lab1.dir/source/assets.cpp.o.requires:
.PHONY : CMakeFiles/Lab1.dir/source/assets.cpp.o.requires

CMakeFiles/Lab1.dir/source/assets.cpp.o.provides: CMakeFiles/Lab1.dir/source/assets.cpp.o.requires
	$(MAKE) -f CMakeFiles/Lab1.dir/build.make CMakeFiles/Lab1.dir/source/assets.cpp.o.provides.build
.PHONY : CMakeFiles/Lab1.dir/source/assets.cpp.o.provides

CMakeFiles/Lab1.dir/source/assets.cpp.o.provides.build: CMakeFiles/Lab1.dir/source/assets.cpp.o

CMakeFiles/Lab1.dir/source/util.c.o: CMakeFiles/Lab1.dir/flags.make
CMakeFiles/Lab1.dir/source/util.c.o: source/util.c
	$(CMAKE_COMMAND) -E cmake_progress_report //home/efiske/slumber/lab1/Cmake/CMakeFiles $(CMAKE_PROGRESS_11)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/Lab1.dir/source/util.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/Lab1.dir/source/util.c.o   -c //home/efiske/slumber/lab1/Cmake/source/util.c

CMakeFiles/Lab1.dir/source/util.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Lab1.dir/source/util.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E //home/efiske/slumber/lab1/Cmake/source/util.c > CMakeFiles/Lab1.dir/source/util.c.i

CMakeFiles/Lab1.dir/source/util.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Lab1.dir/source/util.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S //home/efiske/slumber/lab1/Cmake/source/util.c -o CMakeFiles/Lab1.dir/source/util.c.s

CMakeFiles/Lab1.dir/source/util.c.o.requires:
.PHONY : CMakeFiles/Lab1.dir/source/util.c.o.requires

CMakeFiles/Lab1.dir/source/util.c.o.provides: CMakeFiles/Lab1.dir/source/util.c.o.requires
	$(MAKE) -f CMakeFiles/Lab1.dir/build.make CMakeFiles/Lab1.dir/source/util.c.o.provides.build
.PHONY : CMakeFiles/Lab1.dir/source/util.c.o.provides

CMakeFiles/Lab1.dir/source/util.c.o.provides.build: CMakeFiles/Lab1.dir/source/util.c.o

CMakeFiles/Lab1.dir/dependencies/glad/src/glad.c.o: CMakeFiles/Lab1.dir/flags.make
CMakeFiles/Lab1.dir/dependencies/glad/src/glad.c.o: dependencies/glad/src/glad.c
	$(CMAKE_COMMAND) -E cmake_progress_report //home/efiske/slumber/lab1/Cmake/CMakeFiles $(CMAKE_PROGRESS_12)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/Lab1.dir/dependencies/glad/src/glad.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/Lab1.dir/dependencies/glad/src/glad.c.o   -c //home/efiske/slumber/lab1/Cmake/dependencies/glad/src/glad.c

CMakeFiles/Lab1.dir/dependencies/glad/src/glad.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Lab1.dir/dependencies/glad/src/glad.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E //home/efiske/slumber/lab1/Cmake/dependencies/glad/src/glad.c > CMakeFiles/Lab1.dir/dependencies/glad/src/glad.c.i

CMakeFiles/Lab1.dir/dependencies/glad/src/glad.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Lab1.dir/dependencies/glad/src/glad.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S //home/efiske/slumber/lab1/Cmake/dependencies/glad/src/glad.c -o CMakeFiles/Lab1.dir/dependencies/glad/src/glad.c.s

CMakeFiles/Lab1.dir/dependencies/glad/src/glad.c.o.requires:
.PHONY : CMakeFiles/Lab1.dir/dependencies/glad/src/glad.c.o.requires

CMakeFiles/Lab1.dir/dependencies/glad/src/glad.c.o.provides: CMakeFiles/Lab1.dir/dependencies/glad/src/glad.c.o.requires
	$(MAKE) -f CMakeFiles/Lab1.dir/build.make CMakeFiles/Lab1.dir/dependencies/glad/src/glad.c.o.provides.build
.PHONY : CMakeFiles/Lab1.dir/dependencies/glad/src/glad.c.o.provides

CMakeFiles/Lab1.dir/dependencies/glad/src/glad.c.o.provides.build: CMakeFiles/Lab1.dir/dependencies/glad/src/glad.c.o

# Object files for target Lab1
Lab1_OBJECTS = \
"CMakeFiles/Lab1.dir/source/main.cpp.o" \
"CMakeFiles/Lab1.dir/source/actor.cpp.o" \
"CMakeFiles/Lab1.dir/source/gamestate.cpp.o" \
"CMakeFiles/Lab1.dir/source/windowsetup.cpp.o" \
"CMakeFiles/Lab1.dir/source/GLSL.cpp.o" \
"CMakeFiles/Lab1.dir/source/tiny_obj_loader.cc.o" \
"CMakeFiles/Lab1.dir/source/RenderingHelper.cpp.o" \
"CMakeFiles/Lab1.dir/source/camera.cpp.o" \
"CMakeFiles/Lab1.dir/source/control.cpp.o" \
"CMakeFiles/Lab1.dir/source/assets.cpp.o" \
"CMakeFiles/Lab1.dir/source/util.c.o" \
"CMakeFiles/Lab1.dir/dependencies/glad/src/glad.c.o"

# External object files for target Lab1
Lab1_EXTERNAL_OBJECTS =

Lab1: CMakeFiles/Lab1.dir/source/main.cpp.o
Lab1: CMakeFiles/Lab1.dir/source/actor.cpp.o
Lab1: CMakeFiles/Lab1.dir/source/gamestate.cpp.o
Lab1: CMakeFiles/Lab1.dir/source/windowsetup.cpp.o
Lab1: CMakeFiles/Lab1.dir/source/GLSL.cpp.o
Lab1: CMakeFiles/Lab1.dir/source/tiny_obj_loader.cc.o
Lab1: CMakeFiles/Lab1.dir/source/RenderingHelper.cpp.o
Lab1: CMakeFiles/Lab1.dir/source/camera.cpp.o
Lab1: CMakeFiles/Lab1.dir/source/control.cpp.o
Lab1: CMakeFiles/Lab1.dir/source/assets.cpp.o
Lab1: CMakeFiles/Lab1.dir/source/util.c.o
Lab1: CMakeFiles/Lab1.dir/dependencies/glad/src/glad.c.o
Lab1: CMakeFiles/Lab1.dir/build.make
Lab1: dependencies/glfw/src/libglfw3.a
Lab1: /usr/lib64/libX11.so
Lab1: /usr/lib64/libXrandr.so
Lab1: /usr/lib64/libXinerama.so
Lab1: /usr/lib64/libXi.so
Lab1: /usr/lib64/libXxf86vm.so
Lab1: /usr/lib64/librt.so
Lab1: /usr/lib64/libm.so
Lab1: /usr/lib64/libXcursor.so
Lab1: /usr/lib64/libGL.so
Lab1: CMakeFiles/Lab1.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable Lab1"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Lab1.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Lab1.dir/build: Lab1
.PHONY : CMakeFiles/Lab1.dir/build

CMakeFiles/Lab1.dir/requires: CMakeFiles/Lab1.dir/source/main.cpp.o.requires
CMakeFiles/Lab1.dir/requires: CMakeFiles/Lab1.dir/source/actor.cpp.o.requires
CMakeFiles/Lab1.dir/requires: CMakeFiles/Lab1.dir/source/gamestate.cpp.o.requires
CMakeFiles/Lab1.dir/requires: CMakeFiles/Lab1.dir/source/windowsetup.cpp.o.requires
CMakeFiles/Lab1.dir/requires: CMakeFiles/Lab1.dir/source/GLSL.cpp.o.requires
CMakeFiles/Lab1.dir/requires: CMakeFiles/Lab1.dir/source/tiny_obj_loader.cc.o.requires
CMakeFiles/Lab1.dir/requires: CMakeFiles/Lab1.dir/source/RenderingHelper.cpp.o.requires
CMakeFiles/Lab1.dir/requires: CMakeFiles/Lab1.dir/source/camera.cpp.o.requires
CMakeFiles/Lab1.dir/requires: CMakeFiles/Lab1.dir/source/control.cpp.o.requires
CMakeFiles/Lab1.dir/requires: CMakeFiles/Lab1.dir/source/assets.cpp.o.requires
CMakeFiles/Lab1.dir/requires: CMakeFiles/Lab1.dir/source/util.c.o.requires
CMakeFiles/Lab1.dir/requires: CMakeFiles/Lab1.dir/dependencies/glad/src/glad.c.o.requires
.PHONY : CMakeFiles/Lab1.dir/requires

CMakeFiles/Lab1.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Lab1.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Lab1.dir/clean

CMakeFiles/Lab1.dir/depend:
	cd //home/efiske/slumber/lab1/Cmake && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" //home/efiske/slumber/lab1/Cmake //home/efiske/slumber/lab1/Cmake //home/efiske/slumber/lab1/Cmake //home/efiske/slumber/lab1/Cmake //home/efiske/slumber/lab1/Cmake/CMakeFiles/Lab1.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Lab1.dir/depend

