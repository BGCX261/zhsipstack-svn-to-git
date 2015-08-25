// ThreadCtrl.cpp: implementation of the CThreadCtrl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ThreadNet.h"
#include "SipMessage.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CThreadNet::CThreadNet()
	:m_strXmlHeader("<?xml version=\"1.0\" encoding=\"UTF-8\"?>")
{
	InitSocket(2,2);
	
	m_pBuffTemp = new char[BUFFER_TEMP_SIZE];
	memset(m_pBuffTemp,0,BUFFER_TEMP_SIZE);
	
	memset(&m_CurSockAddrFrom,0,sizeof(m_CurSockAddrFrom));
	
	InitMessageParser();

	m_csSvrIP = "";
	m_usSvrPort = 0;

	m_pcLocalIp = NULL;
	m_usLocalPort = 0;
	m_iTransFamily = AF_INET;
	m_iProtocol = SOCK_DGRAM;
}

CThreadNet::~CThreadNet()
{
	if (m_pcLocalIp != NULL)
	{
		delete m_pcLocalIp;
	}
}

bool CThreadNet::InitSocket(int iMainVer,int iMinVer)
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	
	wVersionRequested = MAKEWORD( iMainVer, iMinVer );
	err = WSAStartup(wVersionRequested,&wsaData);
	if (err!=0)
	{
		return false;
	}
	
	if ( LOBYTE( wsaData.wVersion ) != iMainVer || HIBYTE( wsaData.wVersion ) != iMinVer ) 
	{
		/* Tell the user that we could not find a usable */
		/* WinSock DLL.                                  */
		WSACleanup( );
		return false; 
	}
	
	return true;
}

bool CThreadNet::FreeSocket()
{
	return WSACleanup() == 0 ? true : false;
}

bool CThreadNet::SetLocalSocketInfo(int iTransFamily,int iProtocol, unsigned short usLocalPort,const char* pcLocalIp)
{	
	if (!InitSocket(2,2))
	{
		return false;
	}
	
	char *pIP = CZhSocket::GetLocalIp();
	if ( !m_sokcet.Create(iTransFamily,iProtocol,pcLocalIp,usLocalPort) )
	{
		return false;
	}
	else
	{
		m_sokcet.SetSockAsync(false);
	}
	
	m_iTransFamily = iTransFamily;
	m_iProtocol = iProtocol;
	m_pcLocalIp = strdup(pcLocalIp);
	m_usLocalPort = usLocalPort;

	return true;
}

/*
bool CThreadNet::SetServerInfo( const TCHAR *pcIP, int iSvrPort )
{
	m_csSvrIP = pcIP;
	m_iSvrPort = iSvrPort;
	
	if (!InitSocket(2,2))
	{
		return false;
	}
	
	if ( !m_sokcet.Create(SOCK_DGRAM) )
	{
		return false;
	}
	else
	{
		m_sokcet.SetSockAsync(false);
	}
	
	return m_sokcet.Connect(m_csSvrIP,m_iSvrPort);
}*/

/*
bool CThreadNet::SetLocalInfo( const TCHAR *pcIP, int iSvrPort )
{
	m_csSvrIP = pcIP;
	m_iSvrPort = iSvrPort;

	if (!InitSocket(2,2))
	{
		return false;
	}
	
	if ( !m_sokcet.Create(SOCK_DGRAM) )
	{
		return false;
	}
	else
	{
		m_sokcet.SetSockAsync(false);
	}
	
	return m_sokcet.Connect(m_csSvrIP,m_iSvrPort);
}*/


bool CThreadNet::ConnectServer(char *pcSvrIp ,unsigned short usSvrPort)
{
	m_csSvrIP = pcSvrIp;
	m_usSvrPort = usSvrPort;

	if (m_sokcet.GetSocket() == INVALID_SOCKET)
	{
		if ( !m_sokcet.Create(m_iTransFamily,m_iProtocol,m_pcLocalIp,m_usLocalPort) )
		{
			return false;
		}
	}
	
	return m_sokcet.Connect(m_csSvrIP,m_usSvrPort);
}

bool CThreadNet::OnThreadCreate()
{
	return true;
}

bool CThreadNet::OnThreadDestory()
{
	return FreeSocket();
}


bool CThreadNet::ThreadWork()
{
	if (!OnThreadCreate())
		return false;	
	
	int iSockAddrLen = sizeof(m_CurSockAddrFrom);
	int iRecvLen = 0;
	while (!m_bThreadExit)
	{
		memset(m_pBuffTemp,0,BUFFER_TEMP_SIZE);
		memset(&m_CurSockAddrFrom,0,iSockAddrLen);
		
		iRecvLen = m_sokcet.ReadSelect(m_pBuffTemp,BUFFER_TEMP_SIZE,(struct sockaddr*)&m_CurSockAddrFrom,&iSockAddrLen);
		if (iRecvLen <= 0)
			continue;
		
		//TRACE(m_pBuffTemp);
		
		if ( ParseMessage(m_pBuffTemp,iRecvLen) )
		{
			
		}
		
	}
	
	return true;
}

int CThreadNet::InitMessageParser()
{
	
	
	return 0;
}

bool CThreadNet::ParseMessage( char *pcMessage ,int iLen)
{
	bool bResult = false;
	
	CSipMessageParse sipParse;
	
	CSipMessage *sip;
	
	sipParse.Parse(&sip,pcMessage,iLen);

	CSipTransactionManager *psipManager = CSipTransactionManager::getInstance();
	
	psipManager->HandleInComming(sip);

	
	return bResult;
}

SOCKADDR_IN* CThreadNet::GetLastCommAddr()
{
	return &m_CurSockAddrFrom;	
}

int CThreadNet::SendMessage( TCHAR *pcMessage,int iLen )
{
	
	return m_sokcet.WriteTo(pcMessage,iLen,m_csSvrIP,m_usSvrPort);
}

int CThreadNet::SendMessage( TCHAR *pcMessage,int iLen,SOCKADDR_IN *pAddr )
{
	if (pAddr == NULL) return -1;
	
	return m_sokcet.WriteTo(pAddr,pcMessage,iLen);
}

int CThreadNet::SendMessage( CSipTransaction *pTransaction,CSipMessage *pSipMessage,int iLen )
{
	if (pTransaction == NULL || pSipMessage == NULL)
	{
		return -1;
	}



	return 0;
}

int CThreadNet::GetSockInfo( char **pcIp,int *iPort )
{
	if (m_sokcet.GetSocket() == INVALID_SOCKET)
	{
		return -1;
	}
		
	*pcIp = m_sokcet.GetSockName();


	return 0;
}

static char dtls_firewall_ip[64];
static char dtls_firewall_port[10];

int CThreadNet::eXtl_update_local_target(CSipMessage* req)
{
	int pos = 0;
	
	if (dtls_firewall_ip != '\0')
	{
		while (!req->m_contacts.IsListEof(pos))
		{
			CContact* co;
			
			co = (CContact*) req->m_contacts.GetAt(pos);
			pos++;
			if (co !=
				NULL &&
				co->url !=
				NULL &&
				co->url->host !=
				NULL &&
				0 ==
				zstr_strcasecmp(co->url->host,dtls_firewall_ip))
			{
				if (co->url->port == NULL &&
					0 != zstr_strcasecmp(dtls_firewall_port,"5061"))
				{
					co->url->port = zstr_strdup(dtls_firewall_port);
				}
				else if (co->url->port != NULL &&
					0 != zstr_strcasecmp(dtls_firewall_port,co->url->port))
				{
					delete(co->url->port);
					co->url->port = zstr_strdup(dtls_firewall_port);
				}
			}
		}
	}
	
	return 0;
}

int CThreadNet::SendOutMessage( CSipTransaction*tr, CSipMessage*sip, char*host, int iport, int outsocket )
{
	int len = 0;
	size_t length = 0;
	
	char* message;
	
	int i = 0;
	
	if (host == NULL)
	{
		host = sip->m_pRequestUrl->host;
		if (sip->m_pRequestUrl->port != NULL)
			iport = zstr_atoi(sip->m_pRequestUrl->port);
		else
			iport = 5060;
	}
	
	if (MSG_IS_REQUEST(sip))
	{
		if (MSG_IS_REGISTER(sip) ||
			MSG_IS_INVITE(sip) ||
			MSG_IS_SUBSCRIBE(sip) ||
			MSG_IS_NOTIFY(sip))
			eXtl_update_local_target(sip);
	}
	
	SOCKADDR_IN addr;
	memset(&addr,0,sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(host);
	addr.sin_port = htons(iport);
	
	sip->ToStr(&message,&length);
	
	if (0 > sendto(m_sokcet.GetSocket(), (const char *) message, length, 0,
		(struct sockaddr *) &addr, len))
	{
#ifdef WIN32
		if (WSAECONNREFUSED == WSAGetLastError())
#else
			if (ECONNREFUSED == errno)
#endif
			{
				delete(message);
				return 1;
			}
			else
			{
				delete(message);
				return -1;
			}
	}
	
	return 0;
}
