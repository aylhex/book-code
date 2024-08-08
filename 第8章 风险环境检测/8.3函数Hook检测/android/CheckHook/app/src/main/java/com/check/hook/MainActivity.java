package com.check.hook;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.View;

import com.check.hook.databinding.ActivityMainBinding;
import com.check.utils.tools.MContext;
import com.check.utils.tools.MLog;

import java.util.Map;

public class MainActivity extends AppCompatActivity implements View.OnClickListener {

    // Used to load the 'hook' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }


    private ActivityMainBinding binding;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        MContext.init(getApplicationContext());
        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());
        binding.gothookBtn.setOnClickListener(this);
        binding.inlinehookBtn.setOnClickListener(this);
        binding.javahookBtn.setOnClickListener(this);
    }


    @Override
    public void onClick(View view) {
        switch (view.getId()){
            case R.id.gothook_btn:
                String got = checkGotHook();
                binding.gotText.setText(got);
                break;
            case R.id.inlinehook_btn:
                String inline = checkInlineHook();
                binding.inlineText.setText(inline);
                break;
            case R.id.javahook_btn:
                Map resutl = CheckJavaHook.getCheckJavaMethodInfo();
                MLog.i("JavaHook", resutl.toString());
                break;
        }
    }

    /**
     * A native method that is implemented by the 'hook' native library,
     * which is packaged with this application.
     */
    public native String checkGotHook();

    public native String checkInlineHook();
}