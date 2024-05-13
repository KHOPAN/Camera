package com.khopan.media.camera.decoder;

import java.awt.image.BufferedImage;

import com.khopan.media.camera.Decoder;

public class YUY2Decoder implements Decoder {
	@Override
	public BufferedImage decode(byte[] data, int width, int height) {
		BufferedImage image = new BufferedImage(width, height, BufferedImage.TYPE_INT_RGB);

		for(int i = 0; i < data.length; i += 4) {
			byte u = data[i + 1];
			byte v = data[i + 3];
			int index = i / 2;
			image.setRGB(index % width, index / width, ColorSpaceConverter.yuvToPackedRGB(data[i], u, v));
			index++;
			image.setRGB(index % width, index / width, ColorSpaceConverter.yuvToPackedRGB(data[i + 2], u, v));
		}

		return image;
	}
}
