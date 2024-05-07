#pragma once

#include <jni.h>
#include <mfapi.h>

jobject newCameraInstance(JNIEnv* environment, jclass cameraClass, jmethodID cameraConstructor, IMFActivate* activate);
