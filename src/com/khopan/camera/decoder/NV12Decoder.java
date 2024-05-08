package com.khopan.camera.decoder;

import java.awt.image.BufferedImage;

import com.khopan.camera.Decoder;

public class NV12Decoder implements Decoder {
	@Override
	public BufferedImage decode(byte[] data, int width, int height) {		
		BufferedImage image = new BufferedImage(width, height, BufferedImage.TYPE_INT_RGB);
		int pixels = width * height;
		int halfWidth = width / 2;

		for(int y = 0; y < height; y++) {
			for(int x = 0; x < width; x++) {
				int index = (y / 2 * halfWidth + x / 2) * 2 + pixels;
				image.setRGB(x, y, ColorSpaceConverter.yuvToPackedRGB(data[y * width + x], data[index], data[index + 1]));
			}
		}

		return image;
	}
}
