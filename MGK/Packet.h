#pragma once
#pragma once
#include "TCP_header.h"

// ��Ŷ Ÿ�� ������ ����, �ݵ�� ������ Ŭ�� ������ ���� �����
enum PacketTypeEnum {
	PACKET_TYPE_NONE, // Ÿ�� ����
	PACKET_TYPE_LOBBY, // �κ� ��Ŷ
	PACKET_TYPE_PLAYER_ADD, // �÷��̾� �߰� ��Ŷ
	PACKET_TYPE_PLAYER_DELETE, // �÷��̾� ���� ��Ŷ
};

////////////////////////// ��Ŷ ����ü ����

#pragma pack(push, 1)
// �κ� (Ŭ�� -> ����)
typedef struct {
	char PlayerTag[20];    // �÷��̾� �̸�
	char GunType[20];      // ���� Ÿ��
	bool ReadyState;       // �غ� ����
}CS_LOBBY_PACKET;

// �κ� (���� -> Ŭ��)
typedef struct {
	char PlayerTag[20];    // �÷��̾� �̸�
	char GunType[20];      // ���� Ÿ��
	bool ReadyState;       // �غ� ����
}SC_LOBBY_PACKET;
#pragma pack(pop)