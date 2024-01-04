package com.mgg.skiatest;

import android.opengl.EGL14;
import android.opengl.EGLConfig;
import android.opengl.EGLDisplay;
import android.util.Log;

public class MyGLRenderer {
    private static final int EGL_OPENGL_ES2_BIT = 4;
    private static final int EGL_CONTEXT_CLIENT_VERSION = 0x3098;
    private static final int[] CONFIG_ATTRIBS = {
            EGL14.EGL_RED_SIZE, 8,
            EGL14.EGL_GREEN_SIZE, 8,
            EGL14.EGL_BLUE_SIZE, 8,
            EGL14.EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
            EGL14.EGL_NONE
    };
    private static final int[] CONTEXT_ATTRIBS = {
            EGL_CONTEXT_CLIENT_VERSION, 2,
            EGL14.EGL_NONE
    };

    public boolean isAlphaSizeSupported() {
        EGLDisplay display = EGL14.eglGetDisplay(EGL14.EGL_DEFAULT_DISPLAY);
        if (display == EGL14.EGL_NO_DISPLAY) {
            Log.e("MyGLRenderer", "Failed to get default display");
            return false;
        }

        int[] version = new int[2];
        if (!EGL14.eglInitialize(display, version, 0, version, 1)) {
            Log.e("MyGLRenderer", "Failed to initialize EGL");
            return false;
        }

        EGLConfig[] configs = new EGLConfig[1];
        int[] numConfigs = new int[1];
        if (!EGL14.eglChooseConfig(display, CONFIG_ATTRIBS, 0, configs, 0, configs.length, numConfigs, 0)) {
            Log.e("MyGLRenderer", "Failed to choose config");
            return false;
        }

        EGLConfig config = configs[0];
        int[] attributes = {EGL14.EGL_ALPHA_SIZE};
        int[] value = new int[1];
        if (!EGL14.eglGetConfigAttrib(display, config, attributes[0], value, 0)) {
            Log.e("MyGLRenderer", "Failed to get config attribute");
            return false;
        }

        boolean isSupported = (value[0] > 0);

        EGL14.eglTerminate(display);

        return isSupported;
    }
}
