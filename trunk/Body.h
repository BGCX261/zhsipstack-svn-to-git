// Body.h: interface for the CBody class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BODY_H__53B71DF5_0057_4C2E_9231_9E02676655E6__INCLUDED_)
#define AFX_BODY_H__53B71DF5_0057_4C2E_9231_9E02676655E6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ContentType.h"
#include "Header.h"

class CBody : public CNodeRoot  
{
public:
	CBody();
	virtual ~CBody();

	int		Init();
	void	Free();
	int		Parse(const char* start_of_body,int iLength);
	virtual int		ToStr(char** dest);
	int		ToStr(char** dest, size_t* str_length);

	int		ParseMime(const char* start_of_body,int iLength);
	int		ParseHeader(const char* start_of_osip_body_header, const char** next_body);
	
	int		SetContentType(const char* hvalue);
	int		SetHeader(const char* hname,const char* hvalue);

	
	char*	body; 					   /**< buffer containing data */
	size_t	length;  				   /**< length of data */
	CZhList headers;   		   /**< List of headers (when mime is used) */
	CContentType* content_type; /**< Content-Type (when mime is used) */

};

int osip_body_clone(CBody* body, CBody** dest);

#endif // !defined(AFX_BODY_H__53B71DF5_0057_4C2E_9231_9E02676655E6__INCLUDED_)
