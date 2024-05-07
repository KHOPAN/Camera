package com.khopan.camera;

import java.awt.image.BufferedImage;

public interface Decoder {
	BufferedImage decode(byte[] data, int width, int height);
}
