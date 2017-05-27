LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := lbcrashhandler
LOCAL_SRC_FILES := liblbcrashhandler.so
include $(PREBUILT_SHARED_LIBRARY)