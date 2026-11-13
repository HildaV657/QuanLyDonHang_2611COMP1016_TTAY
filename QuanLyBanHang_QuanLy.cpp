#include <iostream>
#include <string>

#include "QuanLyBanHang.h"
#include "TienIch.h"

using namespace std;

namespace
{
	const int SO_LAN_DANG_NHAP_TOI_DA = 3;

	/**********************************************************
	* @Description In khung đăng nhập hệ thống
	**********************************************************/
	void hienThiManHinhDangNhap()
	{
		TienIch::xoaManHinh();
		TienIch::inThongBao("  * * * * * * * * * * * * * * * * * * * * *", MAU_VANG);
		TienIch::inThongBao("  *          DANG NHAP HE THONG           *", MAU_XANH_LA);
		TienIch::inThongBao("  * * * * * * * * * * * * * * * * * * * * *", MAU_VANG);
		TienIch::inThongBao("  (Nhấn ESC để thoát)\n", MAU_MAC_DINH);
	}

	/**********************************************************
	* @Description Nhập user (hiện chữ) và password (hiện dấu *)
	*
	* @return false nếu người dùng nhấn ESC
	**********************************************************/
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

/**********************************************************
* @Description Chức năng Quản lý: phải đăng nhập thành công mới vào
*              được menu quản lý
**********************************************************/
void QuanLyBanHang::quanLy()
{
	if (this->dangNhap())
	{
		this->menuQuanLy();
	}
}

/**********************************************************
* @Description Đăng nhập nhân viên, tối đa 3 lần
*
* @return true nếu đăng nhập thành công
* @attention Nhập sai 3 lần hoặc nhấn ESC thì kết thúc chương trình
**********************************************************/
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

/**********************************************************
* @Description Kiểm tra user và password có tồn tại trong Admin.txt không
*
* @return true nếu khớp một tài khoản
**********************************************************/
bool QuanLyBanHang::kiemTraTaiKhoan(const string& strUser, const string& strPassword) const
{
	const Admin* pAdmin = this->m_danhSachAdmin.timDau([&strUser, &strPassword](const Admin& admin)
	{
		return admin.kiemTra(strUser, strPassword);
	});
	return pAdmin != nullptr;
}

/**********************************************************
* @Description Menu quản lý: xử lý đơn hàng hoặc quản lý hàng hóa
**********************************************************/
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
		const string strLuaChon = TienIch::docLuaChon("Chọn chức năng: ");
		const char cLuaChon = (strLuaChon.size() == 1) ? strLuaChon[0] : '\0';
		switch (cLuaChon)
		{
			case '1':
				this->xuLyDonHang();
				TienIch::tamDung();
				break;
			case '2':
				this->menuQuanLyHangHoa();
				break;
			case '0':
				bTiepTuc = false;
				break;
			default:
				TienIch::inLoi("Lựa chọn không hợp lệ.");
				TienIch::tamDung();
				break;
		}
	}
}

/**********************************************************
* @Description Kiểm tra kho có đủ hàng cho toàn bộ đơn hàng không
*
* @return true nếu mọi mã hàng đều còn tồn tại và đủ số lượng
**********************************************************/
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

/**********************************************************
* @Description Xử lý đơn hàng đặt sớm nhất (đầu hàng đợi, FIFO) rồi bỏ
*              khỏi hàng đợi
*
* @attention Đủ hàng thì trừ kho, thiếu hàng thì hủy đơn. Ghi lại cả
*            hai file hàng hóa và đơn hàng
**********************************************************/
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

/**********************************************************
* @Description Trừ số lượng hàng trong kho theo đơn và chuyển đơn sang
*              trạng thái đã xử lý
*
* @attention Chỉ gọi sau khi kiemTraDuHang trả về true
**********************************************************/
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

/**********************************************************
* @Description Hủy đơn hàng vì không đủ hàng và báo cho nhân viên
**********************************************************/
void QuanLyBanHang::huyDonHang(DonHang& donHang)
{
	donHang.setTrangThai(DA_HUY);
	donHang.hienThi();
	TienIch::inLoi("Không đủ hàng hoặc mã hàng đã bị xóa, đơn hàng bị hủy.");
}