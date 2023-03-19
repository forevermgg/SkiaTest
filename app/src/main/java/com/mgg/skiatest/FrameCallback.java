package com.mgg.skiatest;

import android.view.Choreographer;

public class FrameCallback  implements Choreographer.FrameCallback {

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
        // flutterJNI.onVsync(delay, refreshPeriodNanos, cookie);
        // frameCallback = this;
    }
}
