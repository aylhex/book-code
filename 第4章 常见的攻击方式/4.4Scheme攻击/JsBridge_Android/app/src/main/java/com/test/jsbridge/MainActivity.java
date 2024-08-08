package com.test.jsbridge;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

public class MainActivity extends AppCompatActivity {

    Button jsbt;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        jsbt = findViewById(R.id.bridge_btn);
        jsbt.setOnClickListener(new MyClick());
    }
    private class MyClick implements View.OnClickListener {

        @Override
        public void onClick(View v) {
            Intent mIntent = new Intent(MainActivity.this, WebViewActivity.class);
            startActivity(mIntent);
        }
    }
}

