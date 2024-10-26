#include "PlayMode.h"
#include "Scene.h"
#include "MouseUtil.h"

// 추가가 필요한 오브젝트의 헤더파일을 포함한다.
#include "Player.h"


void PlayMode::Start() {
	// 배경색을 밝은 색으로 바꾼다.
	Framework::SetBackColor(0.5, 0.5, 0.5);

	// playmode에 필요한 리소스들을 로드한다.
	imageUtil.Import(PlayerImage, "res//commando_right.png", IMAGE_TYPE_NEAREST);

	// playmode에 필요한 오브젝트들을 scene에 추가한다.
	// player의 경우 2번 레이어에 추가헸다.
	scene.AddObject(new Player, "player", LAYER_2);

	scene.RegisterController(Controller, MODE_TYPE_DEFAULT);
	scene.RegisterDestructor(Destructor);
	scene.RegisterModeName("PlayMode");
}

void PlayMode::Destructor() {
	// 모드가 끝나면 로드했던 리소스를 메모리에서 해제한다.
	imageUtil.Release(PlayerImage);
}

void PlayMode::ProcessKeyboard(int State, unsigned char NormalKey, int SpecialKey) {
	// 개발 시에는 활성화 해둠. 즉시 종료 역할
	if (State == NORMAL_KEY_DOWN) {
		switch (NormalKey) {
		case NK_ESCAPE:
			scene.Exit();
			break;
		}
	}

	// 컨트롤러 입력이 필요한 객체에 컨트롤러를 연결한다.
	// Start()에서 추가할 때 입력했던 오브젝트 태그로 오브젝트의 포인터를 찾는다.
	if (auto player = scene.Find("player"); player)
		player->InputKey(State, NormalKey, SpecialKey);
}

void PlayMode::ProcessMouseButton(int State) {
}

void PlayMode::ProcessMouseWheel(int State) {
}



void PlayMode::KeyDown(unsigned char KEY, int X, int Y) {
	ProcessKeyboard(NORMAL_KEY_DOWN, KEY, NULL);
}

void PlayMode::KeyUp(unsigned char KEY, int X, int Y) {
	ProcessKeyboard(NORMAL_KEY_UP, KEY, NULL);
}

void PlayMode::SpecialKeyDown(int KEY, int X, int Y) {
	ProcessKeyboard(SPECIAL_KEY_DOWN, NULL, KEY);
}

void PlayMode::SpecialKeyUp(int KEY, int X, int Y) {
	ProcessKeyboard(SPECIAL_KEY_UP, NULL, KEY);
}

void PlayMode::MouseMotion(int X, int Y) {
	mouse.ConvertPosition(X, Y);
}

void PlayMode::MousePassiveMotion(int X, int Y) {
	mouse.ConvertPosition(X, Y);
}

void PlayMode::MouseWheel(int Button, int Wheel, int X, int Y) {
	if (Wheel > 0)
		ProcessMouseWheel(WHEEL_UP);
	else if (Wheel < 0)
		ProcessMouseWheel(WHEEL_DOWN);
}

void PlayMode::MouseButton(int Button, int State, int X, int Y) {
	if (State == GLUT_DOWN) {
		if (Button == GLUT_LEFT_BUTTON)
			ProcessMouseButton(LEFT_BUTTON_DOWN);
		else if (Button == GLUT_RIGHT_BUTTON)
			ProcessMouseButton(RIGHT_BUTTON_DOWN);
		else if (Button == GLUT_MIDDLE_BUTTON)
			ProcessMouseButton(MIDDLE_BUTTON_DOWN);
	}
	else if (State == GLUT_UP) {
		if (Button == GLUT_LEFT_BUTTON)
			ProcessMouseButton(LEFT_BUTTON_UP);
		else if (Button == GLUT_RIGHT_BUTTON)
			ProcessMouseButton(RIGHT_BUTTON_UP);
		else if (Button == GLUT_MIDDLE_BUTTON)
			ProcessMouseButton(MIDDLE_BUTTON_UP);
	}
}

void PlayMode::Controller() {
	glutMotionFunc(MouseMotion);
	glutPassiveMotionFunc(MousePassiveMotion);
	glutKeyboardFunc(KeyDown);
	glutKeyboardUpFunc(KeyUp);
	glutMouseWheelFunc(MouseWheel);
	glutMouseFunc(MouseButton);
	glutSpecialFunc(SpecialKeyDown);
	glutSpecialUpFunc(SpecialKeyUp);
}