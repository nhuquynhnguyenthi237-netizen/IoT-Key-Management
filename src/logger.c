#include "logger.h"
#include <stdio.h>
#include <time.h>
#include <string.h>

static void write_log_to_file(const char *level, const char *msg) {
    FILE *fp = fopen("logs/system.log", "a");
    if (!fp) return;
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    char timestamp[20];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", tm_info);
    fprintf(fp, "[%s] [%s] %s\n", timestamp, level, msg);
    fclose(fp);
}

void log_message(const char *level, const char *msg) {
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    char timestamp[20];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", tm_info);
    printf("[%s] [%s] %s\n", timestamp, level, msg);
    write_log_to_file(level, msg);
}

void print_masked_key(const uint8_t *key, size_t len) {
    if (len >= 2) {
        printf("%02x%02x", key[0], key[1]);
        for (size_t i = 2; i < len; i++) {
            printf("**");
        }
    } else {
        printf("**");
    }
}