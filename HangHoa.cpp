#include "HangHoa.h"
#include "TienIch.h"
#include <iostream>
#include <vector>

using namespace std;

const size_t SO_TRUONG_HANG_HOA = 7;  // 7 trường thông tin: Ma, Ten, NoiSX, MauSac, Gia, NgayNhap, SoLuong

HangHoa::HangHoa()
{
    _lGiaBan = 0;
    _iSoLuong = 0;
}

HangHoa::HangHoa(const string& strMaHang, const string& strTenHang,
    const string& strNoiSanXuat, const string& strMauSac,
    const long long lGiaBan, const string& strNgayNhapKho, const int iSoLuong)
    : _strMaHang(strMaHang), _strTenHang(strTenHang), _strNoiSanXuat(strNoiSanXuat),
    _strMauSac(strMauSac), _lGiaBan(lGiaBan), _strNgayNhapKho(strNgayNhapKho), _iSoLuong(iSoLuong)
{}

HangHoa::HangHoa(const HangHoa& hangHoa)
    : _strMaHang(hangHoa._strMaHang), _strTenHang(hangHoa._strTenHang), _strNoiSanXuat(hangHoa._strNoiSanXuat),
    _strMauSac(hangHoa._strMauSac), _lGiaBan(hangHoa._lGiaBan), _strNgayNhapKho(hangHoa._strNgayNhapKho), _iSoLuong(hangHoa._iSoLuong)
{}

HangHoa::~HangHoa() {}

string HangHoa::getMaHang() const
{
    return this->_strMaHang;
}

string HangHoa::getTenHang() const
{
    return this->_strTenHang;
}

long long HangHoa::getGiaBan() const
{
    return this->_lGiaBan;
}

int HangHoa::getSoLuong() const
{
    return this->_iSoLuong;
}

void HangHoa::setSoLuong(const int iSoLuong)
{
    this->_iSoLuong = iSoLuong;
}

/**********************************************************
* @Description Chuyển hàng hóa thành một dòng để ghi file
* @attention Định dạng: Ma|Ten|NoiSX|MauSac|Gia|NgayNhap|SoLuong
**********************************************************/
string HangHoa::chuyenThanhDong() const
{
    return this->_strMaHang + DAU_NGAN_CACH + this->_strTenHang + DAU_NGAN_CACH
        + this->_strNoiSanXuat + DAU_NGAN_CACH + this->_strMauSac + DAU_NGAN_CACH
        + to_string(this->_lGiaBan) + DAU_NGAN_CACH + this->_strNgayNhapKho + DAU_NGAN_CACH
        + to_string(this->_iSoLuong);
}

/**********************************************************
* @Description Đọc hàng hóa từ một dòng của file
* @return false nếu dòng sai định dạng
**********************************************************/
bool HangHoa::docTuDong(const string& strDong)
{
    vector<string> vecTruong = TienIch::tach(strDong, DAU_NGAN_CACH);
    if (vecTruong.size() != SO_TRUONG_HANG_HOA) return false;
    if (!TienIch::laSoNguyen(vecTruong[4]) || !TienIch::laSoNguyen(vecTruong[6]))
    {
        return false;
    }
    this->_strMaHang = vecTruong[0];
    this->_strTenHang = vecTruong[1];
    this->_strNoiSanXuat = vecTruong[2];
    this->_strMauSac = vecTruong[3];
    this->_lGiaBan = stoll(vecTruong[4]);
    this->_strNgayNhapKho = vecTruong[5];
    this->_iSoLuong = stoi(vecTruong[6]);
    return true;
}

void HangHoa::hienThiTieuDe()
{
    TienIch::datMau(MAU_VANG);
    cout << TienIch::canTrai("Mã hàng", 9) << TienIch::canTrai("Tên hàng", 32)
        << TienIch::canTrai("Nơi sản xuất", 16) << TienIch::canTrai("Màu sắc", 12)
        << TienIch::canTrai("Giá bán", 14) << TienIch::canTrai("Ngày nhập", 12)
        << "Số lượng" << endl;
    cout << string(106, '-') << endl;
    TienIch::datMau(MAU_MAC_DINH);
}

void HangHoa::hienThi() const
{
    TienIch::datMau(MAU_XANH_DUONG);
    cout << TienIch::canTrai(this->_strMaHang, 9);
    TienIch::datMau(MAU_MAC_DINH);
    cout << TienIch::canTrai(this->_strTenHang, 32)
        << TienIch::canTrai(this->_strNoiSanXuat, 16)
        << TienIch::canTrai(this->_strMauSac, 12)
        << TienIch::canTrai(TienIch::dinhDangTien(this->_lGiaBan), 14)
        << TienIch::canTrai(this->_strNgayNhapKho, 12)
        << this->_iSoLuong << endl;
}