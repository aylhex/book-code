LOCAL_PATH := $(call my-dir)

# 定义头文件
INCLUDES += $(LOCAL_PATH)/include

SRC_FILES := $(wildcard $(LOCAL_PATH)/*.cpp)

include $(CLEAR_VARS)
LOCAL_MODULE    := test
LOCAL_SRC_FILES := $(SRC_FILES)
LOCAL_C_INCLUDES := $(INCLUDES)

include $(BUILD_EXECUTABLE)