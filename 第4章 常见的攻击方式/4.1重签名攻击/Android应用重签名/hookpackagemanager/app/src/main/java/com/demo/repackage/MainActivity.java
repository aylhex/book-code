package com.demo.repackage;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;

import com.demo.tools.ToolUtils;

public class MainActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        ToolUtils.getSignature2(getApplicationContext());

        if (ToolUtils.checkSignature(getApplicationContext())){
            Log.i("checkSignature", "True");
        }else {
            Log.i("checkSignature", "False");
        }
    }
}