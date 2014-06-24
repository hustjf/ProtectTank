LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/HelloWorldScene.cpp \
                   ../../Classes/Wave.cpp \
                   ../../Classes/DataModel.cpp \
                   ../../Classes/GameHud.cpp \
                   ../../Classes/Creep.cpp \
                   ../../Classes/Projectile.cpp \
                   ../../Classes/Tower.cpp \
                   ../../Classes/WayPoint.cpp \
                   ../../Classes/StartScene.cpp \
                   ../../Classes/Home.cpp \
                   ../../Classes/SecondScene.cpp \
                   ../../Classes/ThirdScene.cpp
                   

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes \
					$(LOCAL_PATH)/../../cocos2d \
					$(LOCAL_PATH)/../../cocos2d/cocos/editor-support

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += box2d_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocostudio_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_gui_static


include $(BUILD_SHARED_LIBRARY)

$(call import-module,2d)
$(call import-module,audio/android)
$(call import-module,Box2D)
$(call import-module,extensions)
$(call import-module,ui)
$(call import-module,editor-support/cocostudio)


NDK_TOOLCHAIN_VERSION := 4.8
APP_STL := gnustl_shared
APP_CPPFLAGS += -std=c++11