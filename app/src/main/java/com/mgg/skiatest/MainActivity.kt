package com.mgg.skiatest

import android.annotation.TargetApi
import android.os.Bundle
import android.view.*
import androidx.appcompat.app.AppCompatActivity
import com.mgg.skiatest.databinding.ActivityMainBinding
import com.mgg.skiatest.java.impl.Log

open class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding
    private var testData: TestData ? = null
    private var nativeChoreographer: NativeChoreographer ? = null

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)
        testData()
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

    fun updateFPS(fFPS: Float) {
        this.runOnUiThread { binding.sampleText.text = String.format("%2.2f FPS", fFPS) }
    }
}