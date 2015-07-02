package org.cocos2dx.cpp;

import com.mitty.herotalk.R;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.animation.AlphaAnimation;
import android.view.animation.Animation;
import android.view.animation.Animation.AnimationListener;
import android.widget.ImageView;
import android.widget.RelativeLayout;
import org.cocos2dx.cpp.AppActivity;

public class MittySplashActivity extends Activity
{
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		setContentView(R.layout.splash);
		AlphaAnimation ani = new AlphaAnimation(0, 1);
		ani.setRepeatMode(AlphaAnimation.REVERSE);
		ani.setRepeatCount(0);
		ani.setDuration(2000);
		ani.setFillAfter(true);
		ImageView image = (ImageView) findViewById(R.id.splash_img);
		if(image ==null)
		{
			RelativeLayout layout = (RelativeLayout) LayoutInflater.from(this).inflate(R.id.splash_layout, null);
			image = (ImageView) layout.getChildAt(0);
		}
		image.setAnimation(ani);
		ani.setAnimationListener(new AnimationListener() {
			
			@Override
			public void onAnimationStart(Animation animation) {
				// TODO Auto-generated method stub
				
			}
			
			@Override
			public void onAnimationRepeat(Animation animation) {
				// TODO Auto-generated method stub
				
			}
			
			@Override
			public void onAnimationEnd(Animation animation) {
				// TODO Auto-generated method stub
				MittySplashActivity.this.startGameActivity();
			}
		});
		
	}
	
	private void startGameActivity()
	{
		Intent intent = new Intent(this, AppActivity.class);
		startActivity(intent);
		finish();
	}
	
	
}
