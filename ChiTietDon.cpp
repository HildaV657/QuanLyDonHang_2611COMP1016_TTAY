#include "ChiTietDon.h"

ChiTietDon::ChiTietDon()
{
    _strMaHang="";
    _iSoLuong=0;
    _lThanhTien=0;
}

ChiTietDon::ChiTietDon(string maHang, int soLuong, long long thanhTien)
{
    _strMaHang=maHang;
    _iSoLuong=soLuong;
    _lThanhTien=thanhTien;
}

ChiTietDon::~ChiTietDon(){};

string ChiTietDon::getMaHang() const
{
    return _strMaHang;
}
int ChiTietDon::getSoLuong() const
{
    return _iSoLuong;
}
long long ChiTietDon::getThanhTien() const
{
    return _lThanhTien;
}