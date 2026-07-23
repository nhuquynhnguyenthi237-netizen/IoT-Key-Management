#include "device.h"
#include "logger.h"
#include <stdio.h>
#include <string.h>

void init_device(Device *dev, int id) {
    dev->id = id;
    memset(dev->key, 0, KEY_SIZE);
    dev->state = PROVISIONED;
    dev->key_active = 1;
}

void display_device(const Device *dev) {
    if (!dev) return;
    printf("Device ID: %d\n", dev->id);
    printf("Key: ");
    print_masked_key(dev->key, KEY_SIZE);
    printf("\nStatus: ");
    switch (dev->state) {
        case PROVISIONED: printf("Provisioned"); break;
        case ACTIVE:      printf("Active"); break;
        case ROTATED:     printf("Rotated"); break;
        case REVOKED:     printf("Revoked"); break;
        case DESTROYED:   printf("Destroyed"); break;
        default:          printf("Unknown");
    }
    printf("\nKey active flag: %d\n", dev->key_active);
}