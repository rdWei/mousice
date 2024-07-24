#ifndef AUDIO_UTILS_H
#define AUDIO_UTILS_H

#if defined(__FreeBSD__)
    #define FFMPEG_PATH "/usr/local/bin/ffmpeg"
#elif defined(__linux__)
    #define FFMPEG_PATH "/usr/bin/ffmpeg"
#else
    #error "Unsupported operating system"
#endif

#include <mpv/client.h>

void ConvMp4(const char *mp4_path);
void start_audio(mpv_handle *mpv, const char *filename);
void pause_audio(mpv_handle *mpv);
void play_audio(mpv_handle *mpv);
char* get_audio_time(mpv_handle *mpv);

#endif // AUDIO_UTILS_H

