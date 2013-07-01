#include "stdafx.h"
#ifdef _WIN32
#include "curl-7.26.0/curl.h"
#else
#include <curl/curl.h>
#endif
#include "adapt_define.h"
#include "adapt_easy.h"

int curl_adapt_global_init(long flags) { return curl_global_init(flags); }

void curl_adapt_global_cleanup() { curl_global_cleanup(); }

void* curl_adapt_easy_init() { return curl_easy_init(); }

void curl_adapt_easy_cleanup(void* curl) { curl_easy_cleanup(curl); }

int curl_adapt_easy_seturl(void* curl, const char* url) { return curl_easy_setopt(curl, CURLOPT_URL, url); }

int curl_adapt_easy_setwritefunction(void* curl, curl_write_function fp) { return curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, fp); }

int curl_adapt_easy_setwritedata(void* curl, void* p) { return curl_easy_setopt(curl, CURLOPT_WRITEDATA, p); }

int curl_adapt_easy_perform(void* curl) { return curl_easy_perform(curl); }

int curl_adapt_easy_getresponsecode(void* curl, long* responseCode) { return curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, responseCode); }
