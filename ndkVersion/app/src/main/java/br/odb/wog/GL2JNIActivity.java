/*
 * Copyright (C) 2007 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package br.odb.wog;

import android.app.Activity;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.view.KeyEvent;
import android.view.View;

import java.io.IOException;

public class GL2JNIActivity extends Activity {

    GL2JNIView mView;
    boolean running = false;
    static AssetManager assets;


    @Override
    protected void onCreate(Bundle icicle) {
        super.onCreate(icicle);

        assets = getAssets();
        GL2JNILib.onCreate(assets);

        try {
            GL2JNILib.setTexture(new Bitmap[]{
                    BitmapFactory.decodeStream(assets.open("floor.png")),
                    BitmapFactory.decodeStream(assets.open("wall.png")),
                    BitmapFactory.decodeStream(assets.open("facingN.png")),
                    BitmapFactory.decodeStream(assets.open("facingE.png")),
                    BitmapFactory.decodeStream(assets.open("facingS.png")),
                    BitmapFactory.decodeStream(assets.open("facingW.png")),
                    BitmapFactory.decodeStream(assets.open("fireball.png")),
                    BitmapFactory.decodeStream(assets.open("door.png")),
		            BitmapFactory.decodeStream(assets.open("door.png")),
                    BitmapFactory.decodeStream(assets.open("cuco.png")),
                    BitmapFactory.decodeStream(assets.open("nimph.png")),
                    BitmapFactory.decodeStream(assets.open("jewel.png"))
            }, new char[] {
		            '.',
		            '#',
		            '^',
		            '>',
		            'V',
		            '<',
		            '*',
		            'B',
		            'E',
		            '@',
		            'W',
		            '&'
            });

        } catch (IOException e) {
        }


		setContentView(R.layout.layout);
	    mView = (GL2JNIView) findViewById(R.id.view);

	    findViewById(R.id.up).setOnClickListener(new View.OnClickListener() {
		    @Override
		    public void onClick(View v) {
			    GL2JNILib.moveForward();
		    }
	    });

	    findViewById(R.id.left).setOnClickListener(new View.OnClickListener() {
		    @Override
		    public void onClick(View v) {
			    GL2JNILib.turnLeft();
		    }
	    });

	    findViewById(R.id.right).setOnClickListener(new View.OnClickListener() {
		    @Override
		    public void onClick(View v) {
			    GL2JNILib.turnRight();
		    }
	    });

	    findViewById(R.id.down).setOnClickListener(new View.OnClickListener() {
		    @Override
		    public void onClick(View v) {
			    GL2JNILib.moveBackward();
		    }
	    });

	    findViewById(R.id.strafeleft).setOnClickListener(new View.OnClickListener() {
		    @Override
		    public void onClick(View v) {
			    GL2JNILib.strafeLeft();
		    }
	    });

	    findViewById(R.id.straferight).setOnClickListener(new View.OnClickListener() {
		    @Override
		    public void onClick(View v) {
			    GL2JNILib.strafeRight();
		    }
	    });

	    findViewById(R.id.fire).setOnClickListener(new View.OnClickListener() {
		    @Override
		    public void onClick(View v) {
			    GL2JNILib.fire();
		    }
	    });

    }

    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {

	    synchronized (mView.render.lock) {
		    switch (keyCode) {
			    case KeyEvent.KEYCODE_T:
				    return true;

			    case KeyEvent.KEYCODE_BUTTON_R1:
				    GL2JNILib.strafeRight();
				    return true;

			    case KeyEvent.KEYCODE_BUTTON_L1:
				    GL2JNILib.strafeLeft();
				    return true;


			    case KeyEvent.KEYCODE_BUTTON_A:
				    GL2JNILib.fire();
				    return true;

			    case KeyEvent.KEYCODE_DPAD_UP:
				    GL2JNILib.moveForward();
				    return true;

			    case KeyEvent.KEYCODE_DPAD_RIGHT:
				    GL2JNILib.turnRight();
				    return true;

			    case KeyEvent.KEYCODE_DPAD_DOWN:
				    GL2JNILib.moveBackward();
				    return true;

			    case KeyEvent.KEYCODE_DPAD_LEFT:
				    GL2JNILib.turnLeft();
				    return true;

			    case KeyEvent.KEYCODE_R:
				    GL2JNILib.reset();
				    return true;
		    }
	    }

        return super.onKeyDown(keyCode, event);
    }

    @Override
    protected void onPause() {
        super.onPause();

        running = false;
        mView.onPause();
    }

    @Override
    protected void onResume() {
        super.onResume();
        mView.onResume();

        new Thread(new Runnable() {
            @Override
            public void run() {
                running = true;
                while (running) {
                    try {
                        Thread.sleep(20);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
	                synchronized (mView.render.lock) {
                        GL2JNILib.tick();
                    }
                }
            }
        }).start();
    }

    @Override
    protected void onDestroy() {

        GL2JNILib.onDestroy();

        super.onDestroy();
    }
}
