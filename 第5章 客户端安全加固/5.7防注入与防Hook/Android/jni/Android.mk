LOCAL_PATH := $(call my-dir)

cmd-strip = $(TOOLCHAIN_PREFIX)strip --strip-debug -x $1

# 需要进行代码混淆的文件
LOCAL_OBFUSCATE_SRC_FILES := $(wildcard $(LOCAL_PATH)/test.c)
include $(CLEAR_VARS)
LOCAL_MODULE    := test
LOCAL_SRC_FILES := $(LOCAL_OBFUSCATE_SRC_FILES)
include $(BUILD_EXECUTABLE)


