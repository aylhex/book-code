package com.verify.signature;

import android.content.Context;
import android.content.DialogInterface;
import android.os.Bundle;
import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import com.verify.signature.databinding.ActivityMainBinding;

public class MainActivity extends AppCompatActivity {

    private ActivityMainBinding binding;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());
        Context context = getApplicationContext();
        Log.i("Current sign ===>", Tools.getAppSignature(context));
        Log.i("Reflect sign ===>", Tools.getReflectSignature(context));
        Log.i("Native sign ===>", Tools.getSignByJni(context));

        binding.fab.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                Tools.getSignatureFileCrc(context);

                if (!Tools.checkSignature(context)){
                    new AlertDialog.Builder(MainActivity.this).setTitle("安全提醒")//设置对话框标题
                    .setMessage("当前使用的客户端为非官方版本，存在极大的安全隐患。为了您的财产安全请在官方渠道下载重新安装！")
                    .setPositiveButton("确定", new DialogInterface.OnClickListener() {

                        @Override
                        public void onClick(DialogInterface dialog, int which) {
                            System.exit(0);
                        }
                    }).show();
                }

            }
        });

    }
}

