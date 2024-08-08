LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

# 定义头文件
RISK_INCLUDES := $(LOCAL_PATH)/include
RISK_INCLUDES += $(LOCAL_PATH)/include/rapidjson
RISK_INCLUDES += $(LOCAL_PATH)/include/mlog
RISK_INCLUDES += $(LOCAL_PATH)/include/tools
RISK_INCLUDES += $(LOCAL_PATH)/include/hash
RISK_INCLUDES += $(LOCAL_PATH)/include/device
RISK_INCLUDES += $(LOCAL_PATH)/include/simulator
RISK_INCLUDES += $(LOCAL_PATH)/include/anti_fraud

RISK_SRC_FILES += $(wildcard $(LOCAL_PATH)/device/*.cpp)
RISK_SRC_FILES += $(wildcard $(LOCAL_PATH)/onload/*.cpp)
RISK_SRC_FILES += $(wildcard $(LOCAL_PATH)/tools/*.cpp)
RISK_SRC_FILES += $(wildcard $(LOCAL_PATH)/hash/*.cpp)
RISK_SRC_FILES += $(wildcard $(LOCAL_PATH)/simulator/*.cpp)
RISK_SRC_FILES += $(wildcard $(LOCAL_PATH)/anti_fraud/*.cpp)
RISK_SRC_FILES += $(wildcard $(LOCAL_PATH)/*.cpp)

LOCAL_MODULE    := native-lib
LOCAL_SRC_FILES := $(RISK_SRC_FILES)
LOCAL_C_INCLUDES := $(RISK_INCLUDES)
LOCAL_LDLIBS  += -llog
LOCAL_CFLAGS := -fvisibility=hidden
include $(BUILD_SHARED_LIBRARY)





