#include "Player.h"
#include "Framework.h"
#include <cmath>

void Player::InputKey(KeyType Type, KeyState State, unsigned char NormalKey, int SpecialKey) {
	switch (Type) {
	case KeyType::NormalKey:
		switch (State) {
		case KeyState::Down:
			switch (NormalKey) {
				// �˿��̵� Ȱ��ȭ
			case 'a':
				MoveLeft = true;
				break;
			case 'd':
				MoveRight = true;
				break;

				// �����̽���
				// ���� Ȱ��ȭ
			case 32:
				// �÷��̾ �� ���� ���� ���� ���� ����
				if(psUtil.IsOnFloor(Height, FLOOR_HEIGHT))
					// ���� ���Ӱ� �߰�, �������� ���·� ����
					psUtil.AddGravityAcc(5.0);
				break;
			}
			break;

		case KeyState::Up:
			switch(NormalKey) {
				// �¿� �̵� ��Ȱ��ȭ
			case 'a':
				MoveLeft = false;
				break;
			case 'd':
				MoveRight = false;
				break;
			}
			break;
		}
		break;
	}
}

GLfloat Player::GetPositionX() {
	return PositionX;
}

GLfloat Player::GetHeight() {
	return Height;
}

GLfloat Player::GetViewportPositionX() {
	return ViewportPositionX;
}

GLfloat Player::GetViewportHeight() {
	return ViewportHeight;
}

Player::Player() {
	SetImage(Image, "player");
}

void Player::UpdateViewportPosition() {
	// ȭ�� ���� ��ǥ ������Ʈ
	ViewportPositionX = ViewportPosition().x;
	ViewportHeight = ViewportPosition().y;
}

void Player::UpdateLook() {
	if (auto Target = framework.Find("target"); Target) {
		// �÷��̾� ��ġ�� ������ ��ġ�� ���� ���� ������ �޶�����
		if (ViewportPositionX < Target->GetPositionX())
			LookDir = Right;
		else if (ViewportPositionX > Target->GetPositionX())
			LookDir = Left;
	}
}

void Player::UpdateAnimation(float FT) {
	// �����̴� �������� ���� ����δ�
	if ((!MoveRight && !MoveLeft) || (MoveRight && MoveLeft)) {
		Rotation = std::lerp(Rotation, 0, FT * 10);
		ScaleValue = std::lerp(ScaleValue, 0, FT * 10);
	}

	else {
		if (MoveRight)
			Rotation = std::lerp(Rotation, -5, FT * 10);
		if (MoveLeft)
			Rotation = std::lerp(Rotation, 5, FT * 10);

		ScaleValue = std::lerp(ScaleValue, 0.03, FT * 10);
	}

	// ���� ������ �ٲ��� ���� �������� ȸ���Ѵ�
	if (LookDir == Right)
		H_Rotation = std::lerp(H_Rotation, 0, FT * 10);
	else if (LookDir == Left)
		H_Rotation = std::lerp(H_Rotation, 180, FT * 10);

	Size = lsAni.Update(ScaleValue, FT * 10);
}

void Player::UpdateMove(float FT) {
	// A �Ǵ� D ���� �� ����, �� Ű�� �ϳ��� ������ �ʰų� �� Ű�� ��� ���� �� ����
	if ((!MoveRight && !MoveLeft) || (MoveRight && MoveLeft))
		psUtil.LinearDcc(CurrentPlayerSpeed, 10, FT);
	else {
		if (MoveRight)
			psUtil.LinearAcc(CurrentPlayerSpeed, 1, MaxPlayerSpeed, 10, FT);
		if (MoveLeft)
			psUtil.LinearAcc(CurrentPlayerSpeed, -1, MaxPlayerSpeed, 10, FT);
	}

	// �÷��̾� �ӵ��� 0 �̸��̰ų� �ʰ��� �� �̵�
	MoveStraight(PositionX, CurrentPlayerSpeed, FT);
}

void Player::UpdateJump(float FT) {
	// ���� ���ӵ��� �߰��� �Ŀ��� �߷��� �ۿ�
	psUtil.Fall(Height, 10.0, FT);

	// �÷��̾� ���̰� -0.3�� �����ϴ��� �˻�, ���� �� ���� ���ӵ��� 0�� �Ǿ� �߷� �ۿ� �ߴ�
	// FLOOR_HEIGHT�� EngineHeader.h�� constexpr�� ���ǵǾ�����, ����.
	psUtil.LandFloor(Height, FLOOR_HEIGHT);
}

void Player::Update(float FT) {
	// �÷��̾� �̵� ������Ʈ
	UpdateMove(FT);

	// �÷��̾� ���� ������Ʈ
	UpdateJump(FT);

	// �÷��̾� ���� ������Ʈ
	UpdateLook();

	// �÷��̾� �ִϸ��̼� ������Ʈ
	UpdateAnimation(FT);
}

void Player::Render() {
	BeginProcess(ImageRenderMode::Default);
	SetPosition(PositionX, Height + Size * 0.5);
	RotateAxis(Rotation, 0.0, -0.1);
	RotateVertical(H_Rotation);
	Scale(0.3, 0.3 + Size);
	RenderImage(Image);

	SetPosition(0.0, -Size * 0.5);
	UpdateViewportPosition();
}