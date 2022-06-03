#include "DxLib.h"
#include <time.h>

#define INTERVAL 30

void preInit() {
	// ウインドウモードで起動
	ChangeWindowMode(TRUE);

	// 透過ウインドウ設定
	SetUseBackBufferTransColorFlag(TRUE);

	// 動き続けさせる(バックグラウンド処理)
	SetAlwaysRunFlag(TRUE);
}

void afterInit(int* handlep) {
	// 画像セット
	char basepng[] = "おどるあかりBB_res ";
	char pngname[128];
	for (int i = 0; i < 59; i++) {
		sprintf_s(pngname, "%s(%d).png", basepng, i + 1);
		*(handlep + i) = LoadGraph(pngname);
	}

	//TOPにおく
	HWND hwnd = GetMainWindowHandle();
	SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, (SWP_NOMOVE | SWP_NOSIZE));
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	int BBHandle[59];
	int Screen;
	int SoftImage;

	preInit();

	// ＤＸライブラリの初期化
	if (DxLib_Init() < 0) return -1;

	// 描画対象にできるアルファチャンネル付き画面を作成
	Screen = MakeScreen(640, 480, TRUE);

	// 画面取り込み用のソフトウエア用画像を作成
	SoftImage = MakeARGB8ColorSoftImage(640, 480);

	// 画像を読み込む際にアルファ値をRGB値に乗算するように設定する
	SetUsePremulAlphaConvertLoad(TRUE);

	// dxInit後処理
	afterInit(BBHandle);

	// 描画先を描画対象にできるアルファチャンネル付き画面にする
	SetDrawScreen(Screen);

	// メインループ
	int i = 0;

	clock_t oTime, nTime;
	oTime = clock();

	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		// 画面をクリア
		ClearDrawScreen();


		// 画像を描画
		DrawGraph(0, 0, BBHandle[i], TRUE);

		// 描画先の画像をソフトイメージに取得する
		GetDrawScreenSoftImage(0, 0, 640, 480, SoftImage);

		// 取り込んだソフトイメージを使用して透過ウインドウの状態を更新する
		UpdateLayerdWindowForPremultipliedAlphaSoftImage(SoftImage);

		// 少し待つ
		nTime = clock();
		if ((double)oTime + INTERVAL < (double)nTime ) {
			oTime = nTime;
			i++;
			i %= 59;
		}
	}

	// ＤＸライブラリの後始末
	DxLib_End();

	// ソフトの終了
	return 0;
}