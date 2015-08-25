// NictCallBack.cpp: implementation of the CNictCallBack class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "zhSipStack.h"
#include "NictCallBack.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNictCallBack::CNictCallBack()
{

}

CNictCallBack::~CNictCallBack()
{

}

void CNictCallBack::nicb_RegisterSend( int type, CSipTransaction* tr, CSipMessage* sip )
{
	TRACE("nicb_RegisterSend");
}

void CNictCallBack::nicb_ByeSend( int type, CSipTransaction* tr, CSipMessage* sip )
{
	TRACE("nicb_ByeSend");
}

void CNictCallBack::nicb_OptionsSend( int type, CSipTransaction* tr, CSipMessage* sip )
{
	TRACE("nicb_OptionsSend");
}

void CNictCallBack::nicb_InfoSend( int type, CSipTransaction* tr, CSipMessage* sip )
{
	TRACE("nicb_InfoSend");
}

void CNictCallBack::nicb_CancelSend( int type, CSipTransaction* tr, CSipMessage* sip )
{
	TRACE("nicb_CancelSend");
}

void CNictCallBack::nicb_NotifySend( int type, CSipTransaction* tr, CSipMessage* sip )
{
	TRACE("nicb_NotifySend");
}

void CNictCallBack::nicb_SubscribeSend( int type, CSipTransaction* tr, CSipMessage* sip )
{
	TRACE("nicb_SubscribeSend");	
}

void CNictCallBack::nicb_UnknowRequestSend( int type, CSipTransaction* tr, CSipMessage* sip )
{
	TRACE("nicb_UnknowRequestSend");	
}

void CNictCallBack::nicb_RequestSendAgain( int type, CSipTransaction* tr, CSipMessage* sip )
{
	TRACE("nicb_RequestSendAgain");	
}

void CNictCallBack::nicb_Rev1xx( int type, CSipTransaction* tr, CSipMessage* sip )
{
	
}

void CNictCallBack::nicb_Rev2xx( int type, CSipTransaction* tr, CSipMessage* sip )
{
	
}

void CNictCallBack::nicb_Rev2xxAgain( int type, CSipTransaction* tr, CSipMessage* sip )
{
	
}

void CNictCallBack::nicb_Rev3xx( int type, CSipTransaction* tr, CSipMessage* sip )
{
	
}

void CNictCallBack::nicb_Rev4xx( int type, CSipTransaction* tr, CSipMessage* sip )
{
	
}

void CNictCallBack::nicb_Rev5xx( int type, CSipTransaction* tr, CSipMessage* sip )
{
	
}

void CNictCallBack::nicb_Rev6xx( int type, CSipTransaction* tr, CSipMessage* sip )
{
	
}

void CNictCallBack::nicb_Rev3456xx( int type, CSipTransaction* tr, CSipMessage* sip )
{
	
}
