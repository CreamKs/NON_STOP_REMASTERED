#include "QueueThread.h"
#include "Packet.h"
#include "Container.h"
#include <print>


// 패킷타입에 따라 다른 패킷을 클라이언트에 전송한다.
// 자신의 클라이언트를 제외한 나머지 클라이언트에 전송한다.
void SendToOther(ClientInfo* Info, const char* PacketType, const char* Packet, int Size) {
    int ReturnValue{};
    EnterCriticalSection(&ThreadSection);
    auto& Local = ConnectedClients;
    LeaveCriticalSection(&ThreadSection);

    for (auto const& Other : Local) {
        if (Other != Info) {
            ReturnValue = send(Other->ClientSocket, PacketType, sizeof(uint8_t), 0);
            //std::println("Send {}: {} to {}", PacketType, Info->ID, Other->ID);
            if (ReturnValue == SOCKET_ERROR)
                continue;

            ReturnValue = send(Other->ClientSocket, Packet, Size, 0);
            if (ReturnValue == SOCKET_ERROR)
                continue;
        }
    }
}

// 서버 -> 클라이언트 큐 스레드
DWORD WINAPI ClientQueueThread(LPVOID lpParam) {
    while (true) {
        // 큐가 비어있지 않으면 큐에 있는 원소를 하나씩 꺼낸다.
        while (!ClientPacketQueue.empty()) {
            ClientPacketInfo Q_PacketInfo{};
            ClientPacketQueue.pop(Q_PacketInfo);

            switch (Q_PacketInfo.PacketType) {
            case PACKET_TYPE_ENTER:
                SendToOther(Q_PacketInfo.Client, (char*)&Q_PacketInfo.PacketType, (char*)&Q_PacketInfo.SC_LobbyPacket, sizeof(SC_LOBBY_PACKET));
                break;

            case PACKET_TYPE_MOVE:
                SendToOther(Q_PacketInfo.Client, (char*)&Q_PacketInfo.PacketType, (char*)&Q_PacketInfo.SC_MovePacket, sizeof(SC_PLAYER_MOVE_PACKET));
                break;
            }
        }
    }

    return 0;
}
