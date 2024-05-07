package com.khopan.camera;

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

	public byte[] capture() {
		return this.capture(SortRule.values());
	}

	public byte[] capture(SortRule... rules) {
		return this.capture(this.getSortedMediaType(rules).getFirst());
	}

	public byte[] capture(MediaType type) {
		return this.captureInternal(type);
	}

	private native byte[] captureInternal(MediaType type);

	public static native Camera[] list();
}
