#include "QuanLyBanHang.h"
#include "TienIch.h"
#include "DonHang.h"

#include <iostream>
#include <string>

using namespace std;

void QuanLyBanHang::datHang()
{
    TienIch::xoaManHinh();
    TienIch::inThongBao("=== ĐẶT HÀNG ===", MAU_VANG);

    DonHang donHang;

    if (!nhapThongTinKhach(donHang))
    {
        TienIch::inThongBao("Đã hủy việc đặt hàng.", MAU_DO);
        return;
    }

    if (!nhapChiTietDon(donHang))
    {
        TienIch::inThongBao("Đã hủy việc đặt hàng.", MAU_DO);
        return;
    }

    donHang.setSoThuTu(_iSoThuTuTiepTheo);
    ++_iSoThuTuTiepTheo;

    _dsDonHang.themCuoi(donHang);
    _hangDoiChoXuLy.enqueue(donHang.getSoThuTu());

    ghiFileDonHang();

    TienIch::inThongBao(
        "Đơn hàng đang chờ xử lý.",
        MAU_XANH_LA
    );
}

bool QuanLyBanHang::nhapThongTinKhach(DonHang& donHang) const
{
    string strTenKhach;
    string strDiaChi;
    string strSoDienThoai;
    string strNgayDat;

    if (!TienIch::docChuoi("Tên khách hàng: ", strTenKhach))
        return false;

    if (!TienIch::docChuoi("Địa chỉ khách hàng: ", strDiaChi))
        return false;

    if (!TienIch::docChuoi("Số điện thoại: ", strSoDienThoai))
        return false;

    if (!TienIch::laSoNguyen(strSoDienThoai) ||
        strSoDienThoai.size() < SDT_TOI_THIEU ||
        strSoDienThoai.size() > SDT_TOI_DA)
    {
        TienIch::inLoi("Số điện thoại phải gồm 9 đến 11 chữ số.");
        return false;
    }

    if (!TienIch::docChuoi("Ngày đặt hàng (dd/mm/yyyy): ", strNgayDat))
        return false;

    if (!TienIch::laNgayHopLe(strNgayDat))
    {
        TienIch::inLoi("Ngày đặt hàng không hợp lệ.");
        return false;
    }

    donHang.setThongTinKhach(strTenKhach, strDiaChi, strSoDienThoai, strNgayDat);

    return true;
}

bool QuanLyBanHang::nhapChiTietDon(DonHang& donHang) const
{
    string strMaHang;
    string strTraLoi;

    do
    {
        if (!TienIch::docChuoi("Mã hàng: ", strMaHang))
            return false;

        CHangHoa* pHangHoa = timHangHoa(strMaHang);

        if (pHangHoa == nullptr)
        {
            TienIch::inLoi("Mã hàng không tồn tại.");
            return false;
        }

        if (pHangHoa->getSoLuong() <= 0)
        {
            TienIch::inLoi("Mặt hàng này đã hết hàng.");
            return false;
        }

        if (donHang.coMaHang(pHangHoa->getMaHang()))
        {
            TienIch::inLoi("Mã hàng này đã có trong đơn.");
            return false;
        }

        long long lSoLuong = 0;

        if (!TienIch::docSoNguyen("Số lượng: ", lSoLuong, 1))
            return false;

        if (lSoLuong > pHangHoa->getSoLuong())
        {
            TienIch::inLoi( "Số lượng đặt vượt quá số lượng hiện có (" + to_string(pHangHoa->getSoLuong()) + ")." );
            return false;
        }

        CChiTietDon chiTiet(
            pHangHoa->getMaHang(),
            static_cast<int>(lSoLuong),
            pHangHoa->getGiaBan() * lSoLuong
        );

        donHang.themChiTiet(chiTiet);

        if (!TienIch::docChuoi(
            "Thêm mã hàng khác vào đơn? (c/k): ",
            strTraLoi))
        {
            return false;
        }

    } while (TienIch::chuThuong(strTraLoi) == "c");

    return true;
}
