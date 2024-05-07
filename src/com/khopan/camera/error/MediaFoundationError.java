package com.khopan.camera.error;

public class MediaFoundationError extends Win32Error {
	private static final long serialVersionUID = -784647946375028588L;

	public MediaFoundationError() {
		super();
	}

	public MediaFoundationError(String message) {
		super(message);
	}

	public MediaFoundationError(String message, Throwable cause) {
		super(message, cause);
	}

	public MediaFoundationError(Throwable cause) {
		super(cause);
	}

	public MediaFoundationError(String message, Throwable cause, boolean enableSuppression, boolean writableStackTrace) {
		super(message, cause, enableSuppression, writableStackTrace);
	}
}
