/*
*********************************************************************************************************
*                                                
*                                             工程名称：SIP协议应用
*
*                                                工程号：2006003
*
*                                                负责人：张梦晗
*
*                                               属性：本科毕业设计
* 
*                                              大连理工大学电信学院
*********************************************************************************************************
*/
#define SIP_GLOBALS
#include "config.h"

char* SipMsgStack[SIPSTACKMAX];
char ibuffer[100];

void Int2String(char* string,uint16 value)
{
	uint16 i;
	uint16 temp[20];
	uint16 k;
	for(i=0;i<20;i++)
	{
		temp[i]=value%10;
		value=value/10;
		if(value==0)
			break;
	}
	k=i+1;
	for(i=0;i<k;i++)
		*(string+i)=(char)(48+temp[k-i-1]);
	return;
}

void String2Int(uint16* value,char* string)
{
	uint16 i;
	uint16 k;
	k=strlen(string);
	*value=0;
	for(i=0;i<k;i++)
		*value=*value+(uint16)(*(string+i)-48)*(uint16)pow(10,k-i-1);
    return;
}

uint16 ProtoJudgement(char * ProtoHeader,struct sip_packet *packet)
{
	uint16 i,result;
	uint16 j=0;
	uint16 k=0;
	char * temp;
	char * lProtoHeader=ProtoHeader;
	for(i=0;i<50;i++)
	{
		if(j==0)
			ibuffer[0]='\0';
		if((*(ProtoHeader+j)=='\r')&&(*(ProtoHeader+j+1)=='\n'))
		{
            ibuffer[j]='\0';
            temp=ibuffer;
			strcpy(packet->message.msg[k],temp);
            break;
		}
		if(*(ProtoHeader+j)==' '&&k<2)
		{
			ProtoHeader=ProtoHeader+j+1;
			ibuffer[j]='\0';
            temp=ibuffer;
			strcpy(packet->message.msg[k],temp);
			k++;
			j=0;
			continue;
		}

        ibuffer[j]= *(lProtoHeader+i);
		j++;
	}
    ibuffer[0]='\0';
	if(i!=50)
	{
		result=CompareSipLinerName(packet->message.msg[0]);
		if(result==SIPVERSION)
			String2Int(&result,packet->message.msg[1]);
	}
	else
		result=0;
	return result;
}
char * SipMsgAnalysis(char * SipMsg,uint16 MsgLength,struct sip_packet *packet)
{
	uint16 i;
	uint16 j=0;
	uint16 k=0;
	char * lSipMsg=SipMsg;
	for(i=0;i<MsgLength;i++)
	{
		if(j==0)
			ibuffer[0]='\0';
		if((*(SipMsg+j)=='\r')&&(*(SipMsg+j+1)=='\n'))
		{
            SipMsg=SipMsg+j+2;
	        if(j==0)
				break;
			ibuffer[j]='\0';
            SipMsgStack[k]=ibuffer;
			if(k!=0)
			   SipLinerAnalysis(SipMsgStack[k],packet);
			k++;
			i++;
			j=0;
		    continue;
		}
        ibuffer[j]= *(lSipMsg+i);
		j++;
	}
    ibuffer[0]='\0';

	return SipMsg;
}

void SipLinerAnalysis(char * SipLiner,struct sip_packet *packet)
{
	uint16 i=0;
	char * SipLinerName;
	while(1)
	{
		if((*(SipLiner+i)==':')&&(*(SipLiner+i+1)==' '))
		{
            SipLiner=SipLiner+i+2;
            ibuffer[i]='\0';
            SipLinerName=ibuffer;
			switch(CompareSipLinerName(SipLinerName))
			{
			case VIA:
				SipViaAnaylsis(SipLiner,packet);
				break;
            case FROM:
				SipFromAnaylsis(SipLiner,packet);
				break;
			case MAX_FORWARDS:
                SipMaxForwardsAnaylsis(SipLiner,packet);
				break;
            case TO:
                SipToAnaylsis(SipLiner,packet);
				break;
			case CALL_ID:
				SipCallIDAnaylsis(SipLiner,packet);
				break;
			case CSEQ:
				SipCSeqAnaylsis(SipLiner,packet);
				break;
            case CONTACT:
				SipContactAnaylsis(SipLiner,packet);
				break;
            case USER_AGENT:
				SipUserAgentAnaylsis(SipLiner,packet);
				break;
            case CONTENT_TYPE:
				SipCtypeAnaylsis(SipLiner,packet);
				break;
			case CONTENT_LENGTH:
				SipClengthAnaylsis(SipLiner,packet);
				break;
			default:
				break;
			}
		    break;
		}
        ibuffer[i]=*(SipLiner+i);
		i++;
	}
    ibuffer[0]='\0';
	return;
}

uint16 CompareSipLinerName(char * SipLinerName)
{
	if(!strcmp(SipLinerName,"INVITE"))
		return INVITE;
    if(!strcmp(SipLinerName,"ACK"))
		return ACK;
    if(!strcmp(SipLinerName,"BYE"))
		return BYE;
	if(!strcmp(SipLinerName,"OPTION"))
		return OPTION;
	if(!strcmp(SipLinerName,"CANCEL"))
		return CANCEL;
	if(!strcmp(SipLinerName,"REGISTER"))
		return REGISTER;
    if(!strcmp(SipLinerName,"SIP/2.0"))
		return SIPVERSION;
	if(!strcmp(SipLinerName,"Via"))
		return VIA;
	if(!strcmp(SipLinerName,"Max-Forwards"))
		return MAX_FORWARDS;
    if(!strcmp(SipLinerName,"From"))
		return FROM;
	if(!strcmp(SipLinerName,"To"))
		return TO;
    if(!strcmp(SipLinerName,"Call-ID"))
		return CALL_ID;
    if(!strcmp(SipLinerName,"CSeq"))
		return CSEQ;
    if(!strcmp(SipLinerName,"Contact"))
		return CONTACT;
    if(!strcmp(SipLinerName,"User-Agent"))
		return USER_AGENT;
    if(!strcmp(SipLinerName,"Content-Type"))
		return CONTENT_TYPE;
    if(!strcmp(SipLinerName,"Content-Length"))
		return CONTENT_LENGTH;
	return 0;
}

void SipViaAnaylsis(char * SipLiner,struct sip_packet *packet)
{
	uint16 i;
	uint16 j=0;
	uint16 k=0;
	char * lSipLiner=SipLiner;
	char * temp;
	k=(uint16)strlen(lSipLiner);
	for(i=0;i<k;i++)
	{
		if(j==0)
			ibuffer[0]='\0';
		if(*(SipLiner+j)==' ')
		{
            SipLiner=SipLiner+j+1;
            ibuffer[j]='\0';
            temp=ibuffer;
            strcpy(packet->via.protocol,temp);
			j=0;
		    continue;
		}
		if(*(SipLiner+j)==':')
		{
            SipLiner=SipLiner+j+1;
            ibuffer[j]='\0';
            temp=ibuffer;
			strcpy(packet->via.hostaddr,temp);
			strcpy(packet->via.hostport,SipLiner);
			j=0;
		    continue;
		}
        ibuffer[j]= *(lSipLiner+i);
		j++;
	}
    ibuffer[0]='\0';

	return;
}

void SipMaxForwardsAnaylsis(char * SipLiner,struct sip_packet *packet)
{
	strcpy(packet->maxforwards.max,SipLiner);
    return;
}

void SipFromAnaylsis(char * SipLiner,struct sip_packet *packet)
{
	uint16 i;
	uint16 j=0;
	char * lSipLiner=SipLiner;
	char * temp;
	uint16 k;
	k=(uint16)strlen(lSipLiner);
	for(i=0;i<k;i++)
	{
		if(j==0)
			ibuffer[0]='\0';
		if((*(SipLiner+j)=='\"')&&(*(SipLiner+j+1)!=' '))
		{
            SipLiner=SipLiner+j+1;
			j=0;
		    continue;
		}
		if((*(SipLiner+j)=='\"')&&(*(SipLiner+j+1)==' '))
        {
			SipLiner=SipLiner+j+2;
			ibuffer[j]='\0';
            temp=ibuffer;
			strcpy(packet->from.display_name,temp);
			i++;
			j=0;
		    continue;
		}
		if(*(SipLiner+j)=='<')
		{
			SipLiner=SipLiner+j+1;
			j=0;
		    continue;
		}
        if((*(SipLiner+j)=='>')&&(*(SipLiner+j+1)=='\0'))
		{
            SipLiner=SipLiner+j+1;
            ibuffer[j]='\0';
            temp=ibuffer;
			strcpy(packet->from.URI,temp);
			j=0;
		    continue;
		}
		if((*(SipLiner+j)=='>')&&(*(SipLiner+j+1)==';'))
		{
			SipLiner=SipLiner+j+2;
			ibuffer[j]='\0';
			temp=ibuffer;
			strcpy(packet->from.URI,temp);
			i++;
			j=0;
		    continue;
		}
		if(*(SipLiner+j)=='=')
		{
			SipLiner=SipLiner+j+1;
			ibuffer[j]='\0';
            temp=ibuffer;
			if(!strcmp(temp,"tag"))
				strcpy(packet->from.tag,SipLiner);
            j=0;
			continue;
		}
        ibuffer[j]= *(lSipLiner+i);
		j++;
	}
    ibuffer[0]='\0';
    return;
}

void SipToAnaylsis(char * SipLiner,struct sip_packet *packet)
{
	uint16 i;
	uint16 j=0;
	char * lSipLiner=SipLiner;
	char * temp;
	uint16 k=(uint16)strlen(lSipLiner);
	for(i=0;i<k;i++)
	{
		if(j==0)
			ibuffer[0]='\0';
		if((*(SipLiner+j)=='\"')&&(*(SipLiner+j+1)!=' '))
		{
            SipLiner=SipLiner+j+1;
			j=0;
		    continue;
		}
		if((*(SipLiner+j)=='\"')&&(*(SipLiner+j+1)==' '))
        {
			SipLiner=SipLiner+j+2;
			ibuffer[j]='\0';
            temp=ibuffer;
			strcpy(packet->to.display_name,temp);
			i++;
			j=0;
		    continue;
		}
		if(*(SipLiner+j)=='<')
		{
			SipLiner=SipLiner+j+1;
			j=0;
		    continue;
		}
        if((*(SipLiner+j)=='>')&&(*(SipLiner+j+1)=='\0'))
		{
            SipLiner=SipLiner+j+1;
            ibuffer[j]='\0';
            temp=ibuffer;
			strcpy(packet->to.URI,temp);
			j=0;
		    continue;
		}
		if((*(SipLiner+j)=='>')&&(*(SipLiner+j+1)==';'))
		{
			SipLiner=SipLiner+j+2;
			ibuffer[j]='\0';
			temp=ibuffer;
			strcpy(packet->to.URI,temp);
			i++;
			j=0;
		    continue;
		}
		if(*(SipLiner+j)=='=')
		{
			SipLiner=SipLiner+j+1;
			ibuffer[j]='\0';
            temp=ibuffer;
			if(!strcmp(temp,"tag"))
				strcpy(packet->to.tag,SipLiner);
            j=0;
			continue;
		}
        ibuffer[j]= *(lSipLiner+i);
		j++;
	}
    ibuffer[0]='\0';
    return;
}

void SipCallIDAnaylsis(char * SipLiner,struct sip_packet *packet)
{
	uint16 i;
	uint16 j=0;
	char * lSipLiner=SipLiner;
	char * temp;
	uint16 k=(uint16)strlen(lSipLiner);
	for(i=0;i<k;i++)
	{
		if(j==0)
			ibuffer[0]='\0';
		if(*(SipLiner+j)=='@')
		{
            SipLiner=SipLiner+j+1;
            ibuffer[j]='\0';
            temp=ibuffer;
			strcpy(packet->call_id.id,temp);
			strcpy(packet->call_id.URI,SipLiner);
			j=0;
		    continue;
		}
        ibuffer[j]= *(lSipLiner+i);
		j++;
	}
    ibuffer[0]='\0';
    return;
}

void SipCSeqAnaylsis(char * SipLiner,struct sip_packet *packet)
{
	uint16 i;
	uint16 j=0;
	char * lSipLiner=SipLiner;
	char * temp;
	uint16 k=(uint16)strlen(lSipLiner);
	for(i=0;i<k;i++)
	{
		if(j==0)
			ibuffer[0]='\0';
		if(*(SipLiner+j)==' ')
		{
            SipLiner=SipLiner+j+1;
            ibuffer[j]='\0';
            temp=ibuffer;
			strcpy(packet->cseq.value,temp);
			strcpy(packet->cseq.method,SipLiner);
			j=0;
		    continue;
		}
        ibuffer[j]= *(lSipLiner+i);
		j++;
	}
    ibuffer[0]='\0';
    return;
}

void SipContactAnaylsis(char * SipLiner,struct sip_packet *packet)
{
	uint16 i;
	uint16 j=0;
	uint16 k=0;
	char * lSipLiner=SipLiner;
	char * temp;
	uint16 m=(uint16)strlen(lSipLiner);
	for(i=0;i<m;i++)
	{
		if(j==0)
			ibuffer[0]='\0';
		if((*(SipLiner+j)=='\"')&&(*(SipLiner+j+1)!=' '))
		{
            SipLiner=SipLiner+j+1;
			j=0;
		    continue;
		}
		if((*(SipLiner+j)=='\"')&&(*(SipLiner+j+1)==' '))
        {
			SipLiner=SipLiner+j+2;
			ibuffer[j]='\0';
            temp=ibuffer;
			strcpy(packet->contact.display_name,temp);
			i++;
			j=0;
		    continue;
		}
		if(*(SipLiner+j)=='<')
		{
			k=1;
			SipLiner=SipLiner+j+1;
			j=0;
		    continue;
		}
        if(*(SipLiner+j)==':'&&k==1)
		{   k=2;
			SipLiner=SipLiner+j+1;
			j=0;
		    continue;
		}
		if(*(SipLiner+j)==':'&&k==2)
		{   k=3;
			SipLiner=SipLiner+j+1;
			ibuffer[j]='\0';
			temp=ibuffer;
			strcpy(packet->contact.URI,temp);
			j=0;
		    continue;
		}
        if((*(SipLiner+j)=='>')&&(*(SipLiner+j+1)=='\0')&&k==3)
		{
			k=0;
            SipLiner=SipLiner+j+1;
            ibuffer[j]='\0';
            temp=ibuffer;
			strcpy(packet->contact.port,temp);
			j=0;
		    continue;
		}
		if((*(SipLiner+j)=='>')&&(*(SipLiner+j+1)==';')&&k==3)
		{
            k=0;
			SipLiner=SipLiner+j+2;
			ibuffer[j]='\0';
			temp=ibuffer;
			strcpy(packet->contact.port,temp);
			i++;
			j=0;
		    continue;
		}
		if(*(SipLiner+j)=='=')
		{
			SipLiner=SipLiner+j+1;
			ibuffer[j]='\0';
            temp=ibuffer;
			if(!strcmp(temp,"q"))
				strcpy(packet->contact.q,SipLiner);
			else
                strcpy(packet->contact.expires,SipLiner);
            j=0;
			continue;
		}
        ibuffer[j]= *(lSipLiner+i);
		j++;
	}
    ibuffer[0]='\0';
    return;
}

void SipUserAgentAnaylsis(char * SipLiner,struct sip_packet *packet)
{
	strcpy(packet->user_agent.name,SipLiner);
	return;
}

void SipCtypeAnaylsis(char * SipLiner,struct sip_packet *packet)
{
	strcpy(packet->content_type.protocol,SipLiner);
    return;
}

void SipClengthAnaylsis(char * SipLiner,struct sip_packet *packet)
{
	strcpy(packet->content_length.value,SipLiner);
    return;
}
void SipInviteBuilding(char* SendMsg,struct connect_info *ihostinfo)
{
	int j,bp;
	j=sprintf(SendMsg,"%s sip:%s %s\r\n","INVITE",ihostinfo->targetip,ihostinfo->sipversion);
	j+=sprintf(SendMsg+j,"%s: %s/%s %s:%s\r\n","Via",ihostinfo->sipversion,ihostinfo->baseproto,ihostinfo->hostip,ihostinfo->hostport);
    j+=sprintf(SendMsg+j,"%s: %s\r\n","Max-Forwards",ihostinfo->maxforwards);
	j+=sprintf(SendMsg+j,"%s: \"%s\" <sip:%s>;tag=%s\r\n","From",ihostinfo->username,ihostinfo->hostname,ihostinfo->tag);
	j+=sprintf(SendMsg+j,"%s: <sip:%s>\r\n","To",ihostinfo->targetip);
	j+=sprintf(SendMsg+j,"%s: %s@%s\r\n","Call-ID",ihostinfo->callid,ihostinfo->hostip);
	j+=sprintf(SendMsg+j,"%s: 1 %s\r\n","CSeq","INVITE");
	j+=sprintf(SendMsg+j,"%s: <sip:%s:%s>\r\n","Contact",ihostinfo->hostip,ihostinfo->hostport);
	j+=sprintf(SendMsg+j,"%s: %s\r\n","User-Agent",ihostinfo->agent);
	j+=sprintf(SendMsg+j,"%s: %s\r\n","Content-Type",ihostinfo->proto);
	j+=sprintf(SendMsg+j,"%s: %s\r\n\r\n","Content-Length","   ");
    bp=j;
    j=SdpInviteBuilding(SendMsg,ihostinfo,j);
	Int2String(SendMsg+bp-7,j-bp);
}

void SipTryingBuilding(char* SendMsg,struct connect_info *ihostinfo,struct sip_packet *packeta,struct sdp_packet *packetb)
{
	int j;
	j=sprintf(SendMsg,"%s %d %s\r\n",ihostinfo->sipversion,TRYING,"Trying");
	j+=sprintf(SendMsg+j,"%s: %s %s:%s\r\n","Via",packeta->via.protocol,packeta->via.hostaddr,packeta->via.hostport);
	j+=sprintf(SendMsg+j,"%s: \"%s\" <%s>;tag=%s\r\n","From",packeta->from.display_name,packeta->from.URI,packeta->from.tag);
	j+=sprintf(SendMsg+j,"%s: \"%s\" <%s>;tag=%s\r\n","To",ihostinfo->username,packeta->to.URI,ihostinfo->tag);
	j+=sprintf(SendMsg+j,"%s: %s@%s\r\n","Call-ID",packeta->call_id.id,packeta->call_id.URI);
	j+=sprintf(SendMsg+j,"%s: %s %s\r\n","CSeq",packeta->cseq.value,packeta->cseq.method);
	j+=sprintf(SendMsg+j,"%s: %s\r\n","User-Agent",ihostinfo->agent);
	j+=sprintf(SendMsg+j,"%s: %s\r\n\r\n","Content-Length","0");
}

void SipRingingBuilding(char* SendMsg,struct connect_info *ihostinfo,struct sip_packet *packeta,struct sdp_packet *packetb)
{
	int j;
	j=sprintf(SendMsg,"%s %d %s\r\n",ihostinfo->sipversion,RINGING,"Ringing");
	j+=sprintf(SendMsg+j,"%s: %s %s:%s\r\n","Via",packeta->via.protocol,packeta->via.hostaddr,packeta->via.hostport);
	j+=sprintf(SendMsg+j,"%s: \"%s\" <%s>;tag=%s\r\n","From",packeta->from.display_name,packeta->from.URI,packeta->from.tag);
	j+=sprintf(SendMsg+j,"%s: \"%s\" <%s>;tag=%s\r\n","To",ihostinfo->username,packeta->to.URI,ihostinfo->tag);
	j+=sprintf(SendMsg+j,"%s: %s@%s\r\n","Call-ID",packeta->call_id.id,packeta->call_id.URI);
	j+=sprintf(SendMsg+j,"%s: %s %s\r\n","CSeq",packeta->cseq.value,packeta->cseq.method);
	j+=sprintf(SendMsg+j,"%s: %s\r\n","User-Agent",ihostinfo->agent);
	j+=sprintf(SendMsg+j,"%s: %s\r\n\r\n","Content-Length","0");
}

void SipOKBuilding(char* SendMsg,struct connect_info *ihostinfo,struct sip_packet *packeta,struct sdp_packet *packetb)
{	int j,bp;
    j=sprintf(SendMsg,"%s %d %s\r\n",ihostinfo->sipversion,OK,"OK");
	j+=sprintf(SendMsg+j,"%s: %s %s:%s\r\n","Via",packeta->via.protocol,packeta->via.hostaddr,packeta->via.hostport);
	j+=sprintf(SendMsg+j,"%s: \"%s\" <%s>;tag=%s\r\n","From",packeta->from.display_name,packeta->from.URI,packeta->from.tag);
	j+=sprintf(SendMsg+j,"%s: \"%s\" <sip:%s>;tag=%s\r\n","To",ihostinfo->username,ihostinfo->hostip,ihostinfo->tag);
	j+=sprintf(SendMsg+j,"%s: %s@%s\r\n","Call-ID",packeta->call_id.id,packeta->call_id.URI);
	j+=sprintf(SendMsg+j,"%s: %s %s\r\n","CSeq",packeta->cseq.value,packeta->cseq.method);
	if(!strcmp(packeta->cseq.method,"INVITE"))
	{
		j+=sprintf(SendMsg+j,"%s: <sip:%s:%s>\r\n","Contact",ihostinfo->hostip,ihostinfo->hostport);
	    j+=sprintf(SendMsg+j,"%s: %s\r\n","User-Agent",ihostinfo->agent);
        j+=sprintf(SendMsg+j,"%s: %s\r\n","Content-Type",ihostinfo->proto);
	    j+=sprintf(SendMsg+j,"%s: %s\r\n\r\n","Content-Length","   ");
        bp=j;
        j=SdpOKBuilding(SendMsg,ihostinfo,packeta,packetb,j);
	    Int2String(SendMsg+bp-7,j-bp);
	}
	if(!strcmp(packeta->cseq.method,"BYE"))
	{
		j+=sprintf(SendMsg+j,"%s: %s\r\n","User-Agent",ihostinfo->agent);
	    j+=sprintf(SendMsg+j,"%s: %s\r\n\r\n","Content-Length","0");
	}
}

void SipAckBuilding(char* SendMsg,struct connect_info *ihostinfo,struct sip_packet *packeta,struct sdp_packet *packetb)
{
	//注意对方port换了
	int j;
	strcpy(ihostinfo->targetip,packeta->contact.URI);
	strcpy(ihostinfo->targetport,packeta->contact.port);
	j=sprintf(SendMsg,"%s sip:%s:%s %s\r\n","ACK",ihostinfo->targetip,ihostinfo->targetport,ihostinfo->sipversion);
	j+=sprintf(SendMsg+j,"%s: %s/%s %s:%s\r\n","Via",ihostinfo->sipversion,ihostinfo->baseproto,ihostinfo->hostip,ihostinfo->hostport);
    j+=sprintf(SendMsg+j,"%s: %s\r\n","Max-Forwards",ihostinfo->maxforwards);
	j+=sprintf(SendMsg+j,"%s: \"%s\" <sip:%s>;tag=%s\r\n","From",ihostinfo->username,ihostinfo->hostname,ihostinfo->tag);
	j+=sprintf(SendMsg+j,"%s: \"%s\" <%s>;tag=%s\r\n","To",packeta->to.display_name,packeta->to.URI,packeta->to.tag);
	j+=sprintf(SendMsg+j,"%s: %s@%s\r\n","Call-ID",ihostinfo->callid,ihostinfo->hostip);
	j+=sprintf(SendMsg+j,"%s: 1 %s\r\n","CSeq","ACK");
	j+=sprintf(SendMsg+j,"%s: %s\r\n","User-Agent",ihostinfo->agent);
	j+=sprintf(SendMsg+j,"%s: %s\r\n\r\n","Content-Length","0");

}

void SipByeBuilding(char* SendMsg,struct connect_info *ihostinfo,struct sip_packet *packeta,struct sdp_packet *packetb)
{
	int j;
	j=sprintf(SendMsg,"%s sip:%s:%s %s\r\n","BYE",ihostinfo->targetip,ihostinfo->targetport,ihostinfo->sipversion);
	j+=sprintf(SendMsg+j,"%s: %s/%s %s:%s\r\n","Via",ihostinfo->sipversion,ihostinfo->baseproto,ihostinfo->hostip,ihostinfo->hostport);
    j+=sprintf(SendMsg+j,"%s: %s\r\n","Max-Forwards",ihostinfo->maxforwards);
	j+=sprintf(SendMsg+j,"%s: \"%s\" <sip:%s>;tag=%s\r\n","From",ihostinfo->username,ihostinfo->hostname,ihostinfo->tag);
	j+=sprintf(SendMsg+j,"%s: \"%s\" <%s>;tag=%s\r\n","To",packeta->to.display_name,packeta->to.URI,packeta->to.tag);
	j+=sprintf(SendMsg+j,"%s: %s@%s\r\n","Call-ID",ihostinfo->callid,ihostinfo->hostip);
	j+=sprintf(SendMsg+j,"%s: 2 %s\r\n","CSeq","BYE");
	j+=sprintf(SendMsg+j,"%s: %s\r\n","User-Agent",ihostinfo->agent);
	j+=sprintf(SendMsg+j,"%s: %s\r\n\r\n","Content-Length","0");

}

uint16 Info_Init(struct connect_info* ihostinfo)
{
	strcpy(ihostinfo->sipversion,"SIP/2.0");
    strcpy(ihostinfo->baseproto,"UDP");
	strcpy(ihostinfo->maxforwards,"70");
	strcpy(ihostinfo->username,"enyaxp");
	strcpy(ihostinfo->callid,"0001");
	strcpy(ihostinfo->agent,"ENYAXP1.3");
	strcpy(ihostinfo->proto,"application/sdp");
	strcpy(ihostinfo->tag,"abcd");

    strcpy(ihostinfo->sdpversion,"0");
	strcpy(ihostinfo->net_type,"IN");
    strcpy(ihostinfo->addr_type,"IP4");
	strcpy(ihostinfo->session,"joy_talking");
	strcpy(ihostinfo->BWmodifier,"CT");
	strcpy(ihostinfo->BWvalue,"1000");

    strcpy(ihostinfo->host_key[0].modifier,"base64");
    strcpy(ihostinfo->host_key[1].modifier,"base64");
    strcpy(ihostinfo->host_key[0].value,"fplsbn");
    strcpy(ihostinfo->host_key[1].value,"SdtjN7");


	strcpy(ihostinfo->host_media[0].media,"audio");
    strcpy(ihostinfo->host_media[1].media,"video");
    strcpy(ihostinfo->host_media[2].media,"application");

	strcpy(ihostinfo->host_media[0].port,"3757");
	strcpy(ihostinfo->host_media[1].port,"3323");
	strcpy(ihostinfo->host_media[2].port,"1503");

	strcpy(ihostinfo->host_media[0].transport,"RTP/AVP");
	strcpy(ihostinfo->host_media[1].transport,"RTP/AVP");
	strcpy(ihostinfo->host_media[2].transport,"tcp");

	strcpy(ihostinfo->host_media[0].fmtlist[0],"112");
    strcpy(ihostinfo->host_media[0].fmtlist[1],"3");
    strcpy(ihostinfo->host_media[1].fmtlist[0],"34");
	strcpy(ihostinfo->host_media[2].fmtlist[0],"msdata");


	strcpy(ihostinfo->host_media[0].attributes[0].attribute,"rtpmap");
    strcpy(ihostinfo->host_media[0].attributes[1].attribute,"fmtp");
    strcpy(ihostinfo->host_media[0].attributes[2].attribute,"rtpmap");
    strcpy(ihostinfo->host_media[0].attributes[3].attribute,"encryption");
    strcpy(ihostinfo->host_media[0].attributes[0].fmt,"112");
    strcpy(ihostinfo->host_media[0].attributes[1].fmt,"112");
    strcpy(ihostinfo->host_media[0].attributes[2].fmt,"3");
    strcpy(ihostinfo->host_media[0].attributes[0].value,"G7221/16000");
    strcpy(ihostinfo->host_media[0].attributes[1].value,"bitrate=24000");
    strcpy(ihostinfo->host_media[0].attributes[2].value,"GSM/8000");
    strcpy(ihostinfo->host_media[0].attributes[3].value,"optional");

    strcpy(ihostinfo->host_media[1].attributes[0].attribute,"rtpmap");
    strcpy(ihostinfo->host_media[1].attributes[1].attribute,"encryption");
    strcpy(ihostinfo->host_media[1].attributes[0].fmt,"34");
    strcpy(ihostinfo->host_media[1].attributes[0].value,"H263/9000");
    strcpy(ihostinfo->host_media[1].attributes[1].value,"optional");

    strcpy(ihostinfo->host_media[2].attributes[0].attribute,"sendonly");
    strcpy(ihostinfo->host_media[2].attributes[1].attribute,"encryption");
    strcpy(ihostinfo->host_media[2].attributes[1].value,"optional");

	return 0;
}