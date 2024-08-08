package com.device.risk.utils.sensors;

import android.content.Context;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import org.json.JSONException;
import org.json.JSONObject;


import static android.content.Context.SENSOR_SERVICE;

public class SensorData {
    private SensorManager mSensorManager = null;
    private Sensor stepSensor = null;
    private Sensor orientationSensor = null;
    private Sensor gyroscopeSensor = null;
    private Sensor accelerometerSensor = null;

    private boolean gyroscopeState = true;   //是否处于运动状态
    private double oriValueGyroscope = 0;  //原始值
    private double lstValueGyroscope = 0;  //上次的值
    private double curValueGyroscope = 0;  //当前值

    private boolean accelerometerState = true;   //是否处于运动状态
    private double oriValueAccelerometer = 0;  //原始值
    private double lstValueAccelerometer = 0;  //上次的值
    private double curValueAccelerometer = 0;  //当前值

    private boolean orientationState = true;   //是否处于运动状态
    private double curValueOrientation = 0;
    private double lstValueOrientation = 0;
    private double oriValueOrientation = 0;

    private JSONObject mJson = null;

    public SensorData() {
        mJson = new JSONObject();
        try {
            mJson.put("step_detector", 0);

            mJson.put("orientation", 0);

            mJson.put("gyroscope", 0);

            mJson.put("accelerometer", 0);
        } catch (JSONException e) {
            //e.printStackTrace();
        }
    }

    public void startRegisterSensor(Context context) {

        mSensorManager = (SensorManager) context.getSystemService(SENSOR_SERVICE);
        /*通过SensorManager获取相应的（压力传感器）Sensor类型对象*/
        // 计步感应器
        stepSensor = mSensorManager.getDefaultSensor(Sensor.TYPE_STEP_DETECTOR);
        // 检测传感器是否存在
        if (stepSensor != null) {
            //计步感应器
            mSensorManager.registerListener(mSensorEventListener, stepSensor, Sensor.TYPE_STEP_DETECTOR);
        }else
        {
            try {
                mJson.put("step_detector", -1);
            } catch (JSONException e) {
                //e.printStackTrace();
            }
        }
        // 陀螺仪传感器
        gyroscopeSensor = mSensorManager.getDefaultSensor(Sensor.TYPE_GYROSCOPE);
        // 检测传感器是否存在
        if (gyroscopeSensor != null) {
            // 距离感应器监听
            mSensorManager.registerListener(mSensorEventListener, gyroscopeSensor, Sensor.TYPE_GYROSCOPE);
        }else{
            try {
                mJson.put("gyroscope", -1);
            } catch (JSONException e) {
                //e.printStackTrace();
            }
        }
        // 加速度传感器
        accelerometerSensor = mSensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
        // 检测传感器是否存在
        if (accelerometerSensor != null) {
            // 加速度感应器监听
            mSensorManager.registerListener(mSensorEventListener, accelerometerSensor, Sensor.TYPE_ACCELEROMETER);
        }else{
            try {
                mJson.put("accelerometer", -1);
            } catch (JSONException e) {
                //e.printStackTrace();
            }
        }
        // 方向感应器
        orientationSensor = mSensorManager.getDefaultSensor(Sensor.TYPE_ORIENTATION);
        // 检测传感器是否存在
        if (orientationSensor != null) {
            // 注册监听方向传感器
            mSensorManager.registerListener(mSensorEventListener, orientationSensor, Sensor.TYPE_ORIENTATION);
        }else{
            try {
                mJson.put("orientation", -1);
            } catch (JSONException e) {
                //e.printStackTrace();
            }
        }
    }

    private final SensorEventListener mSensorEventListener = new SensorEventListener() {

        @Override
        public void onSensorChanged(SensorEvent sensorEvent) {

            switch (sensorEvent.sensor.getType()) {

                case Sensor.TYPE_STEP_DETECTOR:
                    float setup = sensorEvent.values[0];
                    try {
                        mJson.put("step_detector", setup);
                    } catch (JSONException e) {
                        //e.printStackTrace();
                    }
                    break;

                case Sensor.TYPE_GYROSCOPE:
                    double rangeGyroscope = 10;   //设定一个精度范围
                    //需要将弧度转为角度
                    float X = (float) Math.toDegrees(sensorEvent.values[0]);
                    float Y = (float) Math.toDegrees(sensorEvent.values[1]);
                    float Z = (float) Math.toDegrees(sensorEvent.values[2]);

                    curValueGyroscope = magnitude(X, Y, Z);   //计算当前的模
                    //向上加速的状态
                    if (gyroscopeState == true) {
                        if (curValueGyroscope >= lstValueGyroscope) {
                            lstValueGyroscope = curValueGyroscope;
                        } else {
                            //检测到一次峰值
                            if (Math.abs(curValueGyroscope - lstValueGyroscope) > rangeGyroscope) {
                                oriValueGyroscope = curValueGyroscope;
                                gyroscopeState = false;

                                try {
                                    mJson.put("gyroscope", Math.abs(curValueGyroscope - lstValueGyroscope));
                                } catch (JSONException e) {
                                    //e.printStackTrace();
                                }
                            }
                        }
                    }

                    //向下加速的状态
                    if (gyroscopeState == false) {
                        if (curValueGyroscope <= lstValueGyroscope) {
                            lstValueGyroscope = curValueGyroscope;
                        } else {
                            if (Math.abs(curValueGyroscope - lstValueGyroscope) > rangeGyroscope) {
                                //检测到一次峰值
                                oriValueGyroscope = curValueGyroscope;
                                gyroscopeState = true;
                            }
                        }
                    }
                    break;

                case Sensor.TYPE_ACCELEROMETER:

                    double rangeAccelerometer = 1;   //设定一个精度范围
                    float[] value = sensorEvent.values;
                    curValueAccelerometer = magnitude(value[0], value[1], value[2]);   //计算当前的模
                    //向上加速的状态
                    if (accelerometerState == true) {
                        if (curValueAccelerometer >= lstValueAccelerometer) {
                            lstValueAccelerometer = curValueAccelerometer;
                        } else {
                            //检测到一次峰值
                            if (Math.abs(curValueAccelerometer - lstValueAccelerometer) > rangeAccelerometer) {
                                oriValueAccelerometer = curValueAccelerometer;
                                accelerometerState = false;

                                try {
                                    mJson.put("accelerometer", Math.abs(curValueAccelerometer - lstValueAccelerometer));
                                } catch (JSONException e) {
                                    //e.printStackTrace();
                                }
                            }
                        }
                    }

                    //向下加速的状态
                    if (accelerometerState == false) {
                        if (curValueAccelerometer <= lstValueAccelerometer) {
                            lstValueAccelerometer = curValueAccelerometer;
                        } else {
                            if (Math.abs(curValueAccelerometer - lstValueAccelerometer) > rangeAccelerometer) {
                                //检测到一次峰值
                                oriValueAccelerometer = curValueAccelerometer;
                                accelerometerState = true;
                            }
                        }
                    }
                    break;

                case Sensor.TYPE_ORIENTATION:
                    double rangeOrientation = 0.5;
                    curValueOrientation = magnitude(sensorEvent.values[0], sensorEvent.values[1], sensorEvent.values[2]);   //计算当前的模
                    if (orientationState == true) {
                        if (curValueOrientation >= lstValueOrientation) {
                            lstValueOrientation = curValueOrientation;
                        }
                        if (Math.abs(curValueOrientation - lstValueOrientation) > rangeOrientation) {
                            oriValueOrientation = curValueOrientation;
                            orientationState = false;

                            try {
                                mJson.put("orientation", Math.abs(curValueOrientation - lstValueOrientation));
                            } catch (JSONException e) {
                                //e.printStackTrace();
                            }
                        }
                    }

                    if (orientationState == false) {
                        if (curValueOrientation <= lstValueOrientation) {
                            lstValueOrientation = curValueOrientation;
                        }
                        if (Math.abs(curValueOrientation - lstValueOrientation) > rangeOrientation) {
                            oriValueOrientation = curValueOrientation;
                            orientationState = true;
                        }
                    }
                    break;
            }
        }

        @Override
        public void onAccuracyChanged(Sensor sensor, int accuracy) {

        }
    };

    public void stopGetSensorData() {
        mSensorManager.unregisterListener(mSensorEventListener);
    }

    public String getSensorResult(){
        StringBuilder mResult = new StringBuilder();
        return mResult.append(mJson.toString()).toString();
    }

    //向量求模
    public double magnitude(float x, float y, float z) {
        double magnitude = 0;
        magnitude = Math.sqrt(x * x + y * y + z * z);
        return magnitude;
    }
}
