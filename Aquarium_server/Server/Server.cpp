// Server.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "Server.h"
#include "../Common/S2C2S_proxy.cpp"
#include "../Common/S2C2S_stub.cpp"
#include "../Common/S2C2S_common.cpp"

int main()
{
	Aquarium_server server; // 서버 객체
	server.Run(); // 서버 런타임 시작

	return 0;
}

void Aquarium_server::Run() // 런타임 함수 정의
{
	// 객체 시작
	m_netServer = shared_ptr<CNetServer>(CNetServer::Create());

    // 서버에서 클라 연결을 받으면 호출
    m_netServer->OnClientJoin = [](CNetClientInfo* info)
    {
        cout << "Client " << info->m_HostID << " joined.\n";
    };

    // 클라가 나가면 호출
    m_netServer->OnClientLeave = [this](CNetClientInfo* info, ErrorInfo*, const ByteArray&)
    {
        cout << "Client " << info->m_HostID << " went out.\n";

    };

    //proxy와 stub 객체를 NetServer에 부착
    m_netServer->AttachProxy(&m_proxy);
    m_netServer->AttachStub(this);

    // 서버 config
    CStartServerParameter startConfig; // config 객체
    startConfig.m_protocolVersion = g_protocolVersion; // 프로토콜 버전
    startConfig.m_tcpPorts.Add(g_serverPort); // TCP
    startConfig.m_udpPorts.Add(g_serverPort); // UDP
    startConfig.m_allowServerAsP2PGroupMember = true; // 서버도 P2P 그룹에 추가. 클라이언트 데이터 업데이트 때문에. 

    // 서버는 별도의 thread pool이 있으며 여기서 메시지 수신이나 이벤트가 실행.
    m_netServer->Start(startConfig); // 서버 시작

    // P2P 방 정보 입력
    auto newP2PGroup = make_shared<RoomInfo>();
    newP2PGroup->m_player_num = 0; // 방 인원수는 0명

    // P2P 방 생성
    HostID newP2PID = m_netServer->CreateP2PGroup();
    m_playerGroups[newP2PID] = newP2PGroup;

    // 서버 P2P Join (* 클라이언트 정보 업데이트를 위해서)
    // 대기방에 들어오는 클라이언트에게 다른 클라이언트 정보 업데이트할 때
    m_netServer->JoinP2PGroup(HostID_Server, newP2PID);

    // Hit return to exit
    cout << "Server started. Hit return to exit.\n";
    
    string line;
    getline(std::cin, line);
}

DEFRMI_S2C2S_RequestLogin(Aquarium_server)
{
    cout << "RequestLogin " << StringT2A(id.c_str()).GetString() << " "
        << StringT2A(password.c_str()).GetString() << endl;

    // 잠금 
    CriticalSectionLock lock(m_critSec, true);

    // 이미 있는 유저인가?
    auto it = m_remoteClients.find(remote);
    if (it != m_remoteClients.end())
    {
        // 있으면 로그인 실패 처리
        m_proxy.NotifyLoginFailed(remote, RmiContext::ReliableSend, L"Already logged in.");
        return true;
    }

    // 플레이어 추가하기
    auto newRC = make_shared<RemoteClient>();
    newRC->m_userID = id; // key 값은 Hostid
    m_remoteClients[remote] = newRC;

    // 로그인 성공!
    m_proxy.NotifyLoginSuccess(remote, RmiContext::ReliableSend);
    return true;

}

DEFRMI_S2C2S_JoinGameRoom(Aquarium_server)
{
    cout << "JoinGameRoom is called.\n";

    // 잠금 
    CriticalSectionLock lock(m_critSec, true);

    // 클라이언트 데이터에 있는지 확인
    auto it = m_remoteClients.find(remote);
    if (it == m_remoteClients.end())
    {
        // 없다면 리턴
        return true;
    }

    //  클라이언트 정보 업데이트
    auto& rc = it->second;

    rc->m_character = (MyCharacter)character_num;

    // P2P 방 Key
    HostID RoomID;
    
    // 플레이어 방 검색, 게임 방 조인
    for(auto it = m_playerGroups.begin(); it != m_playerGroups.end(); ++it)
    {
        cout << "P2P 정보" << it->first << it->second << endl;

        // 인원 수가 4 적을 경우 입장
        if (it->second->m_player_num < 4)
        {
            RoomID = HostID(it->first);

            // P2P 서버에 조인
            // 해당 클라이언트는 P2PMemberJoinHandler 이벤트 콜백
            m_netServer->JoinP2PGroup(remote, RoomID);

            // 인원 수 증가
            it->second->m_player_num++;
            
            break;
        }

        // 마지막 방이고 인원 수가 모두 찼을 경우
        if (it == m_playerGroups.end())
        {
            // P2P 방 정보 입력
            auto newP2PGroup = make_shared<RoomInfo>();
            newP2PGroup->m_player_num = 1; // 방 인원수는 1명, 이제 입장할 예정이므로

            // P2P 방 생성
            HostID newP2PID = m_netServer->CreateP2PGroup();
            m_playerGroups[newP2PID] = newP2PGroup;

            RoomID = newP2PID;

            // 서버 P2P Join (* 클라이언트 정보 업데이트를 위해서)
            // 대기방에 들어오는 클라이언트에게 다른 클라이언트 정보 업데이트할 때
            m_netServer->JoinP2PGroup(HostID_Server, RoomID);

            // P2P 서버에 조인
            // 해당 클라이언트는 P2PMemberJoinHandler 이벤트 콜백
            m_netServer->JoinP2PGroup(remote, RoomID);

            break;
        }

    }

    // it의 key 값이 remote와 다를 경우 에러 발생.
    assert(it->first == remote);

    // 받은 플레이어 캐릭터 정보를 업데이트한다.
    // 기존 클라이언트 목록을 순회하면서, 기존 클라이언트에게 새 클라이언트의 등장을 알림.
    // 마찬가지로, 새 클라이언트에게 기존 클라이언트의 등장을 알림.
    for (auto otherIter : m_remoteClients)
    {
        if (otherIter.first != it->first)
        {
            auto& otherRC = otherIter.second; // 기존 클라이언트 정보

            // 기존 클라이언트에게 새 클라이언트를 알림
            m_proxy.Room_Appear((HostID)it->first, RmiContext::ReliableSend,
                (HostID)otherIter.first, rc->m_character, rc->m_team);

            // 새 클라이언트에게 기존 클라이언트를 알림
            m_proxy.Room_Appear((HostID)otherIter.first, RmiContext::ReliableSend,
                (HostID)it->first, rc->m_character, rc->m_team);
        }
    }

    return true;

}