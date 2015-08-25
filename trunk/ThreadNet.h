// ThreadCtrl.h: interface for the CThreadCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_THREADCTRL_H__5AC8AEE7_2600_6E00_9FF0_EFF1E4C3F79B__INCLUDED_)
#define AFX_THREADCTRL_H__5AC8AEE7_2600_6E00_9FF0_EFF1E4C3F79B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "zhSocket.h"
#include "ZhMessage.h"
#include "Singleton.h"
#include "SipMessageParse.h"
#include "ThreadBase.h"
#include "SipTransactionManager.h"

#define  BUFFER_TEMP_SIZE 4096

class CThreadNet : public CThreadBase, public Singleton<CThreadNet>
{
public:
	//winsocket 初始化和释放接口
	bool InitSocket(int iMainVer = 2,int iMinVer = 2);
	bool FreeSocket();
	//普通通信信息线程
	virtual bool OnThreadCreate();
	virtual bool OnThreadDestory();
	bool ThreadWork();
	//服务器信息
	bool SetLocalSocketInfo(int iTransFamily,int iProtocol, unsigned short usLocalPort,const char* pcLocalIp);
	bool ConnectServer(char *pcSvrIp ,unsigned short usSvrPort);
	//bool SetLocalInfo(const TCHAR *pcIP, int iSvrPort);
	//协议处理类初始化管理
	int InitMessageParser();
	bool ParseMessage(char *pcMessage,int iLen);

	SOCKADDR_IN* GetLastCommAddr();

	int SendMessage(TCHAR *pcMessage,int iLen);
	int SendMessage(CSipTransaction *pTransaction,CSipMessage *pSipMessage,int iLen);
	int SendMessage(TCHAR *pcMessage,int iLen,SOCKADDR_IN *pAddr);

	int GetSockInfo(char **pcIp,int *iPort);

	SOCKET CreateFromPort(unsigned short &uiPort);

	int		SendOutMessage( CSipTransaction*tr, CSipMessage*sip, char*host, int iport, int outsocket );
protected:
	
	
	CZhSocket m_sokcet;
	char *m_pBuffTemp;
	SOCKADDR_IN m_CurSockAddrFrom;

	CString m_csSvrIP;
	int m_usSvrPort;
	
	char *m_pcLocalIp;
	int m_usLocalPort;
	int m_iTransFamily;
	int m_iProtocol;

	string m_strXmlHeader;
	HWND m_hWndMainDlg;
private:
	CThreadNet();
	virtual ~CThreadNet();
	int eXtl_update_local_target(CSipMessage* req);
	
	friend class Singleton<CThreadNet>;

};

#endif // !defined(AFX_THREADCTRL_H__5AC8AEE7_2600_4E00_9FF0_EFF1E4C3F79B__INCLUDED_)
