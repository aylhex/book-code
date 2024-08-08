package com.device.risk.utils.tools;

import android.Manifest;
import android.annotation.SuppressLint;
import android.content.Context;
import android.content.pm.PackageManager;
import android.location.Location;
import android.location.LocationManager;

import androidx.core.app.ActivityCompat;

import com.device.risk.utils.device.MContext;

import java.util.List;

public class LocationUtils {

    private static String provider;



    @SuppressLint("MissingPermission")
    public static Location getLocation() {

        if (provider != null) {
            if (ActivityCompat.checkSelfPermission(MContext.getContext(), Manifest.permission.ACCESS_FINE_LOCATION) != PackageManager.PERMISSION_GRANTED &&
                    ActivityCompat.checkSelfPermission(MContext.getContext(), Manifest.permission.ACCESS_COARSE_LOCATION) != PackageManager.PERMISSION_GRANTED) {

                return null;
            }
        }

        //  获取定位服务
        LocationManager locationManager = (LocationManager) MContext.getContext().getSystemService(Context.LOCATION_SERVICE);
        //  获取当前可用的位置控制器
        List<String> list = locationManager.getProviders(true);

        if (list.contains(locationManager.GPS_PROVIDER)) {
            //  GPS位置控制器
            provider = locationManager.GPS_PROVIDER;//GPS定位
        }

        Location lastKnownLocation = locationManager.getLastKnownLocation(provider);
        if (lastKnownLocation != null){
            return null;
        }


        if (list.contains(locationManager.NETWORK_PROVIDER)) {
            //  网络位置控制器
            provider = locationManager.NETWORK_PROVIDER;//网络定位
        }

        Location lastKnownLocation2 = locationManager.getLastKnownLocation(provider);
        return lastKnownLocation2;
    }
}
