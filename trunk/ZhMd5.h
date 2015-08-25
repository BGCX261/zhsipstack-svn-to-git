// ZhMd5.h: interface for the CZhMd5 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ZHMD5_H__1A1B8E2B_2BC5_4858_B6D9_13892C960136__INCLUDED_)
#define AFX_ZHMD5_H__1A1B8E2B_2BC5_4858_B6D9_13892C960136__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// #ifndef UINT4
// 	#define UINT4 unsigned int
// #endif

	/* modified for oSIP: GCC supports this feature */
#define PROTOTYPES 1

#ifndef PROTOTYPES
#define PROTOTYPES 0
#endif

	/* POINTER defines a generic pointer type */
	typedef unsigned char* POINTER;

	/* UINT2 defines a two byte word */
	typedef unsigned short int UINT2;

	/* UINT4 defines a four byte word */
	typedef unsigned int UINT4;

	/* PROTO_LIST is defined depending on how PROTOTYPES is defined above.
	If using PROTOTYPES, then PROTO_LIST returns the list, otherwise it
	  returns an empty list.
	 */
#if PROTOTYPES
#define PROTO_LIST(list) list
#else
#define PROTO_LIST(list) ()
#endif


class CZhMd5  
{
public:
	CZhMd5();
	virtual ~CZhMd5();

	UINT4 state[4]; 			/* state (ABCD) */
	UINT4 count[2]; 			/* number of bits, modulo 2^64 (lsb first) */
	unsigned char buffer[64];   /* input buffer */

	void MD5Init();
	void MD5Update(unsigned char *, unsigned int);
	void MD5Final(unsigned char[16]);

};

#endif // !defined(AFX_ZHMD5_H__1A1B8E2B_2BC5_4858_B6D9_13892C960136__INCLUDED_)
