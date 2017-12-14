# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/home/opencv/DIPPractice/Fourier Transform"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/home/opencv/DIPPractice/Fourier Transform"

# Include any dependencies generated for this target.
include CMakeFiles/ft.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/ft.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/ft.dir/flags.make

CMakeFiles/ft.dir/FourierTransform.cpp.o: CMakeFiles/ft.dir/flags.make
CMakeFiles/ft.dir/FourierTransform.cpp.o: FourierTransform.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/opencv/DIPPractice/Fourier Transform/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/ft.dir/FourierTransform.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ft.dir/FourierTransform.cpp.o -c "/home/opencv/DIPPractice/Fourier Transform/FourierTransform.cpp"

CMakeFiles/ft.dir/FourierTransform.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ft.dir/FourierTransform.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/opencv/DIPPractice/Fourier Transform/FourierTransform.cpp" > CMakeFiles/ft.dir/FourierTransform.cpp.i

CMakeFiles/ft.dir/FourierTransform.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ft.dir/FourierTransform.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/opencv/DIPPractice/Fourier Transform/FourierTransform.cpp" -o CMakeFiles/ft.dir/FourierTransform.cpp.s

CMakeFiles/ft.dir/FourierTransform.cpp.o.requires:

.PHONY : CMakeFiles/ft.dir/FourierTransform.cpp.o.requires

CMakeFiles/ft.dir/FourierTransform.cpp.o.provides: CMakeFiles/ft.dir/FourierTransform.cpp.o.requires
	$(MAKE) -f CMakeFiles/ft.dir/build.make CMakeFiles/ft.dir/FourierTransform.cpp.o.provides.build
.PHONY : CMakeFiles/ft.dir/FourierTransform.cpp.o.provides

CMakeFiles/ft.dir/FourierTransform.cpp.o.provides.build: CMakeFiles/ft.dir/FourierTransform.cpp.o


# Object files for target ft
ft_OBJECTS = \
"CMakeFiles/ft.dir/FourierTransform.cpp.o"

# External object files for target ft
ft_EXTERNAL_OBJECTS =

ft: CMakeFiles/ft.dir/FourierTransform.cpp.o
ft: CMakeFiles/ft.dir/build.make
ft: /usr/local/lib/libopencv_dnn.so.3.3.1
ft: /usr/local/lib/libopencv_ml.so.3.3.1
ft: /usr/local/lib/libopencv_objdetect.so.3.3.1
ft: /usr/local/lib/libopencv_shape.so.3.3.1
ft: /usr/local/lib/libopencv_stitching.so.3.3.1
ft: /usr/local/lib/libopencv_superres.so.3.3.1
ft: /usr/local/lib/libopencv_videostab.so.3.3.1
ft: /usr/local/lib/libopencv_calib3d.so.3.3.1
ft: /usr/local/lib/libopencv_features2d.so.3.3.1
ft: /usr/local/lib/libopencv_flann.so.3.3.1
ft: /usr/local/lib/libopencv_highgui.so.3.3.1
ft: /usr/local/lib/libopencv_photo.so.3.3.1
ft: /usr/local/lib/libopencv_video.so.3.3.1
ft: /usr/local/lib/libopencv_videoio.so.3.3.1
ft: /usr/local/lib/libopencv_imgcodecs.so.3.3.1
ft: /usr/local/lib/libopencv_imgproc.so.3.3.1
ft: /usr/local/lib/libopencv_core.so.3.3.1
ft: CMakeFiles/ft.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/home/opencv/DIPPractice/Fourier Transform/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ft"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ft.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/ft.dir/build: ft

.PHONY : CMakeFiles/ft.dir/build

CMakeFiles/ft.dir/requires: CMakeFiles/ft.dir/FourierTransform.cpp.o.requires

.PHONY : CMakeFiles/ft.dir/requires

CMakeFiles/ft.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/ft.dir/cmake_clean.cmake
.PHONY : CMakeFiles/ft.dir/clean

CMakeFiles/ft.dir/depend:
	cd "/home/opencv/DIPPractice/Fourier Transform" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/home/opencv/DIPPractice/Fourier Transform" "/home/opencv/DIPPractice/Fourier Transform" "/home/opencv/DIPPractice/Fourier Transform" "/home/opencv/DIPPractice/Fourier Transform" "/home/opencv/DIPPractice/Fourier Transform/CMakeFiles/ft.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/ft.dir/depend

