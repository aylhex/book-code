TARGET := iphone:clang:latest:7.0

include $(THEOS)/makefiles/common.mk

LIBRARY_NAME = SwizzleHook

SwizzleHook_FILES = SwizzleHook.m
SwizzleHook_CFLAGS = -fobjc-arc
SwizzleHook_INSTALL_PATH = /usr/local/lib

include $(THEOS_MAKE_PATH)/library.mk
