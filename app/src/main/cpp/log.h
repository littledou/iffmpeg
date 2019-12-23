#ifndef MEDIAAPP_LOG_H
#define MEDIAAPP_LOG_H
#include <android/log.h>
#define LOCAL_DEBUG

#ifdef LOCAL_DEBUG

#include <android/log.h>

#define LOG_TAG "DLOG_FFMPEG"
#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__))
#else
#define LOGD(...)
#endif
#endif //MEDIAAPP_LOG_H