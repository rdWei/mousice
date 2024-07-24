#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

void set_noecho_mode() {
    struct termios tty;
    tcgetattr(STDIN_FILENO, &tty);
    tty.c_lflag &= ~(ECHO);  // Disabilita l'eco dei caratteri
    tcsetattr(STDIN_FILENO, TCSANOW, &tty);
}

void reset_terminal_mode() {
    struct termios tty;
    tcgetattr(STDIN_FILENO, &tty);
    tty.c_lflag |= ECHO;  // Riabilita l'eco dei caratteri
    tcsetattr(STDIN_FILENO, TCSANOW, &tty);
}

void hide_cursor() {
    // Sequenza ANSI per nascondere il cursore
    printf("\033[?25l");
    fflush(stdout);
}

void show_cursor() {
    // Sequenza ANSI per mostrare il cursore
    printf("\033[?25h");
    fflush(stdout);
}

int kbhit(void) {
    struct termios oldt, newt;
    int ch;
    int oldf;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON);  // Disabilita la modalità canonica
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    if(ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }
    return 0;
}

int main() {
    hide_cursor();  // Nasconde il cursore
    set_noecho_mode();  // Disabilita l'eco dei caratteri
    printf("Premi 'q' per uscire...\n");

    while (1) {
        if (kbhit()) {
            int ch = getchar();  // Legge il carattere premuto

            if (ch == 'q') {
                printf("\nUscita...\n");
                break;  // Esce dal ciclo se 'q' è premuto
            }
        }
        usleep(100000);  // Attende 0.1 secondi prima di controllare di nuovo
    }

    reset_terminal_mode();  // Riabilita l'eco dei caratteri
    show_cursor();  // Mostra nuovamente il cursore
    return 0;
}

