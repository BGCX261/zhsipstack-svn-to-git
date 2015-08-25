// WwwAuthenticate.h: interface for the CWwwAuthenticate class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WWWAUTHENTICATE_H__13637524_7498_48A0_B4AB_1154F0D0D68D__INCLUDED_)
#define AFX_WWWAUTHENTICATE_H__13637524_7498_48A0_B4AB_1154F0D0D68D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CWwwAuthenticate : public CNodeRoot  
{
public:
	CWwwAuthenticate();
	virtual ~CWwwAuthenticate();


	int		Init();
	void	Free();
	int		Parse(const char* hvalue);
	int		ToStr(char** dest);

	/**
	 * Get value of the auth_type parameter from a Www-Authenticate element.
	 * @param header The element to work on.
	 */
	char*	GetAuthType();
	/**
	 * Add the auth_type parameter from a Www-Authenticate element.
	 * @param header The element to work on.
	 * @param value The value of the new parameter.
	 */
	void	SetAuthType(char* value);
	/**
	 * Get value of the realm parameter from a Www-Authenticate element.
	 * @param header The element to work on.
	 */
	char*	GetRealm();
	/**
	 * Add the realm parameter from a Www-Authenticate element.
	 * @param header The element to work on.
	 * @param value The value of the new parameter.
	 */
	void	SetRealm(char* value);
	/**
	 * Get value of the domain parameter from a Www-Authenticate element.
	 * @param header The element to work on.
	 */
	char*	GetDomain();
	/**
	 * Add the domain parameter from a Www-Authenticate element.
	 * @param header The element to work on.
	 * @param value The value of the new parameter.
	 */
	void	SetDomain(char* value);
	/**
	 * Get value of the nonce parameter from a Www-Authenticate element.
	 * @param header The element to work on.
	 */
	char*	GetNonce();
	/**
	 * Add the nonce parameter from a Www-Authenticate element.
	 * @param header The element to work on.
	 * @param value The value of the new parameter.
	 */
	void	SetNonce(char* value);
	/**
	 * Get value of the opaque parameter from a Www-Authenticate element.
	 * @param header The element to work on.
	 */
	char*	GetOpaque();
	/**
	 * Add the opaque parameter from a Www-Authenticate element.
	 * @param header The element to work on.
	 * @param value The value of the new parameter.
	 */
	void	SetOpaque(char* value);
	/**
	 * Get value of the stale parameter from a Www-Authenticate element.
	 * @param header The element to work on.
	 */
	char*	GetStale();
	/**
	 * Add the stale parameter in a Www-Authenticate element.
	 * @param header The element to work on.
	 * @param value The value of the new parameter.
	 */
	void	SetStale(char* value);
	/**
	 * Add a stale parameter set to "true" in a Www-Authenticate element.
	 * @param header The element to work on.
	 */
#define osip_www_authenticate_set_stale_true(header) osip_www_authenticate_set_stale(header,zstr_strdup("true"))
	/**
	 * Add a stale parameter set to "false" in a Www-Authenticate element.
	 * @param header The element to work on.
	 */
#define osip_www_authenticate_set_stale_false(header) osip_www_authenticate_set_stale(header,zstr_strdup("false"))
	/**
	 * Get value of the algorithm parameter from a Www-Authenticate element.
	 * @param header The element to work on.
	 */
	char*	GetAlgorithm();
	/**
	 * Add the algorithm parameter in a Www-Authenticate element.
	 * @param header The element to work on.
	 * @param value The value of the new parameter.
	 */
	void	SetAlgorithm(char* value);
	/**
	 * Add the algorithm parameter set to "MD5" in a Www-Authenticate element.
	 * @param header The element to work on.
	 */
#define osip_www_authenticate_set_algorithm_MD5(header) osip_www_authenticate_set_algorithm(header,zstr_strdup("MD5"))
	/**
	 * Get value of the qop_options parameter from a Www-Authenticate element.
	 * @param header The element to work on.
	 */
	char*	GetQopOptions();
	/**
	 * Add the qop_options parameter from a Www-Authenticate element.
	 * @param header The element to work on.
	 * @param value The value of the new parameter.
	 */
	void	SetQopOptions(char* value);

	char*	auth_type;		/**< Authentication Type (Basic or Digest */
	char*	realm;		/**< realm (as a quoted-string) */
	char*	domain;		/**< domain (optional) */
	char*	nonce;		/**< nonce (optional)*/
	char*	opaque;		/**< opaque (optional) */
	char*	stale;		/**< stale (optional) */
	char*	algorithm;		/**< algorythm (optional) */
	char*	qop_options;		/**< qop option (optional)  */
	char*	auth_param;		/**< other parameters (optional) */

};

int osip_www_authenticate_clone(const CWwwAuthenticate* wwwa, CWwwAuthenticate** dest);

#endif // !defined(AFX_WWWAUTHENTICATE_H__13637524_7498_48A0_B4AB_1154F0D0D68D__INCLUDED_)
