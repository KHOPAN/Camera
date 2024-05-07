package com.khopan.camera.error;

public class ComAPIError extends Win32Error {
	private static final long serialVersionUID = -6046569929302405181L;

	public ComAPIError() {
		super();
	}

	public ComAPIError(String message) {
		super(message);
	}

	public ComAPIError(String message, Throwable cause) {
		super(message, cause);
	}

	public ComAPIError(Throwable cause) {
		super(cause);
	}

	public ComAPIError(String message, Throwable cause, boolean enableSuppression, boolean writableStackTrace) {
		super(message, cause, enableSuppression, writableStackTrace);
	}
}
