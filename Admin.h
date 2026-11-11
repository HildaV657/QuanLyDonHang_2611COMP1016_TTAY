#ifndef ADMIN_H_INCLUDED_
#define ADMIN_H_INCLUDED_

#include <string>

class Admin
{
private:
    std::string _strUser;
    std::string _strPassword;

public:
    Admin(const std::string& strUser = "", const std::string& strPassword = "");
    Admin(const Admin& admin);
    ~Admin();

    std::string getUser() const;
    std::string getPassword() const;
    void setUser(const std::string& strUser);
    void setPassword(const std::string& strPassword);

    bool kiemTra(const std::string& strUser, const std::string& strPassword) const;
    std::string chuyenThanhDong() const;
    bool docTuDong(const std::string& strDong);
};

#endif