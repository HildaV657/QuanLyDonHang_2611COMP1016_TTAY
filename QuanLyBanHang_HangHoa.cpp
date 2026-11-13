#include <iostream>
#include <string>

#include "QuanLyBanHang.h"
#include "TienIch.h"

using namespace std;

namespace
{
	const size_t DO_DAI_MA_HANG = 4;
	const int GIA_BAN_TOI_THIEU = 1;
	const int SO_LUONG_KHO_TOI_THIEU = 0;   // số lượng nhỏ nhất của hàng trong kho (0 = hết hàng)
}

/**********************************************************
* @Description Menu quản lý hàng hóa: thêm, xóa, cập nhật số lượng
**********************************************************/
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
		const string strLuaChon = TienIch::docLuaChon("Chọn chức năng: ");
		const char cLuaChon = (strLuaChon.size() == 1) ? strLuaChon[0] : '\0';
		switch (cLuaChon)
		{
			case '1':
				this->themHangHoa();
				break;
			case '2':
				this->xoaHangHoa();
				break;
			case '3':
				this->capNhatSoLuong();
				break;
			case '0':
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

/**********************************************************
* @Description Nhập mã hàng mới
*
* @return true nếu mã gồm đúng 4 ký tự, không có khoảng trắng và
*         chưa tồn tại
**********************************************************/
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

/**********************************************************
* @Description Nhập thông tin và thêm một hàng hóa mới
*
* @attention Có lỗi ở bước nào thì hủy, không thêm
**********************************************************/
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
	if (!TienIch::docChuoi("Ngày nhập kho (dd/mm/yyyy): ", strNgayNhapKho))
	{
		return;
	}
	if (!TienIch::laNgayHopLe(strNgayNhapKho))
	{
		TienIch::inLoi("Ngày nhập kho không hợp lệ.");
		return;
	}
	long long lSoLuong = 0;
	if (!TienIch::docSoNguyen("Số lượng: ", lSoLuong, SO_LUONG_KHO_TOI_THIEU))
	{
		return;
	}
	this->m_danhSachHangHoa.themCuoi(HangHoa(strMaHang, strTenHang, strNoiSanXuat, strMauSac,
		lGiaBan, strNgayNhapKho, static_cast<int>(lSoLuong)));
	this->ghiFileHangHoa();
	TienIch::inThongBao("Thêm hàng hóa thành công.", MAU_XANH_LA);
}

/**********************************************************
* @Description Xóa hàng hóa theo mã hàng
*
* @attention Đơn đang chờ có mã hàng này sẽ bị hủy khi được xử lý
*            vì mã hàng không còn tồn tại
**********************************************************/
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

/**********************************************************
* @Description Cập nhật số lượng của một hàng hóa theo mã hàng
**********************************************************/
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