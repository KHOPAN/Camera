package test;

import com.khopan.camera.Camera;

public class Main {
	public static void main(String[] args) {
		Camera[] list = Camera.list();

		for(int i = 0; i < list.length; i++) {
			System.out.println(list[0].getName() + " " + list[0].getSymbolicLink());
		}
	}
}
