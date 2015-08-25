// SipMessageParse.h: interface for the CSipMessageParse class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SIPMESSAGEPARSE_H__2F7DBBAD_5248_4A52_8FFD_5035315113BE__INCLUDED_)
#define AFX_SIPMESSAGEPARSE_H__2F7DBBAD_5248_4A52_8FFD_5035315113BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SipMessage.h"

class CSipMessageParse  
{
public:
	CSipMessageParse();
	virtual ~CSipMessageParse();

	

	int		Parse(CSipMessage **dest,char *pcBuffer,int iBufferLen);

	void	ReplaceAllLws(char* sip_message);
	int		FindNextCrlf(const char* start_of_header, const char** end_of_header);

	int		SipMessageStartLineParse(CSipMessage* dest, const char* buf, const char** headers);
	int		SipMessageStartLineParseResp(CSipMessage* dest, const char* buf, const char** headers);
	int		SipMessageStartLineParseReq(CSipMessage* dest, const char* buf, const char** headers);

	int		SipMessageParse(CSipMessage* dest,const char* buf, size_t length, int sipfrag=0);
	int		MessageHeadersParse(CSipMessage* sip, const char* start_of_header, const char** body);
	int		SipBodyParse(CSipMessage* sip, const char* start_of_buf, const char** next_body, size_t length);
	int		SipParseMultipleHeader(CSipMessage* sip, char* hname, char* hvalue);
	int		SipParseHeader(CSipMessage* sip, const char* hname, const char* hvalue);
	int		SipSetBody(CSipMessage* sip, const char* buf, size_t length);
	int		SipSetBodyMime(CSipMessage* sip, const char* buf, size_t length);	
	
	static	int	InitParsers();
	static	ParseMethod	FindParseMethod(const char *pcMethod);
	static	map<string,ParseMethod>	m_mapParseMethod;

public:
	//CSipMessage *m_pMessage;

};



#endif // !defined(AFX_SIPMESSAGEPARSE_H__2F7DBBAD_5248_4A52_8FFD_5035315113BE__INCLUDED_)
