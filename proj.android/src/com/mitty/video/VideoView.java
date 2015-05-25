package com.mitty.video;

import java.io.IOException;

import android.app.Activity;
import android.content.res.AssetFileDescriptor;
import android.media.MediaPlayer;
import android.net.Uri;
import android.util.Log;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;

public class VideoView extends SurfaceView implements 
			SurfaceHolder.Callback, 
			View.OnTouchListener, 
			MediaPlayer.OnPreparedListener, 
			MediaPlayer.OnErrorListener, 
			MediaPlayer.OnInfoListener,
			MediaPlayer.OnCompletionListener {
	private static final String TAG = "VideoView";
	
	private MediaPlayer mPlayer; // MediaPlayer����
	private Activity gameActivity;
	private Uri resUri;
	private AssetFileDescriptor fd;
	private boolean surfaceCreated;
	private OnFinishListener onFinishListener;
	

	public VideoView(Activity context) {
		super(context);

		this.gameActivity = context;

		final SurfaceHolder holder = getHolder();
		holder.addCallback(this); // ���ûص��ӿ�
		holder.setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS); // ����ΪBuffer���ͣ�������Ƶ&CameraԤ����
		setOnTouchListener(this);

		mPlayer = new MediaPlayer();
//		mPlayer.setDisplay(getHolder()); //��ʱholder��δ�����������ڴ˴�����
		mPlayer.setScreenOnWhilePlaying(true);

		mPlayer.setOnPreparedListener(this);
		mPlayer.setOnCompletionListener(this);
		mPlayer.setOnErrorListener(this);
		mPlayer.setOnInfoListener(this);
	}
	
	public VideoView setOnFinishListener(OnFinishListener onFinishListener) {
		this.onFinishListener = onFinishListener;
		
		return this;
	}

	public void setVideo(Uri resUri) {
		this.resUri = resUri;

		try {
			mPlayer.setDataSource(gameActivity, resUri);
		} catch (Exception e) {
		}
	}
	
	public void setVideo(AssetFileDescriptor fd) {
		this.fd = fd;

		try {
			//ֱ�ӵ��� fd.getFileDescriptor() �ǲ��е�
			mPlayer.setDataSource(fd.getFileDescriptor(), fd.getStartOffset(), fd.getLength());
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	@Override
	public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
	}

	@Override
	public void surfaceCreated(final SurfaceHolder holder) {
		Log.i(TAG, "surfaceCreated");

		surfaceCreated = true;

		mPlayer.setDisplay(holder); // ָ��SurfaceHolder
		try {
			mPlayer.prepare();
		} catch (Exception e1) {
		}
	}

	@Override
	public void surfaceDestroyed(SurfaceHolder holder) {
		Log.i(TAG, "surfaceDestroyed");
		surfaceCreated = false;
		
		if(mPlayer != null){
			mPlayer.stop();
			mPlayer.reset();
		}
	}

	@Override
	public void onPrepared(MediaPlayer player) {
		Log.i(TAG, "onPrepared");

		int wWidth = getWidth();
		int wHeight = getHeight();

		/* �����Ƶ�? */
		int vWidth = mPlayer.getVideoWidth();
		int vHeight = mPlayer.getVideoHeight();

		/* ������Ļ */
		float wRatio = (float) vWidth / (float) wWidth; // ��ȱ�
		float hRatio = (float) vHeight / (float) wHeight; // �߶ȱ�
		float ratio = Math.max(wRatio, hRatio); // �ϴ�ı�
		vWidth = (int) Math.ceil((float) vWidth / ratio); // ����Ƶ���
		vHeight = (int) Math.ceil((float) vHeight / ratio); // ����Ƶ�߶�

		// �ı�SurfaceHolder��С
		getHolder().setFixedSize(vWidth, vHeight);
		mPlayer.seekTo(posttion);
		mPlayer.start();
	}
	
	private void dispose() {
		mPlayer.release();
		mPlayer = null;
		resUri = null;
		if (fd != null) {
			try {
				fd.close();
			} catch (IOException e) {
				e.printStackTrace();
			}
			fd = null;
		}
	}

	@Override
	public void onCompletion(MediaPlayer mp) {
		Log.i(TAG, "onCompletion");

		dispose();
		
		if(onFinishListener != null)
			onFinishListener.onVideoFinish();
	}

	@Override
	public boolean onInfo(MediaPlayer mp, int what, int extra) {
		return true;
	}

	@Override
	public boolean onError(MediaPlayer mp, int what, int extra) {
		return true;
	}

	@Override
	public boolean onTouch(View v, MotionEvent event) {
		if (event.getAction() == MotionEvent.ACTION_DOWN) {
			stop();
		}

		return true;
	}

	public void stop() {
		mPlayer.stop(); // ��Ų������ MediaPlayer.onCompletion
		dispose();
		if(onFinishListener != null)
			onFinishListener.onVideoFinish();
	}
	
	int posttion;
	public void pause() {
		posttion = mPlayer.getCurrentPosition();
		mPlayer.pause();
	}

	/**
	 * ��ͣ��ʱ��ϵͳ����� SurfaceView ��������resume��ʱ���������������MediaPlayer
	 */
	public void resume() {
		if(surfaceCreated){
			mPlayer.start();
		}else {
			try {
				if(resUri != null)
					mPlayer.setDataSource(gameActivity, resUri);
				else if (fd != null) {
					mPlayer.setDataSource(fd.getFileDescriptor(), fd.getStartOffset(), fd.getLength());
				}
			} catch (Exception e) {
			}
		}
	}
	
	public interface OnFinishListener {
		public void onVideoFinish();
	}
}
