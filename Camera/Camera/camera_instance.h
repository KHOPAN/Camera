#pragma once

#include <jni.h>
#include <mfapi.h>

jobject newCameraInstance(JNIEnv* environment, jclass cameraClass, jclass mediaTypeClass, jmethodID cameraConstructor, jmethodID mediaTypeConstructor, jfieldID indexField, IMFActivate* activate);
jobject newMediaTypeInstance(JNIEnv* environment, jclass mediaTypeClass, jmethodID mediaTypeConstructor, IMFMediaType* mediaType);
