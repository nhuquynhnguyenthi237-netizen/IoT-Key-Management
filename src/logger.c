#include "logger.h"
#include <stdio.h>
#include <time.h>

void writeLog(int device_id, const char *action) {
    FILE *fp = fopen("logs.txt", "a");
    if (fp) {
        time_t now = time(NULL);
        struct tm *t = localtime(&now);
        fprintf(fp, "[%04d-%02d-%02d %02d:%02d:%02d] Device %d: %s\n",
                t->tm_year+1900, t->tm_mon+1, t->tm_mday,
                t->tm_hour, t->tm_min, t->tm_sec,
                device_id, action);
        fclose(fp);
    }
}