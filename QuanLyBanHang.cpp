#include <exception>
#include <fstream>
#include <iostream>
#include <string>

#include "QuanLyBanHang.h"
#include "TienIch.h"

using namespace std;

namespace
{
	const string FILE_HANG_HOA = "HangHoa.txt";
	const string FILE_DON_HANG = "DonHang.txt";
	const string FILE_ADMIN = "Admin.txt";
	const string ADMIN_MAC_DINH = "admin";
	const string MAT_KHAU_MAC_DINH = "123456";
	const int SO_THU_TU_DAU_TIEN = 1;

	// Các lựa chọn của từng menu
	enum class MenuChinh
	{
		KHONG_HOP_LE = -1,
		THOAT = 0,
		HIEN_THI_HANG_HOA = 1,
		TIM_KIEM_HANG_HOA = 2,
		DAT_HANG = 3,
		QUAN_LY = 4
	};

	template <class TMenu>
	TMenu chuyenSangMenu(const string& strNhap, const TMenu eNhoNhat, const TMenu eLonNhat)
	{
		try
		{
			const int iLuaChon = stoi(strNhap);
			const bool bTrongMenu = iLuaChon >= static_cast<int>(eNhoNhat)
				&& iLuaChon <= static_cast<int>(eLonNhat);
			if (bTrongMenu && to_string(iLuaChon) == strNhap)
			{
				return static_cast<TMenu>(iLuaChon);
			}
		}
		catch (const exception&)
		{

		}
		return TMenu::KHONG_HOP_LE;
	}
	template <class T>
	void docDanhSachTuFile(const string& strTenFile, DanhSach<T>& danhSach)
	{
		ifstream ifsFile(strTenFile);
		string strDong;
		while (getline(ifsFile, strDong))
		{
			strDong = TienIch::catKhoangTrang(strDong);
			T phanTu;
			if (!strDong.empty() && phanTu.docTuDong(strDong))
			{
				danhSach.themCuoi(phanTu);
			}
		}
		ifsFile.close();
	}
	template <class T>
	void ghiDanhSachRaFile(const string& strTenFile, const DanhSach<T>& danhSach)
	{
		ofstream ofsFile(strTenFile);
		if (!ofsFile.is_open())
		{
			TienIch::inLoi("Không ghi được file " + strTenFile + ".");
			return;
		}
		danhSach.duyet([&ofsFile](const T& phanTu)
		{
			ofsFile << phanTu.chuyenThanhDong() << endl;
		});
		ofsFile.close();
	}
}

QuanLyBanHang::QuanLyBanHang() : m_iSoThuTuTiepTheo(SO_THU_TU_DAU_TIEN), m_bDangChay(true)
{
	this->docFileHangHoa();
	this->docFileDonHang();
	this->docFileAdmin();
}

void QuanLyBanHang::chay()
{
	while (this->m_bDangChay)
	{
		this->hienThiMenuChinh();
		const MenuChinh eLuaChon = chuyenSangMenu(TienIch::docLuaChon("Chọn chức năng: "),
			MenuChinh::THOAT, MenuChinh::QUAN_LY);
		switch (eLuaChon)
		{
			case MenuChinh::HIEN_THI_HANG_HOA:
				this->hienThiHangHoa();
				break;
			case MenuChinh::TIM_KIEM_HANG_HOA:
				this->timKiemHangHoa();
				break;
			case MenuChinh::DAT_HANG:
				this->datHang();
				break;
			case MenuChinh::QUAN_LY:
				this->quanLy();
				break;
			case MenuChinh::THOAT:
				this->m_bDangChay = false;
				break;
			default:
				TienIch::inLoi("Lựa chọn không hợp lệ.");
				break;
		}
		if (this->m_bDangChay)
		{
			TienIch::tamDung();
		}
	}
	TienIch::inThongBao("Kết thúc chương trình.", MAU_VANG);
}

// ==================== Đọc / ghi file ====================

void QuanLyBanHang::docFileHangHoa()
{
	docDanhSachTuFile(FILE_HANG_HOA, this->m_danhSachHangHoa);
}
void QuanLyBanHang::docFileDonHang()
{
	ifstream ifsFile(FILE_DON_HANG);
	string strDong;
	while (getline(ifsFile, strDong))
	{
		strDong = TienIch::catKhoangTrang(strDong);
		DonHang donHang;
		if (strDong.empty() || !donHang.docTuDong(strDong))
		{
			continue;
		}
		this->m_danhSachDonHang.themCuoi(donHang);
		if (donHang.getTrangThai() == CHO_XU_LY)
		{
			this->m_hangDoiChoXuLy.enqueue(donHang.getSoThuTu());
		}
		if (donHang.getSoThuTu() >= this->m_iSoThuTuTiepTheo)
		{
			this->m_iSoThuTuTiepTheo = donHang.getSoThuTu() + 1;
		}
	}
	ifsFile.close();
}
void QuanLyBanHang::docFileAdmin()
{
	docDanhSachTuFile(FILE_ADMIN, this->m_danhSachAdmin);
	if (this->m_danhSachAdmin.isEmpty())
	{
		this->m_danhSachAdmin.themCuoi(Admin(ADMIN_MAC_DINH, MAT_KHAU_MAC_DINH));
		this->ghiFileAdmin();
	}
}
void QuanLyBanHang::ghiFileHangHoa() const
{
	ghiDanhSachRaFile(FILE_HANG_HOA, this->m_danhSachHangHoa);
}
void QuanLyBanHang::ghiFileDonHang() const
{
	ghiDanhSachRaFile(FILE_DON_HANG, this->m_danhSachDonHang);
}
void QuanLyBanHang::ghiFileAdmin() const
{
	ghiDanhSachRaFile(FILE_ADMIN, this->m_danhSachAdmin);
}

// ==================== Tìm kiếm nội bộ ====================
HangHoa* QuanLyBanHang::timHangHoa(const string& strMaHang)
{
	const QuanLyBanHang* pDoiTuongConst = this;
	return const_cast<HangHoa*>(pDoiTuongConst->timHangHoa(strMaHang));
}

const HangHoa* QuanLyBanHang::timHangHoa(const string& strMaHang) const
{
	const string strMaThuong = TienIch::chuThuong(strMaHang);
	return this->m_danhSachHangHoa.timDau([&strMaThuong](const HangHoa& hangHoa)
	{
		return TienIch::chuThuong(hangHoa.getMaHang()) == strMaThuong;
	});
}
DonHang* QuanLyBanHang::timDonHang(const int iSoThuTu)
{
	const QuanLyBanHang* pDoiTuongConst = this;
	return const_cast<DonHang*>(pDoiTuongConst->timDonHang(iSoThuTu));
}

const DonHang* QuanLyBanHang::timDonHang(const int iSoThuTu) const
{
	return this->m_danhSachDonHang.timDau([iSoThuTu](const DonHang& donHang)
	{
		return donHang.getSoThuTu() == iSoThuTu;
	});
}

// ==================== Menu chính ====================

void QuanLyBanHang::hienThiMenuChinh() const
{
	TienIch::xoaManHinh();
	TienIch::inThongBao("********** WEBSITE BÁN HÀNG ONLINE **********", MAU_VANG);
	TienIch::inThongBao("  1. Hiển thị thông tin hàng hóa", MAU_TRANG);
	TienIch::inThongBao("  2. Tìm kiếm thông tin hàng hóa", MAU_TRANG);
	TienIch::inThongBao("  3. Đặt hàng", MAU_TRANG);
	TienIch::inThongBao("  4. Quản lý", MAU_TRANG);
	TienIch::inThongBao("  0. Thoát chương trình", MAU_TRANG);
	TienIch::inThongBao("*********************************************", MAU_VANG);
}

void QuanLyBanHang::hienThiHangHoa() const
{
	TienIch::xoaManHinh();
	TienIch::inThongBao("=== DANH SÁCH HÀNG HÓA ===", MAU_VANG);
	if (this->m_danhSachHangHoa.isEmpty())
	{
		TienIch::inThongBao("Hiện chưa có hàng hóa nào.", MAU_DO);
		return;
	}
	HangHoa::hienThiTieuDe();
	this->m_danhSachHangHoa.duyet([](const HangHoa& hangHoa)
	{
		hangHoa.hienThi();
	});
}
void QuanLyBanHang::timKiemHangHoa() const
{
	TienIch::xoaManHinh();
	TienIch::inThongBao("=== TÌM KIẾM HÀNG HÓA ===", MAU_VANG);
	string strTuKhoa;
	if (!TienIch::docChuoi("Nhập tên hàng cần tìm: ", strTuKhoa))
	{
		return;
	}
	const string strTuKhoaThuong = TienIch::chuThuong(strTuKhoa);
	int iSoKetQua = 0;
	this->m_danhSachHangHoa.duyet([&strTuKhoaThuong, &iSoKetQua](const HangHoa& hangHoa)
	{
		if (TienIch::chuThuong(hangHoa.getTenHang()).find(strTuKhoaThuong) == string::npos)
		{
			return;
		}
		if (iSoKetQua == 0)
		{
			HangHoa::hienThiTieuDe();
		}
		hangHoa.hienThi();
		++iSoKetQua;
	});
	if (iSoKetQua == 0)
	{
		TienIch::inLoi("Không tìm thấy hàng hóa nào có tên \"" + strTuKhoa + "\".");
	}
}