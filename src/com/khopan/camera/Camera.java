package com.khopan.camera;

public class Camera {
	private final String name;
	private final String symbolicLink;

	private Camera(String name, String symbolicLink) {
		this.name = name;
		this.symbolicLink = symbolicLink;
	}

	public String getName() {
		return this.name;
	}

	public String getSymbolicLink() {
		return this.symbolicLink;
	}

	public static native Camera[] list();
}
