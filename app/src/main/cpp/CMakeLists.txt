cmake_minimum_required(VERSION 3.22.1)

project("skiatest")

add_library( # Sets the name of the library.
        skiatest

        # Sets the library as a shared library.
        SHARED
        impl/managed.cc
        renderengine/DisplaySettings.h
        renderengine/RenderEngineCreationArgs.h
        TestData.h
        TestData.cc
        native-lib.cpp)

target_link_libraries( # Specifies the target library.
        skiatest
        EGL
        GLESv2
        GLESv3
        OpenSLES
        android
        atomic
        jnigraphics
        nativewindow
        log)