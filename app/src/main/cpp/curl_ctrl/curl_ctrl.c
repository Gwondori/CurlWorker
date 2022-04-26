#include <android/log.h>
#include <curl/curl.h>
#include "curl_ctrl.h"

#define  LOG_TAG    "CURL-CTRL"

#define  LOGUNK(...)  __android_log_print(ANDROID_LOG_UNKNOWN,LOG_TAG,__VA_ARGS__)
#define  LOGDEF(...)  __android_log_print(ANDROID_LOG_DEFAULT,LOG_TAG,__VA_ARGS__)
#define  LOGV(...)  __android_log_print(ANDROID_LOG_VERBOSE,LOG_TAG,__VA_ARGS__)
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGW(...)  __android_log_print(ANDROID_LOG_WARN,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#define  LOGF(...)  __android_log_print(ANDROID_FATAL_ERROR,LOG_TAG,__VA_ARGS__)
#define  LOGS(...)  __android_log_print(ANDROID_SILENT_ERROR,LOG_TAG,__VA_ARGS__)

char *user_agent[10] = {
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

uint8_t curl_a_website( const char *url, const int ua_id )
{
	CURL *curl = curl_easy_init();

	if (NULL == curl) {
		LOGE("Fail to initialize CURL\n");
		return 0;
	}

	LOGD("User Agent: [%s], URL [%s]\n", user_agent[ua_id], url);

	curl_easy_setopt( curl, CURLOPT_URL, url );
//#ifdef __ANDROID__
	// For https requests, you need to specify the ca-bundle path
	curl_easy_setopt(curl, CURLOPT_CAINFO, CA_BUNDLE_PATH);
	curl_easy_setopt(curl, CURLOPT_SSLVERSION, CURL_SSLVERSION_TLSv1);
	curl_easy_setopt(curl, CURLOPT_USERAGENT, user_agent[ua_id]);
//#endif

	uint8_t success = 0;
	CURLcode response = curl_easy_perform( curl );

	if( response == CURLE_OK )
	{
		success = 1;
	}
	else
	{
		LOGD("Error Mesg: [%s]\n", curl_easy_strerror(response));
	}

	curl_easy_cleanup( curl );
	LOGD("Is Success? [%d]\n", success);

	return success;
}

const char *get_curl_version()
{
	curl_version_info_data *data = curl_version_info( CURLVERSION_NOW );
	return data->version;
}
