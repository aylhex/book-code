package com.dynamic.debug;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Toast;

import com.dynamic.debug.databinding.ActivityMainBinding;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'debug' library on application startup.
    static {
        System.loadLibrary("debug");
    }

    private ActivityMainBinding binding;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        binding.btn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                String input = binding.edit.getText().toString();
                if (checkInput(input.isEmpty()? "None":input )){
                    Toast.makeText(getApplicationContext(),"对比成功",Toast.LENGTH_SHORT).show();
                }else {
                    Toast.makeText(getApplicationContext(),"对比失败",Toast.LENGTH_SHORT).show();
                }
            }
        });
    }

    /**
     * A native method that is implemented by the 'debug' native library,
     * which is packaged with this application.
     */
    public native boolean checkInput(String input);
}