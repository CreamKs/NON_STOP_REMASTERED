#include "Target.h"
#include "MouseUtil.h"
#include "Framework.h"
#include <cmath>

GLfloat Target::GetPositionX() {
	return PositionX;
}

GLfloat Target::GetHeight() {
	return PositionY;
}

Target::Target() {
	SetImage(TargetImage, "target");
}

void Target::GiveRecoil(GLfloat Value) {
	PositionY += Value;
	PositionX += randomUtil.Gen(Dist::Real, -Value, Value);
}

void Target::Update(float FT) {
	// ���콺�� ������ ��ŭ �������� �����δ�
	PositionX += mouse.x;
	PositionY += mouse.y;

	// �������� ȭ�� ������ ����� �ʵ��� �Ѵ�
	if (PositionX > ASP(1.0))
		PositionX = ASP(1.0);

	else if (PositionX < ASP(-1.0))
		PositionX = ASP(-1.0);

	if (PositionY > 1.0)
		PositionY = 1.0;

	else if (PositionY < -1.0)
		PositionY = -1.0;
}

void Target::Render() {
	// ������
	BeginProcess(ImageRenderMode::Static);
	Scale(0.3, 0.3);
	SetPosition(PositionX, PositionY);
	RenderImage(TargetImage);
;}