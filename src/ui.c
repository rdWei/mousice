#include "../include/ui.h"

#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

void set_noecho_mode() {
  struct termios tty;
  tcgetattr(STDIN_FILENO, & tty);
  tty.c_lflag &= ~(ECHO); // Disabilita l'eco dei caratteri
  tcsetattr(STDIN_FILENO, TCSANOW, & tty);
}

void set_echo_mode() {
  struct termios tty;
  tcgetattr(STDIN_FILENO, & tty); // Ottieni le attuali impostazioni del terminale
  tty.c_lflag |= ECHO; // Abilita l'eco dei caratteri
  tcsetattr(STDIN_FILENO, TCSANOW, & tty); // Applica le modifiche immediatamente
}

void reset_terminal_mode() {
  struct termios tty;
  tcgetattr(STDIN_FILENO, & tty);
  tty.c_lflag |= ECHO; // Riabilita l'eco dei caratteri
  tcsetattr(STDIN_FILENO, TCSANOW, & tty);
}

int kbhit(void) {
  struct termios oldt, newt;
  int ch;
  int oldf;
  tcgetattr(STDIN_FILENO, & oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON); // Disabilita la modalità canonica
  tcsetattr(STDIN_FILENO, TCSANOW, & newt);
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
  ch = getchar();
  tcsetattr(STDIN_FILENO, TCSANOW, & oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);
  if (ch != EOF) {
    ungetc(ch, stdin);
    return 1;
  }
  return 0;
}
