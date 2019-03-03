#include <jni.h>
#include <string>
#include <cec-native.h>


extern "C" JNIEXPORT jstring JNICALL
Java_io_argskwargs_cecjni_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C plus plus";
    return env->NewStringUTF(hello.c_str());
}

extern "C" JNIEXPORT void JNICALL
Java_io_argskwargs_cecjni_MainActivity_cecDriverInfo(
        JNIEnv *env,
        jobject /* this */) {

    cec_info_driver_info();
}
