package com.mgg.skiatest

import android.annotation.TargetApi
import android.os.Bundle
import android.view.*
import androidx.appcompat.app.AppCompatActivity
import com.mgg.skiatest.databinding.ActivityMainBinding
import com.mgg.skiatest.java.impl.Log
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
        binding.sampleText.setText(getAbiString())
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
}