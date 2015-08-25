// ContentEncoding.h: interface for the CContentEncoding class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONTENTENCODING_H__074EE78B_FF1C_49E1_9374_3F957544960B__INCLUDED_)
#define AFX_CONTENTENCODING_H__074EE78B_FF1C_49E1_9374_3F957544960B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CContentEncoding : public CNodeRoot  
{
public:
	CContentEncoding();
	virtual ~CContentEncoding();

	int		Init();
	void	Free();
	
	int		Parse(const char *hvalue);
	int		ToStr(char** dest);

	int		Clone(CContentEncoding** dest);

	char*	value;	/**< value for Content-Length (size of attachments) */

};

#endif // !defined(AFX_CONTENTENCODING_H__074EE78B_FF1C_49E1_9374_3F957544960B__INCLUDED_)
