#include <stdio.h>
#include <stdlib.h>
#include <mpv/client.h>

int main() {
    mpv_handle *mpv = mpv_create();
    if (!mpv) {
        fprintf(stderr, "Failed to create MPV context.\n");
        return 1;
    }

    mpv_set_option_string(mpv, "vo", "null");

    mpv_set_option_string(mpv, "audio-file", "test.mp4");

    if (mpv_initialize(mpv) < 0) {
        fprintf(stderr, "Failed to initialize MPV.\n");
        mpv_destroy(mpv);
        return 1;
    }

    mpv_command(mpv, (const char *[]){"loadfile", "https://yt.artemislena.eu/latest_version?id=sSprv3lZ-vs&itag=18&local=true", NULL});

    while (1) {
        mpv_event *event = mpv_wait_event(mpv, 1000.0);
        if (event->event_id == MPV_EVENT_SHUTDOWN) {
            break;
        }
    }

    mpv_terminate_destroy(mpv);

    return 0;
}

