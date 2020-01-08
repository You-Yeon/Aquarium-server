#pragma once

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
{
public:
	// 클라이언트의 연결을 받기위한 객체
	shared_ptr<CNetServer> m_netServer;

	Aquarium_server()
	{
	}

	~Aquarium_server()
	{
	}

	void Run(); // 서버 런타임 함수

private:

};