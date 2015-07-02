LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp


# 遍历目录及子目录的函数
define walk
  $(wildcard $(1)) $(foreach e, $(wildcard $(1)/*), $(call walk, $(e)))
endef

# 遍历Classes目录
ALLFILES = $(call walk, $(LOCAL_PATH)/../../Classes)
           
FILE_LIST := hellocpp/main.cpp
# 从所有文件中提取出所有.cpp文件
FILE_LIST += $(filter %.cpp, $(ALLFILES))
FILE_LIST += $(filter %.c, $(ALLFILES))

LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)


LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../Classes/Cell
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../Classes/Scene
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../Classes/ScriptEngine
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../Classes/common
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../Classes/Gif
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../Classes/Gif/gif_lib


LOCAL_STATIC_LIBRARIES := cocos2dx_static

include $(BUILD_SHARED_LIBRARY)

$(call import-module,./prebuilt-mk)