# IoT-Key-Management

Chương trình mô phỏng **quản lý vòng đời khóa mật mã (cryptographic key
lifecycle management)** cho hệ thống IoT, viết bằng ngôn ngữ C.

Đề tài 39 — Hướng F (Mật mã, khóa, xác thực và toàn vẹn)
Học phần: Bảo mật IoT (INT4410) — Trường Đại học Văn Hiến

## 1. Mục đích

Mô phỏng 4 thao tác chính trong vòng đời khóa của một thiết bị IoT:

| Thao tác | Ý nghĩa |
|---|---|
| **Provision** | Sinh khóa mới và cấp phát cho thiết bị |
| **Rotate** | Xoay vòng khóa (thay khóa cũ bằng khóa mới, tăng version) |
| **Revoke** | Thu hồi khóa (thiết bị không còn được tin cậy) |
| **Destroy** | Hủy khóa vĩnh viễn (xóa khỏi bộ nhớ, không thể phục hồi) |

Sơ đồ trạng thái: `PROVISIONED → ACTIVE → ROTATED → REVOKED → DESTROYED`

## 2. Cấu trúc thư mục

```
iot-key-management/
├── include/
│   ├── device.h        # struct Device, enum KeyState
│   ├── key_manager.h    # khai báo API quản lý khóa
│   └── logger.h         # khai báo API ghi log
├── src/
│   ├── main.c           # menu tương tác + chế độ demo tự động
│   ├── key_manager.c     # cài đặt Provision/Rotate/Revoke/Destroy
│   └── logger.c          # ghi log có timestamp ra console + file
├── logs/
│   └── system.log        # log sinh ra khi chạy chương trình (không commit log thật)
├── Makefile
└── README.md
```

## 3. Yêu cầu môi trường

- GCC (khuyến nghị 11.0 trở lên), chuẩn C11
- Hệ điều hành: Windows (MSYS2/MinGW), Linux hoặc macOS đều chạy được

## 4. Biên dịch và chạy

```bash
make            # bien dich ra file thuc thi iot_keymgr
./iot_keymgr    # chay che do menu tuong tac
```

Chạy chế độ demo tự động (dùng để tái hiện các kịch bản kiểm thử
TC-01 → TC-06 dùng làm minh chứng trong báo cáo):

```bash
make demo
# hoac
./iot_keymgr --demo
```

Kết quả demo được in ra console **và** ghi vào `logs/system.log`.

## 5. Kịch bản kiểm thử (Valid / Invalid)

| ID | Kịch bản | Kết quả mong đợi |
|---|---|---|
| TC-01 | Provision thiết bị mới hợp lệ | OK |
| TC-02 | Rotate khóa trên thiết bị đang active | OK |
| TC-03 | Rotate trên thiết bị không tồn tại | Từ chối (NOT_FOUND) |
| TC-04 | Rotate sau khi đã Revoke | Từ chối (INVALID_STATE) |
| TC-05 | Revoke sau khi đã Destroy | Từ chối (INVALID_STATE) |
| TC-06 | Provision trùng ID thiết bị | Từ chối (DUPLICATE_ID) |

## 6. Quy tắc an toàn (áp dụng trong repo này)

- Khóa hiển thị trên console/log **luôn được che (mask)** — chỉ hiện 2
  byte đầu, phần còn lại thay bằng `**`. Không bao giờ in toàn bộ khóa
  thật ra ngoài, kể cả ở chế độ demo.
- Khóa sinh ra bằng `rand()` **chỉ phục vụ mô phỏng/học tập**. Sản phẩm
  thật phải dùng bộ sinh số ngẫu nhiên an toàn (CSPRNG), ví dụ
  `RAND_bytes` của OpenSSL hoặc `/dev/urandom`.
- File `logs/*.log` sinh ra khi chạy chương trình không chứa khóa thật
  và không được commit dữ liệu nhạy cảm lên GitHub.
- Không có secret, token hoặc mật khẩu thật nào được lưu trong repo này.

## 7. Tác giả

Nguyễn Thị Như Quỳnh — MSSV 231A010815 — Lớp 253INT441001
GVHD: Hồ Nhật Minh
