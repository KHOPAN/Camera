#pragma once

#include <Windows.h>
#include <jni.h>

BOOL InitializeMediaFoundation(JNIEnv* environment);
BOOL UninitializeMediaFoundation(JNIEnv* environment);
void ThrowWin32Error(JNIEnv* environment, const LPSTR errorClass, DWORD errorCode, const LPWSTR functionName);
