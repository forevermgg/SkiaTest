package com.mgg.skiatest

import android.annotation.TargetApi
import android.graphics.Bitmap
import android.os.Bundle
import android.view.*
import androidx.appcompat.app.AppCompatActivity
import com.mgg.skiatest.databinding.ActivityMainBinding
import com.mgg.skiatest.java.impl.Log
import org.skia.androidkit.*
import java.util.*


open class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding
    private var testData: TestData ? = null
    private var nativeChoreographer: NativeChoreographer ? = null
    private var fAnimationTimer: Timer? = null

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)
        testData()
        // Set a timer that will periodically request an update of the SkiaDrawView
        fAnimationTimer = Timer()
        fAnimationTimer?.schedule(object : TimerTask() {
            override fun run() {
                // This will request an update of the SkiaDrawView, even from other threads
                binding.mSkiaDrawView.postInvalidate()
            }
        }, 0, 100)
        // 0 means no delay before the timer starts; 5 means repeat every 5 milliseconds
        binding.sampleText.text = getAbiString()
        throwsException()

        testBitmap()

        binding.mSurfaceView.holder.addCallback(DemoRuntimeShaderRenderer())
    }

    private fun testData() {
        testData = TestData.makeEmpty()
        nativeChoreographer = NativeChoreographer.makeEmpty()
        val test = CheckEglEnvironment.makeEmpty()
        test.checkSupportEgl()
        test.checkSupportGLExtensions()
        Log.error("checkSupportEgl:${test.checkSupportEgl()}")
        Log.error("checkSupportGLExtensions:${test.checkSupportGLExtensions()}")
    }

    private fun testBitmap() {
        val p = Paint()
        p.setColor(Color(0f, 1f, 0f, 1f))

        /*
         * Draw into a Java Bitmap through software using Skia's native API.
         * Load the Bitmap into an ImageView.
         * Applies Matrix transformations to canvas
         */
        val conf = Bitmap.Config.ARGB_8888
        val bmp = Bitmap.createBitmap(400, 400, conf)
        val bitmapSurface = org.skia.androidkit.Surface(bmp)
        val canvas: Canvas = bitmapSurface.canvas

        canvas.drawRect(0f, 0f, 100f, 100f, p)

        val m = floatArrayOf(
            1f, 0f, 0f, 100f,
            0f, 1f, 0f, 100f,
            0f, 0f, 1f, 0f,
            0f, 0f, 0f, 1f
        )
        p.setColor(Color(0f, 0f, 1f, 1f))
        canvas.save()
        canvas.concat(m)
        canvas.drawRect(0f, 0f, 100f, 100f, p)
        canvas.restore()

        val snapshot: Image = bitmapSurface.makeImageSnapshot()
        canvas.drawImage(snapshot, 0f, 200f)

        try {
            val image: Image = Image.fromStream(resources.openRawResource(R.raw.brickwork_texture))
            // TODO: Canvas.scale
            canvas.concat(Matrix().scale(10f, 10f))
            canvas.drawImage(image, 20f, 0f, SamplingOptions.CATMULLROM())
        } catch (e: Exception) {
            e.printStackTrace()
        }
        binding.mImageView.setImageBitmap(bmp)
    }

    @TargetApi(19)
    override fun onResume() {
        super.onResume()
        nativeChoreographer?.onResume()
    }

    override fun onPause() {
        super.onPause()
        nativeChoreographer?.onPause()
    }

    override fun onDestroy() {
        super.onDestroy()
        fAnimationTimer?.cancel()
        fAnimationTimer = null
    }

    fun updateFPS(fFPS: Float) {
        this.runOnUiThread { binding.sampleText.text = String.format("%2.2f FPS", fFPS) }
    }
    private external fun getAbiString() : String

    private external fun throwsException()
}