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

    CS_LOBBY_PACKET CS_LobbyPacket{};

    // ���� ���� �� ������ ���� ��ȣ�� ������.
    int SendPacketType = PACKET_TYPE_PLAYER_ADD;
    ReturnValue = send(ClientSocket, (char*)&SendPacketType, sizeof(uint8_t), 0);
    if (ReturnValue == SOCKET_ERROR)
        Disconnect();

    while (ConnectState) {
        // ��Ŷ�� �޴´�.
        ReturnValue = recv(ClientSocket, (char*)&RecievePacketType, sizeof(uint8_t), 0);
        if (ReturnValue == SOCKET_ERROR) 
            Disconnect();

        EnterCriticalSection(&ThreadSection);
        std::string CurrentMode = scene.Mode();
        LeaveCriticalSection(&ThreadSection);

        // �κ���
        if (CurrentMode == "LobbyMode") {
            // ��Ŷ Ÿ���� ������.
            int SendPacketType = PACKET_TYPE_LOBBY;
            ReturnValue = send(ClientSocket, (char*)&SendPacketType, sizeof(uint8_t), 0);
            if (ReturnValue == SOCKET_ERROR) 
                Disconnect();

            // Ŭ���̾�Ʈ �ڽ��� ���¸� ��Ŷ�� ����
            memset(&CS_LobbyPacket, 0, sizeof(CS_LOBBY_PACKET));
            strcpy(CS_LobbyPacket.PlayerTag, PlayerTag.c_str());
            strcpy(CS_LobbyPacket.GunType, PlayerGunType.c_str());
            CS_LobbyPacket.ReadyState = PlayerReadyState;

            //  ��Ŷ ������
            ReturnValue = send(ClientSocket, (char*)&CS_LobbyPacket, sizeof(CS_LOBBY_PACKET), 0);
            if (ReturnValue == SOCKET_ERROR) 
                Disconnect();

            // ���� ��Ŷ ó��
            switch (RecievePacketType) {
                // Ÿ Ŭ���̾�Ʈ ���� ��Ŷ
            case PACKET_TYPE_LOBBY:
                SC_LOBBY_PACKET SC_LobbyPacket{};
                ReturnValue = recv(ClientSocket, (char*)&SC_LobbyPacket, sizeof(SC_LOBBY_PACKET), 0);
                if (ReturnValue == SOCKET_ERROR)
                    Disconnect();

                // ������ �÷��̾���� ������ �ֽ�ȭ �Ѵ�.
                EnterCriticalSection(&ThreadSection);
                auto It = std::find_if(begin(ConnectedPlayer), end(ConnectedPlayer), [&](const OtherClient& Other) {return Other.PlayerTag == SC_LobbyPacket.PlayerTag; });
                if (It != end(ConnectedPlayer)) {
                    It->GunType = SC_LobbyPacket.GunType;
                    It->ReadyState = SC_LobbyPacket.ReadyState;
                }
                LeaveCriticalSection(&ThreadSection);
            break;

            // �κ� �÷��̾� �߰� ��Ŷ
            case PACKET_TYPE_PLAYER_ADD:
                SC_LOBBY_PACKET SC_LobbyPacket{};
                ReturnValue = recv(ClientSocket, (char*)&SC_LobbyPacket, sizeof(SC_LOBBY_PACKET), 0);
                if (ReturnValue == SOCKET_ERROR)
                    Disconnect();

                OtherClient Other{};
                Other.PlayerTag = SC_LobbyPacket.PlayerTag;

                // ������ �÷��̾� ����Ʈ�� �� ����ü�� �߰��Ѵ�.
                EnterCriticalSection(&ThreadSection);
                ConnectedPlayer.push_back(Other);
                ++NumPlayerConnected;
                LeaveCriticalSection(&ThreadSection);
                break;
            }
        }
    }

    closesocket(ClientSocket);
    WSACleanup();
    DeleteCriticalSection(&ThreadSection);
    return 0;
}