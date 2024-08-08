LOCAL_PATH := $(call my-dir)

# 定义头文件
CRYPTO_INCLUDES := $(LOCAL_PATH)/include
CRYPTO_INCLUDES += $(LOCAL_PATH)/codes/include

# 需要进行加固处理的文件
CRYPTO_SRC_FILES := $(wildcard $(LOCAL_PATH)/codes/*.c)

include $(CLEAR_VARS)
LOCAL_MODULE    := libcrypto_shared
LOCAL_SRC_FILES := lib/$(TARGET_ARCH_ABI)/libcrypto.a
LOCAL_C_INCLUDES := $(CRYPTO_INCLUDES)
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := crypto
LOCAL_MODULE_FILENAME := libecdh
LOCAL_C_INCLUDES := $(CRYPTO_INCLUDES)
LOCAL_SRC_FILES := $(CRYPTO_SRC_FILES)
LOCAL_STATIC_LIBRARIES := libcrypto_shared
LOCAL_LDLIBS += -llog

LOCAL_CFLAGS := -fvisibility=hidden
include $(BUILD_SHARED_LIBRARY)


