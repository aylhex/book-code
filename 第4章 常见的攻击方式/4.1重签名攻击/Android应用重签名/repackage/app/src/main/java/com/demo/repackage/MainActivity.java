package com.demo.repackage;

import android.content.DialogInterface;
import android.os.Bundle;

import com.google.android.material.snackbar.Snackbar;

import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;

import android.util.Log;
import android.view.View;

import androidx.navigation.NavController;
import androidx.navigation.Navigation;
import androidx.navigation.ui.AppBarConfiguration;
import androidx.navigation.ui.NavigationUI;

import com.demo.repackage.databinding.ActivityMainBinding;

import android.view.Menu;
import android.view.MenuItem;

public class MainActivity extends AppCompatActivity {

    private ActivityMainBinding binding;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        setSupportActionBar(binding.toolbar);

        Log.i("current sign", ToolUtils.getAppSignature(getApplicationContext()));

        if (!ToolUtils.checkSignature(getApplicationContext())){
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
}