#include "TCP_Header.h"
#include "ClientThread.h"
#include "Packet.h"
#include "Container.h"

// Ŭ���̾�Ʈ ������
DWORD WINAPI ClientThread(LPVOID lpParam) {
    bool ConnectState = true; // ���� ����, false�� �� �� ������ ��� ����
    int ReturnValue{};

    // ���� ��Ŷ Ÿ��
    uint8_t RecievePacketType{};

    /////////////// Ŭ���̾�Ʈ ���� �� �ּ�
    SOCKET ClientSocket{}; // Ŭ���̾�Ʈ ����
    struct sockaddr_in ClientAddr {};  // Ŭ���̾�Ʈ ip �ּ�

    // ������ Ŭ�� ���� �ޱ�
    ClientInfo* ThisClient = (ClientInfo*)lpParam;
    ClientSocket = ThisClient->ClientSocket;
    ClientAddr = ThisClient->ClientAddr;
    inet_ntop(AF_INET, &ClientAddr.sin_addr, ThisClient->Addr, sizeof(ThisClient->Addr));

    ClientPacketInfo C_PacketInfo{};
    CS_LOBBY_PACKET CS_LobbyPacket{};

    while (ConnectState) {
        // Ŭ���̾�Ʈ�κ��� ��Ŷ Ÿ���� ���� �޴´�
        ReturnValue = recv(ClientSocket, (char*)&RecievePacketType, sizeof(uint8_t), 0);
        if (ReturnValue == SOCKET_ERROR)
            break;

        // ���� ��Ŷ Ÿ�Կ� ���� �ٸ� ��Ŷ Ÿ���� �޴´�
        switch (RecievePacketType) {
        case PACKET_TYPE_LOBBY:
            // �ޱ� �� ����ü �ʱ�ȭ
            // CS_LobbyPacket �ޱ�
            memset(&CS_LobbyPacket, 0, sizeof(CS_LobbyPacket));
            ReturnValue = recv(ClientSocket, (char*)&CS_LobbyPacket, sizeof(CS_LOBBY_PACKET), 0);
            if (ReturnValue == SOCKET_ERROR) {
                ConnectState = false;
                break;
            }

            // ���� ���� �� ����ü �ʱ�ȭ
            // �÷��̾� �±�, �÷��̾� �� Ÿ��, �÷��̾� �غ���� ������ ����
            memset(&C_PacketInfo, 0, sizeof(C_PacketInfo));
            strcpy(C_PacketInfo.SC_LobbyPacket.PlayerTag, CS_LobbyPacket.PlayerTag);
            strcpy(C_PacketInfo.SC_LobbyPacket.GunType, CS_LobbyPacket.GunType);
            C_PacketInfo.SC_LobbyPacket.ReadyState = CS_LobbyPacket.ReadyState;
            C_PacketInfo.PacketType = RecievePacketType;
            C_PacketInfo.Client = ThisClient;

            // ť�� Ŭ���̾�Ʈ Ŭ���̾�Ʈ�� ���� ��Ŷ ���� �߰�
            ClientPacketQueue.push(C_PacketInfo);
            break;

        case PACKET_TYPE_PLAYER_ADD:
            memset(&C_PacketInfo, 0, sizeof(C_PacketInfo));
            strcpy(C_PacketInfo.SC_LobbyPacket.PlayerTag, CS_LobbyPacket.PlayerTag);
            C_PacketInfo.PacketType = RecievePacketType;
            C_PacketInfo.Client = ThisClient;
            ClientPacketQueue.push(C_PacketInfo);
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