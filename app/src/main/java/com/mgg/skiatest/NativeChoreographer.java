package com.mgg.skiatest;

import android.annotation.TargetApi;
import android.util.Log;
import android.view.Choreographer;

import com.mgg.skiatest.java.impl.Managed;
import com.mgg.skiatest.java.impl.Stats;

public class NativeChoreographer extends Managed implements Choreographer.FrameCallback {

    private boolean useChoreographer = true;
    private NativeChoreographer(long ptr) {
        super(ptr, NativeChoreographer._FinalizerHolder.PTR);
    }

    public static class _FinalizerHolder {
        public static final long PTR = _nGetFinalizer();
    }

    public static NativeChoreographer makeEmpty() {
        Stats.onNativeCall();
        return new NativeChoreographer(_nMakeEmpty());
    }

    public void choreographerCallback(long frameTimeNamos) {
        Stats.onNativeCall();
        nativeChoreographerCallback(frameTimeNamos, _ptr);
    }

    @TargetApi(16)
    @Override
    public void doFrame(long frameTimeNanos) {
        Choreographer.getInstance().postFrameCallback(this);
        choreographerCallback(frameTimeNanos);
    }

    @TargetApi(16)
    public void startChoreographer() {
        Choreographer.getInstance().postFrameCallback(this);
        useChoreographer = true;
    }

    @TargetApi(16)
    public void stopChoreographer() {
        useChoreographer = false;
    }

    @TargetApi(19)
    public void onResume() {
        if (useChoreographer) {
            Choreographer.getInstance().postFrameCallback(this);
        }
    }

    public void  onPause() {
        if (useChoreographer) {
            Choreographer.getInstance().removeFrameCallback(this);
        }
    }

    public void updateFPS(float fFPS) {
        String text = String.format("%2.2f FPS", fFPS);
        Log.e("updateFPS:", text);
    }

    public static native long _nGetFinalizer();

    public static native long _nMakeEmpty();
    public native void nativeChoreographerCallback(long frameTimeNamos, long ptr);
}
