cmake_minimum_required(VERSION 3.13.4)

# To adapt
set(pathToOpenCV /opt/OpenCV-android-sdk)

set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")

include_directories(${pathToOpenCV}/sdk/native/jni/include)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# Gradle automatically packages shared libraries with your APK.
add_library(native-cv SHARED native_cv.cpp format_image.cpp process_image.cpp)

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.
find_library(log-lib log)
# Contains the NDK native APIs
find_library(android-lib android)
find_library(mediandk-lib mediandk)

add_library(opencv-lib SHARED IMPORTED)
get_filename_component(PARENT_DIR ../ ABSOLUTE)
set_target_properties(opencv-lib PROPERTIES IMPORTED_LOCATION ${PARENT_DIR}/jniLibs/${ANDROID_ABI}/libopencv_java4.so)
#get_filename_component(PARENT_DIR ${MYPROJECT_DIR} DIRECTORY)
message("ICI = ${MYPROJECT_DIR}")

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.
target_link_libraries(  native-cv
                        # Links the target library to needed libraries
                        ${log-lib} ${android-lib} ${mediandk-lib} opencv-lib)