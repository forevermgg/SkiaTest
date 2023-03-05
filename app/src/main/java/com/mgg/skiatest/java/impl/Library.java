package com.mgg.skiatest.java.impl;

public class Library {
    public static final String LIBRARY_NAME = "skiatest";
    public static volatile boolean _loaded = false;

    public static void staticLoad() {
        if (!_loaded)
            load();
    }

    public static synchronized void load() {
        if (_loaded) return;
        try {
            _loadFromLibraryPath();
        } catch (UnsatisfiedLinkError e) {
            Log.warn("Please use skija platform jmod when using jlink");
        }
    }

    public static void _loadFromLibraryPath() {
        Log.debug("Loading skija native library from library path");
        System.loadLibrary(LIBRARY_NAME);
        _loaded = true;
    }
}
