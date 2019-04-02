#include <jni.h>
#include <string>
#include <sys/ptrace.h>

extern "C" JNIEXPORT jstring JNICALL
Java_com_aldrin_antidebug_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

void anti_debug() {

    if (ptrace(PTRACE_TRACEME, 0, 0, 0) == -1) {
        exit(0);
    }
}

jint JNI_OnLoad(JavaVM *javaVM, void *reserved) {
    anti_debug();
    JNIEnv *jniEnv;
    if (javaVM->GetEnv(reinterpret_cast<void **>(&jniEnv), JNI_VERSION_1_6) != JNI_OK) {
        return -1;
    }
    return JNI_VERSION_1_6;
}
