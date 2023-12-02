#pragma once

#include <string>
#include <jni.h>

#define CHECK(x) if(FAILED(result)){throwRuntime(environment,x);MFShutdown();CoUninitialize();return NULL;}

void throwClass(JNIEnv*, std::string, std::string);
void throwRuntime(JNIEnv*, std::string);
