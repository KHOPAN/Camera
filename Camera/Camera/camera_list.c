#include <khopanjava.h>
#include "native.h"
#include "common.h"

jobjectArray Camera_list(JNIEnv* environment, jclass cameraClass) {
	if(!InitializeMediaFoundation(environment)) {
		goto uninitialize;
	}
uninitialize:
	UninitializeMediaFoundation(environment);
	return NULL;
}
