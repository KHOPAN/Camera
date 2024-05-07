#include <khopanwin32.h>
#include <khopanjava.h>
#include <khopanstring.h>
#include "common.h"

void ThrowWin32Error(JNIEnv* environment, const LPSTR errorClass, DWORD errorCode, const LPWSTR functionName) {
	LPWSTR messageBuffer = NULL;

	if(!FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, errorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR) &messageBuffer, 0, NULL)) {
		KHJavaThrowInternalErrorW(environment, L"FormatMessageW() failed to format the error message");
		return;
	}

	LPWSTR message = KHFormatMessageW(L"%ws (Error code: %u)", messageBuffer, errorCode);
	LocalFree(messageBuffer);

	if(!message) {
		KHJavaThrowInternalErrorW(environment, L"KHFormatMessageW() failed to format the error message");
		return;
	}

	KHJavaThrowW(environment, errorClass, message);
	LocalFree(message);
}
