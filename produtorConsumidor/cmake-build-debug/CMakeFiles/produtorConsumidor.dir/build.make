# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


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
CMAKE_COMMAND = /home/rui/Documentos/Programas/clion-2020.1.2/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/rui/Documentos/Programas/clion-2020.1.2/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/rui/Desktop/apresentacao/produtorConsumidor

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/rui/Desktop/apresentacao/produtorConsumidor/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/produtorConsumidor.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/produtorConsumidor.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/produtorConsumidor.dir/flags.make

CMakeFiles/produtorConsumidor.dir/main.c.o: CMakeFiles/produtorConsumidor.dir/flags.make
CMakeFiles/produtorConsumidor.dir/main.c.o: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/rui/Desktop/apresentacao/produtorConsumidor/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/produtorConsumidor.dir/main.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/produtorConsumidor.dir/main.c.o   -c /home/rui/Desktop/apresentacao/produtorConsumidor/main.c

CMakeFiles/produtorConsumidor.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/produtorConsumidor.dir/main.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/rui/Desktop/apresentacao/produtorConsumidor/main.c > CMakeFiles/produtorConsumidor.dir/main.c.i

CMakeFiles/produtorConsumidor.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/produtorConsumidor.dir/main.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/rui/Desktop/apresentacao/produtorConsumidor/main.c -o CMakeFiles/produtorConsumidor.dir/main.c.s

# Object files for target produtorConsumidor
produtorConsumidor_OBJECTS = \
"CMakeFiles/produtorConsumidor.dir/main.c.o"

# External object files for target produtorConsumidor
produtorConsumidor_EXTERNAL_OBJECTS =

produtorConsumidor: CMakeFiles/produtorConsumidor.dir/main.c.o
produtorConsumidor: CMakeFiles/produtorConsumidor.dir/build.make
produtorConsumidor: CMakeFiles/produtorConsumidor.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/rui/Desktop/apresentacao/produtorConsumidor/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable produtorConsumidor"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/produtorConsumidor.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/produtorConsumidor.dir/build: produtorConsumidor

.PHONY : CMakeFiles/produtorConsumidor.dir/build

CMakeFiles/produtorConsumidor.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/produtorConsumidor.dir/cmake_clean.cmake
.PHONY : CMakeFiles/produtorConsumidor.dir/clean

CMakeFiles/produtorConsumidor.dir/depend:
	cd /home/rui/Desktop/apresentacao/produtorConsumidor/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/rui/Desktop/apresentacao/produtorConsumidor /home/rui/Desktop/apresentacao/produtorConsumidor /home/rui/Desktop/apresentacao/produtorConsumidor/cmake-build-debug /home/rui/Desktop/apresentacao/produtorConsumidor/cmake-build-debug /home/rui/Desktop/apresentacao/produtorConsumidor/cmake-build-debug/CMakeFiles/produtorConsumidor.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/produtorConsumidor.dir/depend

