// SipTransaction.h: interface for the CSipTransaction class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SIPTRANSACTION_H__A4660C55_A6B5_4D6D_8FFF_98AE334B5C94__INCLUDED_)
#define AFX_SIPTRANSACTION_H__A4660C55_A6B5_4D6D_8FFF_98AE334B5C94__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SipParserHeader.h"
#include "Fifo.h"
#include "siptypes.h"
#include "ZhSip.h"
#include "SipMessage.h"
#include "SipEvent.h"
#include "StateMachine.h"



class CSipTransaction  
{
public:
	CSipTransaction();
	virtual ~CSipTransaction();

	virtual		int		Init(CSipMessage* request);

	virtual     int     AddEvent(CSipEvent *pEvent);
	virtual     void	ExecuteTimer();
	virtual     int		ExecuteTrsanction();

	virtual     int		Execute();

	int					SetTopVia(CVia* topvia);
	int					SetFrom(CFrom* from);
	int					SetTo(CTo* to);
	int					SetCallId(CCallId* call_id);
	int					SetCseq(CSeq* cseq);
	int					SetState(eTransactionState state);
	int					SetInstance(void* instance);
	void*				GetInstance();


	void*				your_instance;		/**< User Defined Pointer. */
	int					transactionid;  	/**< Internal Transaction Identifier. */
	CFifo*				transactionff;		/**< events must be added in this fifo */
	
	CVia*				topvia; 			/**< CALL-LEG definition (Top Via) */
	CFrom*				from;  				/**< CALL-LEG definition (From)    */
	CTo*				to;  				/**< CALL-LEG definition (To)      */
	CCallId*			callid; 			/**< CALL-LEG definition (Call-ID) */
	CSeq*				cseq;  				/**< CALL-LEG definition (CSeq)    */
	
	CSipMessage*		orig_request;		/**< Initial request 		   */
	CSipMessage*		last_response;		/**< Last response   		   */
	CSipMessage*		ack;				/**< ack request sent		   */
	
	eTransactionState	state;  			/**< Current state of the transaction */
	
	time_t				birth_time;  		/**< birth date of transaction  	  */
	time_t				completed_time;  	/**< end   date of transaction  	  */
	
	int					in_socket;  		/**< Optional socket for incoming message */
	int					out_socket; 		/**< Optional place for outgoing message */
	
	void*				config;   			/**@internal transaction is managed by osip_t  */
	
 	eSipFsmType			ctx_type;   /**< Type of the transaction */
 	
	CStateMachine		*m_pStateMachine;
 	
 	osip_srv_record_t	record;   /**@internal */
};

#endif // !defined(AFX_SIPTRANSACTION_H__A4660C55_A6B5_4D6D_8FFF_98AE334B5C94__INCLUDED_)
