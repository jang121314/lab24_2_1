package com.example.arduinterpreterapplication

import android.content.Context

class CodeManager(private val context: Context) {
    companion object {
        const val DEFAULT_CODE =
"""
0 0 0 0

case 31:

setup:

loop:

end:




"""

        private const val PREFS_NAME = "ArduInterpreter"
        private const val KEY_LAST_CODE = "lastCode"

    }

    private val prefs = context.getSharedPreferences(PREFS_NAME, Context.MODE_PRIVATE)

    fun loadCode(): String =
        prefs.getString(KEY_LAST_CODE, DEFAULT_CODE) ?: DEFAULT_CODE

    fun saveCode(code: String) {
        prefs.edit()
            .putString(KEY_LAST_CODE, code)
            .apply()
    }
}