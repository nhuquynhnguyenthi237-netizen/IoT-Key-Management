#ifndef DEVICE_H
#define DEVICE_H

#include "key_manager.h"
#include <stdint.h>

typedef struct {
    int id;
    Key key;
    int status; // 0: inactive, 1: active, 2: revoked, 3: destroyed
} Device;

void initDevice(Device *device, int id);
void printDevice(Device *device);
void printKey(const uint8_t key[]);

#endif