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
	const string TRA_LOI_CO = "c";
	const int SO_THU_TU_DAU_TIEN = 1;
	const int SO_LAN_DANG_NHAP_TOI_DA = 3;
	const int GIA_BAN_TOI_THIEU = 1;
	const int SO_LUONG_DAT_TOI_THIEU = 1;
	const int SO_LUONG_KHO_TOI_THIEU = 0;   // số lượng nhỏ nhất của hàng trong kho (0 = hết hàng)
	const size_t DO_DAI_MA_HANG = 4;
	const size_t SO_DIEN_THOAI_TOI_THIEU = 9;
	const size_t SO_DIEN_THOAI_TOI_DA = 11;

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

	enum class MenuQuanLy
	{
		KHONG_HOP_LE = -1,
		QUAY_LAI = 0,
		XU_LY_DON_HANG = 1,
		QUAN_LY_HANG_HOA = 2
	};

	enum class MenuQuanLyHangHoa
	{
		KHONG_HOP_LE = -1,
		QUAY_LAI = 0,
		THEM_HANG_HOA = 1,
		XOA_HANG_HOA = 2,
		CAP_NHAT_SO_LUONG = 3
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
	bool nhapSoDienThoai(string& strSoDienThoai)
	{
		if (!TienIch::docChuoi("Số điện thoại: ", strSoDienThoai))
		{
			return false;
		}
		const bool bHopLe = TienIch::laSoNguyen(strSoDienThoai)
			&& strSoDienThoai.size() >= SO_DIEN_THOAI_TOI_THIEU
			&& strSoDienThoai.size() <= SO_DIEN_THOAI_TOI_DA;
		if (!bHopLe)
		{
			TienIch::inLoi("Số điện thoại phải gồm " + to_string(SO_DIEN_THOAI_TOI_THIEU)
				+ " đến " + to_string(SO_DIEN_THOAI_TOI_DA) + " chữ số.");
			return false;
		}
		return true;
	}
	bool nhapNgayHopLe(const string& strTenNgay, string& strNgay)
	{
		if (!TienIch::docChuoi("Ngày " + strTenNgay + " (dd/mm/yyyy): ", strNgay))
		{
			return false;
		}
		if (!TienIch::laNgayHopLe(strNgay))
		{
			TienIch::inLoi("Ngày " + strTenNgay + " không hợp lệ.");
			return false;
		}
		return true;
	}
	bool kiemTraMatHangDatDuoc(const HangHoa* pHangHoa, const DonHang& donHang)
	{
		if (pHangHoa == nullptr)
		{
			TienIch::inLoi("Mã hàng không tồn tại.");
			return false;
		}
		if (pHangHoa->getSoLuong() <= SO_LUONG_KHO_TOI_THIEU)
		{
			TienIch::inLoi("Mặt hàng này đã hết hàng.");
			return false;
		}
		if (donHang.coMaHang(pHangHoa->getMaHang()))
		{
			TienIch::inLoi("Mã hàng này đã có trong đơn.");
			return false;
		}
		return true;
	}
	void hienThiManHinhDangNhap()
	{
		TienIch::xoaManHinh();
		TienIch::inThongBao("  * * * * * * * * * * * * * * * * * * * * *", MAU_VANG);
		TienIch::inThongBao("  *          DANG NHAP HE THONG           *", MAU_XANH_LA);
		TienIch::inThongBao("  * * * * * * * * * * * * * * * * * * * * *", MAU_VANG);
		TienIch::inThongBao("  (Nhấn ESC để thoát)\n", MAU_MAC_DINH);
	}
	bool nhapTaiKhoan(string& strUser, string& strPassword)
	{
		TienIch::datMau(MAU_XANH_LA);
		cout << "    User: ";
		TienIch::datMau(MAU_TRANG);
		if (!TienIch::docPhim(strUser, false))
		{
			return false;
		}
		TienIch::datMau(MAU_XANH_LA);
		cout << "    Password: ";
		TienIch::datMau(MAU_TRANG);
		if (!TienIch::docPhim(strPassword, true))
		{
			return false;
		}
		TienIch::datMau(MAU_MAC_DINH);
		return true;
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

// ==================== Đặt hàng ====================

void QuanLyBanHang::datHang()
{
	TienIch::xoaManHinh();
	TienIch::inThongBao("=== ĐẶT HÀNG ===", MAU_VANG);
	DonHang donHang;
	if (!this->nhapThongTinKhach(donHang) || !this->nhapChiTietDon(donHang))
	{
		TienIch::inThongBao("Đã hủy việc đặt hàng.", MAU_DO);
		return;
	}
	donHang.setSoThuTu(this->m_iSoThuTuTiepTheo);
	++this->m_iSoThuTuTiepTheo;
	this->m_danhSachDonHang.themCuoi(donHang);
	this->m_hangDoiChoXuLy.enqueue(donHang.getSoThuTu());
	this->ghiFileDonHang();
	TienIch::inThongBao("Đơn hàng đang chờ xử lý", MAU_XANH_LA);
}

bool QuanLyBanHang::nhapThongTinKhach(DonHang& donHang) const
{
	string strTenKhach;
	string strDiaChi;
	if (!TienIch::docChuoi("Tên khách hàng: ", strTenKhach)
		|| !TienIch::docChuoi("Địa chỉ khách hàng: ", strDiaChi))
	{
		return false;
	}
	string strSoDienThoai;
	string strNgayDat;
	if (!nhapSoDienThoai(strSoDienThoai) || !nhapNgayHopLe("đặt hàng", strNgayDat))
	{
		return false;
	}
	donHang.setThongTinKhach(strTenKhach, strDiaChi, strSoDienThoai, strNgayDat);
	return true;
}

bool QuanLyBanHang::nhapChiTietDon(DonHang& donHang) const
{
	bool bTiepTuc = true;
	while (bTiepTuc)
	{
		if (!this->nhapMotChiTiet(donHang))
		{
			return false;
		}
		string strTraLoi;
		if (!TienIch::docChuoi("Thêm mã hàng khác vào đơn: ", strTraLoi))
		{
			return false;
		}
		bTiepTuc = (TienIch::chuThuong(strTraLoi) == TRA_LOI_CO);
	}
	return true;
}

bool QuanLyBanHang::nhapMotChiTiet(DonHang& donHang) const
{
	string strMaHang;
	if (!TienIch::docChuoi("Mã hàng: ", strMaHang))
	{
		return false;
	}
	const HangHoa* pHangHoa = this->timHangHoa(strMaHang);
	if (!kiemTraMatHangDatDuoc(pHangHoa, donHang))
	{
		return false;
	}
	long long lSoLuong = 0;
	if (!TienIch::docSoNguyen("Số lượng: ", lSoLuong, SO_LUONG_DAT_TOI_THIEU))
	{
		return false;
	}
	if (lSoLuong > pHangHoa->getSoLuong())
	{
		TienIch::inLoi("Số lượng đặt vượt quá số lượng hiện có (" + to_string(pHangHoa->getSoLuong()) + ").");
		return false;
	}
	donHang.themChiTiet(ChiTietDon(pHangHoa->getMaHang(), static_cast<int>(lSoLuong),
		pHangHoa->getGiaBan() * lSoLuong));
	return true;
}

// ==================== Quản lý ====================

void QuanLyBanHang::quanLy()
{
	if (this->dangNhap())
	{
		this->menuQuanLy();
	}
}
bool QuanLyBanHang::dangNhap()
{
	for (int iLanThu = 1; iLanThu <= SO_LAN_DANG_NHAP_TOI_DA; ++iLanThu)
	{
		hienThiManHinhDangNhap();
		string strUser;
		string strPassword;
		if (!nhapTaiKhoan(strUser, strPassword))
		{
			this->m_bDangChay = false;
			return false;
		}
		if (this->kiemTraTaiKhoan(strUser, strPassword))
		{
			return true;
		}
		TienIch::inLoi("Sai user hoặc password (lần " + to_string(iLanThu) + "/"
			+ to_string(SO_LAN_DANG_NHAP_TOI_DA) + ").");
		TienIch::tamDung();
	}
	TienIch::inLoi("Nhập sai " + to_string(SO_LAN_DANG_NHAP_TOI_DA) + " lần. Thoát khỏi hệ thống.");
	this->m_bDangChay = false;
	return false;
}

bool QuanLyBanHang::kiemTraTaiKhoan(const string& strUser, const string& strPassword) const
{
	const Admin* pAdmin = this->m_danhSachAdmin.timDau([&strUser, &strPassword](const Admin& admin)
	{
		return admin.kiemTra(strUser, strPassword);
	});
	return pAdmin != nullptr;
}

void QuanLyBanHang::menuQuanLy()
{
	bool bTiepTuc = true;
	while (bTiepTuc)
	{
		TienIch::xoaManHinh();
		TienIch::inThongBao("=== MENU QUẢN LÝ ===", MAU_VANG);
		TienIch::inThongBao("  1. Xử lý đơn hàng (" + to_string(this->m_hangDoiChoXuLy.getSize()) + " đơn đang chờ)", MAU_TRANG);
		TienIch::inThongBao("  2. Quản lý hàng hóa", MAU_TRANG);
		TienIch::inThongBao("  0. Quay lại menu chính", MAU_TRANG);
		const MenuQuanLy eLuaChon = chuyenSangMenu(TienIch::docLuaChon("Chọn chức năng: "),
			MenuQuanLy::QUAY_LAI, MenuQuanLy::QUAN_LY_HANG_HOA);
		switch (eLuaChon)
		{
			case MenuQuanLy::XU_LY_DON_HANG:
				this->xuLyDonHang();
				TienIch::tamDung();
				break;
			case MenuQuanLy::QUAN_LY_HANG_HOA:
				this->menuQuanLyHangHoa();
				break;
			case MenuQuanLy::QUAY_LAI:
				bTiepTuc = false;
				break;
			default:
				TienIch::inLoi("Lựa chọn không hợp lệ.");
				TienIch::tamDung();
				break;
		}
	}
}

bool QuanLyBanHang::kiemTraDuHang(const DonHang& donHang) const
{
	bool bDuHang = true;
	donHang.getDanhSachChiTiet().duyet([this, &bDuHang](const ChiTietDon& chiTiet)
	{
		const HangHoa* pHangHoa = this->timHangHoa(chiTiet.getMaHang());
		if (pHangHoa == nullptr || pHangHoa->getSoLuong() < chiTiet.getSoLuong())
		{
			bDuHang = false;
		}
	});
	return bDuHang;
}

void QuanLyBanHang::xuLyDonHang()
{
	TienIch::xoaManHinh();
	TienIch::inThongBao("=== XỬ LÝ ĐƠN HÀNG ===", MAU_VANG);
	if (this->m_hangDoiChoXuLy.isEmpty())
	{
		TienIch::inThongBao("Không có đơn hàng nào đang chờ xử lý.", MAU_DO);
		return;
	}
	DonHang* pDonHang = this->timDonHang(*this->m_hangDoiChoXuLy.front());
	this->m_hangDoiChoXuLy.dequeue();
	if (pDonHang == nullptr)
	{
		TienIch::inLoi("Không tìm thấy dữ liệu đơn hàng.");
		return;
	}
	if (this->kiemTraDuHang(*pDonHang))
	{
		this->hoanTatDonHang(*pDonHang);
	}
	else
	{
		this->huyDonHang(*pDonHang);
	}
	this->ghiFileHangHoa();
	this->ghiFileDonHang();
}

void QuanLyBanHang::hoanTatDonHang(DonHang& donHang)
{
	donHang.getDanhSachChiTiet().duyet([this](const ChiTietDon& chiTiet)
	{
		HangHoa* pHangHoa = this->timHangHoa(chiTiet.getMaHang());
		pHangHoa->setSoLuong(pHangHoa->getSoLuong() - chiTiet.getSoLuong());
	});
	donHang.setTrangThai(DA_XU_LY);
	donHang.hienThi();
	TienIch::inThongBao("Đã xử lý xong đơn hàng và cập nhật số lượng hàng hóa.", MAU_XANH_LA);
}

void QuanLyBanHang::huyDonHang(DonHang& donHang)
{
	donHang.setTrangThai(DA_HUY);
	donHang.hienThi();
	TienIch::inLoi("Không đủ hàng hoặc mã hàng đã bị xóa, đơn hàng bị hủy.");
}
void QuanLyBanHang::menuQuanLyHangHoa()
{
	bool bTiepTuc = true;
	while (bTiepTuc)
	{
		TienIch::xoaManHinh();
		TienIch::inThongBao("=== QUẢN LÝ HÀNG HÓA ===", MAU_VANG);
		TienIch::inThongBao("  1. Thêm hàng hóa mới", MAU_TRANG);
		TienIch::inThongBao("  2. Xóa hàng hóa theo mã hàng", MAU_TRANG);
		TienIch::inThongBao("  3. Cập nhật số lượng hàng hóa", MAU_TRANG);
		TienIch::inThongBao("  0. Quay lại", MAU_TRANG);
		const MenuQuanLyHangHoa eLuaChon = chuyenSangMenu(TienIch::docLuaChon("Chọn chức năng: "),
			MenuQuanLyHangHoa::QUAY_LAI, MenuQuanLyHangHoa::CAP_NHAT_SO_LUONG);
		switch (eLuaChon)
		{
			case MenuQuanLyHangHoa::THEM_HANG_HOA:
				this->themHangHoa();
				break;
			case MenuQuanLyHangHoa::XOA_HANG_HOA:
				this->xoaHangHoa();
				break;
			case MenuQuanLyHangHoa::CAP_NHAT_SO_LUONG:
				this->capNhatSoLuong();
				break;
			case MenuQuanLyHangHoa::QUAY_LAI:
				bTiepTuc = false;
				break;
			default:
				TienIch::inLoi("Lựa chọn không hợp lệ.");
				break;
		}
		if (bTiepTuc)
		{
			TienIch::tamDung();
		}
	}
}

bool QuanLyBanHang::nhapMaHangMoi(string& strMaHang) const
{
	if (!TienIch::docChuoi("Mã hàng (" + to_string(DO_DAI_MA_HANG) + " ký tự): ", strMaHang))
	{
		return false;
	}
	const bool bCoKhoangTrang = strMaHang.find(' ') != string::npos;
	if (strMaHang.size() != DO_DAI_MA_HANG || bCoKhoangTrang)
	{
		TienIch::inLoi("Mã hàng phải gồm đúng " + to_string(DO_DAI_MA_HANG) + " ký tự, không chứa khoảng trắng.");
		return false;
	}
	if (this->timHangHoa(strMaHang) != nullptr)
	{
		TienIch::inLoi("Mã hàng đã tồn tại.");
		return false;
	}
	return true;
}

void QuanLyBanHang::themHangHoa()
{
	TienIch::inThongBao("--- Thêm hàng hóa ---", MAU_VANG);
	string strMaHang;
	if (!this->nhapMaHangMoi(strMaHang))
	{
		return;
	}
	string strTenHang;
	string strNoiSanXuat;
	string strMauSac;
	long long lGiaBan = 0;
	if (!TienIch::docChuoi("Tên hàng: ", strTenHang) || !TienIch::docChuoi("Nơi sản xuất: ", strNoiSanXuat)
		|| !TienIch::docChuoi("Màu sắc: ", strMauSac) || !TienIch::docSoNguyen("Giá bán: ", lGiaBan, GIA_BAN_TOI_THIEU))
	{
		return;
	}
	string strNgayNhapKho;
	long long lSoLuong = 0;
	if (!nhapNgayHopLe("nhập kho", strNgayNhapKho)
		|| !TienIch::docSoNguyen("Số lượng: ", lSoLuong, SO_LUONG_KHO_TOI_THIEU))
	{
		return;
	}
	this->m_danhSachHangHoa.themCuoi(HangHoa(strMaHang, strTenHang, strNoiSanXuat, strMauSac,
		lGiaBan, strNgayNhapKho, static_cast<int>(lSoLuong)));
	this->ghiFileHangHoa();
	TienIch::inThongBao("Thêm hàng hóa thành công.", MAU_XANH_LA);
}

void QuanLyBanHang::xoaHangHoa()
{
	TienIch::inThongBao("--- Xóa hàng hóa ---", MAU_VANG);
	string strMaHang;
	if (!TienIch::docChuoi("Mã hàng cần xóa: ", strMaHang))
	{
		return;
	}
	const string strMaThuong = TienIch::chuThuong(strMaHang);
	const int iViTri = this->m_danhSachHangHoa.timViTri([&strMaThuong](const HangHoa& hangHoa)
	{
		return TienIch::chuThuong(hangHoa.getMaHang()) == strMaThuong;
	});
	if (iViTri < 0)
	{
		TienIch::inLoi("Mã hàng không tồn tại.");
		return;
	}
	this->m_danhSachHangHoa.xoaTaiViTri(iViTri);
	this->ghiFileHangHoa();
	TienIch::inThongBao("Đã xóa hàng hóa.", MAU_XANH_LA);
}

void QuanLyBanHang::capNhatSoLuong()
{
	TienIch::inThongBao("--- Cập nhật số lượng ---", MAU_VANG);
	string strMaHang;
	if (!TienIch::docChuoi("Mã hàng cần cập nhật: ", strMaHang))
	{
		return;
	}
	HangHoa* pHangHoa = this->timHangHoa(strMaHang);
	if (pHangHoa == nullptr)
	{
		TienIch::inLoi("Mã hàng không tồn tại.");
		return;
	}
	cout << "Số lượng hiện tại: " << pHangHoa->getSoLuong() << endl;
	long long lSoLuongMoi = 0;
	if (!TienIch::docSoNguyen("Số lượng mới: ", lSoLuongMoi, SO_LUONG_KHO_TOI_THIEU))
	{
		return;
	}
	pHangHoa->setSoLuong(static_cast<int>(lSoLuongMoi));
	this->ghiFileHangHoa();
	TienIch::inThongBao("Cập nhật số lượng thành công.", MAU_XANH_LA);
}
