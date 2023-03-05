package com.mgg.skiatest

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import com.mgg.skiatest.databinding.ActivityMainBinding

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
    }
}