package com.khopan.camera.decoder;

import java.awt.Graphics2D;
import java.awt.RenderingHints;
import java.awt.image.BufferedImage;
import java.io.ByteArrayInputStream;

import javax.imageio.ImageIO;

import com.khopan.camera.Decoder;

public class MJPGDecoder implements Decoder {
	@Override
	public BufferedImage decode(byte[] data, int width, int height) {
		BufferedImage capture;

		try {
			capture = ImageIO.read(new ByteArrayInputStream(data));
		} catch(Throwable Errors) {
			throw new RuntimeException(Errors);
		}

		BufferedImage image = new BufferedImage(width, height, BufferedImage.TYPE_INT_RGB);
		Graphics2D Graphics = image.createGraphics();
		Graphics.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
		Graphics.drawImage(capture.getScaledInstance(width, height, BufferedImage.SCALE_SMOOTH), 0, 0, null);
		Graphics.dispose();
		return image;
	}
}
