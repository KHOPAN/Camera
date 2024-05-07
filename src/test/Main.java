package test;

import java.awt.image.BufferedImage;

import com.khopan.camera.Camera;
import com.khopan.camera.MediaType;
import com.khopan.camera.VideoFormat;

public class Main {
	public static void main(String[] args) {
		System.load("D:\\GitHub Repository\\Camera\\Camera\\x64\\Debug\\CameraDLL.dll");
		Camera[] list = Camera.list();

		if(list == null) {
			return;
		}

		Camera camera = list[0];
		MediaType[] types = camera.getMediaTypes();

		for(int i = 0; i < types.length; i++) {
			MediaType type = types[i];

			if(VideoFormat.YUY2.equals(type.getVideoFormat())) {
				BufferedImage image = camera.capture(type);
				Preview.preview(image);
				return;
			}
		}
	}
}
