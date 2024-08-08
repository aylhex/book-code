package com.test.jsbridge;

import static android.content.ContentValues.TAG;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.graphics.Bitmap;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.util.Log;
import android.webkit.WebResourceRequest;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.webkit.WebViewClient;

import androidx.annotation.Nullable;

import java.util.Set;

public class WebViewActivity extends Activity {

    WebView mWebView = null;

    private WebViewClient webViewClient = new WebViewClient() {
        @Override
        public boolean shouldOverrideUrlLoading(WebView view, WebResourceRequest request) {

            Log.i("JSBridge1", request.getUrl().toString());
            Uri uri = request.getUrl();
            if (uri.toString().startsWith("jsbridge://")) {
                //解析链接的参数
                String methodName = uri.getPath();
                // 将协议中定义给对应的Native函数做处理
                if (methodName.equals("/toast")) {
                    String data = uri.getQueryParameter("arg");
                    JSInterface.jsToast(data);
                }
            }
            return false;
        }

        @Override
        public boolean shouldOverrideUrlLoading(WebView view, String url) {
            Log.i("JSBridge0", url);
            // 判断是否是定义的JsBridge协议，
            if (url.startsWith("jsbridge://")) {
                //解析链接的参数
                Uri uri = Uri.parse(url);
                String methodName = uri.getPath();
                // 将协议中定义给对应的Native函数做处理
                if (methodName.equals("/toast")) {
                    String data = uri.getQueryParameter("arg");
                    JSInterface.jsToast(data);
                }
            }
            return true;
        }

        @Override
        public void onPageStarted(WebView view, String url, Bitmap favicon) {
            Log.i("JSBridge0", url);
            // 判断是否是定义的JsBridge协议，
            if (url.startsWith("jsbridge://")) {
                //解析链接的参数
                Uri uri = Uri.parse(url);
                String methodName = uri.getPath();
                // 将协议中定义给对应的Native函数做处理
                if (methodName.equals("/toast")) {
                    String data = uri.getQueryParameter("arg");
                    JSInterface.jsToast(data);
                }
            }
            super.onPageStarted(view, url, favicon);
        }

    };

    @SuppressLint("SetJavaScriptEnabled")
    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.webview);
        MContext.init(getApplicationContext());

        mWebView = findViewById(R.id.webView);
        WebSettings webSettings = mWebView.getSettings();
        webSettings.setJavaScriptEnabled(true);
        webSettings.setDefaultTextEncodingName("UTF-8");
        mWebView.setWebViewClient(webViewClient);
        mWebView.addJavascriptInterface(new JSInterface(), "jsObj");
        //mWebView.loadUrl("http://10.29.109.93:8000/jsbridge.html");
        mWebView.loadUrl("jsbridge://js.call.native/toast?arg=customagreement");
    }


}
