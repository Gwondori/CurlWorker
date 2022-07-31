package kr.minjinrich.libcurl.modules;

import java.io.File;

public class SysUtils
{
	public native static int getAirplaneStatus();
	public native static void setAirplaneStatus(int value);

	/**
	 * Checks if the device is rooted.
	 * @return true if the device is rooted, false otherwise.
	 */
	public static boolean isRooted()
	{
		String buildTags = android.os.Build.TAGS;

		if( buildTags != null && buildTags.contains( "test-keys" ) )
		{
			return true;
		}

		try
		{
			File file = new File( "/system/app/Superuser.apk" );

			if( file.exists() )
			{
				return true;
			}
		}
		catch( Exception e1 )
		{
			// ignore
		}

		return canExecuteCommand( "/system/xbin/which su" ) || canExecuteCommand( "/system/bin/which su" ) || canExecuteCommand( "which su" );
	}

	private static boolean canExecuteCommand( String command )
	{
		boolean executedSuccesfully;

		try
		{
			Runtime.getRuntime().exec( command );
			executedSuccesfully = true;
		}
		catch( Exception e )
		{
			executedSuccesfully = false;
		}

		return executedSuccesfully;
	}
}
