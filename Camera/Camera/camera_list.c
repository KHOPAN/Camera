#include <khopanjava.h>
#include <mfapi.h>
#include <mfidl.h>
#include "native.h"
#include "common.h"

static jobject initializeCameraObject(JNIEnv* environment, jclass cameraClass, jmethodID cameraConstructor, IMFActivate* activate);

jobjectArray Camera_list(JNIEnv* environment, jclass cameraClass) {
	jmethodID cameraConstructor = (*environment)->GetMethodID(environment, cameraClass, "<init>", "(Ljava/lang/String;Ljava/lang/String;)V");

	if(!cameraConstructor) {
		return NULL;
	}

	if(!InitializeMediaFoundation(environment)) {
		return NULL;
	}

	IMFAttributes* attributes;
	HRESULT result = MFCreateAttributes(&attributes, 1);
	jobjectArray returnResult = NULL;

	if(FAILED(result)) {
		ThrowWin32Error(environment, "com/khopan/camera/error/MediaFoundationError", result, L"MFCreateAttributes");
		goto uninitialize;
	}

	result = attributes->lpVtbl->SetGUID(attributes, &MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE, &MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_GUID);

	if(FAILED(result)) {
		ThrowWin32Error(environment, "com/khopan/camera/error/MediaFoundationError", result, L"IMFAttributes::SetGUID");
		attributes->lpVtbl->Release(attributes);
		goto uninitialize;
	}

	IMFActivate** devices;
	UINT32 count;
	result = MFEnumDeviceSources(attributes, &devices, &count);
	attributes->lpVtbl->Release(attributes);

	if(FAILED(result)) {
		ThrowWin32Error(environment, "com/khopan/camera/error/MediaFoundationError", result, L"MFEnumDeviceSources");
		goto uninitialize;
	}

	if(count < 1) {
		returnResult = (*environment)->NewObjectArray(environment, 0, cameraClass, NULL);
		goto uninitialize;
	}

	jobjectArray cameraArray = (*environment)->NewObjectArray(environment, count, cameraClass, NULL);

	if(!cameraArray) {
		goto freeDevices;
	}

	for(UINT32 i = 0; i < count; i++) {
		IMFActivate* activate = devices[i];
		jobject cameraInstance = initializeCameraObject(environment, cameraClass, cameraConstructor, activate);
		activate->lpVtbl->Release(activate);

		if(!cameraInstance) {
			goto freeDevices;
		}

		(*environment)->SetObjectArrayElement(environment, cameraArray, (jsize) i, cameraInstance);
	}

	returnResult = cameraArray;
freeDevices:
	CoTaskMemFree(devices);
uninitialize:
	UninitializeMediaFoundation(environment);
	return returnResult;
}

static jobject initializeCameraObject(JNIEnv* environment, jclass cameraClass, jmethodID cameraConstructor, IMFActivate* activate) {
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
