// NistCallBack.cpp: implementation of the CNistCallBack class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "zhSipStack.h"
#include "NistCallBack.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNistCallBack::CNistCallBack()
{

}

CNistCallBack::~CNistCallBack()
{

}

void CNistCallBack::nisb_RegisterRcv( int type, CSipTransaction* tr, CSipMessage* sip )
{
	TRACE("nisb_RegisterRcv");
}

void CNistCallBack::nisb_ByeRcv( int type, CSipTransaction* tr, CSipMessage* sip )
{
	TRACE("nisb_ByeRcv");
}

void CNistCallBack::nisb_OptionsRcv( int type, CSipTransaction* tr, CSipMessage* sip )
{
	TRACE("nisb_ByeRcv");
}

void CNistCallBack::nisb_InfoRcv( int type, CSipTransaction* tr, CSipMessage* sip )
{
	TRACE("nisb_ByeRcv");
}

void CNistCallBack::nisb_CancelRcv( int type, CSipTransaction* tr, CSipMessage* sip )
{
	TRACE("nisb_ByeRcv");
}

void CNistCallBack::nisb_NotifyRcv( int type, CSipTransaction* tr, CSipMessage* sip )
{
	TRACE("nisb_ByeRcv");
}

void CNistCallBack::nisb_SubscribeRcv( int type, CSipTransaction* tr, CSipMessage* sip )
{
	TRACE("nisb_ByeRcv");
}

void CNistCallBack::nisb_UnknowRequestRcv( int type, CSipTransaction* tr, CSipMessage* sip )
{
	TRACE("nisb_UnknowRequestRcv");
}

void CNistCallBack::nisb_RequestRcvAgain( int type, CSipTransaction* tr, CSipMessage* sip )
{
	TRACE("nisb_RequestRcvAgain");
}

void CNistCallBack::nisb_Send1xx( int type, CSipTransaction* tr, CSipMessage* sip )
{
	
}

void CNistCallBack::nisb_Send2xx( int type, CSipTransaction* tr, CSipMessage* sip )
{
	
}

void CNistCallBack::nisb_Send2xxAgain( int type, CSipTransaction* tr, CSipMessage* sip )
{
	
}

void CNistCallBack::nisb_Send3xx( int type, CSipTransaction* tr, CSipMessage* sip )
{
	
}

void CNistCallBack::nisb_Send4xx( int type, CSipTransaction* tr, CSipMessage* sip )
{
	
}

void CNistCallBack::nisb_Send5xx( int type, CSipTransaction* tr, CSipMessage* sip )
{
	
}

void CNistCallBack::nisb_Send6xx( int type, CSipTransaction* tr, CSipMessage* sip )
{
	
}

void CNistCallBack::nisb_Send3456xxAgain( int type, CSipTransaction* tr, CSipMessage* sip )
{
	
}