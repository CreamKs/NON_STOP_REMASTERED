#pragma once
#include "GameObject.h"

enum EnumLookDir{
	LOOK_RIGHT,
	LOOK_LEFT
};

// GameObject Ŭ�����κ��� ���
class Player : public GameObject {
private:
	// �÷��̾� ��ġ
	glm::vec2 Position{0.0, -0.5};

	// �÷��̾��� ����Ʈ ���� ��ġ
	glm::vec2 ViewportPosition{};

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

public:
	Player();

	// Ű �Է� ��Ʈ�ѷ�
	void InputKey(int State, unsigned char NormalKey, int SpecialKey);

	// ���콺 �Է� ��Ʈ�ѷ�
	void InputMouse(int State);

	// ������Ʈ �Լ�
	// FT�� ������Ÿ��. SceneŬ�����κ��� �޴´�.
	void UpdateFunc(float FT);

	// ������ �Լ�
	void RenderFunc();

	// �ٸ� ��ü �Ǵ� ��ġ���� �÷��̾��� ��ġ ���� ���� �� �ֵ��� �Ѵ�.
	glm::vec2 GetPosition();
};