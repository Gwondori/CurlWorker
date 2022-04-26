#ifndef CURL_A_WEBSITE_H
#define CURL_A_WEBSITE_H

#ifdef __ANDROID__
extern char *CA_BUNDLE_PATH;
#endif

enum user_agent_t {
	WINDOWS_FIREFOX,
	MAC_FIREFOX,
	LINUX_FIREFOX,
	WINDOWS_CHROME,
	MAC_CHROME,
	LINUX_CHROME,
	WINDOWS_EDGE,
	MAC_EDGE,
	MAC_SAFARI,
	DEFAULT,
};

uint8_t curl_a_website( const char *url, const int ua_id );
const char *get_curl_version();
#endif
