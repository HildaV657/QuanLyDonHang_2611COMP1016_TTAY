#ifndef DANHSACH_H_INCLUDED_
#define DANHSACH_H_INCLUDED_

template <class T>
class DanhSach
{
private:
	struct SNODE
	{
		T Data;
		SNODE* Next;

		SNODE(const T& data) : Data(data), Next(nullptr)
		{
		}
	};

	SNODE* m_pHead;
	SNODE* m_pTail;
	int m_iSize;

	void sao(const DanhSach& other)
	{
		for (SNODE* pNode = other.m_pHead; pNode != nullptr; pNode = pNode->Next)
		{
			this->themCuoi(pNode->Data);
		}
	}

public:

	DanhSach() : m_pHead(nullptr), m_pTail(nullptr), m_iSize(0)
	{
	}

	DanhSach(const DanhSach& other) : m_pHead(nullptr), m_pTail(nullptr), m_iSize(0)
	{
		this->sao(other);
	}

	~DanhSach()
	{
		this->xoaTatCa();
	}
	DanhSach& operator=(const DanhSach& other)
	{
		if (this != &other)
		{
			this->xoaTatCa();
			this->sao(other);
		}
		return *this;
	}

	void themCuoi(const T& data)
	{
		SNODE* pNew = new SNODE(data);
		if (this->m_pHead == nullptr)
		{
			this->m_pHead = pNew;
		}
		else
		{
			this->m_pTail->Next = pNew;
		}
		this->m_pTail = pNew;
		++this->m_iSize;
	}

	bool xoaTaiViTri(const int iViTri)
	{
		if (iViTri < 0 || iViTri >= this->m_iSize)
		{
			return false;
		}
		SNODE* pPrev = nullptr;
		SNODE* pCurrent = this->m_pHead;
		for (int iIndex = 0; iIndex < iViTri; ++iIndex)
		{
			pPrev = pCurrent;
			pCurrent = pCurrent->Next;
		}
		if (pPrev == nullptr)
		{
			this->m_pHead = pCurrent->Next;
		}
		else
		{
			pPrev->Next = pCurrent->Next;
		}
		if (pCurrent == this->m_pTail)
		{
			this->m_pTail = pPrev;
		}
		delete pCurrent;
		--this->m_iSize;
		return true;
	}

	template <class TDieuKien>
	T* timDau(TDieuKien dieuKien)
	{
		for (SNODE* pNode = this->m_pHead; pNode != nullptr; pNode = pNode->Next)
		{
			if (dieuKien(pNode->Data))
			{
				return &pNode->Data;
			}
		}
		return nullptr;
	}

	template <class TDieuKien>
	const T* timDau(TDieuKien dieuKien) const
	{
		return const_cast<DanhSach*>(this)->timDau(dieuKien);
	}

	template <class TDieuKien>
	int timViTri(TDieuKien dieuKien) const
	{
		int iViTri = 0;
		for (const SNODE* pNode = this->m_pHead; pNode != nullptr; pNode = pNode->Next)
		{
			if (dieuKien(pNode->Data))
			{
				return iViTri;
			}
			++iViTri;
		}
		return -1;
	}

	template <class THanhDong>
	void duyet(THanhDong hanhDong)
	{
		for (SNODE* pNode = this->m_pHead; pNode != nullptr; pNode = pNode->Next)
		{
			hanhDong(pNode->Data);
		}
	}

	template <class THanhDong>
	void duyet(THanhDong hanhDong) const
	{
		for (const SNODE* pNode = this->m_pHead; pNode != nullptr; pNode = pNode->Next)
		{
			hanhDong(pNode->Data);
		}
	}

	T* layTaiViTri(const int iViTri)
	{
		if (iViTri < 0 || iViTri >= this->m_iSize)
		{
			return nullptr;
		}
		SNODE* pNode = this->m_pHead;
		for (int iIndex = 0; iIndex < iViTri; ++iIndex)
		{
			pNode = pNode->Next;
		}
		return &pNode->Data;
	}

	const T* layTaiViTri(const int iViTri) const
	{
		return const_cast<DanhSach*>(this)->layTaiViTri(iViTri);
	}

	T* layDau()
	{
		return (this->m_pHead == nullptr) ? nullptr : &this->m_pHead->Data;
	}

	const T* layDau() const
	{
		return const_cast<DanhSach*>(this)->layDau();
	}

	int getSize() const
	{
		return this->m_iSize;
	}

	bool isEmpty() const
	{
		return this->m_iSize == 0;
	}

	void xoaTatCa()
	{
		while (this->m_pHead != nullptr)
		{
			SNODE* pNext = this->m_pHead->Next;
			delete this->m_pHead;
			this->m_pHead = pNext;
		}
		this->m_pTail = nullptr;
		this->m_iSize = 0;
	}
};

#endif