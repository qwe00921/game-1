LOCAL_PATH := $(call my-dir)

LOCAL_SHORT_COMMANDS := true

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/cocos)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/cocos/audio/include)

LOCAL_MODULE := MyGame_shared

LOCAL_MODULE_FILENAME := libMyGame

FILE_LIST_BATTLEELEM := $(wildcard $(LOCAL_PATH)/../../../Classes/battleelem/*.cpp)
FILE_LIST_COMMON := $(wildcard $(LOCAL_PATH)/../../../Classes/common/*.cpp)
FILE_LIST_CONTROL := $(wildcard $(LOCAL_PATH)/../../../Classes/control/*.cpp)
FILE_LIST_JSONCPP := $(wildcard $(LOCAL_PATH)/../../../Classes/jsoncpp/*.cpp)
FILE_LIST_LAYER := $(wildcard $(LOCAL_PATH)/../../../Classes/layer/*.cpp)
FILE_LIST_MAIN := $(wildcard $(LOCAL_PATH)/../../../Classes/main/*.cpp)
FILE_LIST_NEW_UI := $(wildcard $(LOCAL_PATH)/../../../Classes/new_ui/*.cpp)
FILE_LIST_SCREEN := $(wildcard $(LOCAL_PATH)/../../../Classes/screen/*.cpp)
FILE_LIST_SCRIPTS := $(wildcard $(LOCAL_PATH)/../../../Classes/scripts/*.cpp)
FILE_LIST_SCRIPTS_ScriptPara := $(wildcard $(LOCAL_PATH)/../../../Classes/scripts/ScriptPara/*.cpp)
FILE_LIST_AUTO_UPDATE := $(wildcard $(LOCAL_PATH)/../../../Classes/auto_update/*.cpp)
FILE_LIST_TOOLS := $(wildcard $(LOCAL_PATH)/../../../Classes/tools/*.c) $(wildcard $(LOCAL_PATH)/../../../Classes/tools/*.cpp)

LOCAL_SRC_FILES := hellocpp/main.cpp \
	../../../Classes/AppDelegate.cpp \
	../../../Classes/HelloWorldScene.cpp \
	../../../Classes/externsdk/alipay/android/alipay.cpp \
	../../../Classes/externsdk/otherfunction/otherfunction_android.cpp\
	../../../Classes/externsdk/talkingdata/android/TalkingData.cpp \
	../../../Classes/externsdk/talkingdata/android/TDCCAccount.cpp \
	../../../Classes/externsdk/talkingdata/android/TDCCItem.cpp \
	../../../Classes/externsdk/talkingdata/android/TDCCMIssion.cpp \
	../../../Classes/externsdk/talkingdata/android/TDCCTalkingDataGA.cpp \
	../../../Classes/externsdk/talkingdata/android/TDCCVirtualCurrency.cpp \
	../../../Classes/externsdk/talkingdata/android/TDGAJniHelper.cpp \
	../../../Classes/externsdk/wxpay/android/wxpay.cpp \
	../../../Classes/externsdk/sdkpay/android/sdkpay.cpp

LOCAL_SRC_FILES += $(FILE_LIST_BATTLEELEM:$(LOCAL_PATH)/%=%)
LOCAL_SRC_FILES +=$(FILE_LIST_COMMON:$(LOCAL_PATH)/%=%)
LOCAL_SRC_FILES +=$(FILE_LIST_CONTROL:$(LOCAL_PATH)/%=%)
LOCAL_SRC_FILES +=$(FILE_LIST_JSONCPP:$(LOCAL_PATH)/%=%)
LOCAL_SRC_FILES +=$(FILE_LIST_LAYER:$(LOCAL_PATH)/%=%)
LOCAL_SRC_FILES +=$(FILE_LIST_MAIN:$(LOCAL_PATH)/%=%)
LOCAL_SRC_FILES +=$(FILE_LIST_NEW_UI:$(LOCAL_PATH)/%=%)
LOCAL_SRC_FILES +=$(FILE_LIST_SCREEN:$(LOCAL_PATH)/%=%)
LOCAL_SRC_FILES +=$(FILE_LIST_SG_UI:$(LOCAL_PATH)/%=%)
LOCAL_SRC_FILES +=$(FILE_LIST_SCRIPTS:$(LOCAL_PATH)/%=%)
LOCAL_SRC_FILES +=$(FILE_LIST_SCRIPTS_ScriptPara:$(LOCAL_PATH)/%=%)   
LOCAL_SRC_FILES +=$(FILE_LIST_AUTO_UPDATE:$(LOCAL_PATH)/%=%)
LOCAL_SRC_FILES +=$(FILE_LIST_TOOLS:$(LOCAL_PATH)/%=%)
    
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../Classes \
					$(LOCAL_PATH)/../../../Classes/jsoncpp \
					$(LOCAL_PATH)/../../../Classes/battleelem \
					$(LOCAL_PATH)/../../../Classes/common \
					$(LOCAL_PATH)/../../../Classes/control \
					$(LOCAL_PATH)/../../../Classes/externsdk/otherfunction \
					$(LOCAL_PATH)/../../../Classes/externsdk/talkingdata/include \
					$(LOCAL_PATH)/../../../Classes/externsdk/talkingdata/android \
					$(LOCAL_PATH)/../../../Classes/main \
					$(LOCAL_PATH)/../../../Classes/screen \
					$(LOCAL_PATH)/../../../Classes/newscreen \
					$(LOCAL_PATH)/../../../Classes/scripts \
					$(LOCAL_PATH)/../../../Classes/scripts/ScriptPara \
					$(LOCAL_PATH)/../../../Classes/new_ui \
					$(LOCAL_PATH)/../../../Classes/layer \
					$(LOCAL_PATH)/../../../Classes/auto_update \
					$(LOCAL_PATH)/../../../Classes/tools

LOCAL_CFLAGS    += -fvisibility=hidden
LOCAL_CPPFLAGS += -fvisibility=hidden
#LOCAL_SHARED_LIBRARIES := lbcrashhandler

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
#LOCAL_WHOLE_STATIC_LIBRARIES += box2d_static
#LOCAL_WHOLE_STATIC_LIBRARIES += chipmunk_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)
#include $(LOCAL_PATH)/prebuilt/Android.mk

$(call import-module,.)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
