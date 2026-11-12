#include "QuanLyBanHang.h"
#include "TienIch.h"

#include <cctype>
#include <fstream>
#include <iostream>

using namespace std;

const string FILE_HANG_HOA = "HangHoa.txt";
const size_t DO_DAI_MA_HANG = 4;

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
        string strLuaChon = TienIch::docLuaChon("Chọn chức năng: ");
        if (strLuaChon == "0")
        {
            bTiepTuc = false;
            continue;
        }
        if (strLuaChon == "1")
        {
            this->themHangHoa();
        }
        else if (strLuaChon == "2")
        {
            this->xoaHangHoa();
        }
        else if (strLuaChon == "3")
        {
            this->capNhatSoLuong();
        }
        else
        {
            TienIch::inLoi("Lựa chọn không hợp lệ.");
        }
        TienIch::tamDung();
    }
}

void QuanLyBanHang::themHangHoa()
{
    TienIch::inThongBao("--- Thêm hàng hóa ---", MAU_VANG);
    string strMaHang;
    string strTenHang;
    string strNoiSanXuat;
    string strMauSac;
    string strNgayNhapKho;
    long long lGiaBan = 0;
    long long lSoLuong = 0;

    if (!TienIch::docChuoi("Mã hàng (4 ký tự): ", strMaHang))
    {
        return;
    }
    bool bCoKhoangTrang = strMaHang.find(' ') != string::npos;
    if (strMaHang.size() != DO_DAI_MA_HANG || bCoKhoangTrang)
    {
        TienIch::inLoi("Mã hàng phải gồm đúng 4 ký tự, không chứa khoảng trắng.");
        return;
    }
    if (this->timHangHoa(strMaHang) != nullptr)
    {
        TienIch::inLoi("Mã hàng đã tồn tại.");
        return;
    }
    if (!TienIch::docChuoi("Tên hàng: ", strTenHang) || !TienIch::docChuoi("Nơi sản xuất: ", strNoiSanXuat)
        || !TienIch::docChuoi("Màu sắc: ", strMauSac) || !TienIch::docSoNguyen("Giá bán: ", lGiaBan, 1)
        || !TienIch::docChuoi("Ngày nhập kho (dd/mm/yyyy): ", strNgayNhapKho))
    {
        return;
    }
    if (!TienIch::laNgayHopLe(strNgayNhapKho))
    {
        TienIch::inLoi("Ngày nhập kho không hợp lệ.");
        return;
    }
    if (!TienIch::docSoNguyen("Số lượng: ", lSoLuong, 0))
    {
        return;
    }
    this->m_dsHangHoa.themCuoi(HangHoa(strMaHang, strTenHang, strNoiSanXuat, strMauSac,
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
    string strMaThuong = TienIch::chuThuong(strMaHang);
    int iViTri = this->m_dsHangHoa.timViTri([&strMaThuong](const HangHoa& hangHoa)
    {
        return TienIch::chuThuong(hangHoa.getMaHang()) == strMaThuong;
    });
    if (iViTri < 0)
    {
        TienIch::inLoi("Mã hàng không tồn tại.");
        return;
    }
    this->m_dsHangHoa.xoaTaiViTri(iViTri);
    this->ghiFileHangHoa();
    TienIch::inThongBao("Đã xóa hàng hóa.", MAU_XANH_LA);
}

void QuanLyBanHang::capNhatSoLuong()
{
    TienIch::inThongBao("--- Cập nhật số lượng ---", MAU_VANG);
    string strMaHang;
    long long lSoLuongMoi = 0;
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
    if (!TienIch::docSoNguyen("Số lượng mới: ", lSoLuongMoi, 0))
    {
        return;
    }
    pHangHoa->setSoLuong(static_cast<int>(lSoLuongMoi));
    this->ghiFileHangHoa();
    TienIch::inThongBao("Cập nhật số lượng thành công.", MAU_XANH_LA);
}
