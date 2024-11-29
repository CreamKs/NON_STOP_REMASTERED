#pragma once
#include "TCP_header.h"

// ��Ŷ Ÿ�� ������ ����, �ݵ�� ������ Ŭ�� ������ ���� �����
enum PacketTypeEnum {
	PACKET_TYPE_NONE, // Ÿ�� ����
	PACKET_TYPE_ENTER, // �κ� ��Ŷ
	PACKET_TYPE_MOVE,
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

typedef struct { // �÷��̾� �̵�
	char PlayerTag[20];   // �г���
	float x, y;                      // ��ġ
	bool look_dir;                // ���� (0: ����, 1: ������)
	float gun_rotation;          // ���� ȸ�� ��
}CS_PLAYER_MOVE_PACKET;

typedef struct { // �÷��̾� �̵�
	char PlayerTag[20];  // �г���
	float x, y;                    // ��ġ
	bool look_dir;              // ���� (0: ����, 1: ������)
	float gun_rotation;        // ���� ȸ�� ��
}SC_PLAYER_MOVE_PACKET;

#pragma pack(pop)

// ť���� ó���ϴ� ���� -> Ŭ���̾�Ʈ ����ü
typedef struct {
	ClientInfo* Client; // Ŭ���̾�Ʈ ����
	uint8_t PacketType;  // ��Ŷ Ÿ��

	// ��Ŷ ���� �߰� �� ���⿡�� �߰��ؾ� ��
	// ��Ŷ Ÿ�Կ� ���� �ϳ��� ����ü �����͸� ������ ������
	SC_LOBBY_PACKET SC_LobbyPacket;
	SC_PLAYER_MOVE_PACKET SC_MovePacket;
} ClientPacketInfo;