cmake_minimum_required(VERSION 3.13.4)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# Gradle automatically packages shared libraries with your APK.
add_library( # Sets the name of the library.
             native-cv

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
             native_cv.cpp )

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.
find_library(log-lib log)
# Contains the NDK native APIs
find_library(android-lib android)
find_library(mediandk-lib mediandk)

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.
target_link_libraries( # Specifies the target library.
                       native-cv

                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib} ${android-lib} ${mediandk-lib})