#ifndef DEVICE_H
#define DEVICE_H

#include <stdint.h>

#define KEY_SIZE 32
#define MAX_DEVICES 10

typedef enum {
    PROVISIONED,
    ACTIVE,
    ROTATED,
    REVOKED,
    DESTROYED
} KeyState;

typedef struct {
    int id;
    uint8_t key[KEY_SIZE];
    KeyState state;
    int key_active;
} Device;

void init_device(Device *dev, int id);
void display_device(const Device *dev);

#endif