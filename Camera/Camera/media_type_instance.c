#include "common.h"
#include <mfapi.h>
#include "camera_instance.h"

static BOOL assignMediaTypeField(JNIEnv* environment, jobject mediaTypeInstance, jclass mediaTypeClass, IMFMediaType* mediaType, UINT32 index);

jobject newMediaTypeInstance(JNIEnv* environment, jclass mediaTypeClass, jmethodID mediaTypeConstructor, IMFMediaType* mediaType) {
	UINT32 count;
	HRESULT result = mediaType->lpVtbl->GetCount(mediaType, &count);

	if(FAILED(result)) {
		ThrowMediaFoundationError(environment, result, L"IMFMediaType::GetCount");
		return NULL;
	}

	jobject mediaTypeInstance = (*environment)->NewObject(environment, mediaTypeClass, mediaTypeConstructor);

	for(UINT i = 0; i < count; i++) {
		if(!assignMediaTypeField(environment, mediaTypeInstance, mediaTypeClass, mediaType, count - i - 1)) {
			return NULL;
		}
	}

	return mediaTypeInstance;
}

static BOOL assignMediaTypeField(JNIEnv* environment, jobject mediaTypeInstance, jclass mediaTypeClass, IMFMediaType* mediaType, UINT32 index) {
	GUID key = {0};
	PROPVARIANT value;
	memset(&value, 0, sizeof(PROPVARIANT));
	HRESULT result = mediaType->lpVtbl->GetItemByIndex(mediaType, index, &key, &value);

	if(FAILED(result)) {
		ThrowMediaFoundationError(environment, result, L"IMFMediaType::GetItemByIndex");
		return FALSE;
	}

	if(IsEqualGUID(&key, &MF_MT_FRAME_SIZE)) {
		jfieldID frameWidthField = (*environment)->GetFieldID(environment, mediaTypeClass, "frameWidth", "I");

		if(!frameWidthField) {
			return FALSE;
		}

		jfieldID frameHeightField = (*environment)->GetFieldID(environment, mediaTypeClass, "frameHeight", "I");

		if(!frameHeightField) {
			return FALSE;
		}

		UINT64 frameSize = value.uhVal.QuadPart;
		UINT32 width = (UINT32) (frameSize >> 32);
		UINT32 height = (UINT32) frameSize;
		(*environment)->SetIntField(environment, mediaTypeInstance, frameWidthField, (jsize) width);
		(*environment)->SetIntField(environment, mediaTypeInstance, frameHeightField, (jsize) height);
		return TRUE;
	}

	if(IsEqualGUID(&key, &MF_MT_FRAME_RATE)) {
		jfieldID framerateField = (*environment)->GetFieldID(environment, mediaTypeClass, "framerate", "D");

		if(!framerateField) {
			return FALSE;
		}

		UINT64 framerate = value.uhVal.QuadPart;
		UINT32 low = (UINT32) (framerate >> 32);
		UINT32 high = (UINT32) framerate;
		(*environment)->SetDoubleField(environment, mediaTypeInstance, framerateField, ((jdouble) low) / ((jdouble) high));
		return TRUE;
	}
	
	if(IsEqualGUID(&key, &MF_MT_SUBTYPE)) {
		jfieldID formatField = (*environment)->GetFieldID(environment, mediaTypeClass, "format", "Lcom/khopan/camera/VideoFormat;");

		if(!formatField) {
			return FALSE;
		}

		jclass videoFormatEnum = (*environment)->FindClass(environment, "com/khopan/camera/VideoFormat");

		if(!videoFormatEnum) {
			return FALSE;
		}

		LPSTR enumValue;

		if(IsEqualGUID(value.puuid, &MFVideoFormat_MJPG)) {
			enumValue = "MJPG";
		} else if(IsEqualGUID(value.puuid, &MFVideoFormat_YUY2)) {
			enumValue = "YUY2";
		} else if(IsEqualGUID(value.puuid, &MFVideoFormat_NV12)) {
			enumValue = "NV12";
		} else {
			enumValue = "UNKNOWN";
		}

		jfieldID enumValueField = (*environment)->GetStaticFieldID(environment, videoFormatEnum, enumValue, "Lcom/khopan/camera/VideoFormat;");

		if(!enumValueField) {
			return FALSE;
		}

		jobject enumInstance = (*environment)->GetStaticObjectField(environment, mediaTypeClass, enumValueField);
		(*environment)->SetObjectField(environment, mediaTypeInstance, formatField, enumInstance);
		return TRUE;
	}

	return TRUE;
}
