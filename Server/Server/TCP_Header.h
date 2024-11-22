#pragma once
#define _CRT_SECURE_NO_WARNINGS // ���� C �Լ� ��� �� ��� ����
#define _WINSOCK_DEPRECATED_NO_WARNINGS // ���� ���� API ��� �� ��� ����
#pragma comment(lib, "ws2_32") // ws2_32.lib ��ũ

#include <winsock2.h>
#include <ws2tcpip.h>
#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <boost/lockfree/queue.hpp>
#include <vector>
#include <print>
#include "Config.h"

// Ŭ���̾�Ʈ ����
typedef struct {
    SOCKET ClientSocket;
    struct sockaddr_in ClientAddr;
    char Addr[INET_ADDRSTRLEN];  // Ŭ���̾�Ʈ ip �ּ� ���ڿ�
} ClientInfo;

// ť���� ó���ϴ� ���� -> Ŭ���̾�Ʈ ����ü
typedef struct {
    uint8_t PacketType;  // ��Ŷ Ÿ��
    void* PacketPtr;  // Ŭ���̾�Ʈ�� ������ ��Ŷ�� ����Ű�� ������
    ClientInfo* Client; // Ŭ���̾�Ʈ ����
} ClientPacketInfo;

extern std::vector<ClientInfo*> ConnectedClients; // ����� Ŭ���̾�Ʈ ���
extern boost::lockfree::queue<ClientPacketInfo> ClientPacketQueue; // Ŭ���̾�Ʈ ��Ŷ ó�� ť, �� ���� ť�̹Ƿ� �Ӱ豸�� ��� ���ص� ��
extern CRITICAL_SECTION ThreadSection; // �Ӱ豸��

// ���� �Լ� ���� ��� �� ����
void err_quit(const char* msg);

// ���� �Լ� ���� ���
void err_display(const char* msg);

// ���� �Լ� ���� ���
void err_display(int errcode);