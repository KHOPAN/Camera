package test;

import com.khopan.camera.Camera;
import com.khopan.camera.MediaType;

public class Main {
	public static void main(String[] args) {
		System.load("D:\\GitHub Repository\\Camera\\Camera\\x64\\Debug\\CameraDLL.dll");
		Camera[] list = Camera.list();

		if(list == null) {
			return;
		}

		Camera camera = list[0];
		MediaType[] types = camera.getSortedMediaType().toArray(new MediaType[0]);

		for(int i = 0; i < types.length; i++) {
			MediaType type = types[i];
			System.out.println(type.getFrameWidth() + "x" + type.getFrameHeight() + " " + type.getFramerate() + " FPS " + type.getVideoFormat().name());
		}
	}
}
