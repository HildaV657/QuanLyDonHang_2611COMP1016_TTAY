#ifndef HANGDOI_H_INCLUDED_
#define HANGDOI_H_INCLUDED_

#include "DanhSach.h"

template <class T>
class HangDoi
{
private:
	DanhSach<T> m_danhSachPhanTu;

public:
	void enqueue(const T& data)
	{
		this->m_danhSachPhanTu.themCuoi(data);
	}

	bool dequeue()
	{
		return this->m_danhSachPhanTu.xoaTaiViTri(0);
	}

	T* front()
	{
		return this->m_danhSachPhanTu.layDau();
	}

	const T* front() const
	{
		return this->m_danhSachPhanTu.layDau();
	}


	template <class TDieuKien>
	T* timDau(TDieuKien dieuKien)
	{
		return this->m_danhSachPhanTu.timDau(dieuKien);
	}

	template <class TDieuKien>
	const T* timDau(TDieuKien dieuKien) const
	{
		return this->m_danhSachPhanTu.timDau(dieuKien);
	}

	template <class THanhDong>
	void duyet(THanhDong hanhDong)
	{
		this->m_danhSachPhanTu.duyet(hanhDong);
	}

	template <class THanhDong>
	void duyet(THanhDong hanhDong) const
	{
		this->m_danhSachPhanTu.duyet(hanhDong);
	}

	bool isEmpty() const
	{
		return this->m_danhSachPhanTu.isEmpty();
	}

	int getSize() const
	{
		return this->m_danhSachPhanTu.getSize();
	}

	void xoaTatCa()
	{
		this->m_danhSachPhanTu.xoaTatCa();
	}
};

#endif