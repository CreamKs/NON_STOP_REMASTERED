#include "QueueThread.h"
#include "Packet.h"
#include "Container.h"
#include <print>


// ��ŶŸ�Կ� ���� �ٸ� ��Ŷ�� Ŭ���̾�Ʈ�� �����Ѵ�.
// �ڽ��� Ŭ���̾�Ʈ�� ������ ������ Ŭ���̾�Ʈ�� �����Ѵ�.
void SendToOther(ClientInfo* Info, const char* PacketType, const char* Packet, int Size) {
    int ReturnValue{};
    EnterCriticalSection(&ThreadSection);
    auto& Local = ConnectedClients;
    LeaveCriticalSection(&ThreadSection);

    for (auto const& Other : Local) {
        if (Other != Info) {
            ReturnValue = send(Other->ClientSocket, PacketType, sizeof(uint8_t), 0);
            if (ReturnValue == SOCKET_ERROR)
                continue;

            ReturnValue = send(Other->ClientSocket, Packet, Size, 0);
            if (ReturnValue == SOCKET_ERROR)
                continue;
        }
    }
}

// ���� -> Ŭ���̾�Ʈ ť ������
DWORD WINAPI ClientQueueThread(LPVOID lpParam) {
    while (true) {
        // ť�� ������� ������ ť�� �ִ� ���Ҹ� �ϳ��� ������.
        while (!ClientPacketQueue.empty()) {
            ClientPacketInfo Q_PacketInfo{};
            ClientPacketQueue.pop(Q_PacketInfo);

            switch (Q_PacketInfo.PacketType) {
            case PACKET_TYPE_LOBBY:
                SendToOther(Q_PacketInfo.Client, (char*)&Q_PacketInfo.PacketType, (char*)&Q_PacketInfo.SC_LobbyPacket, sizeof(SC_LOBBY_PACKET));
                break;
            }
        }
    }

    return 0;
}
