#pragma once
#include "TCP_header.h"

// ��Ŷ Ÿ�� ������ ����, �ݵ�� ������ Ŭ�� ������ ���� �����
enum PacketTypeEnum {
	PACKET_TYPE_NONE,
	PACKET_TYPE_LOBBY
};

////////////////////////// ��Ŷ ����ü ����
#pragma pack(push, 1)

// �κ� (Ŭ�� -> ����)
typedef struct {
	char PlayerTag[50];    // �÷��̾� �̸�
	char GunType[20];      // ���� Ÿ��
	bool ReadyState;       // �غ� ����
}CS_LOBBY_PACKET;

// �κ� (���� -> Ŭ��)
typedef struct {
	char PlayerTag[50];    // �÷��̾� �̸�
	char GunType[20];      // ���� Ÿ��
	bool ReadyState;       // �غ� ����
}SC_LOBBY_PACKET;

#pragma pack(pop)