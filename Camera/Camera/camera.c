#include "camera.h"
#include "native.h"

static JNINativeMethod CameraNativeMethods[] = {
	{"list", "()[Lcom/khopan/camera/Camera;", (void*) &Camera_list}
};

void __stdcall InitializeCamera(JNIEnv* environment) {
	jclass cameraClass = (*environment)->FindClass(environment, "com/khopan/camera/Camera");

	if(!cameraClass) {
		return;
	}

	(*environment)->RegisterNatives(environment, cameraClass, CameraNativeMethods, sizeof(CameraNativeMethods) / sizeof(CameraNativeMethods[0]));
}
