LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

BIBLEC := ../../biblec
FBRP := ../../fbrp

LOCAL_MODULE := app
LOCAL_CFLAGS := -Wall -O2
LOCAL_SRC_FILES := main.c json.c libuifw/libui.c libuifw/lib.c $(FBRP)/fbrp.c
LOCAL_C_INCLUDES += $(LOCAL_PATH) $(LOCAL_PATH)/libuifw $(LOCAL_PATH)/$(BIBLEC) $(LOCAL_PATH)/$(FBRP)
LOCAL_LDLIBS += -llog
LOCAL_LDFLAGS += -s

include $(BUILD_SHARED_LIBRARY)
