#include "../include/scrape.h"

#include <stdio.h>
#include <string.h>

char * InputToSearchUrl(const char input[]) {
  size_t input_length = strlen(input);
  size_t base_url_length = strlen(INVIDIUS_URL);
  size_t additional_params_length = strlen("&page=1&date=none&type=video&duration=none&sort=relevance");
  size_t url_length = base_url_length + input_length + additional_params_length + 1; // +1 per '\0'

  char * url = (char * ) malloc(url_length);
  if (url == NULL) {
    return NULL;
  }

  strcpy(url, INVIDIUS_URL);

  char * ptr = url + base_url_length;
  for (size_t x = 0; x < input_length; x++) {
    if (input[x] == '_') {
      * ptr++ = '+';
    } else {
      * ptr++ = input[x];
    }
  }

  strcpy(ptr, "&page=1&date=none&type=video&duration=none&sort=relevance");

  return url;
}

void InputToSongName(char input[]) {
  size_t input_length = strlen(input);

  for (int x = 0; x < input_length; x++) {
    if (input[x] == '_') {
      input[x] = ' ';
    }
  }
}

char * GetHrefFromSearchUrl(const char * searchUrl) {
  CURL * curl_handle;
  CURLcode res;

  struct MemoryStruct chunk;
  chunk.memory = malloc(1);
  chunk.size = 0;

  curl_global_init(CURL_GLOBAL_ALL);

  curl_handle = curl_easy_init();
  curl_easy_setopt(curl_handle, CURLOPT_URL, searchUrl);
  curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
  curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void * ) & chunk);

  res = curl_easy_perform(curl_handle);

  char * url = malloc(256);

  if (res != CURLE_OK) {
    fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
  } else {
    char * line = strtok(chunk.memory, "\n");
    while (line != NULL) {
      if (strstr(line, "<a tabindex=\"-1\" href=") != NULL) {
        char * start = strstr(line, "<a tabindex=\"-1\" href=\"");
        if (start != NULL) {
          start += strlen("<a tabindex=\"-1\" href=\"");
          char * end = strchr(start, '"');
          if (end != NULL) {
            * end = '\0';
            strcpy(url, start);
          }
        }
        break;
      }
      line = strtok(NULL, "\n");
    }

    if (line == NULL) {
      strcpy(url, "https://gofuckyourself.com");
    }
  }

  curl_easy_cleanup(curl_handle);
  free(chunk.memory);
  curl_global_cleanup();
  return url;
}

char * GetSourceFromHtml(const char * url) {
  CURL * curl_handle;
  CURLcode res;

  char formattedUrl[60];

  sprintf(formattedUrl, "%s%s", INVIDIUS_VIDEO_URL, url);

  struct MemoryStruct chunk;
  chunk.memory = malloc(1);
  chunk.size = 0;

  curl_global_init(CURL_GLOBAL_ALL);

  curl_handle = curl_easy_init();
  curl_easy_setopt(curl_handle, CURLOPT_URL, formattedUrl);
  curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
  curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void * ) & chunk);

  res = curl_easy_perform(curl_handle);

  char * src = malloc(256);

  if (res != CURLE_OK) {
    fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
  } else {
    char * line = strtok(chunk.memory, "\n");
    while (line != NULL) {
      if (strstr(line, "<source src=") != NULL) {
        char * start = strstr(line, "<source src=\"");
        if (start != NULL) {
          start += strlen("<source src=\"");
          char * end = strchr(start, '"');
          if (end != NULL) {
            * end = '\0';
            strcpy(src, start);
          }
        }
        break;
      }
      line = strtok(NULL, "\n");
    }

    if (line == NULL) {
      strcpy(src, "Not Found");
    }
  }

  curl_easy_cleanup(curl_handle);
  free(chunk.memory);
  curl_global_cleanup();

  char * result = malloc(200);

  sprintf(result, "%s%s&local=true", INVIDIUS_VIDEO_URL, src);

  return result;
}

char * GetImageFromHtml(const char * url) {
  char * start = strchr(url, '='); // Trova il primo '='

  if (start != NULL) {
    // Avanza il puntatore per ignorare '='
    start++;

    // Trova la fine della sottostringa
    char * end = start + strcspn(start, " ");

    // Calcola la lunghezza della sottostringa
    size_t length = end - start - 19;

    // Alloca memoria dinamica per il codice
    char * code = (char * ) malloc(length + 1); // +1 per il terminatore null

    if (code != NULL) {
      // Copia il codice nella variabile
      strncpy(code, start, length);
      code[length] = '\0'; // Aggiungi il terminatore null

      char * final = malloc(200);

      sprintf(final, "https://yt.artemislena.eu/vi/%s/maxres.jpg", code);

      return final;
    } else {
      // Gestisci l'errore di allocazione
      fprintf(stderr, "Errore di allocazione memoria.\n");
      return NULL;
    }
  }

  // Gestisci il caso in cui non è stato trovato '='
  return NULL;
}

static size_t WriteMemoryCallback(void * contents, size_t size, size_t nmemb, void * userp) {
  size_t realsize = size * nmemb;
  struct MemoryStruct * mem = (struct MemoryStruct * ) userp;

  char * ptr = realloc(mem -> memory, mem -> size + realsize + 1);
  if (ptr == NULL) {
    printf("not enough memory (realloc returned NULL)\n");
    return 0;
  }

  mem -> memory = ptr;
  memcpy( & (mem -> memory[mem -> size]), contents, realsize);
  mem -> size += realsize;
  mem -> memory[mem -> size] = 0;

  return realsize;
}
