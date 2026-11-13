#ifndef ADMIN_H_INCLUDED_
#define ADMIN_H_INCLUDED_

#include <string>

using namespace std;

class Admin
{
private:
    string _strUser;
    string _strPassword;

public:
    Admin(const string& strUser = "", const string& strPassword = "");
    Admin(const Admin& admin);
    ~Admin();

    string getUser() const;
    string getPassword() const;
    void setUser(const string& strUser);
    void setPassword(const string& strPassword);

    bool kiemTra(const string& strUser, const string& strPassword) const;
    string chuyenThanhDong() const;
    bool docTuDong(const string& strDong);
};

#endif
