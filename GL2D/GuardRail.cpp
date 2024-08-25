#include "GuardRail.h"

GuardRail::GuardRail() {
	SetImage(Image, "guardrail");
}

void GuardRail::Update(float FT) {
	// ī�޶��� X��ǥ ��ġ�� ���̸� ������ ���巹���� ī�޶� ������ ��ġ�� ��ó�� �����Ѵ�
	if (auto CamController = framework.Find("camera_controller"); CamController) {
		PositionX = CamController->GetPositionX() * 0.5;
		Height = CamController->GetHeight() * 0.2;
	}
}

void GuardRail::Render() {
	BeginProcess(ImageRenderMode::Default);
	SetPosition(PositionX, Height - 0.9);
	Scale(6.0, 3.0);
	RenderImage(Image, 1.0, 4500, 800);
}