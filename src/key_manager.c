#include "key_manager.h"
#include "logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static Device devices[MAX_DEVICES];
static int device_count = 0;

static void generate_key(uint8_t *key, size_t len) {
    static int seeded = 0;
    if (!seeded) {
        srand((unsigned)time(NULL));
        seeded = 1;
    }
    for (size_t i = 0; i < len; i++) {
        key[i] = (uint8_t)(rand() % 256);
    }
}

static Device* find_device(const char *id) {
    int num = atoi(id);
    for (int i = 0; i < device_count; i++) {
        if (devices[i].id == num)
            return &devices[i];
    }
    return NULL;
}

int provision_device(const char *id) {
    if (find_device(id) != NULL) {
        log_message("ERROR", "Provision failed: DUPLICATE_ID");
        return -3;
    }
    if (device_count >= MAX_DEVICES) {
        log_message("ERROR", "Provision failed: MAX_DEVICES");
        return -4;
    }

    Device *dev = &devices[device_count++];
    init_device(dev, atoi(id));
    generate_key(dev->key, KEY_SIZE);
    dev->state = PROVISIONED;
    dev->key_active = 1;

    log_message("INFO", "Device provisioned");
    printf("Device %s provisioned. Key: ", id);
    print_masked_key(dev->key, KEY_SIZE);
    printf("\n");
    return 0;
}

int rotate_key(const char *id) {
    Device *dev = find_device(id);
    if (dev == NULL) {
        log_message("ERROR", "Rotate failed: NOT_FOUND");
        return -1;
    }
    if (dev->state == REVOKED || dev->state == DESTROYED) {
        log_message("ERROR", "Rotate failed: INVALID_STATE");
        return -2;
    }

    generate_key(dev->key, KEY_SIZE);
    dev->state = (dev->state == PROVISIONED) ? ACTIVE : ROTATED;
    dev->key_active = 1;

    log_message("INFO", "Key rotated");
    printf("Device %s rotated. New Key: ", id);
    print_masked_key(dev->key, KEY_SIZE);
    printf("\n");
    return 0;
}

int revoke_device(const char *id) {
    Device *dev = find_device(id);
    if (dev == NULL) {
        log_message("ERROR", "Revoke failed: NOT_FOUND");
        return -1;
    }
    if (dev->state == DESTROYED) {
        log_message("ERROR", "Revoke failed: INVALID_STATE");
        return -2;
    }

    dev->state = REVOKED;
    dev->key_active = 0;
    log_message("INFO", "Device revoked");
    printf("Device %s revoked.\n", id);
    return 0;
}

int destroy_device(const char *id) {
    Device *dev = find_device(id);
    if (dev == NULL) {
        log_message("ERROR", "Destroy failed: NOT_FOUND");
        return -1;
    }

    dev->state = DESTROYED;
    dev->key_active = -1;
    memset(dev->key, 0, KEY_SIZE);
    log_message("INFO", "Device destroyed");
    printf("Device %s destroyed.\n", id);
    return 0;
}

int get_device_count(void) {
    return device_count;
}

Device* get_device_at(int index) {
    if (index >= 0 && index < device_count)
        return &devices[index];
    return NULL;
}

void save_keys(const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        log_message("ERROR", "Cannot save keys");
        return;
    }
    for (int i = 0; i < device_count; i++) {
        fprintf(fp, "%d ", devices[i].id);
        for (int j = 0; j < KEY_SIZE; j++) {
            fprintf(fp, "%02x", devices[i].key[j]);
        }
        fprintf(fp, " %d\n", devices[i].key_active);
    }
    fclose(fp);
    log_message("INFO", "Keys saved");
    printf("[Save] All keys saved to %s\n", filename);
}

void load_keys(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        log_message("ERROR", "Cannot load keys");
        return;
    }
    device_count = 0;
    int id, active;
    uint8_t key[KEY_SIZE];
    while (fscanf(fp, "%d ", &id) == 1) {
        for (int i = 0; i < KEY_SIZE; i++) {
            unsigned int byte;
            fscanf(fp, "%02x", &byte);
            key[i] = (uint8_t)byte;
        }
        fscanf(fp, "%d", &active);
        if (device_count < MAX_DEVICES) {
            devices[device_count].id = id;
            memcpy(devices[device_count].key, key, KEY_SIZE);
            devices[device_count].key_active = active;
            if (active == 1)      devices[device_count].state = ACTIVE;
            else if (active == 0) devices[device_count].state = REVOKED;
            else                  devices[device_count].state = DESTROYED;
            device_count++;
        }
    }
    fclose(fp);
    log_message("INFO", "Keys loaded");
    printf("[Load] Keys loaded from %s\n", filename);
}