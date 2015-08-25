// Fifo.h: interface for the CFifo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FIFO_H__3EFADA67_8EEC_4237_AC7A_C7AFFCCC972A__INCLUDED_)
#define AFX_FIFO_H__3EFADA67_8EEC_4237_AC7A_C7AFFCCC972A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ZhList.h"

#ifndef DOXYGEN

#define MAX_LEN 1000

typedef enum _eFifoState
{
	osip_ok,
	osip_full,
	osip_empty
} eFifoState;

#endif

class CFifo  
{
public:
	CFifo();
	virtual ~CFifo();

	void Init();
	int Add(void* el);
	int Insert(void* el);
	int GetSize();
	void* GetFrist();
	void* TryGetFrist();
	void Free();
	
	CZhList		queue;  		   /**< list of nodes containing elements */
	int			nb_elt; 				   /**< nb of elements */
	eFifoState	state;  		 /**@internal state of the fifo */

};

#endif // !defined(AFX_FIFO_H__3EFADA67_8EEC_4237_AC7A_C7AFFCCC972A__INCLUDED_)
