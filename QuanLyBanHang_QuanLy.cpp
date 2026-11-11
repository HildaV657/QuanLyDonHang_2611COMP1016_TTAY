#include "QuanLyBanHang.h"
#include "TienIch.h"

#include <iostream>
#include <string>

using namespace std;

const int SO_LAN_DANG_NHAP_TOI_DA = 3;

void QuanLyBanHang::quanLy()
{
    if (this->dangNhap())
    {
        this->menuQuanLy();
    }
}

/**********************************************************
* @Description Màn hình đăng nhập quản lý
*
* @return true nếu đăng nhập thành công
* @attention Trả về false và dừng chương trình nếu nhấn ESC
*            hoặc nhập sai quá SO_LAN_DANG_NHAP_TOI_DA lần
**********************************************************/
bool QuanLyBanHang::dangNhap()
{
    for (int iLanThu = 1; iLanThu <= SO_LAN_DANG_NHAP_TOI_DA; ++iLanThu)
    {
        TienIch::xoaManHinh();
        TienIch::inThongBao("  * * * * * * * * * * * * * * * * * * * * *", MAU_VANG);
        TienIch::inThongBao("  *          ĐĂNG NHẬP HỆ THỐNG           *", MAU_XANH_LA);
        TienIch::inThongBao("  * * * * * * * * * * * * * * * * * * * * *", MAU_VANG);
        TienIch::inThongBao("  (Nhấn ESC để thoát)\n", MAU_MAC_DINH);

        string strUser;
        string strPass;

        TienIch::datMau(MAU_XANH_LA);
        cout << "    User: ";
        TienIch::datMau(MAU_TRANG);
        if (!TienIch::docPhim(strUser, false))
        {
            this->_bDangChay = false;
            return false;
        }

        TienIch::datMau(MAU_XANH_LA);
        cout << "    Password: ";
        TienIch::datMau(MAU_TRANG);
        if (!TienIch::docPhim(strPass, true))
        {
            this->_bDangChay = false;
            return false;
        }
        TienIch::datMau(MAU_MAC_DINH);

        for (int iViTri = 0; iViTri < this->_dsAdmin.getSize(); ++iViTri)
        {
            Admin admin = this->_dsAdmin.get(iViTri);
            if (admin.kiemTra(strUser, strPass))
            {
                return true;
            }
        }

        TienIch::inLoi("Sai user hoặc password (lần " + to_string(iLanThu) + "/" + to_string(SO_LAN_DANG_NHAP_TOI_DA) + ").");
        TienIch::tamDung();
    }

    TienIch::inLoi("Nhập sai " + to_string(SO_LAN_DANG_NHAP_TOI_DA) + " lần. Thoát khỏi hệ thống");
    this->_bDangChay = false;
    return false;
}

void QuanLyBanHang::menuQuanLy()
{
    bool bTiepTuc = true;
    while (bTiepTuc)
    {
        TienIch::xoaManHinh();
        TienIch::inThongBao("=== MENU QUẢN LÝ ===", MAU_VANG);
        TienIch::inThongBao("  1. Xử lý đơn hàng (" + to_string(this->_hangDoiChoXuLy.getSize()) + " đơn đang chờ)", MAU_TRANG);
        TienIch::inThongBao("  2. Quản lý hàng hóa", MAU_TRANG);
        TienIch::inThongBao("  0. Quay lại menu chính", MAU_TRANG);

        string strLuaChon = TienIch::docLuaChon("Chọn chức năng: ");

        char cLuaChon = (strLuaChon.size() == 1) ? strLuaChon[0] : '\0';
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
            TienIch::inLoi("Lựa chọn không hợp lệ!");
            TienIch::tamDung();
            break;
        }
    }
}

/**********************************************************
* @Description Kiểm tra kho có đủ hàng cho toàn bộ đơn hàng không
*
* @return true nếu mọi mặt hàng đều tồn tại và đủ số lượng
**********************************************************/
bool QuanLyBanHang::kiemTraDuHang(CDonHang& donHang)
{
    const DanhSach<ChiTietDon>& dsChiTiet = donHang.getDanhSachChiTiet();
    for (int iViTri = 0; iViTri < dsChiTiet.getSize(); ++iViTri)
    {
        CChiTietDon chiTiet = dsChiTiet.get(iViTri);
        HangHoa* pHangHoa = this->timHangHoa(chiTiet.getMaHang());
        if (pHangHoa == nullptr || pHangHoa->getSoLuong() < chiTiet.getSoLuong())
        {
            return false;
        }
    }
    return true;
}

/**********************************************************
* @Description Xử lý đơn hàng đầu hàng đợi: trừ kho nếu đủ hàng,
*              ngược lại hủy đơn
*
* @attention Ghi lại file hàng hóa và file đơn hàng sau khi xử lý
**********************************************************/
void QuanLyBanHang::xuLyDonHang()
{
    TienIch::xoaManHinh();
    TienIch::inThongBao("=== XỬ LÝ ĐƠN HÀNG ===", MAU_VANG);
    if (this->_hangDoiChoXuLy.isEmpty())
    {
        TienIch::inThongBao("Không có đơn hàng nào đang chờ xử lý!", MAU_DO);
        return;
    }

    DonHang* pDonHang = this->timDonHang(this->_hangDoiChoXuLy.front());
    this->_hangDoiChoXuLy.dequeue();
    if (pDonHang == nullptr)
    {
        TienIch::inLoi("Không tìm thấy dữ liệu đơn hàng!");
        return;
    }

    if (this->kiemTraDuHang(*pDonHang))
    {
        const DanhSach<ChiTietDon>& dsChiTiet = pDonHang->getDanhSachChiTiet();
        for (int iViTri = 0; iViTri < dsChiTiet.getSize(); ++iViTri)
        {
            CChiTietDon chiTiet = dsChiTiet.get(iViTri);
            HangHoa* pHangHoa = this->timHangHoa(chiTiet.getMaHang());
            if (pHangHoa != nullptr)
            {
                int iSoLuongConLai = pHangHoa->getSoLuong() - chiTiet.getSoLuong();
                pHangHoa->setSoLuong(iSoLuongConLai);
            }
        }
        pDonHang->setTrangThai(DA_XU_LY);
        pDonHang->hienThi();
        TienIch::inThongBao("Đã xử lý xong đơn hàng và cập nhật số lượng hàng hóa!", MAU_XANH_LA);
    }
    else
    {
        pDonHang->setTrangThai(DA_HUY);
        pDonHang->hienThi();
        TienIch::inLoi("Không đủ hàng hoặc mã hàng đã bị xóa, đơn hàng bị hủy!");
    }

    this->ghiFileHangHoa();
    this->ghiFileDonHang();
}