// AuthorizationInfo.h: interface for the CAuthorizationInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AUTHORIZATIONINFO_H__650CC15C_69B6_417C_9066_DFBCB90FC9F2__INCLUDED_)
#define AFX_AUTHORIZATIONINFO_H__650CC15C_69B6_417C_9066_DFBCB90FC9F2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAuthorizationInfo : public CNodeRoot  
{
public:
	CAuthorizationInfo();
	virtual ~CAuthorizationInfo();
	
	int		Init();
	void	Free();
	int		Parse(const char* hvalue);
	int		ToStr(char** dest);

	int		Clone(CAuthorizationInfo** dest);

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

#endif // !defined(AFX_AUTHORIZATIONINFO_H__650CC15C_69B6_417C_9066_DFBCB90FC9F2__INCLUDED_)
