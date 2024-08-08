LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

APP_ABI := armeabi-v7a arm64-v8a

APP_PLATFORM := android-14

APP_OPTIM := release

#APP_OPTIM := debug

NDK_TOOLCHAIN_VERSION := clang

include $(BUILD_SHARED_LIBRARY)