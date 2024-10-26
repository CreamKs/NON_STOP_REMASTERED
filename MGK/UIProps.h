#pragma once
#include "GameObject.h"

// ��ź�� �ε������� �� ���
class IndBack : public GameObject {
public:
	void RenderFunc() {
		InitMatrix(RENDER_TYPE_STATIC);
		Transform::Move(TranslateMatrix, ASP(1.0) - 0.5, -1.5 + 0.5);
		Render(IndicatorBackImage);
	}
};

class IndAmmo : public GameObject {
private:
	// �÷��̾ ������ ���� �ִ� ��ź��
	int AmmoLimit{};

	// �÷��̾ ������ ���� ���� ��ź��
	int CurrentAmmo{};
	int PrevAmmo{};

	// ��ź�� �ؽ�Ʈ ����
	GLfloat Height{};

	// ��ź���� ǥ���ϱ� ���� �ؽ�Ʈ
	TextUtil text;

public:
	IndAmmo() {
		text.Init(L"���� ���", FW_BOLD);
		PrevAmmo = CurrentAmmo;
	}

	// ���� ��ź���� �Է��Ѵ�
	void InputCurrentAmmo(int Value) {
		CurrentAmmo = Value;
	}

	void UpdateFunc(float FT) {
		// �Ѿ��� �߻��� ���� Ȯ�εǸ� �ؽ�Ʈ �ִϸ��̼��� ����Ѵ�
		if (CurrentAmmo != PrevAmmo) {
			Height = 0.1;
			PrevAmmo = CurrentAmmo;
		}

		Height = Math::Lerp(Height, 0.0, 15, FT);
	}

	void RenderFunc() {
		if (CurrentAmmo > 0) {
			text.SetColorRGB(255, 204, 0);
			text.Render(ASP(1.0) - 0.4, -1.0 + 0.05 + Height, 0.25, 1.0, L"%d", CurrentAmmo);
		}

		else {
			text.SetColor(1.0, 0.0, 0.0);
			text.Render(ASP(1.0) - 0.4, -1.0 + 0.05 + Height, 0.25, 1.0, L"R");
		}

		// ���� ������� �� ǥ��
		InitMatrix(RENDER_TYPE_STATIC);
		SetColor(1.0, 1.0, 1.0);
		Transform::Move(TranslateMatrix, ASP(1.0) - 0.6, -1.0 + 0.13);
		Transform::Scale(ScaleMatrix, 0.3, 0.3);

		if(PlayerGunName == "SCAR_H")
			Render(SCAR_H_Image);
	}
};