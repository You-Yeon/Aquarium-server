#pragma once

namespace S2C2S {


	//Message ID that replies to each RMI method. 
               
    static const ::Proud::RmiID Rmi_RequestLogin = (::Proud::RmiID)(1000+1);
               
    static const ::Proud::RmiID Rmi_NotifyLoginSuccess = (::Proud::RmiID)(1000+2);
               
    static const ::Proud::RmiID Rmi_NotifyLoginFailed = (::Proud::RmiID)(1000+3);
               
    static const ::Proud::RmiID Rmi_JoinGameRoom = (::Proud::RmiID)(1000+4);
               
    static const ::Proud::RmiID Rmi_LeaveGameRoom = (::Proud::RmiID)(1000+5);
               
    static const ::Proud::RmiID Rmi_Room_Appear = (::Proud::RmiID)(1000+6);
               
    static const ::Proud::RmiID Rmi_Room_Disappear = (::Proud::RmiID)(1000+7);
               
    static const ::Proud::RmiID Rmi_GameStart = (::Proud::RmiID)(1000+8);
               
    static const ::Proud::RmiID Rmi_PlayerInfo = (::Proud::RmiID)(1000+9);
               
    static const ::Proud::RmiID Rmi_Player_Move = (::Proud::RmiID)(1000+10);

	// List that has RMI ID.
	extern ::Proud::RmiID g_RmiIDList[];
	// RmiID List Count
	extern int g_RmiIDListCount;

}


 
