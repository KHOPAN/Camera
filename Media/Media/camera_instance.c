#include "common.h"
#include <mfidl.h>
#include "camera_instance.h"

jobject newCameraInstance(JNIEnv* environment, jclass cameraClass, jclass mediaTypeClass, jmethodID cameraConstructor, jmethodID mediaTypeConstructor, jfieldID indexField, IMFActivate* activate) {
	LPWSTR deviceName;
	UINT32 deviceNameLength;
	HRESULT result = activate->lpVtbl->GetAllocatedString(activate, &MF_DEVSOURCE_ATTRIBUTE_FRIENDLY_NAME, &deviceName, &deviceNameLength);

	if(FAILED(result)) {
		ThrowMediaFoundationError(environment, result, L"IMFActivate::GetAllocatedString");
		return NULL;
	}

	LPWSTR symbolicLink;
	UINT32 symbolicLinkLength;
	result = activate->lpVtbl->GetAllocatedString(activate, &MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_SYMBOLIC_LINK, &symbolicLink, &symbolicLinkLength);
	jobject returnObject = NULL;

	if(FAILED(result)) {
		ThrowMediaFoundationError(environment, result, L"IMFActivate::GetAllocatedString");
		goto freeDeviceName;
	}

	IMFAttributes* attributes;
	result = MFCreateAttributes(&attributes, 2);

	if(FAILED(result)) {
		ThrowMediaFoundationError(environment, result, L"MFCreateAttributes");
		goto freeSymbolicLink;
	}

	result = attributes->lpVtbl->SetGUID(attributes, &MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE, &MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_GUID);

	if(FAILED(result)) {
		ThrowMediaFoundationError(environment, result, L"IMFAttributes::SetGUID");
		attributes->lpVtbl->Release(attributes);
		goto freeSymbolicLink;
	}

	result = attributes->lpVtbl->SetString(attributes, &MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_SYMBOLIC_LINK, symbolicLink);
	
	if(FAILED(result)) {
		ThrowMediaFoundationError(environment, result, L"IMFAttributes::SetString");
		attributes->lpVtbl->Release(attributes);
		goto freeSymbolicLink;
	}

	IMFMediaSource* source;
	result = MFCreateDeviceSource(attributes, &source);
	attributes->lpVtbl->Release(attributes);

	if(FAILED(result)) {
		ThrowMediaFoundationError(environment, result, L"MFCreateDeviceSource");
		goto freeSymbolicLink;
	}

	IMFPresentationDescriptor* presentationDescriptor;
	result = source->lpVtbl->CreatePresentationDescriptor(source, &presentationDescriptor);
	source->lpVtbl->Release(source);

	if(FAILED(result)) {
		ThrowMediaFoundationError(environment, result, L"IMFMediaSource::CreatePresentationDescriptor");
		goto freeSymbolicLink;
	}

	BOOL selected;
	IMFStreamDescriptor* streamDescriptor;
	result = presentationDescriptor->lpVtbl->GetStreamDescriptorByIndex(presentationDescriptor, 0, &selected, &streamDescriptor);
	presentationDescriptor->lpVtbl->Release(presentationDescriptor);

	if(FAILED(result)) {
		ThrowMediaFoundationError(environment, result, L"IMFPresentationDescriptor::GetStreamDescriptorByIndex");
		goto freeSymbolicLink;
	}

	IMFMediaTypeHandler* mediaTypeHandler;
	result = streamDescriptor->lpVtbl->GetMediaTypeHandler(streamDescriptor, &mediaTypeHandler);
	streamDescriptor->lpVtbl->Release(streamDescriptor);

	if(FAILED(result)) {
		ThrowMediaFoundationError(environment, result, L"IMFStreamDescriptor::GetMediaTypeHandler");
		goto freeSymbolicLink;
	}

	DWORD mediaTypeCount;
	result = mediaTypeHandler->lpVtbl->GetMediaTypeCount(mediaTypeHandler, &mediaTypeCount);

	if(FAILED(result)) {
		ThrowMediaFoundationError(environment, result, L"IMFMediaTypeHandler::GetMediaTypeCount");
		mediaTypeHandler->lpVtbl->Release(mediaTypeHandler);
		goto freeSymbolicLink;
	}

	jobjectArray mediaTypeArray = (*environment)->NewObjectArray(environment, mediaTypeCount, mediaTypeClass, NULL);

	if(!mediaTypeArray) {
		mediaTypeHandler->lpVtbl->Release(mediaTypeHandler);
		goto freeSymbolicLink;
	}

	for(DWORD i = 0; i < mediaTypeCount; i++) {
		IMFMediaType* mediaType;
		result = mediaTypeHandler->lpVtbl->GetMediaTypeByIndex(mediaTypeHandler, i, &mediaType);

		if(FAILED(result)) {
			ThrowMediaFoundationError(environment, result, L"IMFMediaTypeHandler::GetMediaTypeByIndex");
			mediaTypeHandler->lpVtbl->Release(mediaTypeHandler);
			goto freeSymbolicLink;
		}

		jobject mediaTypeInstance = newMediaTypeInstance(environment, mediaTypeClass, mediaTypeConstructor, mediaType);
		mediaType->lpVtbl->Release(mediaType);

		if(!mediaTypeInstance) {
			mediaTypeHandler->lpVtbl->Release(mediaTypeHandler);
			goto freeSymbolicLink;
		}

		(*environment)->SetIntField(environment, mediaTypeInstance, indexField, (jint) i);
		(*environment)->SetObjectArrayElement(environment, mediaTypeArray, (jsize) i, mediaTypeInstance);
	}

	mediaTypeHandler->lpVtbl->Release(mediaTypeHandler);
	jstring deviceNameJava = (*environment)->NewString(environment, deviceName, (jsize) deviceNameLength);
	jstring symbolicLinkJava = (*environment)->NewString(environment, symbolicLink, (jsize) symbolicLinkLength);
	returnObject = (*environment)->NewObject(environment, cameraClass, cameraConstructor, deviceNameJava, symbolicLinkJava, mediaTypeArray);
freeSymbolicLink:
	CoTaskMemFree(symbolicLink);
freeDeviceName:
	CoTaskMemFree(deviceName);
	return returnObject;
}
