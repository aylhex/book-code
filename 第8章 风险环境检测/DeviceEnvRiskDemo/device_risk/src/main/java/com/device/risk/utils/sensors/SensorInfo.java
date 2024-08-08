package com.device.risk.utils.sensors;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import org.json.JSONObject;
import java.io.File;
import java.util.List;

public class SensorInfo {
    public static int COUNT_ACCELEROMETER = 0;
    public Sensor sensor = null;
    public Sensor mGYROSCOPE = null;
    private SensorManager senserManager = null;
    public static float valueX = 0;
    public static float valueY = 0;
    public static float valueZ = 0;
    public static float X = 0;
    public static float Y = 0;
    public static float Z = 0;
    public static float gX = 0;
    public static float gY = 0;
    public static float gZ = 0;
    private static boolean FLAG_ACCELEROMETER = false;
    private static boolean FLAG_GYROSCOPE = false;
    private int usbstate = 0;

    private final static String ACTION ="android.hardware.usb.action.USB_STATE";

    public SensorInfo(Context context) {

        IntentFilter filter = new IntentFilter();

        filter.addAction(ACTION);
        BroadcastReceiver usBroadcastReceiver = new MyBroadcastReceiver();
        context.registerReceiver(usBroadcastReceiver, filter);

        senserManager = (SensorManager) context.getSystemService(Context.SENSOR_SERVICE);
        sensor = senserManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
        mGYROSCOPE = senserManager.getDefaultSensor(Sensor.TYPE_GYROSCOPE);
        if (COUNT_ACCELEROMETER <= 3) {
            senserManager.registerListener(listener, sensor, SensorManager.SENSOR_DELAY_NORMAL);
            senserManager.registerListener(glistener, mGYROSCOPE, SensorManager.SENSOR_DELAY_NORMAL);
        }
        context.unregisterReceiver(usBroadcastReceiver);
    }

    public String getInfo() {
        if (FLAG_ACCELEROMETER) {
            return getSensorInfo() + checkQemu() + vibratorCheck() + "1$";
        } else {
            return getSensorInfo() + checkQemu() + vibratorCheck() + "0$";
        }
    }

    public String getGYROSCOPEInfo() throws Exception {
        if (FLAG_GYROSCOPE) {
            JSONObject json = new JSONObject();
            json.put("x", gX);
            json.put("y", gY);
            json.put("z", gZ);
            return json.toString();
        } else {
            return "None";
        }
    }

    public int getUSBState() {
        return usbstate;
    }

    private String vibratorCheck() {
        try {
            File file = new File("/sys/class/timed_output/vibrator/enable");
            if (file.exists()) {
                return "vb1$";
            }
        } catch (Throwable e) {
        }
        return "vb0$";
    }

    private String checkQemu() {
        try {
            String[] paths = {"/dev/socket/qemud", "/dev/qemu_pipe", "/system/lib/libc_malloc_debug_qemu.so", "/sys/qemu_trace",
                    "/system/bin/qemu-props"};
            File file = null;
            for (String path : paths) {
                file = new File(path);
                if (file != null) {
                    if (file.exists()) {
                        return "Qe1$";
                    }
                }
            }
        } catch (Throwable e) {

        }
        return "Qe0$";
    }

    private String getSensorInfo() {
        String info = "";
        StringBuilder mStringBuilder = new StringBuilder();
        try {
            info = hasGravitySensor() ? "G1$" : "G0$";
            mStringBuilder.append(info);
        } catch (Throwable e) {
            mStringBuilder.append("G2");
        }
        try {
            info = hasGyroscopeSensor() ? "T1$" : "T0$";
            mStringBuilder.append(info);
        } catch (Throwable e) {
            mStringBuilder.append("T2");
        }
        try {
            info = hasLightSensor() ? "L1$" : "L0$";
            mStringBuilder.append(info);
        } catch (Throwable e) {
            mStringBuilder.append("L2");
        }
        try {
            info = hasLinearAccelerationSensor() ? "A1$" : "A0$";
            mStringBuilder.append(info);
        } catch (Throwable e) {
            mStringBuilder.append("A2");
        }
        try {
            info = hasMagneticFieldSensor() ? "M1$" : "M0$";
            mStringBuilder.append(info);
        } catch (Throwable e) {
            mStringBuilder.append("M2");
        }
        try {
            info = hasProximitySensor() ? "D1$" : "D0$";
            mStringBuilder.append(info);
        } catch (Throwable e) {
            mStringBuilder.append("D2");
        }
        try {
            info = hasTemperatureSensor() ? "W1$" : "W0$";
            mStringBuilder.append(info);
        } catch (Throwable e) {
            mStringBuilder.append("W2");
        }
        try {
            info = hasPressureSensor() ? "P1$" : "P0$";
            mStringBuilder.append(info);
        } catch (Throwable e) {
            mStringBuilder.append("P2");
        }

        return mStringBuilder.toString();
    }

    private boolean hasGravitySensor() {
        boolean flag = false;
        try {
            List v1_1 = this.senserManager.getSensorList(9);
            if (v1_1 == null) {
                return flag;
            }

            if (v1_1.size() <= 0) {
                return flag;
            }
        } catch (Throwable v1) {

            return flag;
        }

        return true;
    }

    private boolean hasGyroscopeSensor() {
        List list = this.senserManager.getSensorList(4);
        boolean flag = list == null || list.size() <= 0 ? false : true;
        return flag;
    }

    private boolean hasLightSensor() {
        List list = this.senserManager.getSensorList(5);
        boolean flag = list == null || list.size() <= 0 ? false : true;
        return flag;
    }

    private boolean hasLinearAccelerationSensor() {
        boolean flag = false;
        try {
            List list = this.senserManager.getSensorList(10);
            if (list == null) {
                return flag;
            }

            if (list.size() <= 0) {
                return flag;
            }
        } catch (Throwable v1) {
            return flag;
        }

        return true;
    }

    private boolean hasMagneticFieldSensor() {
        List list = this.senserManager.getSensorList(2);
        boolean flag = list == null || list.size() <= 0 ? false : true;
        return flag;
    }

    private boolean hasPressureSensor() {
        List list = this.senserManager.getSensorList(6);
        boolean flag = list == null || list.size() <= 0 ? false : true;
        return flag;
    }

    private boolean hasProximitySensor() {
        List list = this.senserManager.getSensorList(8);
        boolean flag = list == null || list.size() <= 0 ? false : true;
        return flag;
    }

    private boolean hasTemperatureSensor() {
        List list = this.senserManager.getSensorList(7);
        boolean flag = list == null || list.size() <= 0 ? false : true;
        return flag;
    }

    MySensorEventListener listener = new MySensorEventListener();
    MySensorEventListener glistener = new MySensorEventListener();



    public void unRegister() {
        try {
            if (null != senserManager) {
                senserManager.unregisterListener(listener);
                senserManager.unregisterListener(glistener);
            }
        } catch (Throwable ignore) {
        }
    }

    private class MySensorEventListener implements SensorEventListener
    {

        @Override
        public void onSensorChanged(SensorEvent event) {
            switch (event.sensor.getType()) {
                case Sensor.TYPE_ACCELEROMETER:
                    if (valueX == 0 && valueY == 0 && valueZ == 0) {
                        valueX = Math.abs(event.values[0]);
                        valueY = Math.abs(event.values[1]);
                        valueZ = Math.abs(event.values[2]);
                    } else {
                        X = Math.abs(event.values[0]);
                        Y = Math.abs(event.values[1]);
                        Z = Math.abs(event.values[2]);

                        if (valueX != X || valueY != Y || valueZ != Z) {
                            FLAG_ACCELEROMETER = true;
                            unRegister();
                        } else {
                            if (COUNT_ACCELEROMETER > 3) {
                                unRegister();
                            }
                            COUNT_ACCELEROMETER++;
                        }
                    }
                    break;
                case Sensor.TYPE_GYROSCOPE:
                    FLAG_GYROSCOPE = true;
                    gX = event.values[0];
                    gY = event.values[1];
                    gZ = event.values[2];
                    break;
                default:
                    break;
            }
        }

        @Override
        public void onAccuracyChanged(Sensor sensor, int accuracy) {

        }
    }

    private class MyBroadcastReceiver extends BroadcastReceiver
    {
        @Override
        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();

            if (action.equals(ACTION)) {

                boolean connected = intent.getExtras().getBoolean("connected");

                if (connected)
                {
                    usbstate = 1;
                }
            }
        }
    }
}

