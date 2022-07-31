LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := ssl
LOCAL_SRC_FILES := libs/openssl/$(TARGET_ARCH_ABI)/lib/libssl.a
LOCAL_EXPORT_CFLAGS := -I$(LOCAL_PATH)/libs/openssl/$(TARGET_ARCH_ABI)/include
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := crypto
LOCAL_SRC_FILES := libs/openssl/$(TARGET_ARCH_ABI)/lib/libcrypto.a
LOCAL_EXPORT_CFLAGS := -I$(LOCAL_PATH)/libs/openssl/$(TARGET_ARCH_ABI)/include
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := curl
LOCAL_SRC_FILES := libs/curl/$(TARGET_ARCH_ABI)/lib/libcurl.a
LOCAL_EXPORT_CFLAGS := -I$(LOCAL_PATH)/libs/curl/$(TARGET_ARCH_ABI)/include
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_C_INCLUDES :=
LOCAL_MODULE := curl_ctrl
LOCAL_SRC_FILES := jni.c curl_ctrl/curl_ctrl.c sys/sys_ctrl.c
LOCAL_STATIC_LIBRARIES := libcurl libssl libcrypto
LOCAL_LDLIBS := -lz -llog
include $(BUILD_SHARED_LIBRARY)
