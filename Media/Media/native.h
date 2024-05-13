#pragma once

#include <jni.h>

jbyteArray Camera_capture(JNIEnv* environment, jobject cameraInstance, jobject mediaTypeInstance);
jobjectArray Camera_list(JNIEnv* environment, jclass cameraClass);
