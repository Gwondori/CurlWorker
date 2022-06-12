package kr.minjinrich.libcurl.modules;

import android.content.Context;
import android.content.res.AssetManager;
import android.util.Log;

import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.io.OutputStream;

public class Curl
{
	private static final String CA_BUNDLE_NAME = "cacert.pem";

	public native static boolean curl( String pathCACert, String url, int ua_id, String[] opts );
	public native static String getCurlVersion();

	// Copy CA Cert in assets folder to internal storage.
	public static void onCopyCACertToInternalStor( Context context )
	{
		AssetManager assetManager = context.getApplicationContext().getAssets();
		InputStream in;
		OutputStream out;

		try
		{
			File caFile = new File( context.getApplicationContext().getFilesDir(), CA_BUNDLE_NAME );
			byte[] buffer = new byte[ 1024 ];
			int read;

			in = assetManager.open( CA_BUNDLE_NAME );
			out = new FileOutputStream( caFile, false );

			while( ( read = in.read( buffer ) ) != -1 )
			{
				out.write( buffer, 0, read );
			}

			in.close();
			out.flush();
			out.close();
		}
		catch( Exception e )
		{
			Log.e( "Curl", e.getMessage() );
		}

		Log.i( "Curl", "CA bundle copied to internal storage" );
	}

	public static String getCaBundlePath( Context context )
	{
		return new File( context.getApplicationContext().getFilesDir(), CA_BUNDLE_NAME ).getAbsolutePath();
	}
}
