#pragma once
#pragma once
#include "TCP_header.h"

// 패킷 타입 열거형 모음, 반드시 서버와 클라가 동일한 것을 써야함
enum PacketTypeEnum {
	PACKET_TYPE_NONE, // 타입 없음
	PACKET_TYPE_LOBBY, // 로비 패킷
	PACKET_TYPE_PLAYER_ADD, // 플레이어 추가 패킷
	PACKET_TYPE_PLAYER_DELETE, // 플레이어 제거 패킷
};

////////////////////////// 패킷 구조체 모음

#pragma pack(push, 1)
// 로비 (클라 -> 서버)
typedef struct {
	char PlayerTag[20];    // 플레이어 이름
	char GunType[20];      // 무기 타입
	bool ReadyState;       // 준비 상태
}CS_LOBBY_PACKET;

// 로비 (서버 -> 클라)
typedef struct {
	char PlayerTag[20];    // 플레이어 이름
	char GunType[20];      // 무기 타입
	bool ReadyState;       // 준비 상태
}SC_LOBBY_PACKET;
#pragma pack(pop)
