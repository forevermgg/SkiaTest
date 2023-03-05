package com.mgg.skiatest;

import com.mgg.skiatest.java.impl.Library;
import com.mgg.skiatest.java.impl.Managed;
import com.mgg.skiatest.java.impl.Stats;

/**
 * Data holds an immutable data buffer.
 */
public class TestData extends Managed {
    static {
        Library.staticLoad();
    }

    public TestData(long ptr) {
        super(ptr, _FinalizerHolder.PTR);
    }

    /**
     *  Returns a new empty dataref (or a reference to a shared empty dataref).
     *  New or shared, the caller must see that {@link #close()} is eventually called.
     */
    public static TestData makeEmpty() {
        Stats.onNativeCall();
        return new TestData(_nMakeEmpty());
    }

    public static class _FinalizerHolder {
        public static final long PTR = _nGetFinalizer();
    }

    public static native long _nGetFinalizer();
    public static native long    _nMakeEmpty();
}