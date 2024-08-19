#include "Player.h"

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
					psUtil.AddGravityAcc(4.0);
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

Player::Player() {
	SetImage(Image, "player");
}

void Player::UpdateMove(float FT) {
	// A �Ǵ� D ���� �� ����, �� Ű�� �ϳ��� ������ �ʰų� �� Ű�� ��� ���� �� ����
	if (MoveRight)
		psUtil.LinearAcc(CurrentPlayerSpeed, 1, MaxPlayerSpeed, 10, FT);

	if (MoveLeft)
		psUtil.LinearAcc(CurrentPlayerSpeed, -1, MaxPlayerSpeed, 10, FT);

	if ((!MoveRight && !MoveLeft) || (MoveRight && MoveLeft))
		psUtil.LinearDcc(CurrentPlayerSpeed, 10, FT);

	// �÷��̾� �ӵ��� 0 �̸��̰ų� �ʰ��� �� �̵�
	MoveStraight(PositionX, CurrentPlayerSpeed, FT);
}

void Player::UpdateJump(float FT) {
	// ���� ���ӵ��� �߰��� �Ŀ��� �߷��� �ۿ�
	psUtil.Fall(Height, 9.0, FT);

	// �÷��̾� ���̰� -0.3�� �����ϴ��� �˻�, ���� �� ���� ���ӵ��� 0�� �Ǿ� �߷� �ۿ� �ߴ�
	// FLOOR_HEIGHT�� EngineHeader.h�� constexpr�� ���ǵǾ�����, ����.
	psUtil.LandFloor(Height, FLOOR_HEIGHT);
}

void Player::Update(float FT) {
	// �÷��̾� �̵� ������Ʈ
	UpdateMove(FT);

	// �÷��̾� ���� ������Ʈ
	UpdateJump(FT);
}

void Player::Render() {
	BeginProcess(ImageRenderMode::Default);
	SetPosition(PositionX, Height);
	Scale(0.3, 0.3);
	RenderImage(Image);
}