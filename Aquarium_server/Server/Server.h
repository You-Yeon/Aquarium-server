#pragma once

#include "../Common/Common.h"
#include "../Common/S2C2S_proxy.h"
#include "../Common/S2C2S_stub.h"

// 클라이언트 캐릭터 종류 
enum	MyCharacter
{
	NONE = 0,
	C_FastFoodGuy,  // 1
	C_Biker,        // 2
	C_FireFighter,  // 3
	C_GamerGirl,    // 4
	C_Gangster,     // 5
	C_Grandma,      // 6
	C_Grandpa,      // 7
	C_HipsterGirl,  // 8
	C_HipsterGuy,   // 9
	C_Hobo,         // 10
	C_Jock,         // 11
	C_Paramedic,    // 12
	C_PunkGirl,     // 13
	C_PunkGuy,      // 14
	C_RoadWorker,   // 15
	C_ShopKeeper,   // 16
	C_SummerGirl,   // 17
	C_Tourist      // 18
};

// 게임 방 정보
class RoomInfo
{
public:
	int m_player_num; // 방 인원 수
	int m_S_Team_num; // 사파이어 팀의 인원 수
	int m_R_Team_num; // 루비 팀의 인원 수
	std::wstring m_weather; // 방 날씨
	
};

// 클라이언트 클래스
class RemoteClient
{
public:
	std::wstring m_userID; // 유저의 HostID 값 (*KEY)
	int m_groupID;// 유저가 속한 P2P Group ID 값
	std::wstring m_Team; // 유저의 팀 정보
	int m_TeamNum; // 유저의 팀 번호 ( * 리스폰 위치와 대기실 순서 )
	MyCharacter m_character; // 유저의 캐릭터 종류 정보
	float m_humidity; // 유저의 습도 ( * 체력 )
	float m_rspX, m_rspY, m_rspZ; // 유저의 리스폰 좌표
	float m_posX, m_posY, m_posZ; // 유저의 위치 값
	float m_velX, m_velY, m_velZ; // 유저의 속력 값
	float m_angleX, m_angleY; // 유저의 회전 값

};

// 서버 클래스
class Aquarium_server
	:public S2C2S::Stub
{
public:

	// RMI 보내기 위한 모듈
	S2C2S::Proxy m_proxy;

	// 클라이언트의 연결을 받기위한 객체
	shared_ptr<CNetServer> m_server;

	// 멀티스레드의 경쟁 상태를 예방하기 위해서 잠금
	CriticalSection m_critSec;

	// 클라이언트의 정보
	// key : client HostID
	unordered_map<int, shared_ptr<RemoteClient> > m_Client_Infos;

	// P2P 방의 정보
	// key : P2PGroup HostID
	unordered_map<int, shared_ptr<RoomInfo> > m_Group_Infos;

	// P2P 그룹의 클라이언트 정보
	// key : P2PGroup HostID
	unordered_map<int, CP2PGroup> m_playerGroups;

	// 방의 최대 인원 수
	int max_player_num = 4;

	Aquarium_server()
	{
	}

	~Aquarium_server()
	{
	}

	void Run(); // 서버 런타임 함수

private:

	// 로그인 원격 함수
	DECRMI_S2C2S_RequestLogin;

	// 게임 방 입장 원격 함수
	DECRMI_S2C2S_JoinGameRoom;

	// 게임 방 퇴장 원격 함수
	DECRMI_S2C2S_LeaveGameRoom;
};