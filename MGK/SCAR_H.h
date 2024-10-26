#pragma once
#include "GameObject.h"
#include "Flame.h"

class SCAR_H : public GameObject {
private:
	// ��ġ
	glm::vec2 Position{};

	// �ٶ󺸴� ����
	int LookDir{};

	// ȸ�� ��
	GLfloat Rotation{};

	// �ݵ�
	GLfloat Recoil{};

	// �ݹ� Ÿ�̹� Ÿ�̸�
	TimerUtil ShootingTimer;

	// �ݹ� Ÿ�̹�
	GLfloat ShootTime{};

	// ��Ƽ� ��� ����
	bool TriggerState{};

	// ���� ����� ���� ä��
	SoundChannel ch{};

	GameObject* CrosshairPtr{};

public:
	// �� �̸� �Է�, �̸��� ���� �߻� �ӵ��� ��������
	SCAR_H(GLfloat Time) {
		ShootTime = Time;
		if (auto crosshair = scene.Find("crosshair"); crosshair)
			CrosshairPtr = crosshair;
	}

	// ��ġ �Է�, �÷��̾��� ��ġ�� �Է¹޴´�
	void InputPosition(glm::vec2 PositionValue) {
		Position = PositionValue;
	}

	// ���� ������ �Է¹޴´�
	void InputLookDir(int Dir) {
		LookDir = Dir;
	}

	// ȸ�� ������ �Է¹޴´�
	void InputRotation(GLfloat Value) {
		Rotation = Value;
	}

	void InputTriggerState(bool State) {
		TriggerState = State;
	}

	void UpdateFunc(float FT) {
		if (ShootingTimer.MiliSec(2) < ShootTime)
			ShootingTimer.Update(FT);

		// ��Ƽ踦 ��� ���¿����� ���� �������� �ݹߵȴ�.
		// �ݹ� �� ���� ��°� �Բ� �Ҳ� ������Ʈ�� �߰��Ѵ�
		// ũ�ν����� �ݵ��� �ش�.
		if (TriggerState) {
			if (ShootingTimer.MiliSec(2) >= ShootTime) {
				ShootingTimer.Reset();
				PlaySound(SCAR_H_Shoot, ch, 0);
				scene.AddObject(new Flame(
					Position.x + cos(glm::radians(Rotation)) * 0.35, 
					Position.y + sin(glm::radians(Rotation)) * 0.35,
					Rotation
				), "flame", LAYER_3);

				Recoil = 0.1;

				CrosshairPtr->GiveRecoil(0.08);
			}
		}

		Recoil = Math::Lerp(Recoil, 0.0, 15, FT);
	}

	void RenderFunc() {
		InitMatrix();
		Transform::Move(TranslateMatrix, Position.x, Position.y);

		// ���� ����
		if (LookDir == 0) {
			Transform::Rotate(TranslateMatrix, Rotation + 180.0);
			Transform::Move(TranslateMatrix, -0.15 + Recoil, 0.0);
			Flip(FLIP_H);
		}
		// ������ ����
		else if (LookDir == 1) {
			Transform::Rotate(TranslateMatrix, Rotation);
			Transform::Move(TranslateMatrix, 0.15 - Recoil, 0.0);
		}

		Transform::Scale(ScaleMatrix, 0.18, 0.18);
		Render(SCAR_H_Image);
	}
};