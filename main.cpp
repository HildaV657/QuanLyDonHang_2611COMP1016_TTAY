//g++ *.cpp -o QuanLyBanHang.exe
//QuanLyBanHang.exe
#include <cstdlib>
#include <exception>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

#include "QuanLyBanHang.h"

using namespace std;

int main()
{
#ifdef _WIN32
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);
#endif

	try
	{
		QuanLyBanHang quanLyBanHang;
		quanLyBanHang.chay();
	}
	catch (const exception& loi)
	{
		cout << "Lỗi nghiêm trọng: " << loi.what() << endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}