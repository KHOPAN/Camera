#include "camera.h"

void __stdcall InitializeCamera(JNIEnv* environment) {
	(*environment)->FatalError(environment, "Camera was loaded successfully!");
}
