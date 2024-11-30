package com.example.arduinterpreterapplication

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.enableEdgeToEdge
import androidx.compose.ui.graphics.Color
import androidx.compose.foundation.layout.*
import androidx.compose.material3.*
import androidx.compose.runtime.Composable
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.dp
import androidx.compose.runtime.remember
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.getValue
import androidx.compose.runtime.setValue
import androidx.activity.viewModels
import androidx.compose.foundation.layout.height
import com.example.arduinterpreterapplication.ui.theme.ArduInterpreterApplicationTheme


class MainActivity : ComponentActivity() {
    private val viewModel: BluetoothViewModel by viewModels()
    private lateinit var codeManager: CodeManager

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()
        viewModel.initializeBluetooth(this)
        viewModel.registerBluetoothReceiver(this)
        codeManager = CodeManager(this)
        viewModel.initCodeManager(this)

        setContent {
            ArduInterpreterApplicationTheme {
                Scaffold(modifier = Modifier.fillMaxSize(), contentWindowInsets = WindowInsets.ime ) { innerPadding ->
                    MainScreen(
                        modifier = Modifier.padding(innerPadding),
                        bluetoothState = viewModel.bluetoothState.value,
                        compileState = viewModel.compileState.value,
                        uploadState = viewModel.uploadState.value,
                        initialText = codeManager.loadCode(),  // 추가
                        onCompileClick = { text -> viewModel.compileCode(text, this) },
                        onUploadClick = { viewModel.uploadData(this) },
                        onBluetoothClick = { viewModel.connectToDevice(this) }
                    )
                }
            }
        }
    }
}

@Composable
fun MainScreen(
    modifier: Modifier = Modifier,
    initialText: String,
    bluetoothState: String,
    compileState: String?,
    uploadState: String?,
    onCompileClick: (String) -> Unit,
    onUploadClick: () -> Unit,
    onBluetoothClick: () -> Unit

) {
    var text by remember { mutableStateOf(initialText) }

    Column(modifier = modifier.padding(16.dp)) {
        Row(
            modifier = Modifier
                .fillMaxWidth()
                .padding(start = 12.dp, end = 24.dp),
            horizontalArrangement = Arrangement.SpaceBetween
        ) {
            Button(
                onClick = { onCompileClick(text) },
                // containerColor 제거하고 기본 색상 유지
            ) {
                Text(
                    text = "Compile",
                    color = when (compileState) {
                        null -> Color.White        // 기본
                        "compiling" -> Color.Gray  // 진행중
                        "failed" -> Color.Red      // 실패
                        else -> Color.White        // 성공
                    }
                )
            }

            Button(
                onClick = { onUploadClick() },
                // containerColor 제거
            ) {
                Text(
                    text = "Upload",
                    color = when (compileState) {
                    null -> Color.White        // 기본
                    "compiling" -> Color.Gray  // 진행중
                    "failed" -> Color.Red      // 실패
                    else -> Color.White        // 성공
                    }
                )
            }

            Button(
                onClick = { onBluetoothClick() },
                modifier = Modifier.align(Alignment.CenterVertically)
            ) {
                Text(
                    text = "Bluetooth: $bluetoothState",
                    color = when (bluetoothState) {
                        "OFF" -> Color.Red
                        "CONNECTED" -> Color.Green
                        else -> Color.White
                    }
                )
            }
        }


        Spacer(modifier = Modifier.height(16.dp))

        TextField(
            value = text,
            onValueChange = { newText -> text = newText },
            modifier = Modifier
                .fillMaxWidth()
                .weight(1f),
            label = { Text("Enter your code here") }
        )
    }
}

@Preview(showBackground = true)
@Composable
fun MainScreenPreview() {
    ArduInterpreterApplicationTheme {
        MainScreen(
            bluetoothState = "OFF",
            compileState = null,
            uploadState = null,
            onCompileClick = { },    // 빈 람다로 처리
            onUploadClick = { },     // 빈 람다로 처리
            onBluetoothClick = { },   // 빈 람다로 처리
            initialText = "0 0 0 0\n\ncase 31:\n\nsetup:\n\nloop:\n\nend:"
        )
    }
}
