#include "Casing.h"

Casing::Casing(GLfloat XPos, GLfloat YPos, int DirValue) {
	CasingPositionX = XPos;
	CasingPositionY = YPos;

	MoveSpeed = randomUtil.Gen(Dist::Real, 0.6, 1.5);
	FallingAcc = randomUtil.Gen(Dist::Real, 1.2, 1.5);

	psUtil.AddGravityAcc(FallingAcc);
	Dir = DirValue;
}

void Casing::Update(float FT) {
	psUtil.Fall(CasingPositionY, 10.0, FT);

	if (psUtil.IsHitFloor(CasingPositionY, FLOOR_HEIGHT - 0.2)) {
		MoveSpeed *= 0.7;
		RotationSpeed *= 0.5;
		PlaySound(CasingHitSound, CasingChannel);
	}

	psUtil.BounceFloor(CasingPositionY, FLOOR_HEIGHT - 0.2, 0.7, 0.5);

	if (MoveActivated) {
		if (Dir == 0) {
			CasingPositionX += MoveSpeed * FT;
			Rotation -= FT * RotationSpeed;
		}

		else {
			CasingPositionX -= MoveSpeed * FT;
			Rotation += FT * RotationSpeed;
		}
	}

	if (!psUtil.GetFallingState()) {
		RotationSpeed = 0;
		MoveSpeed = 0;

		timerUtil.Update(FT);
		if(timerUtil.MiliSec() >= 0.3)
			framework.DeleteSelf(this);
	}
}

void Casing::Render() {
	BeginProcess(ImageRenderMode::Default);
	SetPosition(CasingPositionX, CasingPositionY);
	Rotate(Rotation);
	Scale(0.05, 0.05);

	// ª�� �ð��� �ſ� ���� �󵵷� �����Ǵ� ��ü�̹Ƿ� ���� ����� ���� �ϳ��� ���� �̹����� ��������
	// EngineHeade.h�� ���� ����, ResourceList.cpp�� ���� �Ǿ�����
	RenderImage(CasingImage);
}