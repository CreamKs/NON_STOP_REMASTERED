#include "QueueThread.h"
#include "Packet.h"

// ���� -> Ŭ���̾�Ʈ ť ������
DWORD WINAPI ClientQueueThread(LPVOID lpParam) {
    ClientPacketInfo PacketInfo{};
    int ReturnValue{};

    while (true) {
        // ť�� ������� ������ ť�� �ִ� ���Ҹ� �ϳ��� ������.
        while (!ClientPacketQueue.pop(PacketInfo)) {
            // ��ŶŸ�Կ� ���� �ٸ� ��Ŷ�� Ŭ���̾�Ʈ�� �����Ѵ�.
            // �ڽ��� Ŭ���̾�Ʈ�� ������ ������ Ŭ���̾�Ʈ�� �����Ѵ�.
            switch (PacketInfo.PacketType) {
            case PACKET_TYPE_LOBBY:
                EnterCriticalSection(&ThreadSection);
                for (auto const& Other : ConnectedClients) {
                    if (Other != PacketInfo.Client) {
                        ReturnValue = send(Other->ClientSocket, (char*)&PacketInfo.PacketType, sizeof(uint8_t), 0);
                        if (ReturnValue == SOCKET_ERROR)
                            continue;

                        ReturnValue = send(Other->ClientSocket, (char*)static_cast<SC_LOBBY_PACKET*>(PacketInfo.PacketPtr), sizeof(SC_LOBBY_PACKET), 0);
                        if (ReturnValue == SOCKET_ERROR)
                            continue;
                    }
                }
                LeaveCriticalSection(&ThreadSection);

                // ����Ű�� ��Ŷ ����ü ��ü ����
                delete PacketInfo.PacketPtr;
                break;
            }
        }
    }

    return 0;
}
