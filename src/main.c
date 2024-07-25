#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

#include "../include/scrape.h"
#include "../include/caparezza.h"
#include "../include/ui.h"

#define gotoxy(x, y) printf("\033[%d;%dH", (y), (x))

int main(int argc, char * argv[]) {
  // Check usage
  if (argc != 2) {
    printf("Usage: mouse <song_name>");
    return 0;
  }

  signal(SIGINT, SIG_IGN);

  set_noecho_mode(); 

  printf("\033[?25l");

  char * su = InputToSearchUrl(argv[1]);
  char * href = GetHrefFromSearchUrl(su);
  char * sr = GetSourceFromHtml(href);

  char * imgLink = GetImageFromHtml(sr);

  mpv_handle * mpv = mpv_create();

  mpv_set_option_string(mpv, "vo", "null");
  mpv_set_option_string(mpv, "cache", "yes");
  mpv_set_option_string(mpv, "cache-secs", "20");


  if (mpv_initialize(mpv) < 0) {
    return 1;
  }

  char command[1024]; 

  snprintf(command, sizeof(command), "wget -O - %s 2>/dev/null | chafa --view-size=50x50 > temp", imgLink);

  system(command);

  start_audio(mpv, sr);
  sleep(3);

  system("clear");

  gotoxy(0, 1);

  system("cat temp");

  InputToSongName(argv[1]);

  int paused = 0;

  while (1) {
    char * t = get_audio_time(mpv);

    gotoxy(55, 4);
    printf(" %s - \033[1m%s\033[0m ", argv[1], t);
    gotoxy(55, 9);
    printf("\033[1;30m (q)uit | (p)ause\033[0m");

    if (kbhit()) {
      int ch = getchar(); 

      if (ch == 'q') {
        system("clear");
        break; 
      } else if (ch == 'p') {
        paused ? play_audio(mpv) : pause_audio(mpv);
        paused = !paused;
      }
    }
    usleep(100000); 
  }

  printf("\033[?25h");

  system("rm temp");
  set_echo_mode();
  return 0;
}
