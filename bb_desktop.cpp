#include "DxLib.h"
#include <time.h>

#define INTERVAL 30

void preInit() {
	// �E�C���h�E���[�h�ŋN��
	ChangeWindowMode(TRUE);

	// ���߃E�C���h�E�ݒ�
	SetUseBackBufferTransColorFlag(TRUE);

	// ��������������(�o�b�N�O���E���h����)
	SetAlwaysRunFlag(TRUE);
}

void afterInit(int* handlep) {
	// �摜�Z�b�g
	char basepng[] = "���ǂ邠����BB_res ";
	char pngname[128];
	for (int i = 0; i < 59; i++) {
		sprintf_s(pngname, "%s(%d).png", basepng, i + 1);
		*(handlep + i) = LoadGraph(pngname);
	}

	//TOP�ɂ���
	HWND hwnd = GetMainWindowHandle();
	SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, (SWP_NOMOVE | SWP_NOSIZE));
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	int BBHandle[59];
	int Screen;
	int SoftImage;

	preInit();

	// �c�w���C�u�����̏�����
	if (DxLib_Init() < 0) return -1;

	// �`��Ώۂɂł���A���t�@�`�����l���t����ʂ��쐬
	Screen = MakeScreen(640, 480, TRUE);

	// ��ʎ�荞�ݗp�̃\�t�g�E�G�A�p�摜���쐬
	SoftImage = MakeARGB8ColorSoftImage(640, 480);

	// �摜��ǂݍ��ލۂɃA���t�@�l��RGB�l�ɏ�Z����悤�ɐݒ肷��
	SetUsePremulAlphaConvertLoad(TRUE);

	// dxInit�㏈��
	afterInit(BBHandle);

	// �`����`��Ώۂɂł���A���t�@�`�����l���t����ʂɂ���
	SetDrawScreen(Screen);

	// ���C�����[�v
	int i = 0;

	clock_t oTime, nTime;
	oTime = clock();

	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		// ��ʂ��N���A
		ClearDrawScreen();


		// �摜��`��
		DrawGraph(0, 0, BBHandle[i], TRUE);

		// �`���̉摜���\�t�g�C���[�W�Ɏ擾����
		GetDrawScreenSoftImage(0, 0, 640, 480, SoftImage);

		// ��荞�񂾃\�t�g�C���[�W���g�p���ē��߃E�C���h�E�̏�Ԃ��X�V����
		UpdateLayerdWindowForPremultipliedAlphaSoftImage(SoftImage);

		// �����҂�
		nTime = clock();
		if ((double)oTime + INTERVAL < (double)nTime ) {
			oTime = nTime;
			i++;
			i %= 59;
		}
	}

	// �c�w���C�u�����̌�n��
	DxLib_End();

	// �\�t�g�̏I��
	return 0;
}