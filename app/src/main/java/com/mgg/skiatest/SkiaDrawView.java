package com.mgg.skiatest;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.os.SystemClock;
import android.util.AttributeSet;
import android.view.View;

import androidx.annotation.Nullable;

public class SkiaDrawView extends View {
    Bitmap fSkiaBitmap;

    public SkiaDrawView(Context context) {
        super(context);
    }

    public SkiaDrawView(Context context, @Nullable AttributeSet attrs) {
        super(context, attrs);
    }

    public SkiaDrawView(Context context, @Nullable AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
    }

    public SkiaDrawView(Context context, @Nullable AttributeSet attrs, int defStyleAttr, int defStyleRes) {
        super(context, attrs, defStyleAttr, defStyleRes);
    }

    @Override
    protected void onSizeChanged(int w, int h, int oldw, int oldh)
    {
        // Create a bitmap for skia to draw into
        fSkiaBitmap = Bitmap.createBitmap(w, h, Bitmap.Config.ARGB_8888);
    }

    @Override
    protected void onDraw(Canvas canvas) {
        // Call into our C++ code that renders to the bitmap using Skia
        drawIntoBitmap(fSkiaBitmap, SystemClock.elapsedRealtime());

        // Present the bitmap on the screen
        canvas.drawBitmap(fSkiaBitmap, 0, 0, null);
    }

    private native void drawIntoBitmap(Bitmap image, long elapsedTime);
}

