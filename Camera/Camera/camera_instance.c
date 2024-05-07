#include "common.h"
#include <mfidl.h>
#include "camera_instance.h"

jobject newCameraInstance(JNIEnv* environment, jclass cameraClass, jmethodID cameraConstructor, IMFActivate* activate) {
	LPWSTR deviceName;
	UINT32 deviceNameLength;
	HRESULT result = activate->lpVtbl->GetAllocatedString(activate, &MF_DEVSOURCE_ATTRIBUTE_FRIENDLY_NAME, &deviceName, &deviceNameLength);

	if(FAILED(result)) {
		ThrowWin32Error(environment, "com/khopan/camera/error/MediaFoundationError", result, L"IMFActivate::GetAllocatedString");
		return NULL;
	}

	LPWSTR symbolicLink;
	UINT32 symbolicLinkLength;
	result = activate->lpVtbl->GetAllocatedString(activate, &MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_SYMBOLIC_LINK, &symbolicLink, &symbolicLinkLength);

	if(FAILED(result)) {
		ThrowWin32Error(environment, "com/khopan/camera/error/MediaFoundationError", result, L"IMFActivate::GetAllocatedString");
		CoTaskMemFree(deviceName);
		return NULL;
	}

	jstring deviceNameJava = (*environment)->NewString(environment, deviceName, (jsize) deviceNameLength);
	jstring symbolicLinkJava = (*environment)->NewString(environment, symbolicLink, (jsize) symbolicLinkLength);
	jobject cameraInstance = (*environment)->NewObject(environment, cameraClass, cameraConstructor, deviceNameJava, symbolicLinkJava);
	CoTaskMemFree(deviceName);
	CoTaskMemFree(symbolicLink);
	return cameraInstance;
}
