#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

struct MemoryStruct {
    char *memory;
    size_t size;
};

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    char *ptr = realloc(mem->memory, mem->size + realsize + 1);
    if(ptr == NULL) {
        // out of memory!
        printf("not enough memory (realloc returned NULL)\n");
        return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

int main(void) {
    CURL *curl_handle;
    CURLcode res;

    struct MemoryStruct chunk;

    chunk.memory = malloc(1);  // inizializzazione del blocco di memoria
    chunk.size = 0;    // nessun dato ancora

    char* SEARCH_URL = "https://yt.artemislena.eu/watch?v=Df66-GLnZuI"

    curl_global_init(CURL_GLOBAL_ALL);

    curl_handle = curl_easy_init();

    curl_easy_setopt(curl_handle, CURLOPT_URL, SEARCH_URL); // Sostituisci con l'URL desiderato
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);

    res = curl_easy_perform(curl_handle);

    if(res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    } else {
        // Dividi il contenuto HTML in righe e cerca la prima riga con la corrispondenza
        char *line = strtok(chunk.memory, "\n");
        while (line != NULL) {
            if (strstr(line, "<a tabindex=\"-1\" href=") != NULL) {
                // Trova l'inizio del contenuto dell'href
                char *start = strstr(line, "<a tabindex=\"-1\" href=\"");
                if (start != NULL) {
                    start += strlen("<a tabindex=\"-1\" href=\"");
                    char *end = strchr(start, '"');
                    if (end != NULL) {
                        *end = '\0';
                        printf("Contenuto href: %s\n", start);
                    }
                }
                break;
            }
            line = strtok(NULL, "\n");
        }

        if (line == NULL) {
            printf("Nessuna linea trovata con la corrispondenza specificata.\n");
        }
    }

    curl_easy_cleanup(curl_handle);
    free(chunk.memory);
    curl_global_cleanup();

    return 0;
}

