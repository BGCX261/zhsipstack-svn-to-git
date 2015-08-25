// ZhAuthinfo.h: interface for the CZhAuthinfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ZHAUTHINFO_H__F16A2F31_353B_4538_B62D_710288674CEF__INCLUDED_)
#define AFX_ZHAUTHINFO_H__F16A2F31_353B_4538_B62D_710288674CEF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CZhAuthinfo  
{
public:
	CZhAuthinfo();
	virtual ~CZhAuthinfo();

	int Init();
	void Free();

	char username[50];
	char userid[50];
	char passwd[50];
	char ha1[50];
	char realm[50];
};

#endif // !defined(AFX_ZHAUTHINFO_H__F16A2F31_353B_4538_B62D_710288674CEF__INCLUDED_)
