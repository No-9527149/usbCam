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
CMAKE_SOURCE_DIR = /tmp/tmp.e2eyNA9N2g

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /tmp/tmp.e2eyNA9N2g/cmake-build-release

# Include any dependencies generated for this target.
include snap/CMakeFiles/utils.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include snap/CMakeFiles/utils.dir/compiler_depend.make

# Include the progress variables for this target.
include snap/CMakeFiles/utils.dir/progress.make

# Include the compile flags for this target's objects.
include snap/CMakeFiles/utils.dir/flags.make

snap/CMakeFiles/utils.dir/utils.cpp.o: snap/CMakeFiles/utils.dir/flags.make
snap/CMakeFiles/utils.dir/utils.cpp.o: ../utils/utils.cpp
snap/CMakeFiles/utils.dir/utils.cpp.o: snap/CMakeFiles/utils.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/tmp.e2eyNA9N2g/cmake-build-release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object snap/CMakeFiles/utils.dir/utils.cpp.o"
	cd /tmp/tmp.e2eyNA9N2g/cmake-build-release/snap && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT snap/CMakeFiles/utils.dir/utils.cpp.o -MF CMakeFiles/utils.dir/utils.cpp.o.d -o CMakeFiles/utils.dir/utils.cpp.o -c /tmp/tmp.e2eyNA9N2g/utils/utils.cpp

snap/CMakeFiles/utils.dir/utils.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/utils.dir/utils.cpp.i"
	cd /tmp/tmp.e2eyNA9N2g/cmake-build-release/snap && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /tmp/tmp.e2eyNA9N2g/utils/utils.cpp > CMakeFiles/utils.dir/utils.cpp.i

snap/CMakeFiles/utils.dir/utils.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/utils.dir/utils.cpp.s"
	cd /tmp/tmp.e2eyNA9N2g/cmake-build-release/snap && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /tmp/tmp.e2eyNA9N2g/utils/utils.cpp -o CMakeFiles/utils.dir/utils.cpp.s

# Object files for target utils
utils_OBJECTS = \
"CMakeFiles/utils.dir/utils.cpp.o"

# External object files for target utils
utils_EXTERNAL_OBJECTS =

snap/libutils.a: snap/CMakeFiles/utils.dir/utils.cpp.o
snap/libutils.a: snap/CMakeFiles/utils.dir/build.make
snap/libutils.a: snap/CMakeFiles/utils.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/tmp/tmp.e2eyNA9N2g/cmake-build-release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libutils.a"
	cd /tmp/tmp.e2eyNA9N2g/cmake-build-release/snap && $(CMAKE_COMMAND) -P CMakeFiles/utils.dir/cmake_clean_target.cmake
	cd /tmp/tmp.e2eyNA9N2g/cmake-build-release/snap && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/utils.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
snap/CMakeFiles/utils.dir/build: snap/libutils.a
.PHONY : snap/CMakeFiles/utils.dir/build

snap/CMakeFiles/utils.dir/clean:
	cd /tmp/tmp.e2eyNA9N2g/cmake-build-release/snap && $(CMAKE_COMMAND) -P CMakeFiles/utils.dir/cmake_clean.cmake
.PHONY : snap/CMakeFiles/utils.dir/clean

snap/CMakeFiles/utils.dir/depend:
	cd /tmp/tmp.e2eyNA9N2g/cmake-build-release && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /tmp/tmp.e2eyNA9N2g /tmp/tmp.e2eyNA9N2g/utils /tmp/tmp.e2eyNA9N2g/cmake-build-release /tmp/tmp.e2eyNA9N2g/cmake-build-release/snap /tmp/tmp.e2eyNA9N2g/cmake-build-release/snap/CMakeFiles/utils.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : snap/CMakeFiles/utils.dir/depend

