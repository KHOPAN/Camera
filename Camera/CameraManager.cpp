#include <Windows.h>
#include <mfapi.h>
#include <mfidl.h>
#include <jni.h>
#include "helper.h"
#include "com_khopan_camera_CameraManager.h"

#pragma comment(lib, "Mf.lib")
#pragma comment(lib, "Mfplat.lib")

JNIEXPORT jobjectArray JNICALL Java_com_khopan_camera_CameraManager_getAllCameraDevices(JNIEnv* environment, jclass cameraManagerClass) {
	jclass cameraClass = environment->FindClass("com/khopan/camera/Camera");
	
	if(!cameraClass) {
		throwClass(environment, "java/lang/ClassNotFoundException", "Class 'com.khopan.camera.Camera' not found");
	}

	jmethodID cameraConstructor = environment->GetMethodID(cameraClass, "<init>", "()V");

	if(!cameraConstructor) {
		throwClass(environment, "java/lang/NoSuchMethodException", "Constructor 'Camera()' of signature '()V' not found class 'com.khopan.camera.Camera'");
	}

	jfieldID nameField = environment->GetFieldID(cameraClass, "name", "Ljava/lang/String;");

	if(!nameField) {
		throwClass(environment, "java/lang/NoSuchFieldException", "Field 'name' of type 'java.lang.String' not found in class 'com.khopan.camera.Camera'");
	}

	jfieldID symbolicLinkField = environment->GetFieldID(cameraClass, "symbolicLink", "Ljava/lang/String;");

	if(!symbolicLinkField) {
		throwClass(environment, "java/lang/NoSuchFieldException", "Field 'symbolicLink' of type 'java.lang.String' not found in class 'com.khopan.camera.Camera'");
	}

	HRESULT result = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	CHECK("CoInitializeEx() failed");
	result = MFStartup(MF_VERSION, MFSTARTUP_NOSOCKET);
	CHECK("MFStartup() failed");
	IMFAttributes* attributes = NULL;
	result = MFCreateAttributes(&attributes, 1);
	CHECK("MFCreateAttributes() failed");
	result = attributes->SetGUID(MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE, MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_GUID);
	CHECK("IMFAttributes::SetGUID() failed");
	IMFActivate** activate = NULL;
	UINT32 count = 0;
	result = MFEnumDeviceSources(attributes, &activate, &count);
	CHECK("MFEnumDeviceSources() failed");
	attributes->Release();

	if(count < 1) {
		return environment->NewObjectArray(0, cameraClass, NULL);
	}

	jobjectArray array = environment->NewObjectArray(count, cameraClass, NULL);

	for(UINT32 i = 0; i < count; i++) {
		IMFActivate* device = activate[i];
		LPWSTR name = NULL;
		UINT32 nameLength = 0;
		result = device->GetAllocatedString(MF_DEVSOURCE_ATTRIBUTE_FRIENDLY_NAME, &name, &nameLength);
		CHECK("IMFActivate::GetAllocatedString() failed");
		LPWSTR symbolicLink = NULL;
		UINT32 symbolicLinkLength = 0;
		result = device->GetAllocatedString(MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_SYMBOLIC_LINK, &symbolicLink, &symbolicLinkLength);
		CHECK("IMFActivate::GetAllocatedString() failed");
		jobject cameraInstance = environment->NewObject(cameraClass, cameraConstructor);
		environment->SetObjectField(cameraInstance, nameField, environment->NewString(reinterpret_cast<jchar*>(name), nameLength));
		environment->SetObjectField(cameraInstance, symbolicLinkField, environment->NewString(reinterpret_cast<jchar*>(symbolicLink), symbolicLinkLength));
		environment->SetObjectArrayElement(array, i, cameraInstance);
		CoTaskMemFree(name);
		CoTaskMemFree(symbolicLink);
		device->Release();
	}

	CoTaskMemFree(activate);
	MFShutdown();
	CoUninitialize();
	return array;
}
