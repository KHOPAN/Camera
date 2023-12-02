#include "helper.h"

void throwClass(JNIEnv* environment, std::string exception, std::string message) {
	jclass runtimeExceptionClass = environment->FindClass(exception.c_str());

	if(!runtimeExceptionClass) {
		return;
	}

	jmethodID runtimeExceptionConstructor = environment->GetMethodID(runtimeExceptionClass, "<init>", "(Ljava/lang/String;)V");
	
	if(!runtimeExceptionConstructor) {
		return;
	}
	
	const char* messageRaw = message.c_str();
	jstring messageString = environment->NewStringUTF(messageRaw);
	jobject runtimeExceptionInstance = environment->NewObject(runtimeExceptionClass, runtimeExceptionConstructor, messageString);
	jthrowable throwable = static_cast<jthrowable>(runtimeExceptionInstance);
	environment->Throw(throwable);
}

void throwRuntime(JNIEnv* environment, std::string message) {
	throwClass(environment, "java/lang/RuntimeException", message);
}
