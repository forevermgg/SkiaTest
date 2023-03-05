package com.mgg.skiatest;

import com.mgg.skiatest.java.impl.Library;
import com.mgg.skiatest.java.impl.Managed;
import com.mgg.skiatest.java.impl.Stats;

public class CheckEglEnvironment extends Managed {
    static {
        Library.staticLoad();
    }

    public CheckEglEnvironment(long ptr) {
        super(ptr, CheckEglEnvironment._FinalizerHolder.PTR);
    }

    public static CheckEglEnvironment makeEmpty() {
        Stats.onNativeCall();
        return new CheckEglEnvironment(_nMakeEmpty());
    }

    public boolean checkSupportEgl() {
        Stats.onNativeCall();
        return _nCheckSupportEgl(_ptr);
    }

    public boolean checkSupportGLExtensions() {
        Stats.onNativeCall();
        return _nCheckSupportGLExtensions(_ptr);
    }

    public static class _FinalizerHolder {
        public static final long PTR = _nGetFinalizer();
    }

    public static native long _nGetFinalizer();

    public static native long _nMakeEmpty();

    public static native boolean _nCheckSupportEgl(long ptr);
    public static native boolean _nCheckSupportGLExtensions(long ptr);
}
