# Microsoft Developer Studio Project File - Name="zhSipStack" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=zhSipStack - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "zhSipStack.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "zhSipStack.mak" CFG="zhSipStack - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "zhSipStack - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "zhSipStack - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "zhSipStack - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "zhSipStack - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../Utils" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D _WIN32_WINNT=0X0500 /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "zhSipStack - Win32 Release"
# Name "zhSipStack - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\IctCallBack.cpp
# End Source File
# Begin Source File

SOURCE=.\Phone.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=..\Utils\ThreadBase.cpp
# End Source File
# Begin Source File

SOURCE=.\ThreadNet.cpp
# End Source File
# Begin Source File

SOURCE=.\Uac.cpp
# End Source File
# Begin Source File

SOURCE=.\Utils.cpp
# End Source File
# Begin Source File

SOURCE=.\ZhCall.cpp
# End Source File
# Begin Source File

SOURCE=.\ZhMd5.cpp
# End Source File
# Begin Source File

SOURCE=.\ZhReg.cpp
# End Source File
# Begin Source File

SOURCE=.\ZhSip.cpp
# End Source File
# Begin Source File

SOURCE=.\zhSipStack.cpp
# End Source File
# Begin Source File

SOURCE=.\zhSipStack.rc
# End Source File
# Begin Source File

SOURCE=.\zhSipStackDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\Utils\zhSocket.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\IctCallBack.h
# End Source File
# Begin Source File

SOURCE=.\Phone.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=..\Utils\ThreadBase.h
# End Source File
# Begin Source File

SOURCE=.\ThreadNet.h
# End Source File
# Begin Source File

SOURCE=.\Uac.h
# End Source File
# Begin Source File

SOURCE=.\Utils.h
# End Source File
# Begin Source File

SOURCE=.\ZhCall.h
# End Source File
# Begin Source File

SOURCE=.\ZhMd5.h
# End Source File
# Begin Source File

SOURCE=.\ZhReg.h
# End Source File
# Begin Source File

SOURCE=.\ZhSip.h
# End Source File
# Begin Source File

SOURCE=.\zhSipStack.h
# End Source File
# Begin Source File

SOURCE=.\zhSipStackDlg.h
# End Source File
# Begin Source File

SOURCE=..\Utils\zhSocket.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\zhSipStack.ico
# End Source File
# Begin Source File

SOURCE=.\res\zhSipStack.rc2
# End Source File
# End Group
# Begin Group "SipParser"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Accept.cpp
# End Source File
# Begin Source File

SOURCE=.\Accept.h
# End Source File
# Begin Source File

SOURCE=.\AcceptEncoding.cpp
# End Source File
# Begin Source File

SOURCE=.\AcceptEncoding.h
# End Source File
# Begin Source File

SOURCE=.\AcceptLanguage.cpp
# End Source File
# Begin Source File

SOURCE=.\AcceptLanguage.h
# End Source File
# Begin Source File

SOURCE=.\AlertInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\AlertInfo.h
# End Source File
# Begin Source File

SOURCE=.\Allow.cpp
# End Source File
# Begin Source File

SOURCE=.\Allow.h
# End Source File
# Begin Source File

SOURCE=.\Authorization.cpp
# End Source File
# Begin Source File

SOURCE=.\Authorization.h
# End Source File
# Begin Source File

SOURCE=.\AuthorizationInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\AuthorizationInfo.h
# End Source File
# Begin Source File

SOURCE=.\Body.cpp
# End Source File
# Begin Source File

SOURCE=.\Body.h
# End Source File
# Begin Source File

SOURCE=.\CallId.cpp
# End Source File
# Begin Source File

SOURCE=.\CallId.h
# End Source File
# Begin Source File

SOURCE=.\CallInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\CallInfo.h
# End Source File
# Begin Source File

SOURCE=.\Contact.cpp
# End Source File
# Begin Source File

SOURCE=.\Contact.h
# End Source File
# Begin Source File

SOURCE=.\ContentEncoding.cpp
# End Source File
# Begin Source File

SOURCE=.\ContentEncoding.h
# End Source File
# Begin Source File

SOURCE=.\ContentLength.cpp
# End Source File
# Begin Source File

SOURCE=.\ContentLength.h
# End Source File
# Begin Source File

SOURCE=.\ContentType.cpp
# End Source File
# Begin Source File

SOURCE=.\ContentType.h
# End Source File
# Begin Source File

SOURCE=.\ErrorInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\ErrorInfo.h
# End Source File
# Begin Source File

SOURCE=.\From.cpp
# End Source File
# Begin Source File

SOURCE=.\From.h
# End Source File
# Begin Source File

SOURCE=.\Header.cpp
# End Source File
# Begin Source File

SOURCE=.\Header.h
# End Source File
# Begin Source File

SOURCE=.\MimeVersion.cpp
# End Source File
# Begin Source File

SOURCE=.\MimeVersion.h
# End Source File
# Begin Source File

SOURCE=.\NodeRoot.cpp
# End Source File
# Begin Source File

SOURCE=.\NodeRoot.h
# End Source File
# Begin Source File

SOURCE=.\ProxyAuthenticate.cpp
# End Source File
# Begin Source File

SOURCE=.\ProxyAuthenticate.h
# End Source File
# Begin Source File

SOURCE=.\ProxyAuthenticationInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\ProxyAuthenticationInfo.h
# End Source File
# Begin Source File

SOURCE=.\ProxyAuthorization.cpp
# End Source File
# Begin Source File

SOURCE=.\ProxyAuthorization.h
# End Source File
# Begin Source File

SOURCE=.\RecordRoute.cpp
# End Source File
# Begin Source File

SOURCE=.\RecordRoute.h
# End Source File
# Begin Source File

SOURCE=.\Route.cpp
# End Source File
# Begin Source File

SOURCE=.\Route.h
# End Source File
# Begin Source File

SOURCE=.\SdpMessage.cpp
# End Source File
# Begin Source File

SOURCE=.\SdpMessage.h
# End Source File
# Begin Source File

SOURCE=.\Seq.cpp
# End Source File
# Begin Source File

SOURCE=.\Seq.h
# End Source File
# Begin Source File

SOURCE=.\sip_const.h
# End Source File
# Begin Source File

SOURCE=.\SipMessage.cpp
# End Source File
# Begin Source File

SOURCE=.\SipMessage.h
# End Source File
# Begin Source File

SOURCE=.\SipMessageParse.cpp
# End Source File
# Begin Source File

SOURCE=.\SipMessageParse.h
# End Source File
# Begin Source File

SOURCE=.\SipParserHeader.h
# End Source File
# Begin Source File

SOURCE=.\To.cpp
# End Source File
# Begin Source File

SOURCE=.\To.h
# End Source File
# Begin Source File

SOURCE=.\Url.cpp
# End Source File
# Begin Source File

SOURCE=.\Url.h
# End Source File
# Begin Source File

SOURCE=.\UrlHeader.cpp
# End Source File
# Begin Source File

SOURCE=.\UrlHeader.h
# End Source File
# Begin Source File

SOURCE=.\UrlParam.cpp
# End Source File
# Begin Source File

SOURCE=.\UrlParam.h
# End Source File
# Begin Source File

SOURCE=.\UserAgent.cpp
# End Source File
# Begin Source File

SOURCE=.\UserAgent.h
# End Source File
# Begin Source File

SOURCE=.\Via.cpp
# End Source File
# Begin Source File

SOURCE=.\Via.h
# End Source File
# Begin Source File

SOURCE=.\WwwAuthenticate.cpp
# End Source File
# Begin Source File

SOURCE=.\WwwAuthenticate.h
# End Source File
# Begin Source File

SOURCE=.\ZhList.cpp
# End Source File
# Begin Source File

SOURCE=.\ZhList.h
# End Source File
# End Group
# Begin Group "ZhSip"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Fifo.cpp
# End Source File
# Begin Source File

SOURCE=.\Fifo.h
# End Source File
# Begin Source File

SOURCE=.\IctStateMachine.cpp
# End Source File
# Begin Source File

SOURCE=.\IctStateMachine.h
# End Source File
# Begin Source File

SOURCE=.\IctTransaction.cpp
# End Source File
# Begin Source File

SOURCE=.\IctTransaction.h
# End Source File
# Begin Source File

SOURCE=.\IstStateMachine.cpp
# End Source File
# Begin Source File

SOURCE=.\IstStateMachine.h
# End Source File
# Begin Source File

SOURCE=.\IstTransaction.cpp
# End Source File
# Begin Source File

SOURCE=.\IstTransaction.h
# End Source File
# Begin Source File

SOURCE=.\NictStateMachine.cpp
# End Source File
# Begin Source File

SOURCE=.\NictStateMachine.h
# End Source File
# Begin Source File

SOURCE=.\NictTransaction.cpp
# End Source File
# Begin Source File

SOURCE=.\NictTransaction.h
# End Source File
# Begin Source File

SOURCE=.\NistStateMachine.cpp
# End Source File
# Begin Source File

SOURCE=.\NistStateMachine.h
# End Source File
# Begin Source File

SOURCE=.\NistTransaction.cpp
# End Source File
# Begin Source File

SOURCE=.\NistTransaction.h
# End Source File
# Begin Source File

SOURCE=.\SipDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\SipDialog.h
# End Source File
# Begin Source File

SOURCE=.\SipEvent.cpp
# End Source File
# Begin Source File

SOURCE=.\SipEvent.h
# End Source File
# Begin Source File

SOURCE=.\SipTransaction.cpp
# End Source File
# Begin Source File

SOURCE=.\SipTransaction.h
# End Source File
# Begin Source File

SOURCE=.\SipTransactionManager.cpp
# End Source File
# Begin Source File

SOURCE=.\SipTransactionManager.h
# End Source File
# Begin Source File

SOURCE=.\siptypes.h
# End Source File
# Begin Source File

SOURCE=.\StateMachine.cpp
# End Source File
# Begin Source File

SOURCE=.\StateMachine.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
