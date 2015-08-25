// Authorization.h: interface for the CAuthorization class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AUTHORIZATION_H__BCD84F64_ABFF_4168_B462_471FD76D84EB__INCLUDED_)
#define AFX_AUTHORIZATION_H__BCD84F64_ABFF_4168_B462_471FD76D84EB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAuthorization : public CNodeRoot  
{
public:
	CAuthorization();
	virtual ~CAuthorization();
	
	int		Init();
	void	Free();
	int		Parse(const char* hvalue);
	int		ToStr(char** dest);

	/**
	 * Clone a Authorization-id element.
	 * @param header The element to work on.
	 * @param dest A pointer on the copy of the element.
	 */
	int		Clone(CAuthorization** dest);

		/**
	 * Get value of the auth_type parameter from a Authorization element.
	 * @param header The element to work on.
	 */
	char*	GetAuthType();
	/**
	 * Add the auth_type parameter from a Authorization element.
	 * @param header The element to work on.
	 * @param value The value of the new parameter.
	 */
	void	SetAuthType(char* value);
	/**
	 * Get value of the username parameter from a Authorization element.
	 * @param header The element to work on.
	 */
	char*	GetUserName();
	/**
	 * Add the username parameter from a Authorization element.
	 * @param header The element to work on.
	 * @param value The value of the new parameter.
	 */
	void	SetUserName(char* value);
	/**
	 * Get value of the realm parameter from a Authorization element.
	 * @param header The element to work on.
	 */
	char*	GetRealm();
	/**
	 * Add the realm parameter from a Authorization element.
	 * @param header The element to work on.
	 * @param value The value of the new parameter.
	 */
	void	SetRealm(char* value);
	/**
	 * Get value of the nonce parameter from a Authorization element.
	 * @param header The element to work on.
	 */
	char*	GetNonce();
	/**
	 * Add the nonce parameter from a Authorization element.
	 * @param header The element to work on.
	 * @param value The value of the new parameter.
	 */
	void	SetNonce(char* value);
	/**
	 * Get value of the uri parameter from a Authorization element.
	 * @param header The element to work on.
	 */
	char*	GetUri();
	/**
	 * Add the uri parameter from a Authorization element.
	 * @param header The element to work on.
	 * @param value The value of the new parameter.
	 */
	void	SetUri(char* value);
	/**
	 * Get value of the response parameter from a Authorization element.
	 * @param header The element to work on.
	 */
	char*	GetResponse();
	/**
	 * Add the response parameter from a Authorization element.
	 * @param header The element to work on.
	 * @param value The value of the new parameter.
	 */
	void	SetResponse(char* value);
	/**
	 * Get value of the digest parameter from a Authorization element.
	 * @param header The element to work on.
	 */
	char*	GetDigest();
	/**
	 * Add the digest parameter from a Authorization element.
	 * @param header The element to work on.
	 * @param value The value of the new parameter.
	 */
	void	SetDigest(char* value);
	/**
	 * Get value of the algorithm parameter from a Authorization element.
	 * @param header The element to work on.
	 */
	char*	GetAlgorithm();
	/**
	 * Add the algorithm parameter from a Authorization element.
	 * @param header The element to work on.
	 * @param value The value of the new parameter.
	 */
	void	SetAlgorithm(char* value);
	/**
	 * Get value of the cnonce parameter from a Authorization element.
	 * @param header The element to work on.
	 */
	char*	GetCnonce();
	/**
	 * Add the cnonce parameter from a Authorization element.
	 * @param header The element to work on.
	 * @param value The value of the new parameter.
	 */
	void	SetCnonce(char* value);
	/**
	 * Get value of the opaque parameter from a Authorization element.
	 * @param header The element to work on.
	 */
	char*	GetOpaque();
	/**
	 * Add the opaque parameter from a Authorization element.
	 * @param header The element to work on.
	 * @param value The value of the new parameter.
	 */
	void	SetOpaque(char* value);
	/**
	 * Get value of the message_qop parameter from a Authorization element.
	 * @param header The element to work on.
	 */
	char*	GetMessageQop();
	/**
	 * Add the message_qop parameter from a Authorization element.
	 * @param header The element to work on.
	 * @param value The value of the new parameter.
	 */
	void	SetMessageQop(char* value);
	/**
	 * Get value of the nonce_count parameter from a Authorization element.
	 * @param header The element to work on.
	 */
	char*	GetNonceCount();
	/**
	 * Add the nonce_count parameter from a Authorization element.
	 * @param header The element to work on.
	 * @param value The value of the new parameter.
	 */
	void	SetNonceCount(char* value);


	char*	auth_type;		/**< Authentication Type (Basic or Digest) */
	char*	username;		/**< login */
	char*	realm;		/**< realm (as a quoted-string) */
	char*	nonce;		/**< nonce */
	char*	uri;  		/**< uri */
	char*	response;		/**< response */
	char*	digest;		/**< digest */
	char*	algorithm;		/**< algorithm (optionnal) */
	char*	cnonce;		/**< cnonce (optionnal) */
	char*	opaque;		/**< opaque (optionnal) */
	char*	message_qop;		/**< message_qop (optionnal) */
	char*	nonce_count;		/**< nonce_count (optionnal) */
	char*	auth_param;		/**< other parameters (optionnal) */

};

int osip_authorization_clone(const CAuthorization* auth, CAuthorization** dest);


#endif // !defined(AFX_AUTHORIZATION_H__BCD84F64_ABFF_4168_B462_471FD76D84EB__INCLUDED_)
