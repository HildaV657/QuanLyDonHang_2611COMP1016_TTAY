#include "DonHang.h"
#include "TienIch.h"

#include <iostream>
#include <vector>

using namespace std;

const size_t SO_TRUONG_CO_DINH = 8;
const size_t SO_TRUONG_MOI_CHI_TIET = 3;

DonHang::DonHang()
{
    _iSoThuTu = 0;
    _eTrangThai = CHO_XU_LY;
    _strTenKhach = "";
    _strDiaChi = "";
    _strSoDienThoai = "";
    _strNgayDat = "";
    _lTongTien = 0;
}

DonHang::DonHang(int soThuTu, TrangThaiDon trangThai, const string& tenKhach, const string& diaChi, const string& soDienThoai, const string& ngayDat, long long tongTien)
{
    _iSoThuTu = soThuTu;
    _eTrangThai = trangThai;

    _strTenKhach = tenKhach;
    _strDiaChi = diaChi;
    _strSoDienThoai = soDienThoai;
    _strNgayDat = ngayDat;

    _lTongTien = tongTien;
}

DonHang::~DonHang(){}

int DonHang::getSoThuTu() const
{
    return _iSoThuTu;
}

TrangThaiDon DonHang::getTrangThai() const
{
    return _eTrangThai;
}

string DonHang::getTenKhach() const
{
    return _strTenKhach;
}

string DonHang::getDiaChi() const
{
    return _strDiaChi;
}

string DonHang::getSoDienThoai() const
{
    return _strSoDienThoai;
}

string DonHang::getNgayDat() const
{
    return _strNgayDat;
}

long long DonHang::getTongTien() const
{
    return _lTongTien;
}

const DanhSach<ChiTietDon>& DonHang::getDanhSachChiTiet() const
{
    return _dsChiTiet;
}

void DonHang::setSoThuTu(int soThuTu)
{
    if (soThuTu >= 0)
    {
        _iSoThuTu = soThuTu;
    }
}

void DonHang::setTrangThai(TrangThaiDon trangThai)
{
    if (trangThai >= CHO_XU_LY && trangThai <= DA_HUY)
    {
        _eTrangThai = trangThai;
    }
}

void DonHang::setThongTinKhach( const string& tenKhach, const string& diaChi, const string& soDienThoai, const string& ngayDat)
{
    _strTenKhach = tenKhach;
    _strDiaChi = diaChi;
    _strSoDienThoai = soDienThoai;
    _strNgayDat = ngayDat;
}

bool DonHang::coMaHang(const string& maHang) const
{
    string maCanTim = TienIch::chuThuong(maHang);

    return _dsChiTiet.timDau(
        [&maCanTim](const ChiTietDon& chiTiet)
        {
            return TienIch::chuThuong(chiTiet.getMaHang()) == maCanTim;
        }
    ) != nullptr;
}

void DonHang::themChiTiet(const ChiTietDon& chiTiet)
{
    _dsChiTiet.themCuoi(chiTiet);

    capNhatTongTien();
}

void DonHang::capNhatTongTien()
{
    _lTongTien = 0;

    _dsChiTiet.duyet(
        [this](const ChiTietDon& chiTiet)
        {
            _lTongTien += chiTiet.getThanhTien();
        }
    );
}

void DonHang::xacNhanDonHang()
{
    if (_eTrangThai == CHO_XU_LY)
    {
        _eTrangThai = DA_XU_LY;
    }
}

void DonHang::huyDonHang()
{
    if (_eTrangThai == CHO_XU_LY)
    {
        _eTrangThai = DA_HUY;
    }
}

string DonHang::chuyenThanhDong() const
{
    string strDong = "";

    strDong = to_string(_iSoThuTu)
        + DAU_NGAN_CACH
        + to_string(static_cast<int>(_eTrangThai))
        + DAU_NGAN_CACH
        + _strTenKhach
        + DAU_NGAN_CACH
        + _strDiaChi
        + DAU_NGAN_CACH
        + _strSoDienThoai
        + DAU_NGAN_CACH
        + _strNgayDat
        + DAU_NGAN_CACH
        + to_string(_lTongTien)
        + DAU_NGAN_CACH
        + to_string(_dsChiTiet.getSize());

    _dsChiTiet.duyet(
        [&strDong](const ChiTietDon& chiTiet)
        {
            strDong += DAU_NGAN_CACH
                + chiTiet.getMaHang()
                + DAU_NGAN_CACH
                + to_string(chiTiet.getSoLuong())
                + DAU_NGAN_CACH
                + to_string(chiTiet.getThanhTien());
        }
    );

    return strDong;
}

bool DonHang::docTuDong(const string& strDong)
{
    vector<string> vecTruong =
        TienIch::tach(strDong, DAU_NGAN_CACH);

    if (vecTruong.size() < SO_TRUONG_CO_DINH)
    {
        return false;
    }

    try
    {
        int soThuTu = stoi(vecTruong[0]);
        int trangThai = stoi(vecTruong[1]);
        long long tongTien = stoll(vecTruong[6]);

        int soChiTiet = stoi(vecTruong[7]);

        if (soThuTu < 0 || tongTien < 0 || soChiTiet < 0)
        {
            return false;
        }

        if (trangThai < CHO_XU_LY || trangThai > DA_HUY)
        {
            return false;
        }

        size_t soTruongCanCo = SO_TRUONG_CO_DINH + SO_TRUONG_MOI_CHI_TIET * soChiTiet;

        if (vecTruong.size() != soTruongCanCo)
        {
            return false;
        }

        DanhSach<ChiTietDon> danhSachTam;

        long long tongTienTinhDuoc = 0;

        for (int i = 0; i < soChiTiet; i++)
        {
            size_t viTri = SO_TRUONG_CO_DINH + i * SO_TRUONG_MOI_CHI_TIET;

            string maHang = vecTruong[viTri];

            int soLuong = stoi(vecTruong[viTri + 1]);

            long long thanhTien = stoll(vecTruong[viTri + 2]);

            if (soLuong <= 0 || thanhTien < 0)
            {
                return false;
            }

            ChiTietDon chiTiet(maHang,soLuong,thanhTien);

            danhSachTam.themCuoi(chiTiet);

            tongTienTinhDuoc += thanhTien;
        }

        if (tongTienTinhDuoc != tongTien)
        {
            return false;
        }

        _iSoThuTu = soThuTu;

        _eTrangThai = static_cast<TrangThaiDon>(trangThai);

        setThongTinKhach(vecTruong[2], vecTruong[3], vecTruong[4], vecTruong[5]);

        _dsChiTiet = danhSachTam;

        _lTongTien = tongTienTinhDuoc;
    }
    catch (...)
    {
        return false;
    }

    return true;
}

void DonHang::hienThi() const
{
    string strTrangThai = "Đang chờ xử lý";

    switch (_eTrangThai)
    {
    case DA_XU_LY:
        strTrangThai = "Đã xử lý";
        break;

    case DA_HUY:
        strTrangThai = "Đã hủy (không đủ hàng)";
        break;

    default:
        break;
    }

    TienIch::datMau(MAU_XANH_DUONG);

    cout << "Đơn hàng số " << _iSoThuTu << " - " << strTrangThai << endl;

    TienIch::datMau(MAU_MAC_DINH);

    cout << "Khách hàng : " << _strTenKhach << endl;
    cout << "Địa chỉ    : " << _strDiaChi << endl;
    cout << "Điện thoại : " << _strSoDienThoai << endl;
    cout << "Ngày đặt   : " << _strNgayDat << endl;

    _dsChiTiet.duyet(
        [](const ChiTietDon& chiTiet)
        {
            cout << "  - Mã hàng " << chiTiet.getMaHang() << ", số lượng " << chiTiet.getSoLuong() << ", thành tiền " << TienIch::dinhDangTien(chiTiet.getThanhTien()) << endl;
        }
    );

    cout << "Tổng tiền  : " << TienIch::dinhDangTien(_lTongTien) << endl;
}