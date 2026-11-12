#ifndef TIENICH_H_INCLUDED_
#define TIENICH_H_INCLUDED_

#include <string>
#include <vector>

// Mã màu của console Windows
const int MAU_MAC_DINH = 7;
const int MAU_XANH_LA = 10;
const int MAU_XANH_DUONG = 11;
const int MAU_DO = 12;
const int MAU_VANG = 14;
const int MAU_TRANG = 15;

// Mã phím
const int PHIM_ESC = 27;
const int PHIM_ENTER = 13;
const int PHIM_BACKSPACE = 8;

// Ký tự ngăn cách các trường dữ liệu trong file txt
const char DAU_NGAN_CACH = '|';

/**********************************************************
* @Description Các hàm tiện ích dùng chung cho cả chương trình:
*              màu sắc, nhập liệu, kiểm tra và xử lý chuỗi
**********************************************************/
class TienIch
{
public:
    // Giao diện console
    static void datMau(const int iMau);
    static void xoaManHinh();
    static void tamDung();
    static void inThongBao(const std::string& strNoiDung, const int iMau);
    static void inLoi(const std::string& strNoiDung);

    // Xử lý chuỗi và kiểm tra dữ liệu
    static bool laRong(const std::string& strChuoi);
    static std::string catKhoangTrang(const std::string& strChuoi);
    static std::vector<std::string> tach(const std::string& strChuoi, const char cNganCach);
    static std::string chuThuong(const std::string& strChuoi);
    static std::string canTrai(const std::string& strChuoi, const int iRong);
    static std::string dinhDangTien(const long long lSoTien);
    static bool laSoNguyen(const std::string& strChuoi);
    static bool laNgayHopLe(const std::string& strNgay);

    // Nhập dữ liệu từ bàn phím
    static std::string docLuaChon(const std::string& strNhan);
    static bool docChuoi(const std::string& strNhan, std::string& strKetQua);
    static bool docSoNguyen(const std::string& strNhan, long long& lKetQua, const long long lToiThieu);
    static bool docPhim(std::string& strKetQua, const bool bAn);

private:
    static bool laNamNhuan(const int iNam);
    static int demNgayTrongThang(const int iThang, const int iNam);
};

#endif
