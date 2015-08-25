// CallId.h: interface for the CCallId class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CALLID_H__CE9C5018_D779_4725_B3FD_B5E6992A5CE2__INCLUDED_)
#define AFX_CALLID_H__CE9C5018_D779_4725_B3FD_B5E6992A5CE2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCallId : public CNodeRoot  
{
public:
	CCallId();
	virtual ~CCallId();
	
	int		Init();
	void	Free();
	int		Parse(const char* hvalue);
	int		ToStr(char** dest);
	/**
	 * Clone a Call-id element.
	 * @param header The element to work on.
	 * @param dest A pointer on the copy of the element.
	 */
	int Clone(CCallId** dest);
	/**
	 * Set the number in the Call-Id element.
	 * @param header The element to work on.
	 * @param value The value of the element.
	 */
	void SetNumber(char* value);
	/**
	 * Get the number from a Call-Id header.
	 * @param header The element to work on.
	 */
	char* GetNumber();
	/**
	 * Set the host in the Call-Id element.
	 * @param header The element to work on.
	 * @param value The value of the element.
	 */
	void SetHost(char* value);
	/**
	 * Get the host from a Call-Id header.
	 * @param header The element to work on.
	 */
	char* GetHost();

	/**
	 * Check if the Call-Id headers match.
	 * NOTE: THIS IS AN INTERNAL METHOD ONLY
	 * @param callid1 The initial Call-Id header.
	 * @param callid2 The new Call-Id header.
	 */
	int IdMatch(CCallId* callid);

	char*	number;    /**< Call-ID number */
	char*	host; 	 /**< Call-ID host information */
};

#endif // !defined(AFX_CALLID_H__CE9C5018_D779_4725_B3FD_B5E6992A5CE2__INCLUDED_)
