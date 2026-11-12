#include "TienIch.h"

#include <windows.h>
#include <conio.h>
#include <cctype>
#include <cstdlib>
#include <iostream>

using namespace std;

// Định dạng số tiền
const size_t SO_CHU_SO_MOT_NHOM = 3;
const char DAU_CHAM_NGAN_NHOM = '.';
const char DAU_GACH_NGANG = '-';

// Đọc phím
const char DAU_SAO = '*';
const int KY_TU_IN_DAU_TIEN = 32;
const int KY_TU_IN_CUOI_CUNG = 126;
const int PHIM_MO_RONG_1 = 0;
const int PHIM_MO_RONG_2 = 224;

// Đếm ký tự UTF-8: byte nối tiếp có dạng 10xxxxxx
const int MASK_BYTE_TIEP_THEO = 0xC0;
const int BYTE_TIEP_THEO = 0x80;

// Số nguyên
const size_t SO_CHU_SO_TOI_DA = 15;

// Ngày có dạng dd/mm/yyyy
const char DAU_GACH_CHEO = '/';
const size_t DO_DAI_NGAY = 10;
const size_t VI_TRI_SO_NGAY = 0;
const size_t VI_TRI_GACH_CHEO_1 = 2;
const size_t VI_TRI_SO_THANG = 3;
const size_t VI_TRI_GACH_CHEO_2 = 5;
const size_t VI_TRI_SO_NAM = 6;
const size_t SO_KY_TU_NGAY_THANG = 2;
const size_t SO_KY_TU_NAM = 4;
const int NAM_TOI_THIEU = 1900;
const int NGAY_TOI_THIEU = 1;
const int THANG_TOI_THIEU = 1;
const int SO_THANG_TRONG_NAM = 12;
const int THANG_HAI = 2;

// Phần tử đầu để trống, nhờ đó số tháng 1-12 dùng thẳng làm chỉ số
const int SO_NGAY_CAC_THANG[SO_THANG_TRONG_NAM + 1] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

// Quy tắc năm nhuận
const int CHU_KY_NAM_NHUAN = 4;
const int CHU_KY_THE_KY = 100;
const int CHU_KY_BON_THE_KY = 400;

/**********************************************************
* @Description Đổi màu chữ của console
*
* @attention iMau là mã màu của Windows (ví dụ 10 = xanh lá)
**********************************************************/
void TienIch::datMau(const int iMau)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), iMau);
}

/**********************************************************
* @Description Xóa sạch màn hình console
**********************************************************/
void TienIch::xoaManHinh()
{
    system("cls");
}

/**********************************************************
* @Description Dừng màn hình cho người dùng đọc, nhấn phím
*              bất kỳ thì chạy tiếp
**********************************************************/
void TienIch::tamDung()
{
    TienIch::datMau(MAU_MAC_DINH);
    cout << "\nNhấn phím bất kỳ để tiếp tục...";
    _getch();
}

/**********************************************************
* @Description In một dòng thông báo bằng màu chỉ định, in xong
*              thì trả về màu mặc định
**********************************************************/
void TienIch::inThongBao(const string& strNoiDung, const int iMau)
{
    TienIch::datMau(iMau);
    cout << strNoiDung << endl;
    TienIch::datMau(MAU_MAC_DINH);
}

/**********************************************************
* @Description In thông báo lỗi màu đỏ, có chữ [LỖI] ở đầu
**********************************************************/
void TienIch::inLoi(const string& strNoiDung)
{
    TienIch::inThongBao("[LỖI] " + strNoiDung, MAU_DO);
}

/**********************************************************
* @Description Kiểm tra chuỗi có rỗng hoặc chỉ toàn khoảng trắng
*
* @return true nếu chuỗi rỗng hoặc chỉ có khoảng trắng
**********************************************************/
bool TienIch::laRong(const string& strChuoi)
{
    return TienIch::catKhoangTrang(strChuoi).empty();
}

/**********************************************************
* @Description Bỏ khoảng trắng (space, tab, xuống dòng) ở đầu
*              và cuối chuỗi
*
* @return Chuỗi đã cắt, chuỗi toàn khoảng trắng trả về ""
**********************************************************/
string TienIch::catKhoangTrang(const string& strChuoi)
{
    size_t iDau = 0;
    size_t iCuoi = strChuoi.size();

    while (iDau < iCuoi && isspace(static_cast<unsigned char>(strChuoi[iDau])))
    {
        ++iDau;
    }
    while (iCuoi > iDau && isspace(static_cast<unsigned char>(strChuoi[iCuoi - 1])))
    {
        --iCuoi;
    }
    return strChuoi.substr(iDau, iCuoi - iDau);
}

/**********************************************************
* @Description Tách chuỗi thành nhiều phần theo ký tự ngăn cách
*              (dùng để đọc một dòng trong file txt)
*
* @return Danh sách các phần, ví dụ "a|b||c" tách bằng '|' được
*         {"a", "b", "", "c"}
* @attention Chuỗi rỗng trả về danh sách có đúng 1 phần rỗng
**********************************************************/
vector<string> TienIch::tach(const string& strChuoi, const char cNganCach)
{
    vector<string> vecKetQua;
    string strPhanHienTai = "";

    for (size_t iIndex = 0; iIndex < strChuoi.size(); ++iIndex)
    {
        if (strChuoi[iIndex] == cNganCach)
        {
            vecKetQua.push_back(strPhanHienTai);
            strPhanHienTai = "";
        }
        else
        {
            strPhanHienTai += strChuoi[iIndex];
        }
    }
    vecKetQua.push_back(strPhanHienTai);
    return vecKetQua;
}

/**********************************************************
* @Description Đổi chuỗi sang chữ thường (dùng để so sánh
*              không phân biệt hoa thường)
*
* @attention Chỉ đổi chữ A-Z. Chữ tiếng Việt có dấu được lưu
*            bằng UTF-8 nên giữ nguyên để không bị hỏng
**********************************************************/
string TienIch::chuThuong(const string& strChuoi)
{
    string strKetQua = strChuoi;

    for (size_t iIndex = 0; iIndex < strKetQua.size(); ++iIndex)
    {
        if (strKetQua[iIndex] >= 'A' && strKetQua[iIndex] <= 'Z')
        {
            strKetQua[iIndex] = static_cast<char>(strKetQua[iIndex] - 'A' + 'a');
        }
    }
    return strKetQua;
}

/**********************************************************
* @Description Thêm khoảng trắng vào cuối chuỗi cho đủ iRong
*              ký tự, dùng để in bảng thẳng cột
*
* @attention Đếm theo ký tự nhìn thấy chứ không đếm theo byte,
*            vì một chữ có dấu chiếm nhiều byte trong UTF-8
**********************************************************/
string TienIch::canTrai(const string& strChuoi, const int iRong)
{
    int iSoKyTu = 0;

    for (size_t iIndex = 0; iIndex < strChuoi.size(); ++iIndex)
    {
        int iByte = static_cast<unsigned char>(strChuoi[iIndex]);
        if ((iByte & MASK_BYTE_TIEP_THEO) != BYTE_TIEP_THEO)
        {
            ++iSoKyTu;
        }
    }

    string strKetQua = strChuoi;
    if (iSoKyTu < iRong)
    {
        strKetQua.append(static_cast<size_t>(iRong - iSoKyTu), ' ');
    }
    return strKetQua;
}

/**********************************************************
* @Description Định dạng số tiền có dấu chấm ngăn cách hàng
*              nghìn
*
* @return Ví dụ 15990000 thành "15.990.000"
**********************************************************/
string TienIch::dinhDangTien(const long long lSoTien)
{
    string strSo = to_string(lSoTien);
    string strDau = "";

    if (strSo[0] == DAU_GACH_NGANG)
    {
        strDau = DAU_GACH_NGANG;
        strSo.erase(0, 1);
    }

    string strKetQua = "";
    size_t iSoChuSo = strSo.size();
    for (size_t iViTri = 0; iViTri < iSoChuSo; ++iViTri)
    {
        // Số chữ số còn lại (tính cả chữ số này) chia hết cho 3 thì đứng trước nó là dấu chấm
        if (iViTri > 0 && (iSoChuSo - iViTri) % SO_CHU_SO_MOT_NHOM == 0)
        {
            strKetQua += DAU_CHAM_NGAN_NHOM;
        }
        strKetQua += strSo[iViTri];
    }
    return strDau + strKetQua;
}

/**********************************************************
* @Description Kiểm tra chuỗi chỉ gồm các chữ số 0-9
*
* @return false nếu chuỗi rỗng hoặc có ký tự khác chữ số
**********************************************************/
bool TienIch::laSoNguyen(const string& strChuoi)
{
    if (strChuoi.empty())
    {
        return false;
    }
    for (size_t iIndex = 0; iIndex < strChuoi.size(); ++iIndex)
    {
        if (strChuoi[iIndex] < '0' || strChuoi[iIndex] > '9')
        {
            return false;
        }
    }
    return true;
}

/**********************************************************
* @Description Kiểm tra ngày có đúng định dạng dd/mm/yyyy và
*              là ngày có thật trong lịch hay không
*
* @return true nếu hợp lệ, ví dụ 29/02/2024 hợp lệ còn
*         31/04/2026 hoặc 29/02/2026 thì không
**********************************************************/
bool TienIch::laNgayHopLe(const string& strNgay)
{
    if (strNgay.size() != DO_DAI_NGAY
        || strNgay[VI_TRI_GACH_CHEO_1] != DAU_GACH_CHEO
        || strNgay[VI_TRI_GACH_CHEO_2] != DAU_GACH_CHEO)
    {
        return false;
    }

    string strSoNgay = strNgay.substr(VI_TRI_SO_NGAY, SO_KY_TU_NGAY_THANG);
    string strSoThang = strNgay.substr(VI_TRI_SO_THANG, SO_KY_TU_NGAY_THANG);
    string strSoNam = strNgay.substr(VI_TRI_SO_NAM, SO_KY_TU_NAM);
    if (!TienIch::laSoNguyen(strSoNgay) || !TienIch::laSoNguyen(strSoThang) || !TienIch::laSoNguyen(strSoNam))
    {
        return false;
    }

    int iNgay = stoi(strSoNgay);
    int iThang = stoi(strSoThang);
    int iNam = stoi(strSoNam);
    if (iNam < NAM_TOI_THIEU || iThang < THANG_TOI_THIEU || iThang > SO_THANG_TRONG_NAM)
    {
        return false;
    }
    return iNgay >= NGAY_TOI_THIEU && iNgay <= TienIch::demNgayTrongThang(iThang, iNam);
}

/**********************************************************
* @Description In nhãn rồi đọc lựa chọn trong menu
*
* @return Lựa chọn đã bỏ khoảng trắng hai đầu, nhập rỗng thì
*         trả về "" (nơi gọi sẽ báo lựa chọn không hợp lệ)
**********************************************************/
string TienIch::docLuaChon(const string& strNhan)
{
    TienIch::datMau(MAU_XANH_LA);
    cout << strNhan;
    TienIch::datMau(MAU_TRANG);

    string strDong = "";
    if (!getline(cin, strDong))
    {
        cin.clear();
    }
    TienIch::datMau(MAU_MAC_DINH);
    return TienIch::catKhoangTrang(strDong);
}

/**********************************************************
* @Description Nhập một chuỗi bắt buộc phải có nội dung
*
* @return true nếu nhập hợp lệ, kết quả nằm trong strKetQua
* @attention Trả về false (và đã in lỗi) nếu để trống, chỉ nhập
*            khoảng trắng hoặc có ký tự '|' vì ký tự này dùng
*            để ngăn cách các trường trong file
**********************************************************/
bool TienIch::docChuoi(const string& strNhan, string& strKetQua)
{
    TienIch::datMau(MAU_XANH_LA);
    cout << strNhan;
    TienIch::datMau(MAU_TRANG);

    string strDong = "";
    bool bDocDuoc = static_cast<bool>(getline(cin, strDong));
    TienIch::datMau(MAU_MAC_DINH);
    if (!bDocDuoc)
    {
        cin.clear();
        return false;
    }

    strKetQua = TienIch::catKhoangTrang(strDong);
    if (strKetQua.empty())
    {
        TienIch::inLoi("Thông tin không được để trống hoặc chỉ nhập khoảng trắng.");
        return false;
    }
    if (strKetQua.find(DAU_NGAN_CACH) != string::npos)
    {
        TienIch::inLoi("Thông tin không được chứa ký tự '|'.");
        return false;
    }
    return true;
}

/**********************************************************
* @Description Nhập một số nguyên không âm, không nhỏ hơn
*              lToiThieu
*
* @return true nếu nhập hợp lệ, kết quả nằm trong lKetQua
* @attention Trả về false (và đã in lỗi) nếu nhập sai. Giới hạn
*            15 chữ số để stoll không bị tràn số
**********************************************************/
bool TienIch::docSoNguyen(const string& strNhan, long long& lKetQua, const long long lToiThieu)
{
    string strGiaTri = "";
    if (!TienIch::docChuoi(strNhan, strGiaTri))
    {
        return false;
    }
    if (!TienIch::laSoNguyen(strGiaTri) || strGiaTri.size() > SO_CHU_SO_TOI_DA)
    {
        TienIch::inLoi("Giá trị phải là số nguyên không âm hợp lệ.");
        return false;
    }

    long long lSo = stoll(strGiaTri);
    if (lSo < lToiThieu)
    {
        TienIch::inLoi("Giá trị phải lớn hơn hoặc bằng " + to_string(lToiThieu) + ".");
        return false;
    }
    lKetQua = lSo;
    return true;
}

/**********************************************************
* @Description Đọc từng phím cho đến khi nhấn Enter. Nếu bAn là
*              true thì hiện dấu * thay cho ký tự (dùng nhập
*              password)
*
* @return true khi nhấn Enter, false khi nhấn ESC
* @attention Phím Backspace xóa được ký tự vừa nhập. Phím mũi
*            tên và phím chức năng bị bỏ qua
**********************************************************/
bool TienIch::docPhim(string& strKetQua, const bool bAn)
{
    strKetQua = "";

    while (true)
    {
        int iPhim = _getch();

        if (iPhim == PHIM_ESC)
        {
            return false;
        }
        if (iPhim == PHIM_ENTER)
        {
            cout << endl;
            return true;
        }
        if (iPhim == PHIM_MO_RONG_1 || iPhim == PHIM_MO_RONG_2)
        {
            // Phím đặc biệt gửi thêm một mã nữa, đọc bỏ đi
            _getch();
            continue;
        }
        if (iPhim == PHIM_BACKSPACE && !strKetQua.empty())
        {
            strKetQua.erase(strKetQua.size() - 1);
            cout << "\b \b" << flush;
            continue;
        }
        if (iPhim >= KY_TU_IN_DAU_TIEN && iPhim <= KY_TU_IN_CUOI_CUNG)
        {
            char cKyTu = static_cast<char>(iPhim);
            strKetQua += cKyTu;
            cout << (bAn ? DAU_SAO : cKyTu) << flush;
        }
    }
}

/**********************************************************
* @Description Kiểm tra năm nhuận
*
* @return true nếu chia hết cho 4 mà không chia hết cho 100,
*         hoặc chia hết cho 400
**********************************************************/
bool TienIch::laNamNhuan(const int iNam)
{
    return (iNam % CHU_KY_NAM_NHUAN == 0 && iNam % CHU_KY_THE_KY != 0) || iNam % CHU_KY_BON_THE_KY == 0;
}

/**********************************************************
* @Description Cho biết một tháng có bao nhiêu ngày
*
* @return Số ngày của tháng iThang (1-12) trong năm iNam
* @attention Chỉ gọi khi iThang đã được kiểm tra nằm trong 1-12
**********************************************************/
int TienIch::demNgayTrongThang(const int iThang, const int iNam)
{
    int iSoNgay = SO_NGAY_CAC_THANG[iThang];

    if (iThang == THANG_HAI && TienIch::laNamNhuan(iNam))
    {
        ++iSoNgay;
    }
    return iSoNgay;
}
