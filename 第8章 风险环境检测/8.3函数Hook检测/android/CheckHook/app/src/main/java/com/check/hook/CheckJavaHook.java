package com.check.hook;


import static com.check.hook.HookUtils.*;

import com.check.utils.tools.MStringUtils;

import org.json.JSONObject;
import java.util.LinkedHashMap;
import java.util.Map;


public class CheckJavaHook {
    private static final String TAG = "CheckJavaHook";

    private static JSONObject checkJavaMethod(int start, int end) {

        String className = "";
        JSONObject methodDetail = new JSONObject();
        Map map = new LinkedHashMap();
        for (int i = start;i < end;i++) {
            String[] array = HookUtils.jmethodList[i].split("\\|");

            if (MStringUtils.isEmpty(className)){
                className = array[0];
            }
            map.put(array[1].split("\\(")[0], array[1]);
        }
        methodDetail = Flags.showFlags(className, map);
        return methodDetail;
    }

    public static Map getCheckJavaMethodInfo(){

        Map<String, Object> jopcodeInfo = new LinkedHashMap<>();

        jopcodeInfo.put(j_WifiManager_FLAGOPCODE, checkJavaMethod(0, 3));

        return jopcodeInfo;
    }
}
