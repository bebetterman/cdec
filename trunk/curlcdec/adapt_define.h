#pragma once

#define CURL_GLOBAL_SSL (1<<0)
#define CURL_GLOBAL_WIN32 (1<<1)
#define CURL_GLOBAL_ALL (CURL_GLOBAL_SSL|CURL_GLOBAL_WIN32)
#define CURL_GLOBAL_NOTHING 0
#define CURL_GLOBAL_DEFAULT CURL_GLOBAL_ALL

typedef size_t (*curl_write_function)(void *buffer, size_t size, size_t nmemb, void *user_p);
