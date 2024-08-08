package com.test.jsbridge;

import android.webkit.JavascriptInterface;
import android.widget.Toast;

public class JSInterface {
    @JavascriptInterface
    public void showToast(String data) {
        Toast.makeText(MContext.getContext(), data, Toast.LENGTH_SHORT).show();
    }

    public static void jsToast(String data) {
        Toast.makeText(MContext.getContext(), data, Toast.LENGTH_SHORT).show();
    }
}
