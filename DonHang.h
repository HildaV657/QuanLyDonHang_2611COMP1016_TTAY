
#ifndef DONHANG_H_INCLUDED_
#define DONHANG_H_INCLUDED_

#include <string>

#include "ChiTietDon.h"
#include "DanhSach.h"

using namespace std;

enum TrangThaiDon
{
    CHO_XU_LY = 0,
    DA_XU_LY = 1,
    DA_HUY = 2
};

class DonHang
{
private:

    int _iSoThuTu;
    TrangThaiDon _eTrangThai;

    string _strTenKhach;
    string _strDiaChi;
    string _strSoDienThoai;
    string _strNgayDat;

    long long _lTongTien;
    DanhSach<ChiTietDon> _dsChiTiet;


public:

    DonHang();
    DonHang(int iSoThuTu, TrangThaiDon eTrangThai, const string& strTenKhach, const string& strDiaChi, const string& strSoDienThoai, const string& strNgayDat, long long lTongTien);

    ~DonHang();

    int getSoThuTu() const;
    TrangThaiDon getTrangThai() const;
    string getTenKhach() const;
    string getDiaChi() const;
    string getSoDienThoai() const;
    string getNgayDat() const;
    long long getTongTien() const;
    const DanhSach<ChiTietDon>& getDanhSachChiTiet() const;

    void setSoThuTu(const int iSoThuTu);
    void setTrangThai(const TrangThaiDon eTrangThai);
    void setThongTinKhach(const string& strTenKhach, const string& strDiaChi, const string& strSoDienThoai, const string& strNgayDat);

    bool coMaHang(const string& strMaHang) const;
    
    void themChiTiet(const ChiTietDon& chiTiet);

    void capNhatTongTien();

    void xacNhanDonHang();

    void huyDonHang();

    string chuyenThanhDong() const;

    bool docTuDong(const string& strDong);

    void hienThi() const;
};

#endif