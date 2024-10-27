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


// ��ź�� �ε�������
class IndAmmo : public GameObject {
private:
	// �÷��̾ ������ ���� �ִ� ��ź��
	int AmmoLimit{};

	// �÷��̾ ������ ���� ���� ��ź��
	int CurrentAmmo{};

	// �ε������Ͱ� ����ϴ� ���� ��ź��
	int PrevAmmo{};
	
	// �÷��̾� �� ������ ����
	bool ReloadState{};

	// ������ ���൵ ��ġ
	GLfloat ReloadProgress{}, EntireReloadProgress{};

	// ��ź�� �ؽ�Ʈ ����
	GLfloat Height{};

	// ��ź���� ǥ���ϱ� ���� �ؽ�Ʈ
	TextUtil text;

	// �������� ǥ���ϱ� ���� ��
	LineBrush line;

public:
	IndAmmo() {
		text.Init(L"���� ���", FW_BOLD);
		line.SetRenderType(RENDER_TYPE_STATIC);
		line.SetColor(1.0, 1.0, 1.0);
	}

	// ���� ��ź���� �Է��Ѵ�
	void InputCurrentAmmo(int Value) {
		CurrentAmmo = Value;
	}

	// ������ ���¸� �Է��Ѵ�
	void InputReloadState(bool State) {
		ReloadState = State;
	}

	// ������ ��ô���� �Է��Ѵ�
	void InputReloadProgress(GLfloat CurrentProgress, GLfloat EntireProgress) {
		EntireReloadProgress = EntireProgress;
		ReloadProgress = CurrentProgress;
	}

	void UpdateFunc(float FT) {
		// ��ź�� ������Ʈ�� �����Ǹ� �ؽ�Ʈ �ִϸ��̼��� ����Ѵ�
		if (CurrentAmmo != PrevAmmo) {
			Height = 0.1;
			PrevAmmo = CurrentAmmo;
		}

		Height = Math::Lerp(Height, 0.0, 15, FT);
	}

	void RenderFunc() {
		// ��ź���� 1�� �̻� �ִٸ� ��ź���� ǥ���ϰ�, ��ź���� 0�̶�� R�� ǥ���Ѵ�.
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

		// ������ ���൵ ���� ǥ��
		if (ReloadState) {
			GLfloat PositionX = ASP(1.0) - 0.8;
			line.Draw(PositionX, -1.0 + 0.22, PositionX + 0.7 * (ReloadProgress / EntireReloadProgress), -1.0 + 0.22, 0.02);
		}
	}
};

// ����ź �ε�������
class IndGrenade : public GameObject {
private:
	bool UseEnable{};
	int RemainedTime{};

public:
	void UpdateFunc(float FT) {

	}

	void RenderFunc() {

	}
};

// ü�� �ε�������
class IndHP : public GameObject {
private:
	// �÷��̾� ���� ü��
	int CurrentHP{};

	// �ε������Ͱ� ����ϴ� ���� ü��
	int PrevHP{};

	// ü�� �� ǥ�ÿ� ��
	LineBrush line;

	// ü�� ǥ�ÿ� �ؽ�Ʈ
	TextUtil text;

public:
	IndHP() {
		text.Init(L"���� ���", FW_BOLD);
		text.SetColor(1.0, 1.0, 1.0);
		line.SetRenderType(RENDER_TYPE_STATIC);
	}

	// �÷��̾��� ���� HP�� �Է��Ѵ�.
	void InputCurrentHP(int Value) {
		CurrentHP = Value;
	}

	void UpdateFunc(float FT) {
		if (CurrentHP != PrevHP) {
			
		}
	}

	void RenderFunc() {
		GLfloat PositionX = ASP(-1.0) + 0.1;

		line.SetColor(0.0, 0.0, 0.0);
		line.Draw(PositionX, -1.0 + 0.1, PositionX + 0.7, -1.0 + 0.1, 0.1);

		line.SetColor(1.0, 0.0, 0.0);
		line.Draw(PositionX, -1.0 + 0.1, PositionX + 0.7 * ( (float)CurrentHP / 100.0 ), -1.0 + 0.1, 0.08);

		text.Render(ASP(-1.0) + 0.07, -1.0 + 0.075, 0.1, 1.0, L"%d", CurrentHP);
	}
};