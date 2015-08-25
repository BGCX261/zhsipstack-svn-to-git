// Header.h: interface for the CHeader class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HEADER_H__1284B8E5_CA39_4C28_AFC8_1494F5E96CC8__INCLUDED_)
#define AFX_HEADER_H__1284B8E5_CA39_4C28_AFC8_1494F5E96CC8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CHeader : public CNodeRoot  
{
public:
	CHeader();
	virtual ~CHeader();

	int		Init();
	void	Free();
	void	Parse(const char *pcValue);
	int		ToStr(char** dest);

	char*	GetName();
	char*	GetValue();
	void	SetName(char* name);
	void	SetValue(char* value);

	char*	hname;	 /**< Name of header */
	char*	hvalue;    /**< Value for header */
};

int osip_header_clone(CHeader* header, CHeader** dest);

#endif // !defined(AFX_HEADER_H__1284B8E5_CA39_4C28_AFC8_1494F5E96CC8__INCLUDED_)
