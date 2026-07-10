#ifndef KEY_MANAGER_H
#define KEY_MANAGER_H

#include <stdint.h>
#include <stddef.h>

#define KEY_SIZE 32 // AES-256

// Cấu trúc khóa
typedef struct {
    uint8_t key[KEY_SIZE];
    int is_active; // 1: active, 0: revoked, -1: destroyed
} Key;

// Hàm quản lý khóa
void generateKey(uint8_t *key, size_t size);
void provisionKey(Key *key);
void storeKey(const Key *key, int device_id);
void rotateKey(Key *key);
void revokeKey(Key *key);
void destroyKey(Key *key);
void saveKeys(Key *keys, int num_devices);
void loadKeys(Key *keys, int num_devices);

#endif