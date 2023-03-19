package com.mgg.skiatest;

import android.annotation.TargetApi;
import android.hardware.display.DisplayManager;
import android.view.Choreographer;
import android.view.Display;

import androidx.annotation.NonNull;

import com.mgg.skiatest.java.impl.Library;
import com.mgg.skiatest.java.impl.Managed;
import com.mgg.skiatest.java.impl.Stats;

public class VsyncWaiter extends Managed {
    private static VsyncWaiter instance;
    private static DisplayListener listener;

    private static AsyncWaitForVsyncDelegate mAsyncWaitForVsyncDelegate;
    /**
     * This value is updated by the VsyncWaiter when it is initialized.
     *
     * <p>On API 17+, it is updated whenever the default display refresh rate changes.
     *
     * <p>It is defaulted to 60.
     */
    private static float refreshRateFPS = 60.0f;

    static {
        Library.staticLoad();
    }

    private long refreshPeriodNanos = -1;
    private FrameCallback frameCallback = new FrameCallback(0);
    private final AsyncWaitForVsyncDelegate asyncWaitForVsyncDelegate =
            new AsyncWaitForVsyncDelegate() {

                private Choreographer.FrameCallback obtainFrameCallback(final long cookie) {
                    if (frameCallback != null) {
                        frameCallback.cookie = cookie;
                        FrameCallback ret = frameCallback;
                        frameCallback = null;
                        return ret;
                    }
                    return new FrameCallback(cookie);
                }

                @Override
                public void asyncWaitForVsync(long cookie) {
                    Choreographer.getInstance().postFrameCallback(obtainFrameCallback(cookie));
                }
            };

    private VsyncWaiter(long ptr) {
        super(ptr, VsyncWaiter._FinalizerHolder.PTR);
    }

    /**
     * Returns a new empty dataref (or a reference to a shared empty dataref).
     * New or shared, the caller must see that {@link #close()} is eventually called.
     */
    public static VsyncWaiter makeEmpty() {
        Stats.onNativeCall();
        return new VsyncWaiter(_nMakeEmpty());
    }

    public static native long _nGetFinalizer();

    public static native long _nMakeEmpty();

    @NonNull
    public static VsyncWaiter getInstance(float fps) {
        if (instance == null) {
            instance = VsyncWaiter.makeEmpty();
        }
        setRefreshRateFPS(fps);
        instance.refreshPeriodNanos = (long) (1000000000.0 / fps);
        return instance;
    }

    @TargetApi(17)
    @NonNull
    public static VsyncWaiter getInstance(
            @NonNull DisplayManager displayManager) {
        if (instance == null) {
            instance = VsyncWaiter.makeEmpty();
        }
        if (listener == null) {
            listener = instance.new DisplayListener(displayManager);
            listener.register();
        }
        if (instance.refreshPeriodNanos == -1) {
            final Display primaryDisplay = displayManager.getDisplay(Display.DEFAULT_DISPLAY);
            float fps = primaryDisplay.getRefreshRate();
            instance.refreshPeriodNanos = (long) (1000000000.0 / fps);
            setRefreshRateFPS(fps);
        }
        return instance;
    }

    public static void reset() {
        instance = null;
        listener = null;
    }

    public void init() {
        mAsyncWaitForVsyncDelegate = asyncWaitForVsyncDelegate;
    }

    public static void setRefreshRateFPS(float refreshRateFPS) {
        // This is ok because it only ever tracks the refresh rate of the main
        // display. If we ever need to support the refresh rate of other displays
        // on Android we will need to refactor this. Static lookup makes things a
        // bit easier on the C++ side.
        VsyncWaiter.refreshRateFPS = refreshRateFPS;
        updateRefreshRate();
    }

    public static void updateRefreshRate() {
        nativeUpdateRefreshRate(refreshRateFPS);
    }

    private static native void nativeUpdateRefreshRate(float refreshRateFPS);

    public static class _FinalizerHolder {
        public static final long PTR = _nGetFinalizer();
    }

    private class FrameCallback implements Choreographer.FrameCallback {

        private long cookie;

        FrameCallback(long cookie) {
            this.cookie = cookie;
        }

        @Override
        public void doFrame(long frameTimeNanos) {
            long delay = System.nanoTime() - frameTimeNanos;
            if (delay < 0) {
                delay = 0;
            }
            onVsync(delay, refreshPeriodNanos, cookie);
            frameCallback = this;
        }
    }

    // TODO(mattcarroll): add javadocs
    // Called by native.
    private static void asyncWaitForVsync(final long cookie) {
        if (mAsyncWaitForVsyncDelegate != null) {
            mAsyncWaitForVsyncDelegate.asyncWaitForVsync(cookie);
        } else {
            throw new IllegalStateException(
                    "An AsyncWaitForVsyncDelegate must be registered with FlutterJNI before asyncWaitForVsync() is invoked.");
        }
    }

    private native void nativeOnVsync(long frameDelayNanos, long refreshPeriodNanos, long cookie, long nativePtr);

    public void onVsync(long frameDelayNanos, long refreshPeriodNanos, long cookie) {
        nativeOnVsync(frameDelayNanos, refreshPeriodNanos, cookie, _ptr);
    }

    @TargetApi(17)
    class DisplayListener implements DisplayManager.DisplayListener {
        private final DisplayManager displayManager;

        DisplayListener(DisplayManager displayManager) {
            this.displayManager = displayManager;
        }

        void register() {
            displayManager.registerDisplayListener(this, null);
        }

        @Override
        public void onDisplayAdded(int displayId) {
        }

        @Override
        public void onDisplayRemoved(int displayId) {
        }

        @Override
        public void onDisplayChanged(int displayId) {
            if (displayId == Display.DEFAULT_DISPLAY) {
                final Display primaryDisplay = displayManager.getDisplay(Display.DEFAULT_DISPLAY);
                float fps = primaryDisplay.getRefreshRate();
                VsyncWaiter.this.refreshPeriodNanos = (long) (1000000000.0 / fps);
                setRefreshRateFPS(fps);
            }
        }
    }
}
