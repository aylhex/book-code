package com.device.demo

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.padding
import androidx.compose.material3.Button
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.Surface
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.setValue
import androidx.compose.ui.Modifier
import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.dp
import com.device.demo.ui.theme.DeviceEnvRiskDemoTheme
import com.device.risk.android.Device

class MainActivity : ComponentActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContent {
            DeviceEnvRiskDemoTheme {
                // A surface container using the 'background' color from the theme
                Surface(
                    modifier = Modifier.fillMaxSize(),
                    color = MaterialTheme.colorScheme.background
                ) {
                    Greeting("Android")
                }
            }
        }
    }
}

@Composable
fun Greeting(name: String, modifier: Modifier = Modifier) {
    // 创建一个状态变量来保存显示的文本
    var greetingText by remember { mutableStateOf("待检测") }

    // 获取当前的上下文
    val context = LocalContext.current

    // 垂直布局
    Column(
        modifier = modifier.padding(16.dp),
        verticalArrangement = Arrangement.Top
    ) {
        Button(
            onClick = {
                greetingText = Device.collect(context) } // 点击事件
        ) {
            Text("环境检测") // 按钮上的文本
        }
        Text(
            text = greetingText, // 显示的文本
            modifier = Modifier.padding(bottom = 8.dp) // 底部填充
        )
    }
}

@Preview(showBackground = true)
@Composable
fun GreetingPreview() {
    DeviceEnvRiskDemoTheme {
        Greeting("Android")
    }
}