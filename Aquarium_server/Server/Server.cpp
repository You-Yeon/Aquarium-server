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
    m_server = shared_ptr<CNetServer>(CNetServer::Create());

    // 서버에서 클라 연결을 받으면 호출
    m_server->OnClientJoin = [](CNetClientInfo* info)
    {
        cout << "Client " << info->m_HostID << " joined.\n";
    };

    // 클라가 나가면 호출
    m_server->OnClientLeave = [this](CNetClientInfo* info, ErrorInfo*, const ByteArray&)
    {
        cout << "Client " << info->m_HostID << " went out.\n";

        CriticalSectionLock lock(m_critSec, true);

        // 플레이어가 P2P 방에 있었을 경우
        if (m_Client_Infos[info->m_HostID]->m_groupID != 0)
        {
            // P2P 방 아이디
            HostID GroupID = static_cast<HostID>(m_Client_Infos[info->m_HostID]->m_groupID);

            // 클라이언트의 그룹아이디 값 초기화
            m_Client_Infos[info->m_HostID]->m_groupID = 0;

            // P2P 방에서 클라이언트 삭제
            m_server->LeaveP2PGroup(info->m_HostID, GroupID);

            // 만약 방에 나만 있었다면, P2P 방을 삭제.
            if (m_Group_Infos[GroupID]->m_player_num == 1)
            {
                // P2P 방 삭제
                m_Group_Infos.erase(GroupID);
                m_playerGroups.erase(GroupID);
            }
            // 방에 나 이외에 다른 사람도 있었다면 P2P 방을 계속 보관.
            else if (m_Group_Infos[GroupID]->m_player_num > 1)
            {
                // 플레이어 감소
                m_Group_Infos[GroupID]->m_player_num--;
                if (m_Client_Infos[info->m_HostID]->m_Team.compare(L"Sapphire") == 0)
                {
                    m_Group_Infos[GroupID]->m_S_Team_num--;
                }
                if (m_Client_Infos[info->m_HostID]->m_Team.compare(L"Ruby") == 0)
                {
                    m_Group_Infos[GroupID]->m_R_Team_num--;
                }

                // 멤버 삭제
                m_playerGroups[GroupID].m_members.RemoveOneByValue(info->m_HostID);

                // P2P 방에 있는 클라이언트에게 게임 방 퇴장 알림.
                if (true == m_server->GetP2PGroupInfo(GroupID, m_playerGroups.find(GroupID)->second))
                {
                    for (auto member : m_playerGroups.find(GroupID)->second.m_members)
                    {
                        m_proxy.Room_Disappear(member, RmiContext::ReliableSend, m_Client_Infos[info->m_HostID]->m_TeamNum);
                    }
                }
            }
        }

        // 플레이어 삭제
        m_Client_Infos.erase(info->m_HostID);
    };

    //proxy와 stub 객체를 NetServer에 부착
    m_server->AttachProxy(&m_proxy);
    m_server->AttachStub(this);

    // 서버 config
    CStartServerParameter startConfig; // config 객체
    startConfig.m_protocolVersion = g_protocolVersion; // 프로토콜 버전
    startConfig.m_tcpPorts.Add(g_serverPort); // TCP
    startConfig.m_udpPorts.Add(g_serverPort); // UDP
    startConfig.m_allowServerAsP2PGroupMember = true; // 서버도 P2P 그룹에 추가. 클라이언트 데이터 업데이트 때문에. 

    // 서버는 별도의 thread pool이 있으며 여기서 메시지 수신이나 이벤트가 실행.
    m_server->Start(startConfig); // 서버 시작

    // P2P HOSTID 생성
    HostID newP2PID = m_server->CreateP2PGroup();

    // P2P 방 정보 입력
    auto newGroupInfo = make_shared<RoomInfo>();
    newGroupInfo->m_player_num = 0; // 방 인원 수는 0명
    newGroupInfo->m_S_Team_num = 0; // 사파이어 팀의 인원 수는 0명
    newGroupInfo->m_R_Team_num = 0; // 루비 팀의 인원 수는 0명

    m_Group_Infos[newP2PID] = newGroupInfo;

    // P2P 방 생성
    CP2PGroup newP2PGroup;
    newP2PGroup.m_groupHostID = newP2PID;

    // 서버 P2P Join (* 클라이언트 정보 업데이트를 위해서)
    // 대기방에 들어오는 클라이언트에게 다른 클라이언트 정보 업데이트할 때
    newP2PGroup.m_members.Add(HostID_Server); 

    m_playerGroups[newP2PID] = newP2PGroup;

    //m_server->JoinP2PGroup(HostID_Server, newP2PID);

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
    auto it = m_Client_Infos.find(remote);
    if (it != m_Client_Infos.end())
    {
        // 있으면 로그인 실패 처리
        m_proxy.NotifyLoginFailed(remote, RmiContext::ReliableSend, L"Already logged in.");
        return true;
    }

    // 플레이어 정보 추가하기
    auto newRC = make_shared<RemoteClient>();
    newRC->m_userID = id; // key 값은 Hostid
    newRC->m_groupID = 0; // 그룹 ID값은 0으로 초기화
    m_Client_Infos[remote] = newRC;

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
    auto it = m_Client_Infos.find(remote);
    if (it == m_Client_Infos.end())
    {
        // 없다면 리턴
        return true;
    }

    //  클라이언트 정보 업데이트
    auto& rc = it->second;

    rc->m_character = static_cast<MyCharacter>(character_num);

    // P2P 방 Key
    HostID RoomID;

    // 플레이어 방 검색, 게임 방 조인
    for(auto it = m_Group_Infos.begin(); it != m_Group_Infos.end(); ++it)
    {
        cout << "P2P 정보" << it->first << it->second << endl;

        // 인원 수가 4 적을 경우 입장
        if (it->second->m_player_num < max_player_num)
        {
            cout << "인원 수가 적음. " << endl;

            RoomID = HostID(it->first);

            // P2P 그룹에 추가
            // 해당 클라이언트는 P2PMemberJoinHandler 이벤트 콜백
            m_playerGroups.find(RoomID)->second.m_members.Add(remote);

            //m_server->JoinP2PGroup(remote, RoomID);

            // 인원 수 증가
            it->second->m_player_num++;
            
            // P2PGroup ID 저장
            m_Client_Infos[remote]->m_groupID = RoomID;

            break;
        }

        // 마지막 방이고 인원 수가 모두 찼을 경우
        if (it == --m_Group_Infos.end())
        {
            cout << "새로운 방 생성 " << endl;

            // P2P 방 생성
            RoomID = m_server->CreateP2PGroup();

            // P2P 방 정보 입력
            auto newGroupInfo = make_shared<RoomInfo>();
            newGroupInfo->m_player_num = 1; // 방 인원수는 1명, 이제 입장할 예정이므로
            newGroupInfo->m_S_Team_num = 0; // 사파이어 팀의 인원 수는 0명
            newGroupInfo->m_R_Team_num = 0; // 루비 팀의 인원 수는 0명

            m_Group_Infos[RoomID] = newGroupInfo;

            // P2P 방 생성
            CP2PGroup newP2PGroup;
            newP2PGroup.m_groupHostID = RoomID;

            // 서버 P2P Join (* 클라이언트 정보 업데이트를 위해서)
            // 대기방에 들어오는 클라이언트에게 다른 클라이언트 정보 업데이트할 때
            newP2PGroup.m_members.Add(HostID_Server);

            // 해당 클라이언트는 P2PMemberJoinHandler 이벤트 콜백
            newP2PGroup.m_members.Add(remote); // 클라이언트 ID 추가

            m_playerGroups[RoomID] = newP2PGroup;

            //m_server->JoinP2PGroup(HostID_Server, RoomID);
            //m_server->JoinP2PGroup(remote, RoomID);

            // P2PGroup ID 저장
            m_Client_Infos[remote]->m_groupID = RoomID;

            break;
        }

    }

    // 만약 서버 시작시에 방이 생성되어 1개 이상이지만 그 마저 없다면.
    if (m_Group_Infos.empty())
    {
        cout << "아무것도 없기에 새로운 방 생성 " << endl;

        // P2P 방 생성
        RoomID = m_server->CreateP2PGroup();

        // P2P 방 정보 입력
        auto newGroupInfo = make_shared<RoomInfo>();
        newGroupInfo->m_player_num = 1; // 방 인원수는 1명, 이제 입장할 예정이므로
        newGroupInfo->m_S_Team_num = 0; // 사파이어 팀의 인원 수는 0명
        newGroupInfo->m_R_Team_num = 0; // 루비 팀의 인원 수는 0명

        m_Group_Infos[RoomID] = newGroupInfo;

        // P2P 방 생성
        CP2PGroup newP2PGroup;
        newP2PGroup.m_groupHostID = RoomID;

        // 서버 P2P Join (* 클라이언트 정보 업데이트를 위해서)
        // 대기방에 들어오는 클라이언트에게 다른 클라이언트 정보 업데이트할 때
        newP2PGroup.m_members.Add(HostID_Server);

        // 해당 클라이언트는 P2PMemberJoinHandler 이벤트 콜백
        newP2PGroup.m_members.Add(remote); // 클라이언트 ID 추가

        m_playerGroups[RoomID] = newP2PGroup;

        //m_server->JoinP2PGroup(HostID_Server, RoomID);
        //m_server->JoinP2PGroup(remote, RoomID);

        // P2PGroup ID 저장
        m_Client_Infos[remote]->m_groupID = RoomID;
    }

    int S_num = m_Group_Infos[RoomID]->m_S_Team_num;
    int R_num = m_Group_Infos[RoomID]->m_R_Team_num;

    wstring TeamColor; // 팀 색상

    // 플레이어 팀 색상 및 번호 부여
    if (S_num > R_num) // 2 : 1 or 1 : 0 ( 루비 팀 부여 )
    {
        // 플레이어 팀 업데이트 및 인원 추가 업데이트
        TeamColor = L"Ruby";
        m_Client_Infos[remote]->m_Team = L"Ruby";
        m_Group_Infos[RoomID]->m_R_Team_num++;
    }
    else if (S_num < R_num) // 1 : 2 or 0 : 1 ( 사파이어 팀 부여 )
    {
        // 플레이어 팀 업데이트 및 인원 추가 업데이트
        TeamColor = L"Sapphire";
        m_Client_Infos[remote]->m_Team = L"Sapphire";
        m_Group_Infos[RoomID]->m_S_Team_num++;
    }
    else if (S_num == R_num) // 0 : 0 or 1 : 1 ( 사파이어 팀 부여 )
    {
        // 플레이어 팀 업데이트 및 인원 추가 업데이트
        TeamColor = L"Sapphire";
        m_Client_Infos[remote]->m_Team = L"Sapphire";
        m_Group_Infos[RoomID]->m_S_Team_num++;
    }

    // 루비 팀일 경우에는 2번 혹은 4번
    if (TeamColor.compare(L"Ruby") == 0)
    {
        if (m_Group_Infos[RoomID]->m_R_Team_num == 1) // 본인 클라이언트만 있을 경우
        {
            m_Client_Infos[remote]->m_TeamNum = 2; // 2번 즉, 맨 앞
        }

        else // 같은 팀원이 있을 경우
        {
            for (auto it : m_playerGroups[RoomID].m_members) // 같은 방 팀원의 ID에서
            {
                if (it != HostID_Server && m_Client_Infos[it]->m_Team.compare(L"Ruby") == 0) // 같은 팀원의 ID를 찾자.
                {
                    if (m_Client_Infos[it]->m_TeamNum == 2) // 같은 팀원의 번호가 2일 경우
                    {
                        m_Client_Infos[remote]->m_TeamNum = 4; // 본인은 4번으로
                        break;
                    }
                    else if (m_Client_Infos[it]->m_TeamNum == 4) // 같은 팀원의 번호가 4일 경우
                    {
                        m_Client_Infos[remote]->m_TeamNum = 2; // 본인은 2번으로
                        break;
                    }
                }
            }
        }
    }

    // 사파이어 팀일 경우에는 1번 혹은 3번
    if (TeamColor.compare(L"Sapphire") == 0)
    {
        if (m_Group_Infos[RoomID]->m_S_Team_num == 1) // 본인 클라이언트만 있을 경우
        {
            m_Client_Infos[remote]->m_TeamNum = 1; // 1번 즉, 맨 앞
        }

        else // 같은 팀원이 있을 경우
        {
            for (auto it : m_playerGroups[RoomID].m_members) // 같은 방 팀원의 ID에서
            {
                if (it != HostID_Server && m_Client_Infos[it]->m_Team.compare(L"Sapphire") == 0) // 같은 팀원의 ID를 찾자.
                {
                if (m_Client_Infos[it]->m_TeamNum == 1) // 같은 팀원의 번호가 2일 경우
                {
                    m_Client_Infos[remote]->m_TeamNum = 3; // 본인은 4번으로
                    break;
                }
                else if (m_Client_Infos[it]->m_TeamNum == 3) // 같은 팀원의 번호가 4일 경우
                {
                    m_Client_Infos[remote]->m_TeamNum = 1; // 본인은 2번으로
                    break;
                }
                }
            }
        }
    }


    // it의 key 값이 remote와 다를 경우 에러 발생.
    assert(it->first == remote);

    // 받은 플레이어 캐릭터 정보를 업데이트한다.
    // 기존 클라이언트 목록을 순회하면서, 기존 클라이언트에게 새 클라이언트의 등장을 알림.
    // 마찬가지로, 새 클라이언트에게 기존 클라이언트의 등장을 알림.
    if (true == m_server->GetP2PGroupInfo(RoomID, m_playerGroups.find(RoomID)->second))
    {
        for (auto member : m_playerGroups.find(RoomID)->second.m_members)
        {
            if (member != HostID_Server) // 서버는 제외
            {
                cout << "member : " << member << endl;

                if (member != it->first) // 기존 클라이언트일 경우
                {
                    auto& other = m_Client_Infos.find(member)->second; // 기존 클라이언트 정보

                    // 기존 클라이언트에게 새 클라이언트를 알림
                    m_proxy.Room_Appear((HostID)member, RmiContext::ReliableSend,
                        (HostID)it->first, rc->m_userID, rc->m_character, rc->m_Team, rc->m_TeamNum);

                    // 새 클라이언트에게 기존 클라이언트를 알림
                    m_proxy.Room_Appear((HostID)it->first, RmiContext::ReliableSend,
                        (HostID)member, other->m_userID, other->m_character, other->m_Team, other->m_TeamNum);
                }

                else // 새 클라이언트일 경우
                {
                    // 기존 클라이언트에게 기존 클라이언트의 정보 업데이트
                    m_proxy.Room_Appear((HostID)it->first, RmiContext::ReliableSend,
                        (HostID)it->first, rc->m_userID, rc->m_character, rc->m_Team, rc->m_TeamNum);
                }
            }
        }

    }

    return true;

}

DEFRMI_S2C2S_LeaveGameRoom(Aquarium_server)
{
    cout << "LeaveGameRoom is called.\n";

    // 잠금 
    CriticalSectionLock lock(m_critSec, true);

    // 클라이언트 데이터에 있는지 확인
    auto it = m_Client_Infos.find(remote);
    if (it == m_Client_Infos.end())
    {
        // 없다면 리턴
        return true;
    }

    // P2P 방 아이디
    HostID GroupID = static_cast<HostID>(m_Client_Infos[remote]->m_groupID);

    // it의 key 값이 remote와 다를 경우 에러 발생.
    assert(it->first == remote);

    // 클라이언트의 그룹아이디 값 초기화
    m_Client_Infos[remote]->m_groupID = 0;

    // P2P 방에서 클라이언트 삭제
    m_server->LeaveP2PGroup(remote, GroupID);

    // 만약 방에 나만 있었다면, P2P 방을 삭제.
    if (m_Group_Infos[GroupID]->m_player_num == 1)
    {
        // P2P 방 삭제
        m_Group_Infos.erase(GroupID);
        m_playerGroups.erase(GroupID);
    }
    // 방에 나 이외에 다른 사람도 있었다면 P2P 방을 계속 보관.
    else if (m_Group_Infos[GroupID]->m_player_num > 1)
    {
        // 플레이어 감소
        m_Group_Infos[GroupID]->m_player_num--;
        if (m_Client_Infos[remote]->m_Team.compare(L"Sapphire") == 0)
        {
            m_Group_Infos[GroupID]->m_S_Team_num--;
        }
        if (m_Client_Infos[remote]->m_Team.compare(L"Ruby") == 0)
        {
            m_Group_Infos[GroupID]->m_R_Team_num--;
        }
            
        // 멤버 삭제
        m_playerGroups[GroupID].m_members.RemoveOneByValue(remote);

        // P2P 방에 있는 클라이언트에게 게임 방 퇴장 알림.
        if (true == m_server->GetP2PGroupInfo(GroupID, m_playerGroups.find(GroupID)->second))
        {
            for (auto member : m_playerGroups.find(GroupID)->second.m_members)
            {
                m_proxy.Room_Disappear(member, RmiContext::ReliableSend, m_Client_Infos[remote]->m_TeamNum);
            }
        }
    }

    return true;
}