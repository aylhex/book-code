LOCAL_PATH := $(call my-dir)

cmd-strip = $(TOOLCHAIN_PREFIX)strip --strip-debug -x $1

include $(CLEAR_VARS)

LOCAL_MODULE    := test
LOCAL_SRC_FILES := signature.cpp


LOCAL_LDLIBS  += -llog
#LOCAL_CFLAGS := -fvisibility=hidden

include $(BUILD_SHARED_LIBRARY)