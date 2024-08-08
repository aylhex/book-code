package com.device.risk.utils.tools;

import org.json.JSONException;
import org.json.JSONObject;
public class MJSON {

    private JSONObject mJSONObject = new JSONObject();

    private String error = "Eexcept_error";
    private String defaultValues = "6666";

    public void setScreen(String screen) {
        // screen
        String tmp= "screen";
        try {
            mJSONObject.put(tmp, screen);
        } catch (Exception e) {
            try {
                mJSONObject.put(tmp, error);
            } catch (JSONException e1) {

            }
        }
    }

    public String toString()
    {
        return mJSONObject.toString();
    }

    public void setVersion(String version) {
        // rom
        String tmp="rom";
        try {
            mJSONObject.put(tmp, version);
        } catch (Exception e) {
            try {
                mJSONObject.put(tmp, error);
            } catch (JSONException e1) {

            }
        }
    }

    public void setCellId(String data) {
        // cellid
        String tmp="cellid";
        try {
            mJSONObject.put(tmp, data);
        } catch (Exception e) {
            try {
                mJSONObject.put(tmp, error);
            } catch (JSONException e1) {

            }
        }
    }

    public void setModle(String modle) {
        // model
        String tmp="model";
        try {
            mJSONObject.put(tmp, modle);
        } catch (Exception e) {
            try {
                mJSONObject.put(tmp, error);
            } catch (JSONException e1) {

            }
        }
    }

    public void setInstallationId(String installationId) {
        String tmp="installation_Id";
        try {
            mJSONObject.put(tmp, installationId);
        } catch (Exception e) {
            try {
                mJSONObject.put(tmp, error);
            } catch (JSONException e1) {

            }
        }
    }


    public void setBuildNumber(String band) {
        // buildnumber
        String tmp="buildnumber";
        try {
            mJSONObject.put(tmp, band);
        } catch (Exception e) {
            try {
                mJSONObject.put(tmp, error);
            } catch (JSONException e1) {

            }
        }
    }

    public void setDrmUid(String drmuid) {
        // DrmUid
        String tmp="drmuid";
        try {
            mJSONObject.put(tmp, drmuid);
        } catch (Exception e) {
            try {
                mJSONObject.put(tmp, error);
            } catch (JSONException e1) {

            }
        }
    }
    public void setSDcardPerm(int flag) {
        String tmp="sdcard_perm";
        try {
            mJSONObject.put(tmp, flag);
        } catch (Exception e) {
        }
    }

    public void setPhonePerm(int flag) {
        String tmp="phone_perm";
        try {
            mJSONObject.put(tmp, flag);
        } catch (Exception e) {
        }
    }


    public void setProxy(int proxy) {
        // proxy
        String tmp="proxy";
        try {
            mJSONObject.put(tmp, proxy+"");
        } catch (Exception e) {
            try {
                mJSONObject.put(tmp, error);
            } catch (JSONException e1) {

            }
        }
    }

    public void setDeviceIp(String ip) {
        String tmp= "device_ip";
        try {
            mJSONObject.put(tmp, ip);
        } catch (Exception e) {
            try {
                mJSONObject.put(tmp, error);
            } catch (JSONException e1) {

            }
        }
    }

    public void setHook(int flag) {
        // hook
        String tmp="hook";
        try {
            mJSONObject.put(tmp, flag);
        } catch (Exception e) {
            try {
                mJSONObject.put(tmp, error);
            } catch (JSONException e1) {

            }
        }
    }

    public void setHookMethod(String methods) {
        // hookmethod
        String tmp= "hookmethod";
        try {
            mJSONObject.put(tmp, methods);
        } catch (Exception e) {
            try {
                mJSONObject.put(tmp, error);
            } catch (JSONException e1) {

            }
        }
    }


    public void setFakeApp(String flag) {
        // fakeapp
        String tmp="fakeapp";
        try {
            mJSONObject.put(tmp, flag);
        } catch (Exception e) {
            try {
                mJSONObject.put(tmp, error);
            } catch (JSONException e1) {

            }
        }
    }


    public void setCurrentWifi(JSONObject currentWifi) {
        // currentwifi
        String tmp="currentwifi";
        try {
            if (currentWifi != null)
            {
                mJSONObject.put(tmp, currentWifi);
            }else
            {
                mJSONObject.put(tmp, defaultValues);
            }

        } catch (Exception e) {
            try {
                mJSONObject.put(tmp, error);
            } catch (JSONException e1) {

            }
        }
    }

    public void setSdCardCid(String cid) {
        // cid
        String tmp = "cid";
        try {
            if (!StringUtils.isEmpty(cid)){
                mJSONObject.put(tmp, cid);
            }else{
                mJSONObject.put(tmp, defaultValues);
            }

        } catch (Exception e) {
            try {
                mJSONObject.put(tmp, error);
            } catch (JSONException e1) {
            }
        }
    }


    public void setOaid(String oaid) {
        // oaid
        String tmp = "oaid";
        try {
            mJSONObject.put(tmp, oaid);
        } catch (Exception e) {
            try {
                mJSONObject.put(tmp, error);
            } catch (JSONException e1) {
            }
        }
    }


    public void setNetType(String netType) {
        // nettype
        String tmp = "nettype";
        try {
            mJSONObject.put(tmp, netType);

        } catch (Exception e) {
            try {
                mJSONObject.put(tmp, error);
            } catch (JSONException e1) {

            }
        }
    }

    public void setSensorInfo(String SensorInfo) {
        // sensorInfo
        String tmp="sensorInfo";
        try {
            mJSONObject.put(tmp, SensorInfo);

        } catch (Exception e) {
            try {
                mJSONObject.put(tmp, error);
            } catch (JSONException e1) {

            }
        }
    }

    public void setUSBState(int state) {
        // usbstatus
        String tmp="usbstatus";
        try {
            mJSONObject.put(tmp, state);

        } catch (Exception e) {
            try {
                mJSONObject.put(tmp, error);
            } catch (JSONException e1) {

            }
        }
    }


    public void setGyroscope(String info) {
        // gyroscope
        String tmp="gyroscope";
        try {
            mJSONObject.put(tmp, info);

        } catch (Exception e) {
            try {
                mJSONObject.put(tmp, error);
            } catch (JSONException e1) {

            }
        }
    }

    public void setEnableAdb(String state) {
        // usbdebug
        String tmp="usbdebug";
        try {
            mJSONObject.put(tmp, state);

        } catch (Exception e) {
            try {
                mJSONObject.put(tmp, error);
            } catch (JSONException e1) {

            }
        }
    }

    public void setLocation(JSONObject data) {
        // Location
        String tmp="location";
        try {
            mJSONObject.put(tmp, data);

        } catch (Exception e) {
            try {
                mJSONObject.put(tmp, error);
            } catch (JSONException e1) {

            }
        }
    }


    public void setFakeGps(String state)
    {
        // fakegps
        String tmp="fakegps";
        try {
            mJSONObject.put(tmp, state);

        } catch (Exception e) {
            try {
                mJSONObject.put(tmp, error);
            } catch (JSONException e1) {

            }
        }
    }

    public void setFakeBssid(String state)
    {
        // fakebssid
        String tmp="fakebssid";
        try {
            mJSONObject.put(tmp, state);

        } catch (Exception e) {
            try {
                mJSONObject.put(tmp, error);
            } catch (JSONException e1) {

            }
        }
    }

    public void setFakeMacAdr(String state)
    {
        // fakemac
        String tmp="fakemac";
        try {
            mJSONObject.put(tmp, state);

        } catch (Exception e) {
            try {
                mJSONObject.put(tmp, error);
            } catch (JSONException e1) {

            }
        }
    }


    public void setJavaMac(String state)
    {
        // jmac
        String tmp="jmac";
        try {
            mJSONObject.put(tmp, state);

        } catch (Exception e) {
            try {
                mJSONObject.put(tmp, error);
            } catch (JSONException e1) {

            }
        }
    }

    public void setVpnStatus(String vpnStatus) {
        // vpnstatus
        String tmp="vpnstatus";
        try {
            mJSONObject.put(tmp, vpnStatus);

        } catch (Exception e) {
            try {
                mJSONObject.put(tmp, error);
            } catch (JSONException e1) {

            }
        }
    }

    public void setSimStatus(String simStatus) {
        // simstatus
        String tmp="simstatus";
        try {
            mJSONObject.put(tmp, simStatus);

        } catch (Exception e) {
            try {
                mJSONObject.put(tmp, error);
            } catch (JSONException e1) {

            }
        }
    }

    public void setVirtualPath(String virtualpath) {
        // virtualpath
        String tmp="virtualpath";
        try {
            mJSONObject.put(tmp, virtualpath);

        } catch (Exception e) {
            try {
                mJSONObject.put(tmp, error);
            } catch (JSONException e1) {

            }
        }
    }


    public void setIMEI(String imei) {
        // imei
        String tmp="imei";
        try {
            if (StringUtils.isEmpty(imei)) {
                mJSONObject.put(tmp, defaultValues);
            }else {
                mJSONObject.put(tmp, imei);
            }

        } catch (Exception e) {
            try {
                mJSONObject.put(tmp, defaultValues);
            } catch (JSONException e1) {

            }
        }
    }


    public void setIMSI(String imsi) {
        // imsi
        String tmp="imsi";
        try {
            mJSONObject.put(tmp, imsi);

        } catch (Exception e) {
            try {
                mJSONObject.put(tmp, defaultValues);
            } catch (JSONException e1) {

            }
        }
    }


    public void setAndroidID(String androidID) {
        // imsi
        String tmp="android_id";
        try {
            mJSONObject.put(tmp, androidID);

        } catch (Exception e) {
            try {
                mJSONObject.put(tmp, defaultValues);
            } catch (JSONException e1) {

            }
        }
    }

    public void setSignatureData(String data) {

        String tmp="signData";
        try {
            mJSONObject.put(tmp, data);

        } catch (Exception e) {
            try {
                mJSONObject.put(tmp, error);
            } catch (JSONException e1) {

            }
        }
    }

    public void setSensorData(String sensordata) {
        // sensorData
        String tmp="sensorData";
        try {
            mJSONObject.put(tmp, sensordata);

        } catch (Exception e) {
            try {
                mJSONObject.put(tmp, error);
            } catch (JSONException e1) {

            }
        }
    }
}
