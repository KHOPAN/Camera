package com.khopan.camera;

import java.awt.image.BufferedImage;
import java.util.ArrayList;
import java.util.List;

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

	public List<MediaType> getMediaTypeList() {
		return List.of(this.mediaTypes);
	}

	public List<MediaType> getSortedMediaType(SortRule... rules) {
		List<MediaType> list = new ArrayList<>();
		list.addAll(this.getMediaTypeList());

		if(rules == null || rules.length == 0) {
			return list;
		}

		for(int i = rules.length - 1; i >= 0; i--) {
			list.sort(rules[i].comparator());
		}

		return list;
	}

	public List<MediaType> getSortedMediaType() {
		return this.getSortedMediaType(SortRule.values());
	}

	public MediaType getBestMediaType(SortRule... rules) {
		return this.getSortedMediaType(rules).getFirst();
	}

	public MediaType getBestMediaType() {
		return this.getBestMediaType(SortRule.values());
	}

	@Override
	public boolean equals(Object camera) {
		if(!(camera instanceof Camera)) {
			return false;
		}

		return this.symbolicLink.equals(((Camera) camera).symbolicLink);
	}

	public BufferedImage capture() {
		return this.capture(SortRule.values());
	}

	public BufferedImage capture(SortRule... rules) {
		return this.capture(this.getSortedMediaType(rules).getFirst());
	}

	public BufferedImage capture(MediaType type) {
		VideoFormat format = type.getVideoFormat();
		Decoder decoder = format.newDecoder();
		byte[] data = this.captureInternal(type);
		return decoder.decode(data, type.getFrameWidth(), type.getFrameHeight());
	}

	private native byte[] captureInternal(MediaType type);

	public static native Camera[] list();
}
