﻿  





// Generated by PIDL compiler.
// Do not modify this file, but modify the source .pidl file.

  
#include "S2C2S_stub.h"


const unsigned char sz_S2C2S_stub_hRmi[] = 
{ 0x00, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xff, 0x00, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xff,
0xaa, 0xbb, 0xcc, 0xdd, 0xff,0x99, 0xaa, 0xbb, 0xcc, 0x99, 0xaa, 0xdd, 0xff, 0x00 };   


namespace S2C2S {


	bool Stub::ProcessReceivedMessage(::Proud::CReceivedMessage &pa, void* hostTag) 
	{
#ifndef __FreeBSD__ 
		{
			// unusable but required. you may ignore it, because it does not occur any worthless load.
			unsigned char x = sz_S2C2S_stub_hRmi[0]; 
			x++; 
		}
#endif
		::Proud::HostID remote=pa.GetRemoteHostID();
		if(remote==::Proud::HostID_None)
		{
			ShowUnknownHostIDWarning(remote);
		}

		::Proud::CMessage &__msg=pa.GetReadOnlyMessage();
		int orgReadOffset = __msg.GetReadOffset();

		::Proud::RmiID __rmiID;
		if(!__msg.Read(__rmiID))
			goto __fail;
					
		switch((int)__rmiID) // case is to prevent from clang compile error
		{
			case Rmi_RequestLogin:
				{
					::Proud::RmiContext ctx;
					ctx.m_rmiID = __rmiID;
					ctx.m_sentFrom=pa.GetRemoteHostID();
					ctx.m_relayed=pa.IsRelayed();
					ctx.m_hostTag = hostTag;
					ctx.m_encryptMode = pa.GetEncryptMode();
					ctx.m_compressMode = pa.GetCompressMode();
			
			        if(BeforeDeserialize(remote, ctx, __msg) == false)
			        {
			            // The user don't want to call the RMI function. 
						// So, We fake that it has been already called.
						__msg.SetReadOffset(__msg.GetLength());
			            return true;
			        }
			
					std::wstring id; __msg >> id;
					std::wstring password; __msg >> password;
					m_core->PostCheckReadMessage(__msg,RmiName_RequestLogin);
					
			
					if(m_enableNotifyCallFromStub && !m_internalUse)
					{
						::Proud::String parameterString;
						
						::Proud::AppendTextOut(parameterString,id);	
										
						parameterString += _PNT(", ");
						::Proud::AppendTextOut(parameterString,password);	
						
						NotifyCallFromStub(remote, (::Proud::RmiID)Rmi_RequestLogin, 
							RmiName_RequestLogin,parameterString);
			
			#ifdef VIZAGENT
						m_core->Viz_NotifyRecvToStub(remote, (::Proud::RmiID)Rmi_RequestLogin, 
							RmiName_RequestLogin, parameterString);
			#endif
					}
					else if(!m_internalUse)
					{
			#ifdef VIZAGENT
						m_core->Viz_NotifyRecvToStub(remote, (::Proud::RmiID)Rmi_RequestLogin, 
							RmiName_RequestLogin, _PNT(""));
			#endif
					}
						
					int64_t __t0 = 0;
					if(!m_internalUse && m_enableStubProfiling)
					{
						::Proud::BeforeRmiSummary summary;
						summary.m_rmiID = (::Proud::RmiID)Rmi_RequestLogin;
						summary.m_rmiName = RmiName_RequestLogin;
						summary.m_hostID = remote;
						summary.m_hostTag = hostTag;
						BeforeRmiInvocation(summary);
			
						__t0 = ::Proud::GetPreciseCurrentTimeMs();
					}
						
					// Call this method.
					bool __ret = RequestLogin (remote,ctx , id, password );
						
					if(__ret==false)
					{
						// Error: RMI function that a user did not create has been called. 
						m_core->ShowNotImplementedRmiWarning(RmiName_RequestLogin);
					}
						
					if(!m_internalUse && m_enableStubProfiling)
					{
						::Proud::AfterRmiSummary summary;
						summary.m_rmiID = (::Proud::RmiID)Rmi_RequestLogin;
						summary.m_rmiName = RmiName_RequestLogin;
						summary.m_hostID = remote;
						summary.m_hostTag = hostTag;
						int64_t __t1;
			
						__t1 = ::Proud::GetPreciseCurrentTimeMs();
			
						summary.m_elapsedTime = (uint32_t)(__t1 - __t0);
						AfterRmiInvocation(summary);
					}
				}
				break;
			case Rmi_NotifyLoginSuccess:
				{
					::Proud::RmiContext ctx;
					ctx.m_rmiID = __rmiID;
					ctx.m_sentFrom=pa.GetRemoteHostID();
					ctx.m_relayed=pa.IsRelayed();
					ctx.m_hostTag = hostTag;
					ctx.m_encryptMode = pa.GetEncryptMode();
					ctx.m_compressMode = pa.GetCompressMode();
			
			        if(BeforeDeserialize(remote, ctx, __msg) == false)
			        {
			            // The user don't want to call the RMI function. 
						// So, We fake that it has been already called.
						__msg.SetReadOffset(__msg.GetLength());
			            return true;
			        }
			
					m_core->PostCheckReadMessage(__msg,RmiName_NotifyLoginSuccess);
					
			
					if(m_enableNotifyCallFromStub && !m_internalUse)
					{
						::Proud::String parameterString;
						
									
						NotifyCallFromStub(remote, (::Proud::RmiID)Rmi_NotifyLoginSuccess, 
							RmiName_NotifyLoginSuccess,parameterString);
			
			#ifdef VIZAGENT
						m_core->Viz_NotifyRecvToStub(remote, (::Proud::RmiID)Rmi_NotifyLoginSuccess, 
							RmiName_NotifyLoginSuccess, parameterString);
			#endif
					}
					else if(!m_internalUse)
					{
			#ifdef VIZAGENT
						m_core->Viz_NotifyRecvToStub(remote, (::Proud::RmiID)Rmi_NotifyLoginSuccess, 
							RmiName_NotifyLoginSuccess, _PNT(""));
			#endif
					}
						
					int64_t __t0 = 0;
					if(!m_internalUse && m_enableStubProfiling)
					{
						::Proud::BeforeRmiSummary summary;
						summary.m_rmiID = (::Proud::RmiID)Rmi_NotifyLoginSuccess;
						summary.m_rmiName = RmiName_NotifyLoginSuccess;
						summary.m_hostID = remote;
						summary.m_hostTag = hostTag;
						BeforeRmiInvocation(summary);
			
						__t0 = ::Proud::GetPreciseCurrentTimeMs();
					}
						
					// Call this method.
					bool __ret = NotifyLoginSuccess (remote,ctx  );
						
					if(__ret==false)
					{
						// Error: RMI function that a user did not create has been called. 
						m_core->ShowNotImplementedRmiWarning(RmiName_NotifyLoginSuccess);
					}
						
					if(!m_internalUse && m_enableStubProfiling)
					{
						::Proud::AfterRmiSummary summary;
						summary.m_rmiID = (::Proud::RmiID)Rmi_NotifyLoginSuccess;
						summary.m_rmiName = RmiName_NotifyLoginSuccess;
						summary.m_hostID = remote;
						summary.m_hostTag = hostTag;
						int64_t __t1;
			
						__t1 = ::Proud::GetPreciseCurrentTimeMs();
			
						summary.m_elapsedTime = (uint32_t)(__t1 - __t0);
						AfterRmiInvocation(summary);
					}
				}
				break;
			case Rmi_NotifyLoginFailed:
				{
					::Proud::RmiContext ctx;
					ctx.m_rmiID = __rmiID;
					ctx.m_sentFrom=pa.GetRemoteHostID();
					ctx.m_relayed=pa.IsRelayed();
					ctx.m_hostTag = hostTag;
					ctx.m_encryptMode = pa.GetEncryptMode();
					ctx.m_compressMode = pa.GetCompressMode();
			
			        if(BeforeDeserialize(remote, ctx, __msg) == false)
			        {
			            // The user don't want to call the RMI function. 
						// So, We fake that it has been already called.
						__msg.SetReadOffset(__msg.GetLength());
			            return true;
			        }
			
					std::wstring reason; __msg >> reason;
					m_core->PostCheckReadMessage(__msg,RmiName_NotifyLoginFailed);
					
			
					if(m_enableNotifyCallFromStub && !m_internalUse)
					{
						::Proud::String parameterString;
						
						::Proud::AppendTextOut(parameterString,reason);	
						
						NotifyCallFromStub(remote, (::Proud::RmiID)Rmi_NotifyLoginFailed, 
							RmiName_NotifyLoginFailed,parameterString);
			
			#ifdef VIZAGENT
						m_core->Viz_NotifyRecvToStub(remote, (::Proud::RmiID)Rmi_NotifyLoginFailed, 
							RmiName_NotifyLoginFailed, parameterString);
			#endif
					}
					else if(!m_internalUse)
					{
			#ifdef VIZAGENT
						m_core->Viz_NotifyRecvToStub(remote, (::Proud::RmiID)Rmi_NotifyLoginFailed, 
							RmiName_NotifyLoginFailed, _PNT(""));
			#endif
					}
						
					int64_t __t0 = 0;
					if(!m_internalUse && m_enableStubProfiling)
					{
						::Proud::BeforeRmiSummary summary;
						summary.m_rmiID = (::Proud::RmiID)Rmi_NotifyLoginFailed;
						summary.m_rmiName = RmiName_NotifyLoginFailed;
						summary.m_hostID = remote;
						summary.m_hostTag = hostTag;
						BeforeRmiInvocation(summary);
			
						__t0 = ::Proud::GetPreciseCurrentTimeMs();
					}
						
					// Call this method.
					bool __ret = NotifyLoginFailed (remote,ctx , reason );
						
					if(__ret==false)
					{
						// Error: RMI function that a user did not create has been called. 
						m_core->ShowNotImplementedRmiWarning(RmiName_NotifyLoginFailed);
					}
						
					if(!m_internalUse && m_enableStubProfiling)
					{
						::Proud::AfterRmiSummary summary;
						summary.m_rmiID = (::Proud::RmiID)Rmi_NotifyLoginFailed;
						summary.m_rmiName = RmiName_NotifyLoginFailed;
						summary.m_hostID = remote;
						summary.m_hostTag = hostTag;
						int64_t __t1;
			
						__t1 = ::Proud::GetPreciseCurrentTimeMs();
			
						summary.m_elapsedTime = (uint32_t)(__t1 - __t0);
						AfterRmiInvocation(summary);
					}
				}
				break;
			case Rmi_JoinGameRoom:
				{
					::Proud::RmiContext ctx;
					ctx.m_rmiID = __rmiID;
					ctx.m_sentFrom=pa.GetRemoteHostID();
					ctx.m_relayed=pa.IsRelayed();
					ctx.m_hostTag = hostTag;
					ctx.m_encryptMode = pa.GetEncryptMode();
					ctx.m_compressMode = pa.GetCompressMode();
			
			        if(BeforeDeserialize(remote, ctx, __msg) == false)
			        {
			            // The user don't want to call the RMI function. 
						// So, We fake that it has been already called.
						__msg.SetReadOffset(__msg.GetLength());
			            return true;
			        }
			
					std::wstring id; __msg >> id;
					int character_num; __msg >> character_num;
					m_core->PostCheckReadMessage(__msg,RmiName_JoinGameRoom);
					
			
					if(m_enableNotifyCallFromStub && !m_internalUse)
					{
						::Proud::String parameterString;
						
						::Proud::AppendTextOut(parameterString,id);	
										
						parameterString += _PNT(", ");
						::Proud::AppendTextOut(parameterString,character_num);	
						
						NotifyCallFromStub(remote, (::Proud::RmiID)Rmi_JoinGameRoom, 
							RmiName_JoinGameRoom,parameterString);
			
			#ifdef VIZAGENT
						m_core->Viz_NotifyRecvToStub(remote, (::Proud::RmiID)Rmi_JoinGameRoom, 
							RmiName_JoinGameRoom, parameterString);
			#endif
					}
					else if(!m_internalUse)
					{
			#ifdef VIZAGENT
						m_core->Viz_NotifyRecvToStub(remote, (::Proud::RmiID)Rmi_JoinGameRoom, 
							RmiName_JoinGameRoom, _PNT(""));
			#endif
					}
						
					int64_t __t0 = 0;
					if(!m_internalUse && m_enableStubProfiling)
					{
						::Proud::BeforeRmiSummary summary;
						summary.m_rmiID = (::Proud::RmiID)Rmi_JoinGameRoom;
						summary.m_rmiName = RmiName_JoinGameRoom;
						summary.m_hostID = remote;
						summary.m_hostTag = hostTag;
						BeforeRmiInvocation(summary);
			
						__t0 = ::Proud::GetPreciseCurrentTimeMs();
					}
						
					// Call this method.
					bool __ret = JoinGameRoom (remote,ctx , id, character_num );
						
					if(__ret==false)
					{
						// Error: RMI function that a user did not create has been called. 
						m_core->ShowNotImplementedRmiWarning(RmiName_JoinGameRoom);
					}
						
					if(!m_internalUse && m_enableStubProfiling)
					{
						::Proud::AfterRmiSummary summary;
						summary.m_rmiID = (::Proud::RmiID)Rmi_JoinGameRoom;
						summary.m_rmiName = RmiName_JoinGameRoom;
						summary.m_hostID = remote;
						summary.m_hostTag = hostTag;
						int64_t __t1;
			
						__t1 = ::Proud::GetPreciseCurrentTimeMs();
			
						summary.m_elapsedTime = (uint32_t)(__t1 - __t0);
						AfterRmiInvocation(summary);
					}
				}
				break;
			case Rmi_Room_Appear:
				{
					::Proud::RmiContext ctx;
					ctx.m_rmiID = __rmiID;
					ctx.m_sentFrom=pa.GetRemoteHostID();
					ctx.m_relayed=pa.IsRelayed();
					ctx.m_hostTag = hostTag;
					ctx.m_encryptMode = pa.GetEncryptMode();
					ctx.m_compressMode = pa.GetCompressMode();
			
			        if(BeforeDeserialize(remote, ctx, __msg) == false)
			        {
			            // The user don't want to call the RMI function. 
						// So, We fake that it has been already called.
						__msg.SetReadOffset(__msg.GetLength());
			            return true;
			        }
			
					int hostID; __msg >> hostID;
					int character_num; __msg >> character_num;
					m_core->PostCheckReadMessage(__msg,RmiName_Room_Appear);
					
			
					if(m_enableNotifyCallFromStub && !m_internalUse)
					{
						::Proud::String parameterString;
						
						::Proud::AppendTextOut(parameterString,hostID);	
										
						parameterString += _PNT(", ");
						::Proud::AppendTextOut(parameterString,character_num);	
						
						NotifyCallFromStub(remote, (::Proud::RmiID)Rmi_Room_Appear, 
							RmiName_Room_Appear,parameterString);
			
			#ifdef VIZAGENT
						m_core->Viz_NotifyRecvToStub(remote, (::Proud::RmiID)Rmi_Room_Appear, 
							RmiName_Room_Appear, parameterString);
			#endif
					}
					else if(!m_internalUse)
					{
			#ifdef VIZAGENT
						m_core->Viz_NotifyRecvToStub(remote, (::Proud::RmiID)Rmi_Room_Appear, 
							RmiName_Room_Appear, _PNT(""));
			#endif
					}
						
					int64_t __t0 = 0;
					if(!m_internalUse && m_enableStubProfiling)
					{
						::Proud::BeforeRmiSummary summary;
						summary.m_rmiID = (::Proud::RmiID)Rmi_Room_Appear;
						summary.m_rmiName = RmiName_Room_Appear;
						summary.m_hostID = remote;
						summary.m_hostTag = hostTag;
						BeforeRmiInvocation(summary);
			
						__t0 = ::Proud::GetPreciseCurrentTimeMs();
					}
						
					// Call this method.
					bool __ret = Room_Appear (remote,ctx , hostID, character_num );
						
					if(__ret==false)
					{
						// Error: RMI function that a user did not create has been called. 
						m_core->ShowNotImplementedRmiWarning(RmiName_Room_Appear);
					}
						
					if(!m_internalUse && m_enableStubProfiling)
					{
						::Proud::AfterRmiSummary summary;
						summary.m_rmiID = (::Proud::RmiID)Rmi_Room_Appear;
						summary.m_rmiName = RmiName_Room_Appear;
						summary.m_hostID = remote;
						summary.m_hostTag = hostTag;
						int64_t __t1;
			
						__t1 = ::Proud::GetPreciseCurrentTimeMs();
			
						summary.m_elapsedTime = (uint32_t)(__t1 - __t0);
						AfterRmiInvocation(summary);
					}
				}
				break;
			case Rmi_Room_Disappear:
				{
					::Proud::RmiContext ctx;
					ctx.m_rmiID = __rmiID;
					ctx.m_sentFrom=pa.GetRemoteHostID();
					ctx.m_relayed=pa.IsRelayed();
					ctx.m_hostTag = hostTag;
					ctx.m_encryptMode = pa.GetEncryptMode();
					ctx.m_compressMode = pa.GetCompressMode();
			
			        if(BeforeDeserialize(remote, ctx, __msg) == false)
			        {
			            // The user don't want to call the RMI function. 
						// So, We fake that it has been already called.
						__msg.SetReadOffset(__msg.GetLength());
			            return true;
			        }
			
					int hostID; __msg >> hostID;
					m_core->PostCheckReadMessage(__msg,RmiName_Room_Disappear);
					
			
					if(m_enableNotifyCallFromStub && !m_internalUse)
					{
						::Proud::String parameterString;
						
						::Proud::AppendTextOut(parameterString,hostID);	
						
						NotifyCallFromStub(remote, (::Proud::RmiID)Rmi_Room_Disappear, 
							RmiName_Room_Disappear,parameterString);
			
			#ifdef VIZAGENT
						m_core->Viz_NotifyRecvToStub(remote, (::Proud::RmiID)Rmi_Room_Disappear, 
							RmiName_Room_Disappear, parameterString);
			#endif
					}
					else if(!m_internalUse)
					{
			#ifdef VIZAGENT
						m_core->Viz_NotifyRecvToStub(remote, (::Proud::RmiID)Rmi_Room_Disappear, 
							RmiName_Room_Disappear, _PNT(""));
			#endif
					}
						
					int64_t __t0 = 0;
					if(!m_internalUse && m_enableStubProfiling)
					{
						::Proud::BeforeRmiSummary summary;
						summary.m_rmiID = (::Proud::RmiID)Rmi_Room_Disappear;
						summary.m_rmiName = RmiName_Room_Disappear;
						summary.m_hostID = remote;
						summary.m_hostTag = hostTag;
						BeforeRmiInvocation(summary);
			
						__t0 = ::Proud::GetPreciseCurrentTimeMs();
					}
						
					// Call this method.
					bool __ret = Room_Disappear (remote,ctx , hostID );
						
					if(__ret==false)
					{
						// Error: RMI function that a user did not create has been called. 
						m_core->ShowNotImplementedRmiWarning(RmiName_Room_Disappear);
					}
						
					if(!m_internalUse && m_enableStubProfiling)
					{
						::Proud::AfterRmiSummary summary;
						summary.m_rmiID = (::Proud::RmiID)Rmi_Room_Disappear;
						summary.m_rmiName = RmiName_Room_Disappear;
						summary.m_hostID = remote;
						summary.m_hostTag = hostTag;
						int64_t __t1;
			
						__t1 = ::Proud::GetPreciseCurrentTimeMs();
			
						summary.m_elapsedTime = (uint32_t)(__t1 - __t0);
						AfterRmiInvocation(summary);
					}
				}
				break;
		default:
			goto __fail;
		}		
		return true;
__fail:
		{
			__msg.SetReadOffset(orgReadOffset);
			return false;
		}
	}
	#ifdef USE_RMI_NAME_STRING
	const PNTCHAR* Stub::RmiName_RequestLogin =_PNT("RequestLogin");
	#else
	const PNTCHAR* Stub::RmiName_RequestLogin =_PNT("");
	#endif
	#ifdef USE_RMI_NAME_STRING
	const PNTCHAR* Stub::RmiName_NotifyLoginSuccess =_PNT("NotifyLoginSuccess");
	#else
	const PNTCHAR* Stub::RmiName_NotifyLoginSuccess =_PNT("");
	#endif
	#ifdef USE_RMI_NAME_STRING
	const PNTCHAR* Stub::RmiName_NotifyLoginFailed =_PNT("NotifyLoginFailed");
	#else
	const PNTCHAR* Stub::RmiName_NotifyLoginFailed =_PNT("");
	#endif
	#ifdef USE_RMI_NAME_STRING
	const PNTCHAR* Stub::RmiName_JoinGameRoom =_PNT("JoinGameRoom");
	#else
	const PNTCHAR* Stub::RmiName_JoinGameRoom =_PNT("");
	#endif
	#ifdef USE_RMI_NAME_STRING
	const PNTCHAR* Stub::RmiName_Room_Appear =_PNT("Room_Appear");
	#else
	const PNTCHAR* Stub::RmiName_Room_Appear =_PNT("");
	#endif
	#ifdef USE_RMI_NAME_STRING
	const PNTCHAR* Stub::RmiName_Room_Disappear =_PNT("Room_Disappear");
	#else
	const PNTCHAR* Stub::RmiName_Room_Disappear =_PNT("");
	#endif
	const PNTCHAR* Stub::RmiName_First = RmiName_RequestLogin;

}



