#include <jni.h>
#include <string>
#include <zconf.h>
#include <pthread.h>
#include<android/log.h>

#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,TAG ,__VA_ARGS__) // 定义LOGD类型
#define TAG "Aldrin" // 这个是自定义的LOG的标识

extern "C" JNIEXPORT jstring JNICALL
Java_com_aldrin_antidebug_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

void *anti_debug(void *arg) {
    const int bufsize = 1024;
    char file_name[bufsize];
    char line[bufsize];
    int pid = getpid();

    sprintf(file_name, "/proc/%d/status", pid);//格式化字符串
    FILE *file = fopen(file_name, "r");
    if (file){
        while (1){
            while (feof(file)){
                fclose(file);
                sleep(3);
                file = fopen(file_name, "r");
            }
            fgets(line,256, file);
            if (!strncmp(line,"TracerPid", 9u)){
                LOGD("%s",line);
                if (atoi(&line[10])){
                    fclose(file);
                    exit(0);
                }
            }
        }
    }
}

jint JNI_OnLoad(JavaVM *javaVM, void *reserved) {

    int err;
    pthread_t ntid;

    err = pthread_create(&ntid, NULL, anti_debug, NULL);
    if (err != 0) {
        exit(0);
    }

    JNIEnv *jniEnv;
    if (javaVM->GetEnv(reinterpret_cast<void **>(&jniEnv), JNI_VERSION_1_6) != JNI_OK) {
        return -1;
    }
    return JNI_VERSION_1_6;
}