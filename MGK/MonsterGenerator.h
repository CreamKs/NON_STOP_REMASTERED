#pragma once
#include "GameObject.h"
#include "Regular.h"

class MonsterGenerator : public GameObject {
private:
	// ���� ���� Ÿ�̸�
	TimerUtil Timer;

	// ���� ���� Ÿ�̹�
	// �ʱⰪ 1.0
	GLfloat GenTime{1.0};

public:
	void UpdateFunc(float FT) {
		Timer.Update(FT);
		if (Timer.MiliSec(2) >= GenTime) {
			int RandDir = Random::Gen(DIST_INT, 0, 1);
			scene.AddObject(new RegularMonster(RandDir), "regular_monster", LAYER_2);
			Timer.Interpolate(GenTime);
		}
	}
};
