#最低版本
cmake_minimum_required(VERSION 3.22.1)

#项目信息
project("skiatest")

#头文件目录
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/skia/libs)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/skia/src)
LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/skia/libs/${ANDROID_ABI})

message("ANDROID_ABI : ${ANDROID_ABI}")
set(SK_GL=1)
add_definitions(-DSK_GL=1)
add_library( # Sets the name of the library.
        skiatest

        # Sets the library as a shared library.
        SHARED
        androidkit/Canvas.cpp
        androidkit/ColorFilters.cpp
        androidkit/Gradients.cpp
        androidkit/Image.cpp
        androidkit/ImageFilter.cpp
        androidkit/Matrix.cpp
        androidkit/Paint.cpp
        androidkit/Path.cpp
        androidkit/PathBuilder.cpp
        androidkit/RuntimeShaderBuilder.cpp
        androidkit/Shader.cpp
        androidkit/Surface.cpp
        androidkit/Surface.h
        androidkit/SurfaceThread.h
        androidkit/SurfaceThread.cpp
        androidkit/Utils.cpp
        debug/android_debug.h
        commonpool/CommonPool.h
        commonpool/CommonPool.cc
        DisplayParams.h
        checkenv/CheckEglEnvironment.h
        checkenv/CheckEglEnvironment.cc
        checkenv/ExtensionSet.h
        checkenv/ExtensionSet.cc
        common/types.h
        impl/managed.cc
        renderengine/DisplaySettings.h
        renderengine/RenderEngineCreationArgs.h
        renderengine/SkiaGLRenderEngineStatic.cpp
        utils/Errors.h
        utils/Timing.h
        utils/Macros.h
        utils/ColorMode.h
        egl/EglManager.h
        egl/EglManager.cc
        exception/exception_helper.h
        exception/exception_helper.cpp
        Frame.h
        Frame.cc
        GLWindowContext.h
        GLWindowContext.cc
        GLWindowContext_android.cpp
        load_library.cc
        choreographer/NativeChoreographer.h
        choreographer/NativeChoreographer.cc
        SkiaDrawView.cc
        test/TestData.h
        test/TestData.cc
        native-lib.cpp
        utils/Result.h
        WindowContext.h
        WindowContext.cc
        WindowContextFactory_android.h
        )
# add_library(mxnet STATIC IMPORTED)
# set_property(TARGET mxnet PROPERTY IMPORTED_LOCATION /path/to/libmxnet.a)
# target_link_libraries(project_name mxnet ) #添加libmxnet.a

#添加链接库
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
        log
        libcpu-features.a
        libdng_sdk.a
        libexpat.a
        libfreetype2.a
        libharfbuzz.a
        libicu.a
        libjpeg.a
        libparticles.a
        libpiex.a
        libpng.a
        libskia.a
        libskottie.a
        libskparagraph.a
        libskresources.a
        libsksg.a
        libskshaper.a
        libsvg.a
        libwebp.a
        libwebp_sse41.a
        libzlib.a)