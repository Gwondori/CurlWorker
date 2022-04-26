#include <jni.h>
#include <android/log.h>
#include "./curl_ctrl/curl_ctrl.h"

char *CA_BUNDLE_PATH;

JNIEXPORT jboolean JNICALL
Java_kr_minjinrich_libcurl_modules_Curl_curl( JNIEnv *env, jclass clazz, jstring path_cacert, jstring url, jint ua_id, jobjectArray opts ) {
	// TODO: implement curl()
	CA_BUNDLE_PATH = (char *) (*env)->GetStringUTFChars(env, path_cacert, NULL);
	const char *c_url = (*env)->GetStringUTFChars(env, url, NULL);

	uint8_t success = curl_a_website(c_url, ua_id);

	return success;
}

JNIEXPORT jstring JNICALL
Java_kr_minjinrich_libcurl_modules_Curl_getCurlVersion( JNIEnv *env, jclass clazz ) {
	// TODO: implement getCurlVersion()
	jstring temp = (*env)->NewStringUTF(env, get_curl_version());

	return temp;
}