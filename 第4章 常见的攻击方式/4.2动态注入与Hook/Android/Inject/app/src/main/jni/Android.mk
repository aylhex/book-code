
LOCAL_PATH := $(call my-dir)

CRYPTO_INCLUDES := $(LOCAL_PATH)/include

CRYPTO_SRC_FILES := codes/native-lib.cpp


include $(CLEAR_VARS)
LOCAL_MODULE    := native-lib
LOCAL_C_INCLUDES := $(CRYPTO_INCLUDES)
LOCAL_SRC_FILES := $(CRYPTO_SRC_FILES)
LOCAL_LDLIBS += -llog
include $(BUILD_SHARED_LIBRARY)

