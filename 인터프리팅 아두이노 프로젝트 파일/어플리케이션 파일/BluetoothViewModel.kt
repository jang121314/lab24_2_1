package com.example.arduinterpreterapplication

import android.bluetooth.BluetoothAdapter
import android.bluetooth.BluetoothManager
import android.bluetooth.BluetoothSocket
import android.content.BroadcastReceiver
import android.content.Context
import android.content.Intent
import android.content.IntentFilter
import android.content.pm.PackageManager
import android.os.Build
import android.Manifest
import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import androidx.compose.runtime.State
import androidx.compose.runtime.mutableStateOf
import kotlinx.coroutines.launch
import java.util.UUID
import android.os.Handler
import android.os.Looper
import android.widget.Toast

class BluetoothViewModel : ViewModel() {
    // 상태 변수들 선언
    private val _bluetoothState = mutableStateOf("OFF")
    val bluetoothState: State<String> = _bluetoothState

    private val _compileState = mutableStateOf<String?>(null)
    val compileState: State<String?> = _compileState

    private val _uploadState = mutableStateOf<String?>(null)
    val uploadState: State<String?> = _uploadState

    private var compiledData: CharArray? = null
    private var bluetoothSocket: BluetoothSocket? = null

    private var bluetoothAdapter: BluetoothAdapter? = null

    private lateinit var codeManager: CodeManager


    companion object {
        private val UUID_HC06: UUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB")
    }

    fun initCodeManager(context: Context) {
        codeManager = CodeManager(context)
    }

    // 블루투스 초기화 함수
    fun initializeBluetooth(context: Context) {
        val bluetoothManager = context.getSystemService(Context.BLUETOOTH_SERVICE) as BluetoothManager
        bluetoothAdapter = bluetoothManager.adapter
        updateBluetoothState()
    }

    // 블루투스 상태 업데이트
    fun updateBluetoothState() {
        _bluetoothState.value = when {
            bluetoothSocket?.isConnected == true -> "CONNECTED"
            bluetoothAdapter?.isEnabled == true -> "ON"
            else -> "OFF"
        }
    }


    private val bluetoothReceiver = object : BroadcastReceiver() {
        override fun onReceive(context: Context, intent: Intent) {
            when (intent.action) {
                BluetoothAdapter.ACTION_STATE_CHANGED -> {
                    updateBluetoothState()
                }
            }
        }
    }

    fun registerBluetoothReceiver(context: Context) {
        val filter = IntentFilter(BluetoothAdapter.ACTION_STATE_CHANGED)
        context.registerReceiver(bluetoothReceiver, filter)
    }

    fun unregisterBluetoothReceiver(context: Context) {
        context.unregisterReceiver(bluetoothReceiver)
    }

    fun connectToDevice(context: Context) {
        viewModelScope.launch {
            try {
                // 권한 체크
                if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.S) {
                    if (context.checkSelfPermission(Manifest.permission.BLUETOOTH_CONNECT)
                        != PackageManager.PERMISSION_GRANTED) {
                        _bluetoothState.value = "OFF"
                        showToast(context, "Bluetooth permission not granted")
                        return@launch
                    }
                }

                // 연결 로직
                val device = bluetoothAdapter?.bondedDevices?.find { it.name == " Shim1 " }
                device?.let {
                    bluetoothSocket = it.createRfcommSocketToServiceRecord(UUID_HC06)
                    bluetoothSocket?.connect()
                    _bluetoothState.value = "CONNECTED"
                    showToast(context, "Connected to HC-06")
                } ?: run {
                    _bluetoothState.value = "OFF"
                    showToast(context, "HC-06 not found")
                }
            } catch (e: Exception) {
                _bluetoothState.value = "OFF"
                showToast(context, "Connection failed: ${e.message}")
            }
        }
    }

    // 컴파일 함수
    fun compileCode(text: String, context: Context) {
        viewModelScope.launch {
            try {
                _compileState.value = "compiling"
                codeManager.saveCode(text)

                if (text.isEmpty()) {
                    _compileState.value = "failed"
                    showToast(context, "Error: Empty input")
                    return@launch
                }

                val parser = Parser()
                parser.setInput(text)
                parser.wordRead()
                val output = parser.getOutput()

                // 명시적으로 null 체크 후 할당
                if (output.isNotEmpty()) {
                    compiledData = output
                    _compileState.value = "success"
                    showToast(context, "Compilation successful")
                } else {
                    _compileState.value = "failed"
                    showToast(context, "Error: Parsing failed")
                }

            } catch (e: Exception) {
                _compileState.value = "failed"
                showToast(context, "Compilation failed: ${e.message}")
            }
        }
    }

    // 업로드 함수
    fun uploadData(context: Context) {
        viewModelScope.launch {
            try {
                // 컴파일된 데이터 체크
                if (compiledData == null) {
                    _uploadState.value = "failed"
                    showToast(context, "Error: No compiled data available")
                    return@launch
                }

                _uploadState.value = "uploading"

                // 블루투스 연결 체크
                if (bluetoothSocket?.isConnected != true) {
                    _uploadState.value = "failed"
                    showToast(context, "Error: Bluetooth not connected")
                    return@launch
                }

                // 실제 데이터 전송
                bluetoothSocket?.let { socket ->
                    val output = socket.outputStream
                    output.write(compiledData!!.map { it.code.toByte() }.toByteArray())
                    output.flush()
                    _uploadState.value = "success"
                    showToast(context, "Upload successful")
                }

            } catch (e: Exception) {
                _uploadState.value = "failed"
                showToast(context, "Upload failed: ${e.message}")
            }
        }
    }

    private fun showToast(context: Context, message: String) {
        Handler(Looper.getMainLooper()).post {
            Toast.makeText(context, message, Toast.LENGTH_SHORT).show()
        }
    }

}