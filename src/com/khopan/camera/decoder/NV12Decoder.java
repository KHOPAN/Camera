package com.khopan.camera.decoder;

import java.awt.image.BufferedImage;

import com.khopan.camera.Decoder;

public class NV12Decoder implements Decoder {
	@Override
	public BufferedImage decode(byte[] data, int width, int height) {		
		BufferedImage image = new BufferedImage(width, height, BufferedImage.TYPE_INT_RGB);
		int pixels = width * height;
		byte[] planeY = new byte[pixels];
		byte[] planeUV = new byte[pixels / 2];

		for(int i = 0; i < planeY.length; i++) {
			planeY[i] = data[i];
		}

		for(int i = 0; i < planeUV.length; i++) {
			planeUV[i] = data[i + planeY.length];
		}

		for(int y = 0; y < height; y++) {
			for(int x = 0; x < width; x++) {
				byte y1 = planeY[y * width + x];
				int index = ((y / 2) * (width / 2) + (x / 2)) * 2;
				byte u = planeUV[index];
				byte v = planeUV[index + 1];
				image.setRGB(x, y, this.yuvToPackedRGB(y1, u, v));
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
