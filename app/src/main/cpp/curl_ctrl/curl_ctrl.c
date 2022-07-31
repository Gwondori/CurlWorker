#include <android/log.h>
#include <curl/curl.h>
#include "curl_ctrl.h"

#define  LOG_TAG    "CURL-CTRL"

#define  LOGUNK( ... )  __android_log_print(ANDROID_LOG_UNKNOWN,LOG_TAG,__VA_ARGS__)
#define  LOGDEF( ... )  __android_log_print(ANDROID_LOG_DEFAULT,LOG_TAG,__VA_ARGS__)
#define  LOGV( ... )  __android_log_print(ANDROID_LOG_VERBOSE,LOG_TAG,__VA_ARGS__)
#define  LOGD( ... )  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define  LOGI( ... )  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGW( ... )  __android_log_print(ANDROID_LOG_WARN,LOG_TAG,__VA_ARGS__)
#define  LOGE( ... )  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#define  LOGF( ... )  __android_log_print(ANDROID_FATAL_ERROR,LOG_TAG,__VA_ARGS__)
#define  LOGS( ... )  __android_log_print(ANDROID_SILENT_ERROR,LOG_TAG,__VA_ARGS__)

char *user_agent_pc[10] = {
		"Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:98.0) Gecko/20100101 Firefox/98.0",
		"Mozilla/5.0 (Macintosh; Intel Mac OS X 12.3; rv:99.0) Gecko/20100101 Firefox/99.0",
		"Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:99.0) Gecko/20100101 Firefox/99.0",
		"Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/100.0.4896.127 Safari/537.36",
		"Mozilla/5.0 (Macintosh; Intel Mac OS X 12_3_1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/100.0.4896.127 Safari/537.36",
		"Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/100.0.4896.127 Safari/537.36",
		"Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/100.0.4896.127 Safari/537.36 Edg/99.0.1150.36",
		"Mozilla/5.0 (Macintosh; Intel Mac OS X 12_3_1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/100.0.4896.127 Safari/537.36 Edg/99.0.1150.36",
		"Mozilla/5.0 (Macintosh; Intel Mac OS X 12_3_1) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/15.3 Safari/605.1.15",
		"Mozilla/5.0 (Windows NT 10.0) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/100.0.4896.127 Safari/537.36",
};

// Note: index 0 - SJ 요구사항, 이외 나머지 출저는 이 링크(https://developers.whatismybrowser.com/useragents/explore/software_type_specific/web-browser)에서 가져옴
char *user_agent_mobile[10] = {
		"Mozilla/5.0 (Linux; Android 10; SAMSUNG SM-G965N) AppleWebKit/537.36 (KHTML, like Gecko) SamsungBrowser/17.0 Chrome/96.0.4664.104 Mobile Safari/537.36",
		"Mozilla/5.0 (Linux; Android 11; SM-G781N) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/96.0.4664.92 Mobile Safari/537.36",
		"Mozilla/5.0 (Linux; Android 11; SM-G991N) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/96.0.4664.104 Mobile Safari/537.36",
		"Mozilla/5.0 (Linux; Android 10; SAMSUNG SM-G980N) AppleWebKit/537.36 (KHTML, like Gecko) SamsungBrowser/11.0 Chrome/75.0.3770.143 Mobile Safari/537.36",
		"Mozilla/5.0 (Linux; Android 11; SAMSUNG SM-G977N) AppleWebKit/537.36 (KHTML, like Gecko) SamsungBrowser/16.0 Chrome/92.0.4515.166 Mobile Safari/537.36",
		"Mozilla/5.0 (iPhone; CPU iPhone OS 15_2 like Mac OS X) AppleWebKit/605.1.15 (KHTML, like Gecko) CriOS/96.0.4664.101 Mobile/15E148 Safari/604.1",
		"Mozilla/5.0 (iPhone; CPU iPhone OS 15_2 like Mac OS X) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/15.1 Mobile/15E148 Safari/604.1",
		"Mozilla/5.0 (iPhone; CPU iPhone OS 15_1 like Mac OS X) AppleWebKit/605.1.15 (KHTML, like Gecko) CriOS/96.0.4664.101 Mobile/15E148 Safari/604.1",
		"Mozilla/5.0 (iPhone; CPU iPhone OS 14_8 like Mac OS X) AppleWebKit/605.1.15 (KHTML, like Gecko) CriOS/96.0.4664.101 Mobile/15E148 Safari/604.1",
		"Mozilla/5.0 (iPhone; CPU iPhone OS 14_7 like Mac OS X) AppleWebKit/605.1.15 (KHTML, like Gecko) CriOS/96.0.4664.101 Mobile/15E148 Safari/604.1",
};

// Mode selector: Mobile or PC
int is_mobile = 1;

uint8_t curl_a_website( const char *url, const int ua_id )
{
	CURL *curl = curl_easy_init();

	if( NULL == curl )
	{
		LOGE( "Fail to initialize CURL\n" );
		return 0;
	}

	LOGD( "User Agent: [%s], URL [%s]\n", user_agent_pc[ ua_id ], url );

	curl_easy_setopt( curl, CURLOPT_URL, url );
//#ifdef __ANDROID__
	// For https requests, you need to specify the ca-bundle path
	curl_easy_setopt( curl, CURLOPT_CAINFO, CA_BUNDLE_PATH );
	curl_easy_setopt( curl, CURLOPT_SSLVERSION, CURL_SSLVERSION_TLSv1 );

	if( 1 == is_mobile )
	{
		curl_easy_setopt( curl, CURLOPT_USERAGENT, user_agent_mobile[ ua_id ] );
	}
	else
	{
		curl_easy_setopt( curl, CURLOPT_USERAGENT, user_agent_pc[ ua_id ] );
	}
//#endif

	uint8_t success = 0;
	CURLcode response = curl_easy_perform( curl );

	if( response == CURLE_OK )
	{
		success = 1;
	}
	else
	{
		LOGD( "Error Mesg [%s]\n", curl_easy_strerror( response ));
	}

	curl_easy_cleanup( curl );

	return success;
}

const char *get_curl_version()
{
	curl_version_info_data *data = curl_version_info( CURLVERSION_NOW );
	return data->version;
}
