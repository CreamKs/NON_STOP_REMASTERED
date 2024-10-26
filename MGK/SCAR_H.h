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
	GLfloat RecoilPosition{};

	// �ݹ� Ÿ�̹� Ÿ�̸�
	TimerUtil ShootingTimer;

	// �ݹ� Ÿ�̹�
	GLfloat ShootTime{};

	// ��Ƽ� ��� ����
	bool TriggerState{};

	// ���� ����� ���� ä��
	SoundChannel ch{};

	// ũ�ν���� ������Ʈ ������
	GameObject* CrosshairPtr{};

	// �ε������� ������Ʈ ������
	GameObject* IndPtr{};

	// ���� ��ź��
	int CurrentAmmo{25};

	// ������ ����
	bool ReloadState{};

	// ������ Ÿ�̾�
	TimerUtil reloadTimer;

public:
	// �� �̸� �Է�, �̸��� ���� �߻� �ӵ��� ��������
	SCAR_H(GLfloat Time) {
		ShootTime = Time;

		// ũ�ν� ��� ������ ���
		CrosshairPtr = scene.Find("crosshair");

		// �ε������� �����͸� ��� ���� ��ź���� �����ϱ�
		IndPtr = scene.Find("indammo");
		IndPtr->InputCurrentAmmo(CurrentAmmo);
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

	// ��Ƽ� ��� ���¸� �ο��Ѵ�.
	void InputTriggerState(bool State) {
		TriggerState = State;
	}

	// ���� ������ �Ѵ�.
	void Reload() {
		if (CurrentAmmo < 25 && !ReloadState) {
			PlaySound(AR_Reload, ch, 0);

			// ������ ���� Ȱ��ȭ�� �ε������ͷ� ����
			IndPtr->InputReloadState(true);
			ReloadState = true;
		}
	}

	// �������� ������Ʈ �Ѵ�.
	void UpdateReload(float FT) {
		reloadTimer.Update(FT);

		// ������ ��ô���� �ε������ͷ� �����Ѵ�.
		IndPtr->InputReloadProgress(reloadTimer.MiliSec(2), 0.7);

		if (reloadTimer.MiliSec(1) >= 0.7) {
			CurrentAmmo = 25;
			IndPtr->InputCurrentAmmo(CurrentAmmo);
			reloadTimer.Reset();

			// ������ ���� ��Ȱ��ȭ�� �ε������ͷ� ����
			IndPtr->InputReloadState(false);
			ReloadState = false;
		}
	}

	void UpdateFunc(float FT) {
		if (ShootingTimer.MiliSec(2) < ShootTime)
			ShootingTimer.Update(FT);

		// ��Ƽ踦 ��� ���¿����� ���� �������� �ݹߵȴ�.
		// �ݹ� �� ���� ��°� �Բ� �Ҳ� ������Ʈ�� �߰��Ѵ�
		// ũ�ν����� �ݵ��� �ش�.
		// ī�޶󿡴� ��鸲 ��ġ�� �߰��Ѵ�.
		if (CurrentAmmo != 0 && !ReloadState && TriggerState) {
			if (ShootingTimer.MiliSec(2) >= ShootTime) {
				PlaySound(SCAR_H_Shoot, ch, 0);

				scene.AddObject(new Flame(
					Position.x + cos(glm::radians(Rotation)) * 0.35, 
					Position.y + sin(glm::radians(Rotation)) * 0.35,
					Rotation
				), "flame", LAYER_3);

				RecoilPosition = 0.1;
				CrosshairPtr->GiveRecoil(0.07);
				cameraCon.AddShakeValue(1.0);

				// ũ�ν��� �߻� ���¸� �ο��Ѵ�.
				CrosshairPtr->ShootGun();

				// ź���� �Һ��Ѵ�.
				--CurrentAmmo;

				// �ε������ͷ� ���� ��ź�� ����
				IndPtr->InputCurrentAmmo(CurrentAmmo);

				ShootingTimer.Reset();
			}
		}

		RecoilPosition = Math::Lerp(RecoilPosition, 0.0, 15, FT);

		// ������ ������ ��� ������ ������Ʈ
		if (ReloadState)
			UpdateReload(FT);
	}

	void RenderFunc() {
		InitMatrix();
		Transform::Move(TranslateMatrix, Position.x, Position.y);

		// ���� ����
		if (LookDir == 0) {
			Transform::Rotate(TranslateMatrix, Rotation + 180.0);
			Transform::Move(TranslateMatrix, -0.15 + RecoilPosition, 0.0);
			Flip(FLIP_H);
		}
		// ������ ����
		else if (LookDir == 1) {
			Transform::Rotate(TranslateMatrix, Rotation);
			Transform::Move(TranslateMatrix, 0.15 - RecoilPosition, 0.0);
		}

		Transform::Scale(ScaleMatrix, 0.36, 0.36);
		Render(SCAR_H_Image);
	}
};