#include <jni.h>
#include <string>
#include "log.h"

#ifdef __cplusplus
extern "C" {

#include <libavcodec/avcodec.h>
#endif

JNIEXPORT jstring JNICALL Java_cn_readsense_iffmpeg_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = avcodec_configuration();
    LOGD("avcodec_configuration - 1: \n%s", hello.c_str());
    return env->NewStringUTF(hello.c_str());
}

#ifdef __cplusplus
};

#endif