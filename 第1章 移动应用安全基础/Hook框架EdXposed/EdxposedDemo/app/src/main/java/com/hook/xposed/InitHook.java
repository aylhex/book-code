package com.hook.xposed;

import static de.robv.android.xposed.XposedHelpers.findAndHookMethod;

import de.robv.android.xposed.IXposedHookLoadPackage;
import de.robv.android.xposed.IXposedHookZygoteInit;
import de.robv.android.xposed.XC_MethodHook;
import de.robv.android.xposed.XposedBridge;
import de.robv.android.xposed.XposedHelpers;
import de.robv.android.xposed.callbacks.XC_LoadPackage.LoadPackageParam;

import android.content.Context;
import android.content.pm.PackageManager;
import android.content.res.Resources;
import android.content.res.XmlResourceParser;
import android.util.AttributeSet;

import org.xmlpull.v1.XmlPullParser;

import java.lang.reflect.Field;
import java.lang.reflect.Method;
import java.security.cert.X509Certificate;


public class InitHook implements IXposedHookLoadPackage{


    public void handleLoadPackage(final LoadPackageParam loadPackageParam) throws Throwable {

        findAndHookMethod("com.test.demo.FirstFragment", loadPackageParam.classLoader, "getTextViewShowData",
                new XC_MethodHook() {
                    @Override
                    protected void afterHookedMethod(MethodHookParam param) throws Throwable {
                        String result = "该返回结果已经被Hook！！！";
                        param.setResult(result );
                    }
                }
        );
    }
}


