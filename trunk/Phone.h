// Phone.h: interface for the CPhone class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PHONE_H__0BE1B20D_1460_440D_84A8_868ED79C1452__INCLUDED_)
#define AFX_PHONE_H__0BE1B20D_1460_440D_84A8_868ED79C1452__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SipTransaction.h"

class CPhone  
{
public:
	CPhone();
	virtual ~CPhone();

	void	SetListenAddr(int iTransFamily,int iProtocol, unsigned short iLocalPort,const char* pcLocalAddr,int iSecure);
	void	SetProxyServer(char *pcProxy ,unsigned short usProxyPort);

	void	SetPhoneIdentity(const char *pcUserName,const char *pcPassword,const char *pcDisplayName);
	
	int		Register();
	int		Invite(const char *to);
	
protected:
	int generating_request_out_of_dialog(CSipMessage** dest,const char* method, const char* to, const char* transport,
										 const char* from, const char* proxy);

protected:
	char*	m_pcLocalIp;
	unsigned short m_usLocalPort;
	char*	GetProxyUrl();
	int		m_iTransFamily;
	int		m_iProtocol;

	char*	m_pcProxyIp;
	unsigned short m_usProxyPort;

	char*	m_pcUserName;
	char*	m_pcPassword;
	char*	m_pcDisplayName;
	char*	GetLocalUrl();

	
};

#endif // !defined(AFX_PHONE_H__0BE1B20D_1460_440D_84A8_868ED79C1452__INCLUDED_)
