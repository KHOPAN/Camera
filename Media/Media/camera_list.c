#include <mfapi.h>
#include <mfidl.h>
#include "native.h"
#include "common.h"
#include "camera_instance.h"

jobjectArray Camera_list(JNIEnv* environment, jclass cameraClass) {
	jclass mediaTypeClass = (*environment)->FindClass(environment, "com/khopan/camera/MediaType");

	if(!mediaTypeClass) {
		return NULL;
	}

	jmethodID cameraConstructor = (*environment)->GetMethodID(environment, cameraClass, "<init>", "(Ljava/lang/String;Ljava/lang/String;[Lcom/khopan/camera/MediaType;)V");

	if(!cameraConstructor) {
		return NULL;
	}

	jmethodID mediaTypeConstructor = (*environment)->GetMethodID(environment, mediaTypeClass, "<init>", "()V");

	if(!mediaTypeConstructor) {
		return NULL;
	}

	jfieldID indexField = (*environment)->GetFieldID(environment, mediaTypeClass, "index", "I");

	if(!indexField) {
		return NULL;
	}

	if(!InitializeMediaFoundation(environment)) {
		return NULL;
	}

	IMFAttributes* attributes;
	HRESULT result = MFCreateAttributes(&attributes, 1);
	jobjectArray returnResult = NULL;

	if(FAILED(result)) {
		ThrowMediaFoundationError(environment, result, L"MFCreateAttributes");
		goto uninitialize;
	}

	result = attributes->lpVtbl->SetGUID(attributes, &MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE, &MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_GUID);

	if(FAILED(result)) {
		ThrowMediaFoundationError(environment, result, L"IMFAttributes::SetGUID");
		attributes->lpVtbl->Release(attributes);
		goto uninitialize;
	}

	IMFActivate** devices;
	UINT32 count;
	result = MFEnumDeviceSources(attributes, &devices, &count);
	attributes->lpVtbl->Release(attributes);

	if(FAILED(result)) {
		ThrowMediaFoundationError(environment, result, L"MFEnumDeviceSources");
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
		jobject cameraInstance = newCameraInstance(environment, cameraClass, mediaTypeClass, cameraConstructor, mediaTypeConstructor, indexField, activate);
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
