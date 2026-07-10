#include "device.h"
#include <stdio.h>
#include <string.h>

void initDevice(Device *device, int id) {
    device->id = id;
    generateKey(device->key.key, KEY_SIZE);
    device->key.is_active = 1;
    device->status = 1; // active
    provisionKey(&device->key);
}

void printKey(const uint8_t key[]) {
    for (int i = 0; i < KEY_SIZE; i++) {
        printf("%02x", key[i]);
    }
}

void printDevice(Device *device) {
    printf("Device ID: %d\n", device->id);
    printf("Key: ");
    printKey(device->key.key);
    printf("\n");
    printf("Status: ");
    switch(device->status) {
        case 0: printf("Inactive\n"); break;
        case 1: printf("Active\n"); break;
        case 2: printf("Revoked\n"); break;
        case 3: printf("Destroyed\n"); break;
        default: printf("Unknown\n");
    }
    printf("Key active flag: %d\n", device->key.is_active);
}