package com.khopan.camera;

import java.lang.annotation.Native;

public class Camera {
	@Native
	private String name;

	@Native
	private String symbolicLink;

	public Camera() {

	}

	public String getName() {
		return this.name;
	}

	public String getSymbolicLink() {
		return this.symbolicLink;
	}

	public native byte[] capture();
}
