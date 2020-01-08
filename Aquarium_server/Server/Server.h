#pragma once

#include "../Common/Common.h"
#include "../Common/S2C2S_proxy.h"
#include "../Common/S2C2S_stub.h"

// 클라이언트 클래스
class RemoteClient
{
public:
	std::wstring m_userID; // 유저의 HostID 값 (*KEY)
	float m_x, m_y, m_z; // 유저의 위치 값
	float m_vx, m_vy, m_vz; // 유저의 속력 값
	float m_angle; // 유저의 회전 값

};

// 서버 클래스
class Aquarium_server
	:public S2C2S::Stub // RMI 받는 모듈
{
public:
	// RMI 보내는 모듈 
	S2C2S::Proxy m_proxy;

	// 클라이언트의 연결을 받기위한 객체
	shared_ptr<CNetServer> m_netServer;

	// 멀티스레드로 인한 경쟁상태를 없애기 위해서
	CriticalSection m_critSec;

	// 클라이언트 객채들
	unordered_map<int, shared_ptr<RemoteClient> > m_remoteClients;

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
};