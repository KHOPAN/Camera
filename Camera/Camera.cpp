#include <Windows.h>
#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>
#include <jni.h>
#include "helper.h"
#include "com_khopan_camera_Camera.h"

#pragma comment(lib, "Mf.lib")
#pragma comment(lib, "Mfplat.lib")
#pragma comment(lib, "mfuuid.lib")
#pragma comment(lib, "mfreadwrite.lib")

JNIEXPORT jbyteArray JNICALL Java_com_khopan_camera_Camera_capture(JNIEnv* environment, jobject cameraInstance) {
	jclass classClass = environment->FindClass("java/lang/Class");

	if(!classClass) {
		throwClass(environment, "java/lang/ClassNotFoundException", "Class 'java.lang.Class' not found");
		return NULL;
	}

	jmethodID getSimpleNameMethod = environment->GetMethodID(classClass, "getSimpleName", "()Ljava/lang/String;");
	
	if(!getSimpleNameMethod) {
		throwClass(environment, "java/lang/NoSuchMethodException", "Method 'getSimpleName()' of signature '()Ljava/lang/String;' not found class 'java.lang.Class'");
		return NULL;
	}
	
	jclass cameraClass = environment->GetObjectClass(cameraInstance);
	jstring classNameString = static_cast<jstring>(environment->CallObjectMethod(cameraClass, getSimpleNameMethod));
	const char* className = environment->GetStringUTFChars(classNameString, NULL);
	std::string classNameText(className);
	jfieldID symbolicLinkField = environment->GetFieldID(cameraClass, "symbolicLink", "Ljava/lang/String;");

	if(!symbolicLinkField) {
		throwClass(environment, "java/lang/NoSuchFieldException", "Field 'symbolicLink' of type 'java.lang.String' not found in class '" + classNameText + '\'');
		return NULL;
	}

	jstring symbolicLink = static_cast<jstring>(environment->GetObjectField(cameraInstance, symbolicLinkField));

	if(!symbolicLink) {
		throwRuntime(environment, "Field 'symbolicLink' must not be null");
		return NULL;
	}

	jsize symbolicLinkLength = environment->GetStringLength(symbolicLink);

	if(symbolicLinkLength < 1) {
		throwRuntime(environment, "Field 'symbolicLink' must not be empty");
		return NULL;
	}

	HRESULT result = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	CHECK("CoInitializeEx() failed");
	result = MFStartup(MF_VERSION, MFSTARTUP_NOSOCKET);
	CHECK("MFStartup() failed");
	IMFAttributes* attributes = NULL;
	result = MFCreateAttributes(&attributes, 2);
	CHECK("MFCreateAttributes() failed");
	result = attributes->SetGUID(MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE, MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_GUID);
	CHECK("IMFAttributes::SetGUID() failed");
	const jchar* stringChars = environment->GetStringChars(symbolicLink, NULL);
	const WCHAR* symbolicLinkWide = reinterpret_cast<const WCHAR*>(stringChars);
	result = attributes->SetString(MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_SYMBOLIC_LINK, symbolicLinkWide);
	CHECK("IMFAttributes::SetString() failed");
	IMFMediaSource* source = NULL;
	result = MFCreateDeviceSource(attributes, &source);
	CHECK("MFCreateDeviceSource() failed");
	attributes->Release();
	IMFSourceReader* reader = NULL;
	result = MFCreateSourceReaderFromMediaSource(source, NULL, &reader);
	CHECK("MFCreateSourceReaderFromMediaSource() failed");
	source->Release();
	IMFMediaType* type = NULL;
	result = MFCreateMediaType(&type);
	CHECK("MFCreateMediaType() failed");
	result = type->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video);
	CHECK("IMFMediaType::SetGUID() failed");
	result = type->SetGUID(MF_MT_SUBTYPE, MFVideoFormat_MJPG);
	CHECK("IMFMediaType::SetGUID() failed");
	result = reader->SetCurrentMediaType(MF_SOURCE_READER_FIRST_VIDEO_STREAM, NULL, type);
	CHECK("IMFSourceReader::SetCurrentMediaType() failed");
	type->Release();
	IMFSample* sample = NULL;
	DWORD stream = 0;
	DWORD flags = 0;
	LONGLONG timestamp = 0;

	for(;;) {
		result = reader->ReadSample(MF_SOURCE_READER_FIRST_VIDEO_STREAM, 0, &stream, &flags, &timestamp, &sample);
		CHECK("IMFSourceReader::ReadSample() failed");

		if(flags & MF_SOURCE_READERF_STREAMTICK) {
			continue;
		}

		break;
	}

	IMFMediaBuffer* buffer = NULL;
	result = sample->ConvertToContiguousBuffer(&buffer);
	CHECK("IMFMediaBuffer::ConvertToContiguousBuffer() failed");
	BYTE* data = NULL;
	DWORD dataSize = 0;
	result = buffer->Lock(&data, NULL, &dataSize);
	CHECK("IMFMediaBuffer::Lock() failed");
	jbyteArray array = environment->NewByteArray(dataSize);
	environment->SetByteArrayRegion(array, 0, dataSize, reinterpret_cast<jbyte*>(data));
	result = buffer->Unlock();
	CHECK("IMFMediaBuffer::Unlock() failed");
	buffer->Release();
	sample->Release();
	reader->Release();
	result = MFShutdown();
	CHECK("MFShutdown() failed");
	CoUninitialize();
	return array;
}
