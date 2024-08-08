TARGET := iphone:clang:latest:7.0
# THEOS_DEVICE_IP = 192.168.155.3
# export ARCHS = arm64
include $(THEOS)/makefiles/common.mk

LIBRARY_NAME = MSHook

MSHook_FILES = MSHook.m
MSHook_CFLAGS = -fobjc-arc
MSHook_INSTALL_PATH = /usr/local/lib

include $(THEOS_MAKE_PATH)/library.mk
