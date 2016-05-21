package br.odb.wog;

import android.opengl.GLSurfaceView;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/**
 * Created by monty on 17/03/16.
 */
public class GL2JNIRenderer implements GLSurfaceView.Renderer {

	final public Object lock = new Object();

	@Override
	public void onSurfaceCreated(GL10 gl, EGLConfig config) {
	}

	@Override
	public void onSurfaceChanged(GL10 gl, int width, int height) {
		synchronized (lock) {
			GL2JNILib.init(width, height);
		}
	}

	@Override
	public void onDrawFrame(GL10 gl) {
		synchronized (lock) {
			GL2JNILib.step();
		}
	}
}
