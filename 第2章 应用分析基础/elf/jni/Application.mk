
APP_ABI := armeabi-v7a,arm64-v8a
APP_PLATFORM := android-19

APP_OPTIM := release

NDK_TOOLCHAIN_VERSION := clang
APP_CFLAGS := -DOPENSSL_NO_ASM
APP_CPPFLAGS += -fexceptions

APP_STL := c++_static