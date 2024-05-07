package test;

import com.khopan.camera.Camera;

public class Main {
	public static void main(String[] args) {
		System.load("D:\\GitHub Repository\\Camera\\Camera\\x64\\Debug\\CameraDLL.dll");
		Camera[] list = Camera.list();

		if(list == null) {
			return;
		}

		Camera camera = list[0];
		byte[] image = camera.capture();
		System.out.println(image == null ? "null" : image.length);
	}
}
