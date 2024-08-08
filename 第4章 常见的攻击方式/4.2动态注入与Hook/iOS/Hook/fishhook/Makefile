TARGET := iphone:clang:latest:7.0

include $(THEOS)/makefiles/common.mk

LIBRARY_NAME = FishHook

FishHook_FILES = LoadFishHook.m fishhook.c
FishHook_CFLAGS = -fobjc-arc
FishHook_INSTALL_PATH = /usr/local/lib

include $(THEOS_MAKE_PATH)/library.mk
