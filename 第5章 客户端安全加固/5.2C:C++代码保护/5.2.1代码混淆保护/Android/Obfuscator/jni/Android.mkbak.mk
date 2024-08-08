LOCAL_PATH := $(call my-dir)

cmd-strip = $(TOOLCHAIN_PREFIX)strip --strip-debug -x $1

# 需要进行代码混淆的文件
LOCAL_OBFUSCATE_SRC_FILES := $(wildcard $(LOCAL_PATH)/*.cpp)

include $(CLEAR_VARS)
LOCAL_MODULE    := test
LOCAL_SRC_FILES := $(LOCAL_OBFUSCATE_SRC_FILES)
LOCAL_CFLAGS := -mllvm -enable-bcfobf -mllvm -enable-cffobf -mllvm -bcf_prob=50
include $(BUILD_SHARED_LIBRARY)
