// ProxyAuthenticationInfo.h: interface for the CProxyAuthenticationInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROXYAUTHENTICATIONINFO_H__DAD32321_7552_4517_9926_A9A74B600645__INCLUDED_)
#define AFX_PROXYAUTHENTICATIONINFO_H__DAD32321_7552_4517_9926_A9A74B600645__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CProxyAuthenticationInfo : public CNodeRoot  
{
public:
	CProxyAuthenticationInfo();
	virtual ~CProxyAuthenticationInfo();

	int		Init();
	void	Free();
	int		Parse(const char* hvalue);
	int		ToStr(char** dest);	
	/**
	 * Clone a Authorization-id element.
	 * @param header The element to work on.
	 * @param dest A pointer on the copy of the element.
	 */
	int		Clone(CProxyAuthenticationInfo** dest);

	/**
	 * Get value of the nextnonce parameter from a Authenication-Info element.
	 * @param header The element to work on.
	 */
	char* GetNextNonce();
	/**
	 * Add the nextnonce parameter from a Authenication-Info element.
	 * @param header The element to work on.
	 * @param value The value of the new parameter.
	 */
	void SetNextNonce(char* value);
	/**
	 * Get value of the cnonce parameter from a Authenication-Info element.
	 * @param header The element to work on.
	 */
	char* GetCnonce();
	/**
	 * Add the cnonce parameter from a Authenication-Info element.
	 * @param header The element to work on.
	 * @param value The value of the new parameter.
	 */
	void SetCnonce(char* value);
	/**
	 * Get value of the qop_options parameter from a Authenication-Info element.
	 * @param header The element to work on.
	 */
	char* GetQopOptions();
	/**
	 * Add the qop_options parameter from a Authenication-Info element.
	 * @param header The element to work on.
	 * @param value The value of the new parameter.
	 */
	void SetQopOptions(char* value);
	/**
	 * Get value of the rspauth parameter from a Authenication-Info element.
	 * @param header The element to work on.
	 */
	char* GetRspauth();
	/**
	 * Add the rspauth parameter from a Authenication-Info element.
	 * @param header The element to work on.
	 * @param value The value of the new parameter.
	 */
	void SetRspauth(char* value);
	/**
	 * Get value of the nc parameter from a Authenication-Info element.
	 * @param header The element to work on.
	 */
	char* GetNonceCount();
	/**
	 * Add the nc parameter from a Authenication-Info element.
	 * @param header The element to work on.
	 * @param value The value of the new parameter.
	 */
	void SetNonceCount(char* value);


	char*	nextnonce;	/* optional */
	char*	qop_options;  /* optional */
	char*	rspauth;  	/* optional */
	char*	cnonce;   	/* optional */
	char*	nonce_count;  /* optional */

};

#endif // !defined(AFX_PROXYAUTHENTICATIONINFO_H__DAD32321_7552_4517_9926_A9A74B600645__INCLUDED_)
