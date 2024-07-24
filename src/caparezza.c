#include "../include/caparezza.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void ConvMp4(const char * mp4_path) {
  char formatted_name[256];
  snprintf(formatted_name, sizeof(formatted_name), "%s_to_mp3.mp3", mp4_path);

  execl(FFMPEG_PATH, "ffmpeg", "-i", mp4_path, "-q:a", "0", "-map", "a", formatted_name, (char * ) NULL);

  perror("execl failed");
  exit(EXIT_FAILURE);
}

void start_audio(mpv_handle * mpv,
  const char * filename) {
  mpv_command(mpv, (const char * []) {
    "loadfile",
    filename,
    "replace",
    NULL
  });
}

void pause_audio(mpv_handle * mpv) {
  mpv_set_property_string(mpv, "pause", "yes");
}

void play_audio(mpv_handle * mpv) {
  mpv_set_property_string(mpv, "pause", "no");
}

// Funzione per ottenere il tempo di riproduzione corrente come stringa
char * get_audio_time(mpv_handle * mpv) {
  // Tempo di riproduzione corrente in secondi
  double time;
  // Buffer per la stringa che conterrà il tempo formattato
  char * time_str = NULL;
  char buf[64];

  // Ottieni il tempo di riproduzione corrente
  if (mpv_get_property(mpv, "time-pos", MPV_FORMAT_DOUBLE, & time) < 0) {
    return NULL;
  }

  // Format the time as "MM:SS" (or "HH:MM:SS" if you prefer)
  int minutes = (int)(time / 60);
  int seconds = (int)(time) % 60;

  snprintf(buf, sizeof(buf), "%02d:%02d", minutes, seconds);

  // Allocazione dinamica della stringa di output
  time_str = strdup(buf);
  if (!time_str) {
    return NULL;
  }

  return time_str;
}
