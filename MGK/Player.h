#pragma once
#include "GameObject.h"

enum EnumLookDir{
	LOOK_LEFT,
	LOOK_RIGHT
};

// GameObject Ŭ�����κ��� ���
class Player : public GameObject {
private:
	// �÷��̾� ��ġ
	glm::vec2 Position{0.0, -0.5};

	// �÷��̾� �̵� �ӵ�
	GLfloat Speed{ 2.5 };

	// �÷��̾� �̵� ����
	int LookDir{};

	// �÷��̾� �̵� ����
	bool MoveLeft{}, MoveRight{};

	// ���� ������ ���� ��������
	PhysicsUtil pUtil;

	// �÷��̾� ���� ����
	bool JumpState{};

	// �÷��̾� ��Ƽ� ��� ����
	bool TriggerState{};

	// �÷��̾ ����ִ� �� �̸�
	const char* GunName{"SCAR_H"};

	// �÷��̾ ����ִ� �� ������Ʈ ������
	GameObject* GunPtr{};

public:
	Player();

	// Ű �Է� ��Ʈ�ѷ�
	void InputKey(int State, unsigned char NormalKey, int SpecialKey);

	// ���콺 �Է� ��Ʈ�ѷ�
	void InputMouse(int State);

	void UpdateGun();

	// ������Ʈ �Լ�
	// FT�� ������Ÿ��. SceneŬ�����κ��� �޴´�.
	void UpdateFunc(float FT);

	// ������ �Լ�
	void RenderFunc();

	// �ٸ� ��ü �Ǵ� ��ġ���� �÷��̾��� ��ġ ���� ���� �� �ֵ��� �Ѵ�.
	glm::vec2 GetPosition();
};