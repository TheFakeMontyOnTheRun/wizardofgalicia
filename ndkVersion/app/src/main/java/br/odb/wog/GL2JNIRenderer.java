package br.odb.wog;

import android.opengl.GLSurfaceView;
import android.util.Log;

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

	public static final float HALF_ARC_OVER_PI = (float) (180.0f / Math.PI);
	private float[] forwardVector = new float[3];

	@Override
	public void onNewFrame(HeadTransform headTransform) {

		float angleXZ = extractAngleXZFromHeadtransform( headTransform );

		while( angleXZ < 0 ) {
			angleXZ += 360.0f;
		}

		angleXZ += 45.0f;

		int direction = (int) (angleXZ / 90) % 4;

		while ( direction < 0 ) {
			direction += 4;
		}

		direction = 4 - direction;

		GL2JNILib.setCharacterDirection( direction );
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

	private float extractAngleXYFromHeadtransform(HeadTransform headTransform) {
		headTransform.getEulerAngles(forwardVector, 0);
		return  360.0f - ((float)( forwardVector[ 2 ] * HALF_ARC_OVER_PI));
	}

	private float extractAngleYZFromHeadtransform(HeadTransform headTransform) {
		headTransform.getEulerAngles(forwardVector, 0);
		return  360.0f - ((float)( forwardVector[ 0 ] * HALF_ARC_OVER_PI ));
	}

	private float extractAngleXZFromHeadtransform(HeadTransform headTransform) {
		headTransform.getEulerAngles(forwardVector, 0);
		return  360.0f - ((float)( forwardVector[ 1 ] * HALF_ARC_OVER_PI ));
	}

}
