# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/qianchenxi/Desktop/mprpc

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/qianchenxi/Desktop/mprpc/build

# Include any dependencies generated for this target.
include examples/callee/CMakeFiles/Provider.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include examples/callee/CMakeFiles/Provider.dir/compiler_depend.make

# Include the progress variables for this target.
include examples/callee/CMakeFiles/Provider.dir/progress.make

# Include the compile flags for this target's objects.
include examples/callee/CMakeFiles/Provider.dir/flags.make

examples/callee/CMakeFiles/Provider.dir/friendservice.cc.o: examples/callee/CMakeFiles/Provider.dir/flags.make
examples/callee/CMakeFiles/Provider.dir/friendservice.cc.o: ../examples/callee/friendservice.cc
examples/callee/CMakeFiles/Provider.dir/friendservice.cc.o: examples/callee/CMakeFiles/Provider.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/qianchenxi/Desktop/mprpc/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object examples/callee/CMakeFiles/Provider.dir/friendservice.cc.o"
	cd /home/qianchenxi/Desktop/mprpc/build/examples/callee && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT examples/callee/CMakeFiles/Provider.dir/friendservice.cc.o -MF CMakeFiles/Provider.dir/friendservice.cc.o.d -o CMakeFiles/Provider.dir/friendservice.cc.o -c /home/qianchenxi/Desktop/mprpc/examples/callee/friendservice.cc

examples/callee/CMakeFiles/Provider.dir/friendservice.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Provider.dir/friendservice.cc.i"
	cd /home/qianchenxi/Desktop/mprpc/build/examples/callee && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/qianchenxi/Desktop/mprpc/examples/callee/friendservice.cc > CMakeFiles/Provider.dir/friendservice.cc.i

examples/callee/CMakeFiles/Provider.dir/friendservice.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Provider.dir/friendservice.cc.s"
	cd /home/qianchenxi/Desktop/mprpc/build/examples/callee && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/qianchenxi/Desktop/mprpc/examples/callee/friendservice.cc -o CMakeFiles/Provider.dir/friendservice.cc.s

examples/callee/CMakeFiles/Provider.dir/__/friends.pb.cc.o: examples/callee/CMakeFiles/Provider.dir/flags.make
examples/callee/CMakeFiles/Provider.dir/__/friends.pb.cc.o: ../examples/friends.pb.cc
examples/callee/CMakeFiles/Provider.dir/__/friends.pb.cc.o: examples/callee/CMakeFiles/Provider.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/qianchenxi/Desktop/mprpc/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object examples/callee/CMakeFiles/Provider.dir/__/friends.pb.cc.o"
	cd /home/qianchenxi/Desktop/mprpc/build/examples/callee && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT examples/callee/CMakeFiles/Provider.dir/__/friends.pb.cc.o -MF CMakeFiles/Provider.dir/__/friends.pb.cc.o.d -o CMakeFiles/Provider.dir/__/friends.pb.cc.o -c /home/qianchenxi/Desktop/mprpc/examples/friends.pb.cc

examples/callee/CMakeFiles/Provider.dir/__/friends.pb.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Provider.dir/__/friends.pb.cc.i"
	cd /home/qianchenxi/Desktop/mprpc/build/examples/callee && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/qianchenxi/Desktop/mprpc/examples/friends.pb.cc > CMakeFiles/Provider.dir/__/friends.pb.cc.i

examples/callee/CMakeFiles/Provider.dir/__/friends.pb.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Provider.dir/__/friends.pb.cc.s"
	cd /home/qianchenxi/Desktop/mprpc/build/examples/callee && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/qianchenxi/Desktop/mprpc/examples/friends.pb.cc -o CMakeFiles/Provider.dir/__/friends.pb.cc.s

# Object files for target Provider
Provider_OBJECTS = \
"CMakeFiles/Provider.dir/friendservice.cc.o" \
"CMakeFiles/Provider.dir/__/friends.pb.cc.o"

# External object files for target Provider
Provider_EXTERNAL_OBJECTS =

../bin/Provider: examples/callee/CMakeFiles/Provider.dir/friendservice.cc.o
../bin/Provider: examples/callee/CMakeFiles/Provider.dir/__/friends.pb.cc.o
../bin/Provider: examples/callee/CMakeFiles/Provider.dir/build.make
../bin/Provider: ../lib/libMPRPC.so
../bin/Provider: examples/callee/CMakeFiles/Provider.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/qianchenxi/Desktop/mprpc/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable ../../../bin/Provider"
	cd /home/qianchenxi/Desktop/mprpc/build/examples/callee && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Provider.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
examples/callee/CMakeFiles/Provider.dir/build: ../bin/Provider
.PHONY : examples/callee/CMakeFiles/Provider.dir/build

examples/callee/CMakeFiles/Provider.dir/clean:
	cd /home/qianchenxi/Desktop/mprpc/build/examples/callee && $(CMAKE_COMMAND) -P CMakeFiles/Provider.dir/cmake_clean.cmake
.PHONY : examples/callee/CMakeFiles/Provider.dir/clean

examples/callee/CMakeFiles/Provider.dir/depend:
	cd /home/qianchenxi/Desktop/mprpc/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/qianchenxi/Desktop/mprpc /home/qianchenxi/Desktop/mprpc/examples/callee /home/qianchenxi/Desktop/mprpc/build /home/qianchenxi/Desktop/mprpc/build/examples/callee /home/qianchenxi/Desktop/mprpc/build/examples/callee/CMakeFiles/Provider.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : examples/callee/CMakeFiles/Provider.dir/depend

