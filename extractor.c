#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
 * Tested for version 3.4.0
 * Compilation doesn't require any special flags
 */

#define START_PATTERN "https://webstatic-sea.hoyoverse.com/genshin/event/e20190909gacha-v2"
#define END_PATTERN "&game_biz=hk4e_global"

// This method is not 100% tested. Use at own caution! No refunds for segfaults
void *memmem(void *haystack, void *needle, size_t n_haystack, size_t n_needle) {
    if (n_needle > n_haystack) return NULL;
    if (n_haystack == 0 || n_needle == 0) return haystack;

    for (long i = 0; i < n_haystack - n_needle + 1; i++) {
        if (memcmp(haystack + i, needle, n_needle) == 0) return haystack + i;
    }

    return NULL;
}

int main(int argc, char **argv) {
    char input_file[4096] = {0};
    if (argc > 1) {
        snprintf(input_file, sizeof input_file, "%s", argv[1]);
    } else {
        const char *home = getenv("HOME");
        snprintf(input_file, sizeof input_file, "%s/.local/share/anime-game-launcher/game/drive_c/Program Files/Genshin Impact/GenshinImpact_Data/webCaches/Cache/Cache_Data/data_2", home);
    }

    FILE *file = fopen(input_file, "rb");
    if (file == NULL) {
        fprintf(stderr, "Failed to open file\n");
        return EXIT_FAILURE;
    }

    // Seek to end of file, to determine size
    if (0 > fseek(file, 0L, SEEK_END)) {
        fprintf(stderr, "Failed to seek file\n");
        return EXIT_FAILURE;
    }

    ssize_t bytes = ftell(file);
    if (bytes < 0) {
        fprintf(stderr, "Failed to determine file size\n");
        return EXIT_FAILURE;
    }

    // Seek back to start of file
    if (0 > fseek(file, 0L, SEEK_SET)) {
        fprintf(stderr, "Failed to seek file\n");
        return EXIT_FAILURE;
    }

    // Allocate memory and load file
    char *content = malloc(bytes + 1);
    size_t read = fread(content, 1, bytes, file);
    if (bytes != read) {
        fprintf(stderr, "Failed to read from file: Wanted %ld, received %zu bytes\n", bytes, read);
        return EXIT_FAILURE;
    }
    content[bytes] = '\0';
    fclose(file);

    printf("Read %zd bytes\n", read);

    // Extract last URL
    char *start = NULL,
         *end = content;

    char *url = NULL;

    // Find all URLs but only keep the last one
    int i = 0;
    do {
        start = memmem(end, START_PATTERN, bytes - (end - content), sizeof START_PATTERN - 1);
        if (start == NULL) break;

        end = memmem(start, END_PATTERN, bytes - (start - content), sizeof END_PATTERN - 1);
        if (end == NULL) {
            fprintf(stderr, "Didn't find end pattern!\n");
            return EXIT_FAILURE;
        }
        int n = (int) ((end - start) + sizeof END_PATTERN - 1);

        url = realloc(url, n + 1);
        memcpy(url, start, n);
        url[n] = '\0';
        i++;
    } while (start != NULL && end != NULL);

    // Print summary
    printf("Found %d link%s\n", i, i == 1 ? "" : "s");
    if (url != NULL) printf("Last URL: %s\n", url);

    // Free memory
    free(url);
    free(content);

    return EXIT_SUCCESS;
}
