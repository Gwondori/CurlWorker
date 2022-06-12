package kr.minjinrich.libcurl.modules;

import java.time.Instant;
import java.time.LocalDate;
import java.time.LocalDateTime;
import java.time.ZoneId;
import java.time.format.DateTimeFormatter;

public class TimeUtil
{
	private final String format_only_date = "yyyy-MM-dd";
	private final String format_full = "yyyy-MM-dd HH:mm:ss";
	private final String pattern_only_date = "^[0-9]{4}-[0-9]{1,2}-[0-9]{1,2}$";
	private final String pattern_full = "^[0-9]{4}-[0-9]{1,2}-[0-9]{1,2} [0-9]{1,2}:[0-9]{1,2}:[0-9]{1,2}$";
	private ZoneId timezone = ZoneId.of( "Asia/Seoul" );

	public static int Default = 0;
	public static int OnlyDay = TimeUtil.Default;
	public static int Full = 1;

	public boolean isRight( String date, int type )
	{
		if( type == Default )
		{
			return date.matches( pattern_only_date );
		}
		else if( type == Full )
		{
			return date.matches( pattern_full );
		}
		else
		{
			return false;
		}
	}

	public String toString( long timestamp, int type )
	{
		LocalDateTime localDateTime = LocalDateTime.ofInstant( Instant.ofEpochMilli( timestamp ), this.timezone );
		String res = "";

		if( type == TimeUtil.Default )
		{
			res = localDateTime.format( DateTimeFormatter.ofPattern( format_only_date ) );
		}
		else if( type == TimeUtil.Full )
		{
			res = localDateTime.format( DateTimeFormatter.ofPattern( format_full ) );
		}

		return res;
	}

	public long toTimestamp( String date )
	{
		LocalDate localDate = null;
		LocalDateTime dateTime = null;

		if( this.isRight( date, TimeUtil.OnlyDay ) )
		{
			localDate = LocalDate.parse( date, DateTimeFormatter.ofPattern( format_only_date ) );

			return localDate.atStartOfDay( timezone ).toInstant().toEpochMilli();
		}
		else if( this.isRight( date, TimeUtil.Full ) )
		{
			dateTime = LocalDateTime.parse( date, DateTimeFormatter.ofPattern( format_full ) );

			return dateTime.atZone( this.timezone ).toInstant().toEpochMilli();
		}
		else
		{
			return 0;
		}
	}

	public int getDayDiff( String old_date, String new_date )
	{
		long old_timestamp = toTimestamp( old_date );
		long new_timestamp = toTimestamp( new_date );

		return getDayDiff( old_timestamp, new_timestamp );
	}

	public int getDayDiff( long old_timestamp, long new_timestamp )
	{
		long res_timestamp = new_timestamp - old_timestamp;

		if( 0 > res_timestamp )
		{
			throw new ArithmeticException( "Unexpected Argument." );
		}

		return ( int ) ( res_timestamp / ( 1000 * 60 * 60 * 24 ) );
	}

	public int getCurrentHour()
	{
		return LocalDateTime.now().getHour();
	}

	public String getCurrentDate( int dateType )
	{
		return this.toString( System.currentTimeMillis(), dateType );
	}
}
