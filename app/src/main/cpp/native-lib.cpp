#include <jni.h>
#include <string>
#include "AudioRecordinProbe.h"
#include "AudioRecordinProbe.cpp"
extern "C" JNIEXPORT jstring JNICALL
Java_ru_ric_1kos_soundrecsprobe_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

extern "C" JNIEXPORT jboolean JNICALL
Java_ru_ric_1kos_soundrecsprobe_MainActivity_stopRecording(
        JNIEnv * env,
        jobject MainActivity
) {
    static auto a = AudioRecordinProbe::get();
    a->StopAudioRecorder();
    return true;
}extern "C"
JNIEXPORT jboolean JNICALL
Java_ru_ric_1kos_soundrecsprobe_MainActivity_startRecording(JNIEnv *env, jobject MainActivity,
                                                            jstring fPath) {
    const char *path = (*env).GetStringUTFChars(fPath, 0);

    static auto a = AudioRecordinProbe::get();
    a->StartRecordingOpenSLES(path);
    return true;
}