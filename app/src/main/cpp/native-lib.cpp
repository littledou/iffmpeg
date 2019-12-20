#include <jni.h>
#include <string>

#ifdef __cplusplus
extern "C" {

#include <libavcodec/avcodec.h>
#endif

JNIEXPORT jstring JNICALL Java_cn_readsense_iffmpeg_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = avcodec_configuration();

    return env->NewStringUTF(hello.c_str());
}

#ifdef __cplusplus
};

#endif