LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

BIBLEC := ../../biblec
FBRP := ../../fbrp
UI := ../../libuifw

CORE := main.c json.c $(FBRP)/fbrp.c
CORE += $(UI)/libui.c $(UI)/lib.c $(UI)/table.c

LOCAL_MODULE := app
LOCAL_CFLAGS := -Wall -O2
LOCAL_SRC_FILES := $(CORE)
LOCAL_C_INCLUDES += $(LOCAL_PATH) $(LOCAL_PATH)/libuifw $(LOCAL_PATH)/$(BIBLEC) $(LOCAL_PATH)/$(FBRP)
LOCAL_LDLIBS += -llog
LOCAL_LDFLAGS += -s

include $(BUILD_SHARED_LIBRARY)