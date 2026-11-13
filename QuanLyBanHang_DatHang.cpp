#include <string>

#include "QuanLyBanHang.h"
#include "TienIch.h"

using namespace std;

namespace
{
	const string TRA_LOI_CO = "c";
	const int SO_LUONG_DAT_TOI_THIEU = 1;
	const int SO_LUONG_KHO_TOI_THIEU = 0;   // số lượng nhỏ nhất của hàng trong kho (0 = hết hàng)
	const size_t SO_DIEN_THOAI_TOI_THIEU = 9;
	const size_t SO_DIEN_THOAI_TOI_DA = 11;

	/**********************************************************
	* @Description Nhập và kiểm tra số điện thoại
	*
	* @return true nếu chỉ gồm chữ số và dài từ 9 đến 11 ký tự
	**********************************************************/
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

	/**********************************************************
	* @Description Kiểm tra một mặt hàng có đặt thêm vào đơn được không
	*
	* @return true nếu mã hàng tồn tại, còn hàng và chưa có trong đơn
	* @attention Đã in lỗi nếu trả về false
	**********************************************************/
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
}

/**********************************************************
* @Description Nhập thông tin khách và các mã hàng để tạo đơn hàng mới
*
* @attention Có lỗi ở bất kỳ bước nào thì hủy việc đặt hàng, không lưu
*            đơn. Đặt thành công thì đơn vào hàng đợi chờ xử lý và ghi file
**********************************************************/
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

/**********************************************************
* @Description Nhập tên, địa chỉ, số điện thoại, ngày đặt của khách
*
* @return false nếu nhập sai hoặc để trống
**********************************************************/
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
	if (!nhapSoDienThoai(strSoDienThoai) || !TienIch::docChuoi("Ngày đặt hàng (dd/mm/yyyy): ", strNgayDat))
	{
		return false;
	}
	if (!TienIch::laNgayHopLe(strNgayDat))
	{
		TienIch::inLoi("Ngày đặt hàng không hợp lệ.");
		return false;
	}
	donHang.setThongTinKhach(strTenKhach, strDiaChi, strSoDienThoai, strNgayDat);
	return true;
}

/**********************************************************
* @Description Nhập lần lượt các mã hàng của đơn cho đến khi khách
*              không thêm nữa
*
* @return false nếu có lỗi nhập liệu
**********************************************************/
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

/**********************************************************
* @Description Nhập một mã hàng và số lượng rồi thêm vào đơn
*
* @return false nếu mã hàng không đặt được hoặc số lượng vượt quá
*         số hàng đang có
**********************************************************/
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