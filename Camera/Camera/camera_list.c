#include <khopanjava.h>
#include "native.h"

jobjectArray Camera_list(JNIEnv* environment, jclass cameraClass) {
	KHJavaStandardOutputW(environment, L"Hello, camera!");
	return NULL;
}
