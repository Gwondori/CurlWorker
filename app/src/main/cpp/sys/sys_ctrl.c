//
// Created by KwangMin Kim on 2022/06/12.
//

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <android/log.h>
#include "sys_ctrl.h"

#define  LOG_TAG    "SYS-CTRL"

#define  LOGUNK( ... )  __android_log_print(ANDROID_LOG_UNKNOWN, LOG_TAG, __VA_ARGS__)
#define  LOGDEF( ... )  __android_log_print(ANDROID_LOG_DEFAULT, LOG_TAG, __VA_ARGS__)
#define  LOGV( ... )  __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#define  LOGD( ... )  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define  LOGI( ... )  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define  LOGW( ... )  __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define  LOGE( ... )  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define  LOGF( ... )  __android_log_print(ANDROID_FATAL_ERROR, LOG_TAG, __VA_ARGS__)
#define  LOGS( ... )  __android_log_print(ANDROID_SILENT_ERROR, LOG_TAG, __VA_ARGS__)

uint8_t get_airplane_status()
{
	FILE *fp;
	char res[256] = { 0, };
	char cmd[512] = { 0, };

	sprintf( cmd, "su -c $(which settings) get global airplane_mode_on" );

	fp = popen( cmd, "r" );
	if( fp == NULL)
	{
		LOGD( "Failed to run command(%d)\n", errno );
		return 0;
	}

	fgets( res, sizeof( res ), fp );

	fclose( fp );

	return atoi( res );
}

uint8_t set_airplane_mode( const int value )
{
	if( value == 0 )
	{
		// Off
		// settings put global airplane_mode_on 0
		// am broadcast -a android.intent.action.AIRPLANE_MODE --ez state false
		system( "su -c $(which settings) put global airplane_mode_on 0" );
		system( "su -c $(which am) broadcast -a android.intent.action.AIRPLANE_MODE --ez state false" );
	}
	else if( value == 1 )
	{
		// On
		// settings put global airplane_mode_on 1
		// am broadcast -a android.intent.action.AIRPLANE_MODE --ez state true
		system( "su -c $(which settings) put global airplane_mode_on 1" );
		system( "su -c $(which am) broadcast -a android.intent.action.AIRPLANE_MODE --ez state true" );
	}
	else
	{
		/*Nothing*/
	}

	sleep( 2 );

	return get_airplane_status();
}
