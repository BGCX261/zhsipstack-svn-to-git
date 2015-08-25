// IstCallBack.cpp: implementation of the CIstCallBack class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "zhSipStack.h"
#include "IstCallBack.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIstCallBack::CIstCallBack()
{

}

CIstCallBack::~CIstCallBack()
{

}

void CIstCallBack::isb_RcvInvite( int type, CSipTransaction* tr, CSipMessage* sip )
{
	TRACE("isb_RcvInvite");
}

void CIstCallBack::isb_RcvInviteAgain( int type, CSipTransaction* tr, CSipMessage* sip )
{
	TRACE("isb_RcvInviteAgain");
}

void CIstCallBack::isb_RcvAck( int type, CSipTransaction* tr, CSipMessage* sip )
{
	TRACE("isb_RcvAck");
}

void CIstCallBack::isb_RcvAckAgain( int type, CSipTransaction* tr, CSipMessage* sip )
{
	TRACE("isb_RcvAckAgain");
}

void CIstCallBack::isb_Send1xx( int type, CSipTransaction* tr, CSipMessage* sip )
{
	
}

void CIstCallBack::isb_Send2xx( int type, CSipTransaction* tr, CSipMessage* sip )
{
	
}

void CIstCallBack::isb_Send3xx( int type, CSipTransaction* tr, CSipMessage* sip )
{
	
}

void CIstCallBack::isb_Send4xx( int type, CSipTransaction* tr, CSipMessage* sip )
{
	
}

void CIstCallBack::isb_Send5xx( int type, CSipTransaction* tr, CSipMessage* sip )
{
	
}

void CIstCallBack::isb_Send6xx( int type, CSipTransaction* tr, CSipMessage* sip )
{
	
}

void CIstCallBack::isb_Send3456xx( int type, CSipTransaction* tr, CSipMessage* sip )
{
	
}
