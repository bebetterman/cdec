#pragma once

int curl_adapt_global_init(long flags);

void curl_adapt_global_cleanup();

void* curl_adapt_easy_init();

void curl_adapt_easy_cleanup(void* curl);

int curl_adapt_easy_seturl(void* curl, const char* url);

int curl_adapt_easy_setwritefunction(void* curl, curl_write_function fp);

int curl_adapt_easy_setwritedata(void* curl, void* p);

int curl_adapt_easy_perform(void* curl);

int curl_adapt_easy_getresponsecode(void* curl, long* responseCode);
