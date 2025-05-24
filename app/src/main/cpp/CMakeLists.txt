cmake_minimum_required(VERSION 3.4.1)

add_library( # Sets the name of the library.
        native-lib

        # Sets the type of the library.
        SHARED

        # Specifies the source files for the library.
        native-lib.cpp )

find_library( # Specifies the name of the NDK library that
        # you want CMake to locate.
        log-lib
        log )

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in the
# build script, prebuilt third-party libraries, or system libraries.
target_link_libraries( # Specifies the target library.
        native-lib

        # Links the target library to the log library
        # included in the NDK.
        ${log-lib}

        # OpenGL ES 1.0 ライブラリをリンク
        GLESv1_CM # これは GLES 1.0 の共通プロファイルです
        EGL
)