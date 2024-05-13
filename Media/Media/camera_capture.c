#include <khopanjava.h>
#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>
#include "native.h"
#include "common.h"

jbyteArray Camera_capture(JNIEnv* environment, jobject cameraInstance, jobject mediaTypeInstance) {
	if(!mediaTypeInstance) {
		return NULL;
	}

	jclass mediaTypeClass = (*environment)->FindClass(environment, "com/khopan/camera/MediaType");

	if(!mediaTypeClass) {
		return NULL;
	}

	jfieldID indexField = (*environment)->GetFieldID(environment, mediaTypeClass, "index", "I");

	if(!indexField) {
		return NULL;
	}

	jint mediaTypeIndex = (*environment)->GetIntField(environment, mediaTypeInstance, indexField);
	jclass cameraClass = (*environment)->FindClass(environment, "com/khopan/camera/Camera");

	if(!cameraClass) {
		return NULL;
	}

	jfieldID symbolicLinkField = (*environment)->GetFieldID(environment, cameraClass, "symbolicLink", "Ljava/lang/String;");

	if(!symbolicLinkField) {
		return NULL;
	}

	jstring symbolicLinkJava = (jstring) ((*environment)->GetObjectField(environment, cameraInstance, symbolicLinkField));

	if(!symbolicLinkJava) {
		return NULL;
	}

	LPWSTR symbolicLink = KHJavaToNativeStringW(environment, symbolicLinkJava);

	if(!symbolicLink) {
		KHJavaThrowIllegalArgumentExceptionW(environment, L"Symbolic link cannot be found");
		return NULL;
	}

	if(!InitializeMediaFoundation(environment)) {
		LocalFree(symbolicLink);
		return NULL;
	}

	IMFAttributes* attributes;
	HRESULT result = MFCreateAttributes(&attributes, 2);
	jbyteArray returnResult = NULL;

	if(FAILED(result)) {
		ThrowMediaFoundationError(environment, result, L"MFCreateAttributes");
		LocalFree(symbolicLink);
		goto uninitialize;
	}

	result = attributes->lpVtbl->SetGUID(attributes, &MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE, &MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_GUID);

	if(FAILED(result)) {
		ThrowMediaFoundationError(environment, result, L"IMFAttributes::SetGUID");
		attributes->lpVtbl->Release(attributes);
		LocalFree(symbolicLink);
		goto uninitialize;
	}

	result = attributes->lpVtbl->SetString(attributes, &MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_SYMBOLIC_LINK, symbolicLink);
	LocalFree(symbolicLink);

	if(FAILED(result)) {
		ThrowMediaFoundationError(environment, result, L"IMFAttributes::SetGUID");
		attributes->lpVtbl->Release(attributes);
		goto uninitialize;
	}

	IMFMediaSource* source;
	result = MFCreateDeviceSource(attributes, &source);
	attributes->lpVtbl->Release(attributes);

	if(FAILED(result)) {
		ThrowMediaFoundationError(environment, result, L"MFCreateDeviceSource");
		goto uninitialize;
	}

	IMFPresentationDescriptor* presentationDescriptor;
	result = source->lpVtbl->CreatePresentationDescriptor(source, &presentationDescriptor);

	if(FAILED(result)) {
		ThrowMediaFoundationError(environment, result, L"IMFMediaSource::CreatePresentationDescriptor");
		source->lpVtbl->Release(source);
		goto uninitialize;
	}

	BOOL selected;
	IMFStreamDescriptor* streamDescriptor;
	result = presentationDescriptor->lpVtbl->GetStreamDescriptorByIndex(presentationDescriptor, 0, &selected, &streamDescriptor);
	presentationDescriptor->lpVtbl->Release(presentationDescriptor);

	if(FAILED(result)) {
		ThrowMediaFoundationError(environment, result, L"IMFPresentationDescriptor::GetStreamDescriptorByIndex");
		source->lpVtbl->Release(source);
		goto uninitialize;
	}

	IMFMediaTypeHandler* mediaTypeHandler;
	result = streamDescriptor->lpVtbl->GetMediaTypeHandler(streamDescriptor, &mediaTypeHandler);
	streamDescriptor->lpVtbl->Release(streamDescriptor);

	if(FAILED(result)) {
		ThrowMediaFoundationError(environment, result, L"IMFStreamDescriptor::GetMediaTypeHandler");
		source->lpVtbl->Release(source);
		goto uninitialize;
	}

	IMFMediaType* mediaType;
	result = mediaTypeHandler->lpVtbl->GetMediaTypeByIndex(mediaTypeHandler, mediaTypeIndex, &mediaType);
	mediaTypeHandler->lpVtbl->Release(mediaTypeHandler);

	if(FAILED(result)) {
		ThrowMediaFoundationError(environment, result, L"IMFMediaTypeHandler::GetMediaTypeByIndex");
		source->lpVtbl->Release(source);
		goto uninitialize;
	}

	IMFSourceReader* reader;
	result = MFCreateSourceReaderFromMediaSource(source, NULL, &reader);
	source->lpVtbl->Release(source);

	if(FAILED(result)) {
		ThrowMediaFoundationError(environment, result, L"MFCreateSourceReaderFromMediaSource");
		mediaType->lpVtbl->Release(mediaType);
		goto uninitialize;
	}

	result = reader->lpVtbl->SetCurrentMediaType(reader, MF_SOURCE_READER_FIRST_VIDEO_STREAM, NULL, mediaType);
	mediaType->lpVtbl->Release(mediaType);

	if(FAILED(result)) {
		ThrowMediaFoundationError(environment, result, L"IMFSourceReader::SetCurrentMediaType");
		goto releaseReader;
	}

	DWORD flags;
	IMFSample* sample;

	for(;;) {
		result = reader->lpVtbl->ReadSample(reader, MF_SOURCE_READER_FIRST_VIDEO_STREAM, 0, NULL, &flags, NULL, &sample);

		if(FAILED(result) || flags & MF_SOURCE_READERF_ERROR) {
			ThrowMediaFoundationError(environment, result, L"IMFSourceReader::ReadSample");
			goto releaseReader;
		}

		if(flags & MF_SOURCE_READERF_STREAMTICK) {
			continue;
		}

		break;
	}

	IMFMediaBuffer* mediaBuffer;
	result = sample->lpVtbl->ConvertToContiguousBuffer(sample, &mediaBuffer);
	sample->lpVtbl->Release(sample);

	if(FAILED(result)) {
		ThrowMediaFoundationError(environment, result, L"IMFSample::ConvertToContiguousBuffer");
		goto releaseReader;
	}

	BYTE* data;
	DWORD size;
	result = mediaBuffer->lpVtbl->Lock(mediaBuffer, &data, NULL, &size);

	if(FAILED(result)) {
		ThrowMediaFoundationError(environment, result, L"IMFMediaBuffer::Lock");
		mediaBuffer->lpVtbl->Release(mediaBuffer);
		goto releaseReader;
	}

	jbyteArray byteArray = (*environment)->NewByteArray(environment, (jsize) size);

	if(!byteArray) {
		mediaBuffer->lpVtbl->Release(mediaBuffer);
		goto releaseReader;
	}

	(*environment)->SetByteArrayRegion(environment, byteArray, 0, (jsize) size, data);
	result = mediaBuffer->lpVtbl->Unlock(mediaBuffer);
	mediaBuffer->lpVtbl->Release(mediaBuffer);

	if(FAILED(result)) {
		ThrowMediaFoundationError(environment, result, L"IMFMediaBuffer::Unlock");
		goto releaseReader;
	}

	returnResult = byteArray;
releaseReader:
	reader->lpVtbl->Release(reader);
uninitialize:
	UninitializeMediaFoundation(environment);
	return returnResult;
}
