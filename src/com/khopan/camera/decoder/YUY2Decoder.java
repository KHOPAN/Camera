package com.khopan.camera.decoder;

import java.awt.image.BufferedImage;

import com.khopan.camera.Decoder;

public class YUY2Decoder implements Decoder {
	@Override
	public BufferedImage decode(byte[] data, int width, int height) {
		BufferedImage image = new BufferedImage(width, height, BufferedImage.TYPE_INT_RGB);

		for(int i = 0; i < data.length; i += 4) {
			byte y1 = data[i];
			byte u = data[i + 1];
			byte y2 = data[i + 2];
			byte v = data[i + 3];
			int index = i / 2;
			image.setRGB(index % width, index / width, this.yuvToPackedRGB(y1, u, v));
			index++;
			image.setRGB(index % width, index / width, this.yuvToPackedRGB(y2, u, v));
		}

		return image;
	}

	private int yuvToPackedRGB(byte Y, byte U, byte V) {
		int y = Y & 0xFF;
		int u = (U & 0xFF) - 128;
		int v = (V & 0xFF) - 128;
		int red   = Math.min(Math.max((int) Math.round(y + 1.402d * v),                  0), 255); 
		int green = Math.min(Math.max((int) Math.round(y - 0.34414d * u - 0.71414d * v), 0), 255); 
		int blue  = Math.min(Math.max((int) Math.round(y + 1.772d * u),                  0), 255);
		return ((red & 0xFF) << 16) | ((green & 0xFF) << 8) | (blue & 0xFF);
	}
}
