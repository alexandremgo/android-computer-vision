# Native code

Android NDK is used with CMake to compile C/C++.

# OpenCV setup

1. Download the latest OpenCV Android library at [https://opencv.org/releases](https://opencv.org/releases).
2. Extract the content of the zip file into a folder (in `/opt` folder on Linux for ex).
3. Import OpenCV as a module into your Android project by clicking on `File -> New -> Import Module…`.
4. Choose `<opencv_android_folder>/sdk/java` and wait for the sync to complete.
5. Create a folder named `jniLibs` in `src/main` and copy the content of `<opencv_android_folder>/sdk/native/libs` into it.
It will copy a `libopencv_java4.so` file for each device architecture (arm64, x86, ...). 
6. Look at [src/main/build.gradle](src/main/build.gradle) to add the OpenCV module 
as a dependency and configure cmake.
7. Look at [src/main/cpp/CMakeLists.txt](src/main/cpp/CMakeLists.txt) to 
add the OpenCV library to your native code. Edit the var `pathToOpenCV`
to your OpenCV path.