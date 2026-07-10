#include "key_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// #define USE_MBEDTLS   // Bỏ comment nếu dùng Mbed TLS

#ifdef USE_MBEDTLS
#include <mbedtls/ctr_drbg.h>
#include <mbedtls/entropy.h>
#endif

void generateKey(uint8_t *key, size_t size) {
#ifdef USE_MBEDTLS
    mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;
    const char *pers = "iot_key_gen";
    mbedtls_entropy_init(&entropy);
    mbedtls_ctr_drbg_init(&ctr_drbg);
    mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy,
                          (const unsigned char*)pers, strlen(pers));
    mbedtls_ctr_drbg_random(&ctr_drbg, key, size);
    mbedtls_ctr_drbg_free(&ctr_drbg);
    mbedtls_entropy_free(&entropy);
#else
    static int seeded = 0;
    if (!seeded) {
        srand((unsigned int)time(NULL));
        seeded = 1;
    }
    for (size_t i = 0; i < size; i++) {
        key[i] = (uint8_t)(rand() % 256);
    }
#endif
}

void provisionKey(Key *key) {
    key->is_active = 1;
    printf("Key provisioned.\n");
}

void storeKey(const Key *key, int device_id) {
    FILE *fp = fopen("keys.txt", "a");
    if (fp) {
        fprintf(fp, "Device %d key: ", device_id);
        for (int i = 0; i < KEY_SIZE; i++) {
            fprintf(fp, "%02x", key->key[i]);
        }
        fprintf(fp, " (active=%d)\n", key->is_active);
        fclose(fp);
    }
}

void rotateKey(Key *key) {
    generateKey(key->key, KEY_SIZE);
    key->is_active = 1;
    printf("Key rotated.\n");
}

void revokeKey(Key *key) {
    key->is_active = 0;
    printf("Key revoked.\n");
}

void destroyKey(Key *key) {
    memset(key->key, 0, KEY_SIZE);
    key->is_active = -1;
    printf("Key destroyed.\n");
}

void saveKeys(Key *keys, int num_devices) {
    FILE *fp = fopen("keys.txt", "w");
    if (!fp) {
        perror("Cannot open keys.txt for writing");
        return;
    }
    for (int i = 0; i < num_devices; i++) {
        fprintf(fp, "Device %d key: ", i);
        for (int j = 0; j < KEY_SIZE; j++) {
            fprintf(fp, "%02x", keys[i].key[j]);
        }
        fprintf(fp, " (active=%d)\n", keys[i].is_active);
    }
    fclose(fp);
    printf("[Save] All keys saved to keys.txt\n");
}

void loadKeys(Key *keys, int num_devices) {
    FILE *fp = fopen("keys.txt", "r");
    if (!fp) {
        perror("Cannot open keys.txt for reading");
        return;
    }
    char line[512];
    int device_id = 0;
    while (fgets(line, sizeof(line), fp) && device_id < num_devices) {
        // Tìm "key: "
        char *key_start = strstr(line, "key: ");
        if (!key_start) continue;
        key_start += 5; // bỏ qua "key: "

        // Tìm "(active="
        char *paren = strstr(key_start, " (active=");
        if (!paren) continue;
        *paren = '\0';  // tách phần hex

        // Parse hex key (32 bytes)
        int len = (int)strlen(key_start);
        if (len >= KEY_SIZE * 2) {
            for (int j = 0; j < KEY_SIZE; j++) {
                unsigned int byte;
                if (sscanf(key_start + 2 * j, "%2x", &byte) != 1) {
                    break;
                }
                keys[device_id].key[j] = (uint8_t)byte;
            }
        }

        // Parse active status – dùng strtol để tránh lỗi do ký tự xuống dòng
        char *num_start = paren + 9; // bỏ qua "(active=" (9 ký tự)
        char *endptr;
        long val = strtol(num_start, &endptr, 10);
        if (endptr > num_start) {
            keys[device_id].is_active = (int)val;
        } else {
            keys[device_id].is_active = -1; // không parse được
        }

        device_id++;
    }
    fclose(fp);
    printf("[Load] Keys loaded from keys.txt\n");
}