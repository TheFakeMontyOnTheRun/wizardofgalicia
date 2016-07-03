package br.odb.wog;

import android.opengl.GLSurfaceView;

import com.google.vr.sdk.base.Eye;
import com.google.vr.sdk.base.GvrView;
import com.google.vr.sdk.base.HeadTransform;
import com.google.vr.sdk.base.Viewport;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/**
 * Created by monty on 17/03/16.
 */
public class GL2JNIRenderer implements GvrView.StereoRenderer {

	final public Object lock = new Object();

	@Override
	public void onNewFrame(HeadTransform headTransform) {

	}

	@Override
	public void onDrawEye(Eye eye) {
		synchronized (lock) {
			GL2JNILib.setPerspectiveMatrix( eye.getPerspective(0.1f, 100.0f ) );
			GL2JNILib.setEyeMatrix( eye.getEyeView() );
			GL2JNILib.step();
		}

	}

	@Override
	public void onFinishFrame(Viewport viewport) {

	}

	@Override
	public void onSurfaceChanged(int width, int height) {
		synchronized (lock) {
			GL2JNILib.init(width, height);
		}
	}

	@Override
	public void onSurfaceCreated(EGLConfig eglConfig) {

	}

	@Override
	public void onRendererShutdown() {

	}
}
