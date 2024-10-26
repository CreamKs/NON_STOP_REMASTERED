#pragma once
#include "GameObject.h"
#include "MouseUtil.h"
#include "CameraController.h"

class Crosshair : public GameObject {
private:
	// ũ�ν� �� ǥ�õǴ� ��ġ
	glm::vec2 Position{};

	// ���� ���� ũ�ν���� ��ġ
	glm::vec2 RealPosition{};

	// ���� ���� Ÿ�� ����
	glm::vec2 RandomTarget{};

	// �ݵ� ��ġ
	GLfloat Recoil{};

	// �� �׸��� Ŭ����
	LineBrush line;

public:
	Crosshair() {
		line.SetColor(1.0, 0.0, 0.0);

		// ī�޶��� ���¿� ������ ���� �ʴ´�.
		line.SetRenderType(RENDER_TYPE_STATIC);
	}

	// ũ�ν��� �ݵ��� �ش�
	void GiveRecoil(GLfloat Value) {
		Recoil += Value;
	}

	void UpdateFunc(float FT) {
		// ũ�ν� �� ǥ�õǴ� ��ġ ����
		Position.x = mouse.x;
		Position.y = mouse.y;

		// ���� ���� ũ�ν���� ��ġ ����
		RealPosition.x = mouse.x + cameraCon.Position.x;
		RealPosition.y = mouse.y;

		// �ݵ� ��ġ�� ���� ���δ�
		Recoil -= FT * 0.5;

		// Recoil�� 0.0 �̸����� �������� �ʵ��� �Ѵ�.
		EX::ClampValue(Recoil, 0.0, CLAMP_LESS);
	}

	void RenderFunc() {
		// left
		line.Draw(Position.x - 0.2 - Recoil, Position.y, Position.x - 0.05 - Recoil, Position.y, 0.01);

		// right
		line.Draw(Position.x + 0.05 + Recoil, Position.y, Position.x + 0.2 + Recoil, Position.y, 0.01);

		// upper
		line.Draw(Position.x, Position.y + 0.05 + Recoil, Position.x, Position.y + 0.2 + Recoil, 0.01);

		// under
		line.Draw(Position.x, Position.y - 0.2 - Recoil, Position.x, Position.y - 0.05 - Recoil, 0.01);
	}
};