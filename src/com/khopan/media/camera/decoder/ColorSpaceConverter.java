package com.khopan.media.camera.decoder;

class ColorSpaceConverter {
	private ColorSpaceConverter() {}

	static int yuvToPackedRGB(byte Y, byte U, byte V) {
		int y = Y & 0xFF;
		int u = (U & 0xFF) - 128;
		int v = (V & 0xFF) - 128;
		int red   = Math.min(Math.max((int) Math.round(y + 1.402d * v),                  0), 255); 
		int green = Math.min(Math.max((int) Math.round(y - 0.34414d * u - 0.71414d * v), 0), 255); 
		int blue  = Math.min(Math.max((int) Math.round(y + 1.772d * u),                  0), 255);
		return ((red & 0xFF) << 16) | ((green & 0xFF) << 8) | (blue & 0xFF);
	}
}
