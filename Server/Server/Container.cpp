#include "TCP_Header.h"

std::vector<ClientInfo*> ConnectedClients; // ����� Ŭ���̾�Ʈ ���
boost::lockfree::queue<ClientPacketInfo> ClientPacketQueue; // Ŭ���̾�Ʈ ��Ŷ ó�� ť, �� ���� ť�̹Ƿ� �Ӱ豸�� ��� ���ص� ��
CRITICAL_SECTION ThreadSection; // �Ӱ豸��