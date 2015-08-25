// Seq.h: interface for the CSeq class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEQ_H__60CC52EC_F9B0_4136_8373_B7CEC96269F5__INCLUDED_)
#define AFX_SEQ_H__60CC52EC_F9B0_4136_8373_B7CEC96269F5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSipMessage;

class CSeq : public CNodeRoot  
{
public:
	CSeq();
	virtual ~CSeq();

	int		Init();
	void	Free();
	int		Parse(const char *hvalue);
	int		ToStr(char** dest);

	/**
	 * Clone a CSeq element.
	 * @param header The element to work on.
	 * @param dest A pointer on the copy of the element.
	 */
	int		Clone(CSeq** dest);
	/**
	 * Set the number in the CSeq element.
	 * @param header The element to work on.
	 * @param value The value of the element.
	 */
	void	SetNumber(char* value);
	/**
	 * Get the number from a CSeq header.
	 * @param header The element to work on.
	 */
	char*	GetNumber();
	/**
	 * Set the method in the CSeq element.
	 * @param header The element to work on.
	 * @param value The value of the element.
	 */
	void	SetMethod(char* value);
	/**
	 * Get the method from a CSeq header.
	 * @param header The element to work on.
	 */
	char*	GetMethod();

	/**
	 * Check if the CSeq headers match.
	 * NOTE: THIS IS AN INTERNAL METHOD ONLY
	 * @param cseq1 The initial CSeq header.
	 * @param cseq2 The new CSeq header.
	 */
	int Match(CSeq* cseq);

	char* method;    /**< CSeq method */
	char* number;    /**< CSeq number */

	


};

#endif // !defined(AFX_SEQ_H__60CC52EC_F9B0_4136_8373_B7CEC96269F5__INCLUDED_)
