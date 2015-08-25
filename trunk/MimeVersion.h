// MimeVersion.h: interface for the CMimeVersion class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MIMEVERSION_H__EB6F0E1F_F16C_4874_8138_8B0C9073A06F__INCLUDED_)
#define AFX_MIMEVERSION_H__EB6F0E1F_F16C_4874_8138_8B0C9073A06F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMimeVersion : public CNodeRoot  
{
public:
	CMimeVersion();
	virtual ~CMimeVersion();

	int		Init();
	void	Free();
	
	int		Parse(const char *hvalue);
	int		ToStr(char** dest);

	int		Clone(CMimeVersion** dest);

	
	
	char*	value;	/**< value for Content-Length (size of attachments) */

};

#endif // !defined(AFX_MIMEVERSION_H__EB6F0E1F_F16C_4874_8138_8B0C9073A06F__INCLUDED_)
