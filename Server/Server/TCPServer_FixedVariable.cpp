#include "TCP_Header.h" // ���� �޽��� �Լ� �� TCP ������ �ʿ��� ��� ����
#include "Packet.h"
//#include "LockFreeQueue.h"
#include <queue>
#include <print>

// Ŭ���̾�Ʈ ����
struct ClientInfo {
    SOCKET ClientSocket;
    struct sockaddr_in ClientAddr;
    char Addr[INET_ADDRSTRLEN]{};  // Ŭ���̾�Ʈ ip �ּ� ���ڿ�
};

// ť���� ó���ϴ� ���� -> Ŭ���̾�Ʈ ����ü
typedef struct {
    uint8_t PacketType;  // ��Ŷ Ÿ��
    void* PacketPtr;  // Ŭ���̾�Ʈ�� ������ ��Ŷ�� ����Ű�� ������
    ClientInfo* Client; // Ŭ���̾�Ʈ ����
} ClientPacketInfo;

std::vector<ClientInfo*> ConnectedClients; // ����� Ŭ���̾�Ʈ ���
std::queue<ClientPacketInfo> ClientPacketQueue; // Ŭ���̾�Ʈ ��Ŷ ó�� ť, �� ���� ť�̹Ƿ� �Ӱ豸�� ��� ���ص� ��
CRITICAL_SECTION ThreadSection; // �Ӱ豸��

////////////////////////////////////////////////////////////////

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
            EnterCriticalSection(&ThreadSection);
            ClientPacketQueue.push({ RecievePacketType, SC_LobbyPacket, ThisClient });
            LeaveCriticalSection(&ThreadSection);
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

// ���� -> Ŭ���̾�Ʈ ť ������
DWORD WINAPI ClientQueueThread(LPVOID lpParam) {
    ClientPacketInfo PacketInfo{};
    int ReturnValue{};

    while (true) {
        EnterCriticalSection(&ThreadSection);
        // ť�� ������� ������ ť�� �ִ� ���Ҹ� �ϳ��� ������.
        while (!ClientPacketQueue.empty()) {
            PacketInfo = ClientPacketQueue.front();
            ClientPacketQueue.pop();

            // ��ŶŸ�Կ� ���� �ٸ� ��Ŷ�� Ŭ���̾�Ʈ�� �����Ѵ�.
            // �ڽ��� Ŭ���̾�Ʈ�� ������ ������ Ŭ���̾�Ʈ�� �����Ѵ�.
            switch (PacketInfo.PacketType) {
            case PACKET_TYPE_LOBBY:
                for(auto const& Other : ConnectedClients) {
                    if (Other != PacketInfo.Client) {
                        ReturnValue = send(Other->ClientSocket, (char*)&PacketInfo.PacketType, sizeof(uint8_t), 0);
                        if (ReturnValue == SOCKET_ERROR)
                            continue;

                        ReturnValue = send(Other->ClientSocket, (char*)static_cast<SC_LOBBY_PACKET*>(PacketInfo.PacketPtr), sizeof(SC_LOBBY_PACKET), 0);
                        if (ReturnValue == SOCKET_ERROR) 
                            continue;
                    }
                }

                // ����Ű�� ��Ŷ ����ü ��ü ����
                delete PacketInfo.PacketPtr;
                break;
            }
        }
        LeaveCriticalSection(&ThreadSection);
    }

    return 0;
}

int main(int argc, char* argv[]) {
    int ReturnValue{};

    SOCKET ClientSocket{}; // Ŭ���̾�Ʈ ����
    struct sockaddr_in ClientAddr {};  // Ŭ���̾�Ʈ �ּ�
    int AddrLength{};  // �ּ� ����

   // ���� �ʱ�ȭ
    WSADATA WSA;
    if (WSAStartup(MAKEWORD(2, 2), &WSA) != 0)
        return 1;

    // ���� ����
    SOCKET ListenSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (ListenSocket == INVALID_SOCKET) 
        err_quit("socket()");

    // bind()
    struct sockaddr_in ServerAddr;
    memset(&ServerAddr, 0, sizeof(ServerAddr));
    ServerAddr.sin_family = AF_INET;
    ServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    ServerAddr.sin_port = htons(SERVER_PORT);
    ReturnValue = bind(ListenSocket, (struct sockaddr*)&ServerAddr, sizeof(ServerAddr));
    if (ReturnValue == SOCKET_ERROR) 
        err_quit("bind()");

    // listen()
    ReturnValue = listen(ListenSocket, SOMAXCONN);
    if (ReturnValue == SOCKET_ERROR) 
        err_quit("listen()");

    // �Ӱ豸�� �ʱ�ȭ
    InitializeCriticalSection(&ThreadSection);

    // ť ó�� ������ ����
    HANDLE Thread = CreateThread(NULL, 0, ClientQueueThread, NULL, 0, NULL);
    if (!Thread)
        std::cout << "Failed to create thread." << std::endl;
    else
        CloseHandle(Thread);

    while (true) {
        if (ConnectedClients.size() < MAX_CLIENT) {
            // accept()
            AddrLength = sizeof(ClientAddr);
            ClientSocket = accept(ListenSocket, (struct sockaddr*)&ClientAddr, &AddrLength);
            if (ClientSocket == INVALID_SOCKET) {
                err_display("accept()");
                break;
            }

            // Ŭ���̾�Ʈ�� �����ϸ� Ŭ���̾�Ʈ�� ���ϰ� �ּҸ� ������ �Ķ���ͷ� ����
            ClientInfo* Client = new ClientInfo{ ClientSocket, ClientAddr, NULL };

            // ����� Ŭ���̾�Ʈ ��Ͽ� �߰�
            ConnectedClients.push_back(Client);

            // ���ο� �����带 �����Ͽ� �۾� ����
            HANDLE Thread = CreateThread(NULL, 0, ClientThread, (LPVOID)Client, 0, NULL);
            if (!Thread) {
                std::cout << "Failed to create thread." << std::endl;
                delete Client;
                closesocket(ClientSocket);
            }
            else
                CloseHandle(Thread);
        }
    }

    DeleteCriticalSection(&ThreadSection);

    // ���� �ݱ�
    closesocket(ListenSocket);
    WSACleanup();
}