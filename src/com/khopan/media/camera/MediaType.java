package com.khopan.media.camera;

public class MediaType {
	private int index;
	private int frameWidth;
	private int frameHeight;
	private double framerate;
	private VideoFormat format;

	private MediaType() {

	}

	public int getIndex() {
		return this.index;
	}

	public int getFrameWidth() {
		return this.frameWidth;
	}

	public int getFrameHeight() {
		return this.frameHeight;
	}

	public double getFramerate() {
		return this.framerate;
	}

	public VideoFormat getVideoFormat() {
		return this.format;
	}
}
