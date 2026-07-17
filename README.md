# IoT Key Management System

## Project Overview

This project demonstrates cryptographic key lifecycle management for IoT devices. The system simulates secure key provisioning, storage, rotation, revocation, and destruction using a simple C program.

The project was developed for the IoT Security course and follows the requirements of the Cryptography, Key Management, Authentication, and Integrity direction.

---

## Security Objectives

* Confidentiality: protect cryptographic keys and sensitive device data.
* Integrity: ensure that data and keys are not modified without authorization.
* Authentication: verify the identity of IoT devices before key operations.
* Availability: maintain the ability to provision, rotate, revoke, and destroy keys.

---

## Cryptographic Mechanisms

| Mechanism   | Purpose                         |
| ----------- | ------------------------------- |
| AES-128     | Symmetric encryption simulation |
| SHA-256     | Data integrity checking         |
| HMAC-SHA256 | Message authentication          |
| CTR_DRBG    | Random key generation           |
| Mbed TLS    | Cryptographic library reference |

---

## Key Lifecycle

```
Generate
    ↓
Provision
    ↓
Store
    ↓
Use
    ↓
Rotate
    ↓
Revoke
    ↓
Destroy
```

---

## Project Structure

```
IoT-Key-Management/
│
├── include/
│   ├── device.h
│   ├── key_manager.h
│   └── logger.h
│
├── src/
│   ├── device.c
│   ├── key_manager.c
│   ├── logger.c
│   └── main.c
│
├── README.md
├── .gitignore
└── policy.md
```

---

## Build Instructions

Compile the program using GCC.

```bash
gcc src/main.c src/device.c src/key_manager.c src/logger.c -Iinclude -o iot.exe
```

Run the program:

```bash
./iot.exe
```

On Windows:

```bash
iot.exe
```

---

## Program Menu

```
1. Show Devices
2. Provision Key
3. Rotate Key
4. Revoke Key
5. Destroy Key
6. Provision All Devices
0. Exit
```

---

## Sample Input and Output

Input:

```
Choose: 2
Enter Device ID (1-5): 3
```

Output:

```
Provisioning key...

Device ID : 3
Status    : ACTIVE

Key:
A8 4F 32 91
6D E7 15 B0
4C 21 D8 7A
19 93 62 FE

Provision Successful.
```

---

## Test Parameters

| Parameter             | Value               |
| --------------------- | ------------------- |
| Number of devices     | 5                   |
| Key size              | 16 bytes (128 bits) |
| Language              | C                   |
| Compiler              | GCC                 |
| Platform              | Windows 11          |
| Cryptographic library | Mbed TLS reference  |

---

## Resource and Time Measurements

| Operation     | Example Time |
| ------------- | ------------ |
| Provision Key | 0.14 ms      |
| Rotate Key    | 0.16 ms      |
| Revoke Key    | 0.05 ms      |
| Destroy Key   | 0.03 ms      |

| Resource     | Example Value    |
| ------------ | ---------------- |
| Flash memory | ~80 KB           |
| RAM usage    | ~20 KB           |
| Key storage  | 16 bytes per key |

---

## Valid and Invalid Tests

| Test Case             | Input                  | Expected Output       | Result |
| --------------------- | ---------------------- | --------------------- | ------ |
| Valid Provision       | Device 2               | Provision Successful  | PASS   |
| Valid Rotate          | Device 2               | Rotate Successful     | PASS   |
| Valid Revoke          | Device 2               | Key Revoked           | PASS   |
| Invalid Device ID     | Device 10              | Invalid Device ID     | PASS   |
| Rotate without key    | Device not provisioned | No Active Key         | PASS   |
| Destroy destroyed key | Already destroyed      | Key Already Destroyed | PASS   |

---

## Security Notice

This project uses randomly generated demonstration keys only.

No real cryptographic keys, certificates, passwords, or secrets are included in this repository.

This complies with the requirement that no real secret keys be uploaded to GitHub.

---

## References

* OWASP IoT Security Verification Standard
* Mbed TLS
* TinyCrypt

---

## Author

Nguyễn Thị Như Quỳnh

Faculty of Information Technology

Van Hien University
