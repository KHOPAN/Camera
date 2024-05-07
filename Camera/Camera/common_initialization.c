#include <mfapi.h>
#include "common.h"

BOOL InitializeMediaFoundation(JNIEnv* environment) {
	HRESULT result = CoInitializeEx(NULL, COINIT_MULTITHREADED);

	if(FAILED(result)) {
		ThrowWin32Error(environment, "com/khopan/camera/error/ComAPIError", result, L"CoInitializeEx");
		return FALSE;
	}

	result = MFStartup(MF_VERSION, MFSTARTUP_NOSOCKET);

	if(FAILED(result)) {
		ThrowWin32Error(environment, "com/khopan/camera/error/MediaFoundationError", result, L"MFStartup");
		return FALSE;
	}

	return TRUE;
}

BOOL UninitializeMediaFoundation(JNIEnv* environment) {
	HRESULT result = MFShutdown();

	if(FAILED(result)) {
		ThrowWin32Error(environment, "com/khopan/camera/error/MediaFoundationError", result, L"MFShutdown");
		return FALSE;
	}

	CoUninitialize();
	return TRUE;
}
