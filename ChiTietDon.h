#ifndef CHITIETDON_H_INCLUDED_
#define CHITIETDON_H_INCLUDED_

#include <string>
using namespace std;

class ChiTietDon
{
private:
    string _strMaHang; 
    int _iSoLuong;
    long long _lThanhTien;

public:
    ChiTietDon();

    ChiTietDon(string maHang, int soLuong, long long thanhTien);

    ~ChiTietDon();

    string getMaHang() const;
    int getSoLuong() const;
    long long getThanhTien() const;
};

#endif