package com.khopan.camera;

public class Camera {
	private final String name;
	private final String symbolicLink;
	private final MediaType[] mediaTypes;

	private Camera(String name, String symbolicLink, MediaType[] mediaTypes) {
		this.name = name;
		this.symbolicLink = symbolicLink;
		this.mediaTypes = mediaTypes;
	}

	public String getName() {
		return this.name;
	}

	public String getSymbolicLink() {
		return this.symbolicLink;
	}

	public MediaType[] getMediaTypes() {
		return this.mediaTypes;
	}

	@Override
	public boolean equals(Object camera) {
		if(!(camera instanceof Camera)) {
			return false;
		}

		return this.symbolicLink.equals(((Camera) camera).symbolicLink);
	}

	public static native Camera[] list();
}
