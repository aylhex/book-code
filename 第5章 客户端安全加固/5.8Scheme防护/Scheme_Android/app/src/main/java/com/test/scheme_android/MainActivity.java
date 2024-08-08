package com.test.scheme_android;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.provider.Settings;
import android.util.Log;
import android.view.View;
import android.widget.Button;

import java.lang.reflect.Field;
import java.lang.reflect.Method;
import java.lang.reflect.Modifier;

public class MainActivity extends AppCompatActivity {

    Button mBtn;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mBtn = findViewById(R.id.mbtn);
        mBtn.setOnClickListener(new MyClick());
    }

    public static void exceptionCheck() {
        try {
            throw new Exception("Deteck hook");
        } catch (Exception e) {
            e.printStackTrace();
        }
    }


    public String getId(){
        try {
            Method field = MainActivity.class.getDeclaredMethod("getId");
            if (Modifier.isNative(field.getModifiers())){
                Log.i("test====>", "Is Native");
            }else {
                Log.i("test====>", "Is not Native");
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        exceptionCheck();


        String androidId = Settings.System.getString(getContentResolver(), Settings.Secure.ANDROID_ID);
        return androidId;
    }


    class MyClick implements View.OnClickListener{
        @Override
        public void onClick(View view) {
            String androidId = getId();
            Log.i("test====>", androidId);
        }
    }
}