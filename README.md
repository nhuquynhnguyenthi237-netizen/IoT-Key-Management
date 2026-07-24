# IoT Key Management System

Một chương trình C mô phỏng vòng đời quản lý khóa (key lifecycle) cho thiết bị IoT: cấp phát, xoay vòng, thu hồi, hủy khóa, cùng với lưu/tải khóa và ghi log thao tác.

Dự án được xây dựng cho môn học **IoT Security**, tập trung vào hướng **Key Management, Authentication và Integrity**.

## Mục lục
- [Tổng quan](#tổng-quan)
- [Mục tiêu bảo mật](#mục-tiêu-bảo-mật)
- [Vòng đời khóa](#vòng-đời-khóa)
- [Cấu trúc thư mục](#cấu-trúc-thư-mục)
- [Build & chạy](#build--chạy)
- [Menu chương trình](#menu-chương-trình)
- [Ví dụ chạy demo](#ví-dụ-chạy-demo)
- [Ghi log](#ghi-log)
- [Giới hạn hiện tại](#giới-hạn-hiện-tại)
- [Ghi chú bảo mật](#ghi-chú-bảo-mật)
- [Tham khảo](#tham-khảo)
- [Tác giả](#tác-giả)

---

## Tổng quan

Chương trình mô phỏng một hệ thống quản lý khóa mã hóa cho tối đa 10 thiết bị IoT, hỗ trợ đầy đủ các thao tác: cấp phát (provision), xoay khóa (rotate), thu hồi (revoke), hủy (destroy), cùng với lưu và tải trạng thái khóa từ file.

## Mục tiêu bảo mật

- **Confidentiality**: che (mask) phần lớn nội dung khóa khi hiển thị, chỉ in ra 2 byte đầu.
- **Integrity**: kiểm soát chuyển trạng thái khóa hợp lệ (không cho rotate/revoke khóa đã bị destroy).
- **Authentication**: mỗi thao tác gắn với một Device ID cụ thể, từ chối thao tác trên ID không tồn tại.
- **Availability**: luôn có thể cấp phát, xoay, thu hồi, hủy khóa khi thiết bị còn hợp lệ.
- **Auditability**: mọi thao tác đều được ghi log kèm timestamp.

## Vòng đời khóa

```
Provision
    ↓
Active
    ↓
Rotate  →  Rotated
    ↓
Revoke  →  Revoked
    ↓
Destroy →  Destroyed
```

Các trạng thái khóa (`KeyState`): `PROVISIONED`, `ACTIVE`, `ROTATED`, `REVOKED`, `DESTROYED`.

## Cấu trúc thư mục

```
IoT-Key-Management/
│
├── include/
│   ├── device.h          # Định nghĩa struct Device, KeyState
│   ├── key_manager.h      # API quản lý khóa
│   └── logger.h           # API ghi log
│
├── src/
│   ├── device.c           # Khởi tạo & hiển thị thiết bị
│   ├── key_manager.c      # Provision / Rotate / Revoke / Destroy / Save / Load
│   ├── logger.c           # Ghi log ra console + file
│   └── main.c             # Menu tương tác + chế độ demo
│
├── logs/                  # Log runtime (system.log) - không commit
├── Makefile
├── .gitignore
└── README.md
```

## Build & chạy

Yêu cầu: GCC hỗ trợ chuẩn C11.

```bash
make
```

Lệnh trên biên dịch ra file thực thi `iot_keymgr.exe` (giữ đuôi `.exe` để tương thích khi build trên Windows/MinGW, nhưng chạy được trên cả Linux/macOS).

Chạy chương trình ở chế độ tương tác:

```bash
./iot_keymgr.exe
```

Chạy chế độ demo tự động (6 test case dựng sẵn):

```bash
make demo
```

Dọn file build:

```bash
make clean
```

## Menu chương trình

```
=== IoT Key Management ===
1. Provision device
2. Rotate key
3. Revoke device
4. Destroy device
5. Display all devices
6. Save keys
7. Load keys
0. Exit
```

## Ví dụ chạy demo

```
========== STARTING DEMO ==========

=== TC-01: Provision new device ===
[INFO] Device provisioned
Device 1 provisioned. Key: 7aea************************************************************
[RESULT] OK

=== TC-02: Rotate active device ===
[INFO] Key rotated
Device 1 rotated. New Key: 56c1************************************************************
[RESULT] OK

=== TC-03: Rotate non-existent device ===
[ERROR] Rotate failed: NOT_FOUND
[RESULT] OK (expected NOT_FOUND)

=== TC-04: Rotate revoked device ===
[INFO] Device revoked
Device 1 revoked.
[ERROR] Rotate failed: INVALID_STATE
[RESULT] OK (expected INVALID_STATE)

=== TC-05: Revoke destroyed device ===
[INFO] Device destroyed
Device 1 destroyed.
[ERROR] Revoke failed: INVALID_STATE
[RESULT] OK (expected INVALID_STATE)

=== TC-06: Provision duplicate ID ===
[ERROR] Provision failed: DUPLICATE_ID
[RESULT] OK (expected DUPLICATE_ID)

========== DEMO FINISHED ==========
```

| Test Case | Input | Kết quả mong đợi | Kết quả |
|---|---|---|---|
| Provision hợp lệ | Device 1 | Provision thành công | PASS |
| Rotate khóa đang active | Device 1 | Rotate thành công | PASS |
| Rotate ID không tồn tại | Device 999 | NOT_FOUND | PASS |
| Rotate khóa đã revoke | Device 1 (revoked) | INVALID_STATE | PASS |
| Revoke khóa đã destroy | Device 1 (destroyed) | INVALID_STATE | PASS |
| Provision ID trùng | Device 1 (đã tồn tại) | DUPLICATE_ID | PASS |

## Ghi log

Mọi thao tác (provision, rotate, revoke, destroy, save, load) đều được ghi ra console và append vào `logs/system.log` theo định dạng:

```
[YYYY-MM-DD HH:MM:SS] [LEVEL] Message
```

## Giới hạn hiện tại

Đây là bản mô phỏng phục vụ mục đích học tập, không phải triển khai crypto sẵn sàng cho production:

- Khóa được sinh bằng `rand()` (không phải CSPRNG) — **chưa** dùng thư viện crypto thật như Mbed TLS/OpenSSL. Nếu cần đúng chuẩn CTR_DRBG/AES/SHA-256 như đề bài yêu cầu, cần tích hợp thêm một thư viện crypto thực sự.
- `save_keys()` ghi toàn bộ khóa dưới dạng hex **plaintext** ra `keys.txt` — file này chỉ nên dùng để test cục bộ, không commit lên GitHub.
- Chưa có cơ chế xác thực thiết bị (device authentication) thật sự, chỉ dựa trên Device ID nhập tay.

## Ghi chú bảo mật

Không có khóa thật, chứng chỉ, mật khẩu hay secret nào được commit trong repo này. File `keys.txt` (nếu được tạo ra khi chạy chương trình) đã được thêm vào `.gitignore`.

## Tham khảo

- OWASP IoT Security Verification Standard
- Mbed TLS
- TinyCrypt

## Tác giả

Nguyễn Thị Như Quỳnh
Khoa Công nghệ Thông tin — Trường Đại học Văn Hiến