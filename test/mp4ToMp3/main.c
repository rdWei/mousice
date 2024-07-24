#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input_file.mp4> <output_file.mp3>\n", argv[0]);
        return 1;
    }

    const char *ffmpeg_path = "/usr/local/bin/ffmpeg";  // Modifica questo percorso se necessario

    execl(ffmpeg_path, "ffmpeg", "-i", argv[1], "-q:a", "0", "-map", "a", argv[2], (char *)NULL);

    // Se execl ritorna, c'è stato un errore
    perror("execl failed");
    return 1;
}

