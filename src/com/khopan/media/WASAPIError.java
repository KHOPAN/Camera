package com.khopan.media;

public class WASAPIError extends Win32Error {
	private static final long serialVersionUID = -8970632457890503328L;

	public WASAPIError() {
		super();
	}

	public WASAPIError(String message) {
		super(message);
	}

	public WASAPIError(String message, Throwable cause) {
		super(message, cause);
	}

	public WASAPIError(Throwable cause) {
		super(cause);
	}

	public WASAPIError(String message, Throwable cause, boolean enableSuppression, boolean writableStackTrace) {
		super(message, cause, enableSuppression, writableStackTrace);
	}
}
