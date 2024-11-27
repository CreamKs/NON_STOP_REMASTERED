#include "ClientThread.h"
#include "Packet.h"
#include "Scene.h"

void Disconnect() {
    EnterCriticalSection(&ThreadSection);
    ConnectState = false;
    LeaveCriticalSection(&ThreadSection);
}

DWORD WINAPI ClientThread(LPVOID lpParam) {
    int ReturnValue{};
    int RecievePacketType{};

    // ���� �ʱ�ȭ
    WSADATA WSA;
    if (WSAStartup(MAKEWORD(2, 2), &WSA) != 0)
        return 1;

    // Ŭ���̾�Ʈ ���� ����
    SOCKET ClientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (ClientSocket == INVALID_SOCKET)
        err_quit("socket()");

    // connect()
    struct sockaddr_in ServerAddr;
    memset(&ServerAddr, 0, sizeof(ServerAddr));
    ServerAddr.sin_family = AF_INET;
    inet_pton(AF_INET, SERVER_IP, &ServerAddr.sin_addr);
    ServerAddr.sin_port = htons(SERVER_PORT);
    ReturnValue = connect(ClientSocket, (struct sockaddr*)&ServerAddr, sizeof(ServerAddr));
    if (ReturnValue == SOCKET_ERROR)
        err_quit("connect()");

    InitializeCriticalSection(&ThreadSection);

    // ���� ���� Ȱ��ȭ
    EnterCriticalSection(&ThreadSection);
    ConnectState = true;
    LeaveCriticalSection(&ThreadSection);

    SC_LOBBY_PACKET SC_LobbyPacket{};
    CS_LOBBY_PACKET CS_LobbyPacket{};

    while (ConnectState) {
        // ��Ŷ Ÿ���� ������.
        int SendPacketType = PACKET_TYPE_LOBBY;
        ReturnValue = send(ClientSocket, (char*)&SendPacketType, sizeof(uint8_t), 0);
        if (ReturnValue == SOCKET_ERROR)
            err_quit("send() PACKET_TYPE_LOBBY");

        // Ŭ���̾�Ʈ �ڽ��� ���¸� ��Ŷ�� ����
        CS_LOBBY_PACKET CS_LobbyPacket{};
        EnterCriticalSection(&ThreadSection);
        strcpy(CS_LobbyPacket.PlayerTag, PlayerTag.c_str());
        strcpy(CS_LobbyPacket.GunType, PlayerGunType.c_str());
        CS_LobbyPacket.ReadyState = PlayerReadyState;
        LeaveCriticalSection(&ThreadSection);

        //  ��Ŷ ������
        ReturnValue = send(ClientSocket, (char*)&CS_LobbyPacket, sizeof(CS_LOBBY_PACKET), 0);
        if (ReturnValue == SOCKET_ERROR)
            err_quit("send() CS_LOBBY_PACKET");

        // ��Ŷ Ÿ�� �ޱ�
        ReturnValue = recv(ClientSocket, (char*)&RecievePacketType, sizeof(uint8_t), 0);
        if (ReturnValue == SOCKET_ERROR)
            err_quit("recv() PakcetType");

        // ���� ��Ŷ ó��
        switch (RecievePacketType) {
            // Ÿ Ŭ���̾�Ʈ ���� ��Ŷ
        case PACKET_TYPE_LOBBY:
        {
            SC_LOBBY_PACKET SC_LobbyPacket{};
            ReturnValue = recv(ClientSocket, (char*)&SC_LobbyPacket, sizeof(SC_LOBBY_PACKET), 0);
            if (ReturnValue == SOCKET_ERROR)
                err_quit("recv() SC_LOBBY_PACKET");

            EnterCriticalSection(&ThreadSection);

            // Ŭ���̾�Ʈ�� �̹� ��Ͽ� �ִ��� Ȯ��
            auto it = std::find_if(ConnectedPlayer.begin(), ConnectedPlayer.end(),
                [&](const OTHER_CLIENT& client) {
                    return client.PlayerTag == std::string(SC_LobbyPacket.PlayerTag);
                });

            if (it != ConnectedPlayer.end()) {
                // ���� Ŭ���̾�Ʈ ���� ������Ʈ
                it->GunType = std::string(SC_LobbyPacket.GunType);
                it->ReadyState = SC_LobbyPacket.ReadyState;
            }
            else {
                // ���ο� Ŭ���̾�Ʈ �߰�
                OTHER_CLIENT newClient{};
                newClient.PlayerTag = std::string(SC_LobbyPacket.PlayerTag);
                newClient.GunType = std::string(SC_LobbyPacket.GunType);
                newClient.ReadyState = SC_LobbyPacket.ReadyState;
                ConnectedPlayer.push_back(newClient);
            }

            LeaveCriticalSection(&ThreadSection);
        }
            break;
        }
            
    }

    closesocket(ClientSocket);
    WSACleanup();
    DeleteCriticalSection(&ThreadSection);

    std::cout << "Thread end\n";
    return 0;
}