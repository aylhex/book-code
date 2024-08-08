LOCAL_PATH := $(call my-dir)

cmd-strip = $(TOOLCHAIN_PREFIX)strip --strip-debug -x $1

# 需要进行代码混淆的文件
LOCAL_OBFUSCATE_SRC_FILES := $(wildcard $(LOCAL_PATH)/*.cpp)
LOCAL_OBFUSCATE_SRC_FILES += $(wildcard $(LOCAL_PATH)/anti_debug.c)
include $(CLEAR_VARS)
LOCAL_MODULE    := test
LOCAL_SRC_FILES := $(LOCAL_OBFUSCATE_SRC_FILES)
include $(BUILD_SHARED_LIBRARY)
#include $(BUILD_EXECUTABLE)


