#include <stdio.h>
#include <stdlib.h>
#include <string.h>   // <--- ĐÃ THÊM DÒNG NÀY
#include "device.h"
#include "key_manager.h"
#include "logger.h"

#define MAX_DEVICES 5

int main() {
    Device devices[MAX_DEVICES];
    Key keys[MAX_DEVICES];   // mảng riêng để lưu/save

    printf("=== IoT Key Management Demo ===\n\n");

    // 1. Khởi tạo 5 thiết bị, sinh khóa và cấp phát
    for (int i = 0; i < MAX_DEVICES; i++) {
        initDevice(&devices[i], i);
        memcpy(keys[i].key, devices[i].key.key, KEY_SIZE);
        keys[i].is_active = devices[i].key.is_active;
        writeLog(i, "INIT DEVICE");
    }

    // 2. In thông tin các thiết bị
    printf("=== Device Information (after init) ===\n");
    for (int i = 0; i < MAX_DEVICES; i++) {
        printDevice(&devices[i]);
        printf("\n");
    }

    // 3. Lưu khóa xuống file keys.txt
    saveKeys(keys, MAX_DEVICES);
    writeLog(-1, "SAVE KEYS");

    // 4. Xoay vòng khóa cho thiết bị số 2 (rotate)
    printf("\n=== Rotating key for Device 2 ===\n");
    rotateKey(&devices[2].key);
    memcpy(keys[2].key, devices[2].key.key, KEY_SIZE);
    keys[2].is_active = devices[2].key.is_active;
    devices[2].status = 1;   // active
    writeLog(2, "ROTATE KEY");
    printDevice(&devices[2]);
    printf("\n");

    // 5. Thu hồi khóa thiết bị số 4 (revoke)
    printf("=== Revoking key for Device 4 ===\n");
    revokeKey(&devices[4].key);
    keys[4].is_active = devices[4].key.is_active;
    devices[4].status = 2;   // revoked
    writeLog(4, "REVOKE KEY");
    printDevice(&devices[4]);
    printf("\n");

    // 6. Hủy khóa thiết bị số 1 (destroy)
    printf("=== Destroying key for Device 1 ===\n");
    destroyKey(&devices[1].key);
    // Không cập nhật keys[1] vì khóa đã bị xóa
    devices[1].status = 3;   // destroyed
    writeLog(1, "DESTROY KEY");
    printDevice(&devices[1]);
    printf("\n");

    // 7. Lưu lại trạng thái sau thay đổi
    saveKeys(keys, MAX_DEVICES);
    writeLog(-1, "SAVE KEYS AFTER CHANGES");

    // 8. Tải lại khóa từ file để kiểm tra
    printf("=== Loading keys from keys.txt ===\n");
    Key loaded_keys[MAX_DEVICES];
    loadKeys(loaded_keys, MAX_DEVICES);
    for (int i = 0; i < MAX_DEVICES; i++) {
        printf("Loaded Device %d key: ", i);
        printKey(loaded_keys[i].key);
        printf(" (active=%d)\n", loaded_keys[i].is_active);
    }

    printf("\n=== Demo completed. Check 'keys.txt' and 'logs.txt' ===\n");
    return 0;
}