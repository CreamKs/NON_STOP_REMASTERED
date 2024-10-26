#include "Flame.h"

Flame::Flame(GLfloat X, GLfloat Y, GLfloat RotationValue) {
	Position.x = X;
	Position.y = Y;
	Rotation = RotationValue;
}

// ������ ���������� ������ �����Ѵ�
void Flame::UpdateFunc(float FT) {
	timer.Update(FT);
	if(timer.MiliSec(2) >= 0.03)
		scene.DeleteObject(this);
}

void Flame::RenderFunc() {
	InitMatrix();
	Transform::Move(TranslateMatrix, Position);
	Transform::Rotate(RotateMatrix, Rotation);
	Transform::Scale(ScaleMatrix, 0.2, 0.2);
	Render(FlameImage, FlameTransparent);
}