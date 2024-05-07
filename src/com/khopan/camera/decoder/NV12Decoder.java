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
				image.setRGB(x, y, this.yuvToPackedRGB(data[y * width + x], data[index], data[index + 1]));
			}
		}

		return image;
	}

	private int yuvToPackedRGB(byte y, byte u, byte v) {
		double Y = 1.164d * (((double) (y & 0xFF)) - 16.0d);
		double U = (double) (u & 0xFF) - 128.0d;
		double V = (double) (v & 0xFF) - 128.0d;
		int red   = Math.min(Math.max((int) Math.round(Y + 1.596d * V),              0), 255);
		int green = Math.min(Math.max((int) Math.round(Y - 0.813d * V - 0.391d * U), 0), 255);
		int blue  = Math.min(Math.max((int) Math.round(Y + 2.018d * U),              0), 255);
		return ((red & 0xFF) << 16) | ((green & 0xFF) << 8) | (blue & 0xFF);
	}
}
