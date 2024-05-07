package com.khopan.camera;

import java.util.Comparator;

public enum SortRule {
	BEST_SIZE(Comparator.comparingInt(x -> -x.getFrameWidth() * x.getFrameHeight())),
	BEST_FORMAT(Comparator.comparingInt(x -> x.getVideoFormat().ordinal())),
	BEST_FRAMERATE(Comparator.comparingDouble(x -> -x.getFramerate()));

	private final Comparator<? super MediaType> comparator;

	SortRule(Comparator<? super MediaType> comparator) {
		this.comparator = comparator;
	}

	public Comparator<? super MediaType> comparator() {
		return this.comparator;
	}
}
