package com.test.integritycheck_android;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Context;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;

public class MainActivity extends AppCompatActivity {

    Button btn = null;
    Context mContext = null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mContext = getApplicationContext();
        btn = findViewById(R.id.check);
        btn.setOnClickListener(new MyClick());
    }

    class MyClick implements View.OnClickListener{

        @Override
        public void onClick(View view) {
            new Thread(new Runnable() {
                @Override
                public void run() {
                    UtilTools.getFileCrc(mContext);
                    UtilTools.getAllFileCrc(mContext);
                }
            }).start();
        }
    }
}