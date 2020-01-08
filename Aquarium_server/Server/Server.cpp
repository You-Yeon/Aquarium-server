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

    // Hit return to exit
    cout << "Server started. Hit return to exit.\n";
    
    string line;
    getline(std::cin, line);
}

DEFRMI_S2C2S_RequestLogin(Aquarium_server)
{
    cout << "RequestLogin " << StringT2A(id.c_str()).GetString() << " "
        << StringT2A(password.c_str()).GetString() << endl;

    CriticalSectionLock lock(m_critSec, true);

    // 이미 있는 유저인가?
    auto it = m_remoteClients.find(remote);
    if (it != m_remoteClients.end())
    {
        // 있으면 failed 처리
        m_proxy.NotifyLoginFailed(remote, RmiContext::ReliableSend, L"Already logged in.");
        return true;
    }

    // 플레이어 추가
    auto newRC = make_shared<RemoteClient>();
    newRC->m_userID = id; // key 값은 Hostid
    m_remoteClients[remote] = newRC;

    // 성공!
    m_proxy.NotifyLoginSuccess(remote, RmiContext::ReliableSend);
    return true;
}