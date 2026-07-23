#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "device.h"
#include "key_manager.h"
#include "logger.h"

void print_menu() {
    printf("\n=== IoT Key Management ===\n");
    printf("1. Provision device\n");
    printf("2. Rotate key\n");
    printf("3. Revoke device\n");
    printf("4. Destroy device\n");
    printf("5. Display all devices\n");
    printf("6. Save keys\n");
    printf("7. Load keys\n");
    printf("0. Exit\n");
    printf("Choice: ");
}

int main(int argc, char *argv[]) {
    if (argc >= 2 && strcmp(argv[1], "--demo") == 0) {
        printf("\n========== STARTING DEMO ==========\n");

        printf("\n=== TC-01: Provision new device ===\n");
        int ret = provision_device("1");
        printf("[RESULT] %s\n", ret == 0 ? "OK" : "FAIL");

        printf("\n=== TC-02: Rotate active device ===\n");
        ret = rotate_key("1");
        printf("[RESULT] %s\n", ret == 0 ? "OK" : "FAIL");

        printf("\n=== TC-03: Rotate non-existent device ===\n");
        ret = rotate_key("999");
        printf("[RESULT] %s (expected NOT_FOUND)\n", ret == -1 ? "OK" : "FAIL");

        printf("\n=== TC-04: Rotate revoked device ===\n");
        revoke_device("1");
        ret = rotate_key("1");
        printf("[RESULT] %s (expected INVALID_STATE)\n", ret == -2 ? "OK" : "FAIL");

        printf("\n=== TC-05: Revoke destroyed device ===\n");
        destroy_device("1");
        ret = revoke_device("1");
        printf("[RESULT] %s (expected INVALID_STATE)\n", ret == -2 ? "OK" : "FAIL");

        printf("\n=== TC-06: Provision duplicate ID ===\n");
        ret = provision_device("1");
        printf("[RESULT] %s (expected DUPLICATE_ID)\n", ret == -3 ? "OK" : "FAIL");

        printf("\n========== DEMO FINISHED ==========\n");
        return 0;
    }

    int choice;
    char id[20];
    while (1) {
        print_menu();
        if (scanf("%d", &choice) != 1) break;
        switch (choice) {
            case 1:
                printf("Enter device ID (number): ");
                scanf("%s", id);
                provision_device(id);
                break;
            case 2:
                printf("Enter device ID: ");
                scanf("%s", id);
                rotate_key(id);
                break;
            case 3:
                printf("Enter device ID: ");
                scanf("%s", id);
                revoke_device(id);
                break;
            case 4:
                printf("Enter device ID: ");
                scanf("%s", id);
                destroy_device(id);
                break;
            case 5: {
                int count = get_device_count();
                for (int i = 0; i < count; i++) {
                    Device *d = get_device_at(i);
                    if (d) {
                        display_device(d);
                        printf("\n");
                    }
                }
                if (count == 0) printf("No devices.\n");
                break;
            }
            case 6:
                save_keys("keys.txt");
                break;
            case 7:
                load_keys("keys.txt");
                break;
            case 0:
                printf("Exiting...\n");
                return 0;
            default:
                printf("Invalid choice.\n");
        }
    }
    return 0;
}