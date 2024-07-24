#ifndef SCRAPE_H
#define SCRAPE_H

#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

#define INVIDIUS_URL "https://yewtu.be/search?q="
#define INVIDIUS_VIDEO_URL "https://yt.artemislena.eu"

struct MemoryStruct {
    char *memory;
    size_t size;
};

char* InputToSearchUrl(const char input[]);
void InputToSongName(char input[]);
char* GetHrefFromSearchUrl(const char* searchUrl);
char* GetSourceFromHtml(const char* href);
char* GetImageFromHtml(const char* href);

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);

#endif // SCRAPE_H

