// NodeRoot.h: interface for the CNodeRoot class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NODEROOT_H__62704655_1C9C_4B98_BF15_28A8F0737CF9__INCLUDED_)
#define AFX_NODEROOT_H__62704655_1C9C_4B98_BF15_28A8F0737CF9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CNodeRoot  
{
public:
	CNodeRoot();
	virtual ~CNodeRoot();

	int		Init();
	void	Free();
	int		Parse(const char *hvalue);
	
	int		Clone(CNodeRoot** dest);
	virtual int		ToStr(char** dest) = 0;
};

#endif // !defined(AFX_NODEROOT_H__62704655_1C9C_4B98_BF15_28A8F0737CF9__INCLUDED_)
