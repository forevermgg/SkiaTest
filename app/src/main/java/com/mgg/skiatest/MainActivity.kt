package com.mgg.skiatest

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import com.mgg.skiatest.databinding.ActivityMainBinding
import com.mgg.skiatest.java.impl.Log

class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)
        testData()
    }

    private fun testData() {
        TestData.makeEmpty()
        val test = CheckEglEnvironment.makeEmpty()
        test.checkSupportEgl()
        test.checkSupportGLExtensions()
        Log.error("checkSupportEgl:${test.checkSupportEgl()}")
        Log.error("checkSupportGLExtensions:${test.checkSupportGLExtensions()}")
    }
}