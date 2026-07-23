#ifndef KEY_MANAGER_H
#define KEY_MANAGER_H

#include "device.h"

int provision_device(const char *id);
int rotate_key(const char *id);
int revoke_device(const char *id);
int destroy_device(const char *id);

void save_keys(const char *filename);
void load_keys(const char *filename);

int get_device_count(void);
Device* get_device_at(int index);

#endif