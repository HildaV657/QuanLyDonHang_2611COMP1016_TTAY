#include "Admin.h"
#include "TienIch.h"

#include <vector>

using namespace std;

const int SO_THONG_TIN_ADMIN = 2;
const int VI_TRI_USER = 0;
const int VI_TRI_PASSWORD = 1;

Admin::Admin(const string& strUser, const string& strPassword)
    : _strUser(strUser), _strPassword(strPassword) {}

Admin::Admin(const Admin& admin)
    : _strUser(admin._strUser), _strPassword(admin._strPassword) {}

Admin::~Admin() {}

string Admin::getUser() const
{
    return this->_strUser;
}

string Admin::getPassword() const
{
    return this->_strPassword;
}

void Admin::setUser(const string& strUser)
{
    this->_strUser = strUser;
}

void Admin::setPassword(const string& strPassword)
{
    this->_strPassword = strPassword;
}

/**********************************************************
* @Description Kiểm tra user và password có khớp với admin này không
*
* @return true nếu cả user và password đều khớp
**********************************************************/
bool Admin::kiemTra(const string& strUser, const string& strPassword) const
{
    return this->_strUser == strUser && this->_strPassword == strPassword;
}

/**********************************************************
* @Description Chuyển thông tin admin thành một dòng để ghi file
*
* @return Chuỗi dạng "user<DAU_NGAN_CACH>password"
**********************************************************/
string Admin::chuyenThanhDong() const
{
    return this->_strUser + DAU_NGAN_CACH + this->_strPassword;
}

/**********************************************************
* @Description Đọc thông tin admin từ một dòng của file
*
* @return true nếu dòng hợp lệ, false nếu sai định dạng
* @attention Không thay đổi dữ liệu admin nếu dòng không hợp lệ
**********************************************************/
bool Admin::docTuDong(const string& strDong)
{
    vector<string> vecTruong = TienIch::tach(strDong, DAU_NGAN_CACH);
    if (vecTruong.size() != SO_THONG_TIN_ADMIN || vecTruong[VI_TRI_USER].empty())
    {
        return false;
    }
    this->_strUser = vecTruong[VI_TRI_USER];
    this->_strPassword = vecTruong[VI_TRI_PASSWORD];
    return true;
}
