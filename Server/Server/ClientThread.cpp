#include "TCP_Header.h"
#include "ClientThread.h"
#include "Packet.h"


// Ŭ���̾�Ʈ ������
DWORD WINAPI ClientThread(LPVOID lpParam) {
    bool ConnectState = true; // ���� ����, false�� �� �� ������ ��� ����
    int ReturnValue{};

    // ���� ��Ŷ Ÿ��
    uint8_t RecievePacketType{};

    // �ٸ� Ŭ���̾�Ʈ�� ���� ��Ŷ Ÿ��
    uint8_t SendPacketType{};

    /////////////// Ŭ���̾�Ʈ ���� �� �ּ�
    SOCKET ClientSocket{}; // Ŭ���̾�Ʈ ����
    struct sockaddr_in ClientAddr {};  // Ŭ���̾�Ʈ ip �ּ�

    // ������ Ŭ�� ���� �ޱ�
    ClientInfo* ThisClient = (ClientInfo*)lpParam;
    ClientSocket = ThisClient->ClientSocket;
    ClientAddr = ThisClient->ClientAddr;
    inet_ntop(AF_INET, &ClientAddr.sin_addr, ThisClient->Addr, sizeof(ThisClient->Addr));

    // Ŭ���̾�Ʈ ��Ŷ ó�� ����ü
    ClientPacketInfo PacketInfo{};

    while (ConnectState) {
        // Ŭ���̾�Ʈ�κ��� ��Ŷ Ÿ���� ���� �޴´�
        ReturnValue = recv(ClientSocket, (char*)&RecievePacketType, sizeof(uint8_t), 0);
        if (ReturnValue == SOCKET_ERROR)
            ConnectState = false;

        // ���� ��Ŷ Ÿ�Կ� ���� �ٸ� ��Ŷ Ÿ���� �޴´�
        switch (RecievePacketType) {
        case PACKET_TYPE_LOBBY:
            SC_LOBBY_PACKET* SC_LobbyPacket = new SC_LOBBY_PACKET;
            CS_LOBBY_PACKET CS_LobbyPacket{};

            // CS_LobbyPacket �ޱ�
            ReturnValue = recv(ClientSocket, (char*)&CS_LobbyPacket, sizeof(CS_LOBBY_PACKET), 0);
            if (ReturnValue == SOCKET_ERROR)
                ConnectState = false;

            // �÷��̾� �±�, �÷��̾� �� Ÿ��, �÷��̾� �غ���� ������ ����
            strncpy((char*)SC_LobbyPacket->PlayerTag, (const char*)CS_LobbyPacket.PlayerTag, sizeof(CS_LobbyPacket.PlayerTag));
            strncpy((char*)SC_LobbyPacket->GunType, (const char*)CS_LobbyPacket.GunType, sizeof(CS_LobbyPacket.GunType));
            SC_LobbyPacket->ReadyState = CS_LobbyPacket.ReadyState;

            // ť�� Ŭ���̾�Ʈ Ŭ���̾�Ʈ�� ���� ��Ŷ ���� �߰�
            ClientPacketQueue.push({ RecievePacketType, SC_LobbyPacket, ThisClient });
            break;
        }
    }

    // ���� ���� �� ������ Ŭ���̾�Ʈ ��Ͽ��� ���� �� ���� �ݱ�
    EnterCriticalSection(&ThreadSection);
    auto It = std::find(ConnectedClients.begin(), ConnectedClients.end(), ThisClient);
    if (It != ConnectedClients.end())
        ConnectedClients.erase(It);
    LeaveCriticalSection(&ThreadSection);

    delete ThisClient;
    closesocket(ClientSocket);

    return 0;
}