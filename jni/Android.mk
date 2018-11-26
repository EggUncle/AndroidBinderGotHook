LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := hooker
LOCAL_SRC_FILES := bind_hook_utils.c \
		   binder_hook.c \
		   elf_util.c

LOCAL_C_INCLUDES += $(PROJECT_PATH)../include

LOCAL_LDLIBS += -L$(SYSROOT)/usr/lib -llog

include $(BUILD_SHARED_LIBRARY)
