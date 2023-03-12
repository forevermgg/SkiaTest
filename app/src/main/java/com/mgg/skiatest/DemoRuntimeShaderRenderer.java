package com.mgg.skiatest;


import org.skia.androidkit.Canvas;
import org.skia.androidkit.Color;
import org.skia.androidkit.Paint;
import org.skia.androidkit.RuntimeShaderBuilder;
import org.skia.androidkit.Surface;
import org.skia.androidkit.util.SurfaceRenderer;

class DemoRuntimeShaderRenderer extends SurfaceRenderer {
    private RuntimeShaderBuilder mBuilder = new RuntimeShaderBuilder(SkSLShader);

    private static final String SkSLShader =
            "uniform half u_time;                                  " +
                    "uniform half u_w;                                     " +
                    "uniform half u_h;                                     " +

                    "float f(vec3 p) {                                     " +
                    "   p.z -= u_time * 10.;                               " +
                    "   float a = p.z * .1;                                " +
                    "   p.xy *= mat2(cos(a), sin(a), -sin(a), cos(a));     " +
                    "   return .1 - length(cos(p.xy) + sin(p.yz));         " +
                    "}                                                     " +

                    "half4 main(vec2 fragcoord) {                          " +
                    "   vec3 d = .5 - fragcoord.xy1 / u_h;                 " +
                    "   vec3 p=vec3(0);                                    " +
                    "   for (int i = 0; i < 32; i++) p += f(p) * d;        " +
                    "   return ((sin(p) + vec3(2, 5, 9)) / length(p)).xyz1;" +
                    "}";

    @Override
    protected void onSurfaceInitialized(Surface surface) {}

    @Override
    protected void onRenderFrame(Canvas canvas, long ms) {
        final float kWidth  = 200,
                kHeight = 200,
                kSpeed  = 4;

        canvas.drawColor(0xffffffe0);
        Paint p = new Paint();
        p.setColor(new Color(95f,184f,120f,1.000f));

        canvas.drawRect(0, 0, kWidth, kHeight, p);
    }
}