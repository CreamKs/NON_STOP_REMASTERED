#include "Player.h"
#include "MouseUtil.h"
#include "CameraCOntroller.h"

Player::Player(){
	// ���������� �߷°� �ٴ� ��ġ ����
	pUtil.SetGravity(15);
	pUtil.SetFloorHeight(-0.5);
}

void Player::InputKey(int State, unsigned char NormalKey, int SpecialKey) {
	switch (State) {
	case NORMAL_KEY_DOWN:
		switch (NormalKey) {
		case 'a': MoveLeft = true; break;
		case 'd': MoveRight = true; break;
		case NK_SPACE: 
			if (!JumpState) {
				pUtil.SetGravityAcc(6.0);
				JumpState = true;
			}
			break;
		}
		break;

	case NORMAL_KEY_UP:
		switch (NormalKey) {
		case 'a': MoveLeft = false; break;
		case 'd': MoveRight = false; break;
		}
		break;
	}
}

void Player::InputMouse(int State) {

}

void Player::UpdateFunc(float FT) {
	// ������ ���ο� ���� �̵� ������ �޶�����.
	// ���� ���� ������ �ʵ��� �Ѵ�.
	if (MoveRight) {
		Position.x += Speed * FT;
		if (Position.x > 7.5)
			Position.x = 7.5;
	}

	if (MoveLeft) {
		Position.x -= Speed * FT;
		if (Position.x < -7.5)
			Position.x = -7.5;
	}

	// ���콺 ��ġ�� ���� �ٶ󺸴� ������ �޶�����.
	// ���콺 ��ġ�� ī�޶� ��ġ�� ���Խ��Ѿ� �Ѵ�.
	if      (mouse.x - cameraCon.Position.x > Position.x)  LookDir = LOOK_RIGHT;
	else if (mouse.x - cameraCon.Position.x < Position.x)  LookDir = LOOK_LEFT;

	// �÷��̾� ���� ������Ʈ
	pUtil.UpdateFalling(Position.y, FT);
	if (!pUtil.GetFallingState())
		JumpState = false;
}

void Player::RenderFunc() {
	// ��� �ʱ�ȭ, ������ �� �ݵ�� �����ؾ��Ѵ�.
	InitMatrix();

	// �÷��̾� ��ġ ����ŭ �̵�
	Transform::Move(TranslateMatrix, Position);

	// �÷��̾� ��� ũ�� ����
	Transform::Scale(ScaleMatrix, 0.15, 0.15);

	// ������ �ٶ󺸴� ��� ����������� �̹��� ����
	if (LookDir == LOOK_LEFT)
		Flip(FLIP_H);

	// �������� �ʿ��� �����͵��� ���̴��� ���� �� ���� ������
	Render(PlayerImage);

	// �÷��̾��� ����Ʈ ���� ��ġ ���
	UpdateViewportPosition(ViewportPosition);
}

glm::vec2 Player::GetPosition() {
	return Position;
}
