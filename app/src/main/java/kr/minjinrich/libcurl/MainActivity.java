package kr.minjinrich.libcurl;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.widget.Button;
import android.widget.TextView;

import java.util.Random;

import kr.minjinrich.libcurl.data.TargetURLs;
import kr.minjinrich.libcurl.modules.Curl;
import kr.minjinrich.libcurl.modules.TimeUtil;

// IP 변경(try airplain mode, can rooting)에 대한 방안
// 구조(휴대폰에서 쏘나 PC에서 쏘나 다를 건 없다.) 이대로 가도 됨.
public class MainActivity extends AppCompatActivity
{
	CurlProc curlProcRunnable = new CurlProc();
	Thread curlProcThread = new Thread( curlProcRunnable );
	TargetURLs targetURLs = new TargetURLs();
	TimeUtil timeUtil = new TimeUtil();

	private String pathCACert = null;
	private int max_count = 2;

	private Button buttonStart = null;
	private boolean isStart = false;

	@Override
	protected void onCreate( Bundle savedInstanceState )
	{
		super.onCreate( savedInstanceState );
		setContentView( R.layout.activity_main );

		Log.d( "MainActivity", "Current Time [" + timeUtil.getCurrentDate( TimeUtil.Full ) + "]" );

		System.loadLibrary( "curl_ctrl" );

		Curl.onCopyCACertToInternalStor( this );

		pathCACert = Curl.getCaBundlePath( this );

		buttonStart = this.findViewById( R.id.buttonStart );
		buttonStart.setOnClickListener( view ->
		{
			if( !isStart )
			{
				onStartCURLProc();
			}
			else
			{
				onStopCURLProc();
			}

			isStart = !isStart;
		} );
	}

	private void onStartCURLProc()
	{
		curlProcThread = new Thread( curlProcRunnable );
		curlProcThread.start();
	}

	private void onStopCURLProc()
	{
		curlProcThread.interrupt();
	}

	private void setTextStatus( boolean is_start )
	{
		TextView textViewStatus = this.findViewById( R.id.textViewStatus );

		if( null != textViewStatus )
		{
			textViewStatus.setText( is_start ? "Started" : "Stopped" );
		}
	}

	private void setTextStartTime()
	{
		TextView textViewStartTime = this.findViewById( R.id.textViewStartTime );

		if( null != textViewStartTime )
		{
			String timeStr = timeUtil.getCurrentDate( TimeUtil.Full );

			textViewStartTime.setText( timeStr );
		}
	}

	private void setTextStopTime()
	{
		TextView textViewStopTime = this.findViewById( R.id.textViewStopTime );

		if( null != textViewStopTime )
		{
			String timeStr = timeUtil.getCurrentDate( TimeUtil.Full );

			textViewStopTime.setText( timeStr );
		}
	}

	private void clearTextStopTime()
	{
		TextView textViewStopTime = this.findViewById( R.id.textViewStopTime );

		if( null != textViewStopTime )
		{
			textViewStopTime.setText( "" );
		}
	}

	private void setTextLastStatus( String textStatus )
	{
		TextView textViewLastStatus = this.findViewById( R.id.textViewLastStatus );

		if( null != textViewLastStatus )
		{
			textViewLastStatus.setText( textStatus );
		}
	}

	class CurlProc implements Runnable
	{
		private int p_current_hour = 0;
		private int n_current_hour = 0;
		private int current_cnt = 0;

		@Override
		public void run()
		{
			p_current_hour = 0;
			current_cnt = 0;

			runOnUiThread( () ->
			{
				buttonStart.setText( getResources().getString( R.string.buttonStop ) );
				setTextStartTime();
				clearTextStopTime();
				setTextStatus( true );
			} );

			try
			{
				while( !Thread.currentThread().isInterrupted() )
				{
					n_current_hour = timeUtil.getCurrentHour();

					Log.d( "CurlProcRunnable", "Current Hour [" + n_current_hour + "] / Before Hour [" + p_current_hour + "]" );

					if( 0 < ( n_current_hour - p_current_hour ) )
					{
						long seed = System.currentTimeMillis();
						Random rand = new Random( seed );

						//boolean res = Curl.curl( pathCACert, "https://www.naver.com", 5, null );
						for( int i = 0; i < max_count; i++ )
						{
							String url = targetURLs.getURLByIndex( current_cnt );
							int rand_time = ( rand.nextInt( 1500 - 600 ) + 600 ) * 1000;
							boolean res = Curl.curl( pathCACert, url, 5, null );

							runOnUiThread( () -> setTextLastStatus( Boolean.toString( res ) ) );

							Log.d( "CurlProcRunnable", "Sleep Random Time [" + rand_time + "] / Url [" + url + "] / Result [" + res + "]" );
							if( ( max_count - 1 ) > i )
							{
								Thread.sleep( rand_time );
							}

							current_cnt += 1;
						}

						p_current_hour = n_current_hour;
					}

					Thread.sleep( 10000 );
				}
			}
			catch( InterruptedException e )
			{
				Log.d( "CurlProcRunnable", "Interrupted!" );
			}
			finally
			{
				runOnUiThread( () ->
				{
					buttonStart.setText( getResources().getString( R.string.buttonStart ) );
					setTextStopTime();
					setTextStatus( false );
				} );
			}
		}
	}
}