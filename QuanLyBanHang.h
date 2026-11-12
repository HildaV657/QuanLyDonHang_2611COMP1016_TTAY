#ifndef QUANLYBANHANG_H_INCLUDED_
#define QUANLYBANHANG_H_INCLUDED_

#include <string>

#include "Admin.h"
#include "DanhSach.h"
#include "DonHang.h"
#include "HangDoi.h"
#include "HangHoa.h"

class QuanLyBanHang
{
public:
	QuanLyBanHang();
	void chay();

private:
	DanhSach<HangHoa> m_danhSachHangHoa;
	DanhSach<DonHang> m_danhSachDonHang;
	DanhSach<Admin> m_danhSachAdmin;
	HangDoi<int> m_hangDoiChoXuLy;  // lưu số thứ tự các đơn đang chờ xử lý
	int m_iSoThuTuTiepTheo;
	bool m_bDangChay;

	// Đọc/ghi file
	void docFileHangHoa();
	void docFileDonHang();
	void docFileAdmin();
	void ghiFileHangHoa() const;
	void ghiFileDonHang() const;
	void ghiFileAdmin() const;

	// Tìm kiếm nội bộ
	HangHoa* timHangHoa(const std::string& strMaHang);
	const HangHoa* timHangHoa(const std::string& strMaHang) const;
	DonHang* timDonHang(const int iSoThuTu);
	const DonHang* timDonHang(const int iSoThuTu) const;

	// Menu chính
	void hienThiMenuChinh() const;
	void hienThiHangHoa() const;
	void timKiemHangHoa() const;

	// Đặt hàng
	void datHang();
	bool nhapThongTinKhach(DonHang& donHang) const;
	bool nhapChiTietDon(DonHang& donHang) const;
	bool nhapMotChiTiet(DonHang& donHang) const;

	// Quản lý
	void quanLy();
	bool dangNhap();
	bool kiemTraTaiKhoan(const std::string& strUser, const std::string& strPassword) const;
	void menuQuanLy();
	bool kiemTraDuHang(const DonHang& donHang) const;
	void xuLyDonHang();
	void hoanTatDonHang(DonHang& donHang);
	void huyDonHang(DonHang& donHang);
	void menuQuanLyHangHoa();
	bool nhapMaHangMoi(std::string& strMaHang) const;
	void themHangHoa();
	void xoaHangHoa();
	void capNhatSoLuong();
};

#endif
