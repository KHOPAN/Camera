package test;

import com.khopan.camera.Camera;

public class Main {
	public static void main(String[] args) {
		System.load("D:\\GitHub Repository\\Camera\\Camera\\x64\\Debug\\CameraDLL.dll");
		Camera[] list = Camera.list();

		if(list == null) {
			return;
		}

		for(int i = 0; i < list.length; i++) {
			System.out.println(list[i].getName() + " " + list[i].getSymbolicLink());
		}
	}
}
