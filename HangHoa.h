#ifndef HANGHOA_H_INCLUDED_
#define HANGHOA_H_INCLUDED_

#include <string>
using namespace std;

class HangHoa
{
private:
    string _strMaHang;
    string _strTenHang;
    string _strNoiSanXuat;
    string _strMauSac;
    long long _lGiaBan;
    string _strNgayNhapKho;
    int _iSoLuong;

public:
    HangHoa();
    HangHoa(const string& strMaHang, const string& strTenHang,
        const string& strNoiSanXuat, const string& strMauSac,
        const long long lGiaBan, const string& strNgayNhapKho, const int iSoLuong);
    HangHoa(const HangHoa& hangHoa);
    ~HangHoa();

    string getMaHang() const;
    string getTenHang() const;
    long long getGiaBan() const;
    int getSoLuong() const;
    void setSoLuong(const int iSoLuong);

    string chuyenThanhDong() const;
    bool docTuDong(const string& strDong);
    void hienThi() const;
    static void hienThiTieuDe();
};

#endif