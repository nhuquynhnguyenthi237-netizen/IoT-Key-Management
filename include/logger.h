#ifndef LOGGER_H
#define LOGGER_H

#include <stddef.h>
#include <stdint.h>

void log_message(const char *level, const char *msg);
void print_masked_key(const uint8_t *key, size_t len);

#endif