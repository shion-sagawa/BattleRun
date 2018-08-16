#include"main.h"
#include"TEAMLOGO_Control.h"
#include"TEAMLOGO_Render.h"
#include"TITLE_Control.h"
#include"TITLE_Render.h"
#include"GAME_Control.h"
#include"GAME_Render.h"
#include"RESULT_Control.h"
#include"RESULT_Render.h"

//Directx関係----------------------------
LPDIRECT3DTEXTURE9	  g_pTexture[TEXMAX];	//	画像の情報を入れておく為のポインタ配列
IDirect3DDevice9*	  g_pD3Device;		//	Direct3Dのデバイス
D3DPRESENT_PARAMETERS g_D3dPresentParameters;		//	パラメータ
D3DDISPLAYMODE		  g_D3DdisplayMode;
IDirect3D9*			  g_pDirect3D;		//	Direct3Dのインターフェイス
LPDIRECTINPUT8 pDinput = NULL;//キー入力のための変数
LPDIRECTINPUTDEVICE8 pKeyDevice = NULL;
HRESULT InitDinput(HWND hWnd);
LPD3DXFONT g_pFont[FONTMAX];//DXフォント
//---------------------------------------

RECT debugFont;
int scene = TEAMLOGO_SCENE;
int MapData01[MAP_01_HEIGHT][MAP_01_WIDTH];
int MapData02[MAP_02_HEIGHT][MAP_02_WIDTH];
SoundLib::SoundsManager soundsManager;

//マップデータを読み込む関数
void ReadMapData(const char* FileName, int* MapData, int MapWidth)
{
	FILE *fp1;
	char data[4];
	int c, i = 0, x = 0, y = 0;

	if ((fopen_s(&fp1, FileName, "r")) != 0)
	{
		exit(1);
	}

	while ((c = getc(fp1)) != EOF)
	{
		if (isdigit(c))
		{
			data[i] = (char)c;
			i++;
		}
		else
		{
			data[i] = '\0';
			*(MapData + y * MapWidth + x) = atoi(data);
			x++;
			i = 0;
			if (x == MapWidth) {
				y++;
				x = 0;
			}
		}
	}
	fclose(fp1);
}

//テクスチャを読み込む関数
void ReadTexture(void) {
	D3DXCreateTextureFromFile(
		g_pD3Device,
		"teamlogo.png",
		&g_pTexture[TEAMLOGO_TEX]);

	D3DXCreateTextureFromFile(
		g_pD3Device,
		"title_BKG.jpg",
		&g_pTexture[TITLE_BKG_TEX]);

	D3DXCreateTextureFromFile(
		g_pD3Device,
		"title_logo.png",
		&g_pTexture[TITLE_LOGO_TEX]);

	D3DXCreateTextureFromFile(
		g_pD3Device,
		"title_botton.png",
		&g_pTexture[TITLE_BOTTON_TEX]);

	D3DXCreateTextureFromFile(
		g_pD3Device,
		"game_BKG.png",
		&g_pTexture[GAME_BKG_TEX]);

	D3DXCreateTextureFromFile(
		g_pD3Device,
		"game_player.png",
		&g_pTexture[GAME_PLAYER_TEX]);

	D3DXCreateTextureFromFile(
		g_pD3Device,
		"result_BKG.png",
		&g_pTexture[RESULT_BKG_TEX]);

	D3DXCreateTextureFromFile(
		g_pD3Device,
		"Player_Move_Right.png",
		&g_pTexture[PLAYER_RIGHT_TEX]);

	D3DXCreateTextureFromFile(
		g_pD3Device,
		"Player_Move_Left.png",
		&g_pTexture[PLAYER_LEFT_TEX]);

	D3DXCreateTextureFromFile(
		g_pD3Device,
		"Player_Move_Right.png",
		&g_pTexture[PLAYER_2P_RIGHT_TEX]);

	D3DXCreateTextureFromFile(
		g_pD3Device,
		"Player_Move_Left.png",
		&g_pTexture[PLAYER_2P_LEFT_TEX]);

	D3DXCreateTextureFromFile(
		g_pD3Device,
		"block.png",
		&g_pTexture[SCAFFOLD_TEX]);

	D3DXCreateTextureFromFile(
		g_pD3Device,
		"blackBlock.png",
		&g_pTexture[GROUND_BLOCK_TEX]);

	D3DXCreateTextureFromFile(
		g_pD3Device,
		"trampoline.png",
		&g_pTexture[TRAMPOLINE_TEX]);

	D3DXCreateTextureFromFile(
		g_pD3Device,
		"hole.png",
		&g_pTexture[MANHOLE_TEX]);

	D3DXCreateTextureFromFile(
		g_pD3Device,
		"goal.png",
		&g_pTexture[GOAL_TEX]);

	D3DXCreateTextureFromFile(
		g_pD3Device,
		"1P_win.png",
		&g_pTexture[WIN_1P_TEX]);

	D3DXCreateTextureFromFile(
		g_pD3Device,
		"2P_win.png",
		&g_pTexture[WIN_2P_TEX]);
}


//メッセージ処理
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_KEYDOWN:

		switch (wp)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, msg, wp, lp);
}

//ダイレクトインプットの初期化関数
HRESULT InitDinput(HWND hWnd)
{
	HRESULT hr;
	//「DirectInput」オブジェクトの作成
	if (FAILED(hr = DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&pDinput, NULL)))
	{
		return hr;
	}
	//「DirectInputデバイス」
	if (FAILED(hr = pDinput->CreateDevice(GUID_SysKeyboard, &pKeyDevice, NULL)))
	{
		return hr;
	}
	//デバイスをキーボードに設定
	if (FAILED(hr = pKeyDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return hr;
	}
	//協調レベルの設定
	if (FAILED(hr = pKeyDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND)))
	{
		return hr;
	}
	//デバイスを「取得」する
	pKeyDevice->Acquire();
	return S_OK;
}

//メモリ解放を行う関数
void FreeDx()
{
	for (int i = 0; i <= TEXMAX; i++)
	{
		SAFE_RELEASE(g_pTexture[i]);
	}
	SAFE_RELEASE(g_pD3Device);
	SAFE_RELEASE(g_pDirect3D);
	SAFE_RELEASE(pKeyDevice);
	SoundLibCWrapper_Free();
}

void SoundConfiguration(void) {

	// 初期化
	// SoundsManagerインスタンス生成後に1度のみ行う。
	bool isSuccess = soundsManager.Initialize();

	// 音声ファイルオープン
	// 第2引数は音声ファイルを識別するための任意の文字列をキーとして指定する。
	// この後の操作関数の呼び出し時には、ここで設定したキーを指定して音声を識別する。
	const TCHAR* filePath = _T("game_BGM.mp3");
	isSuccess = soundsManager.AddFile(filePath, _T("gameBGM"));
	const TCHAR* filePath2 = _T("titleBGM2.mp3");
	isSuccess = soundsManager.AddFile(filePath2, _T("titleBGM"));
	const TCHAR* filePath3 = _T("attack03.mp3");
	isSuccess = soundsManager.AddFile(filePath3, _T("titleBotton"));
	const TCHAR* filePath4 = _T("jump01.mp3");
	isSuccess = soundsManager.AddFile(filePath4, _T("gamePlayerJump"));
	const TCHAR* filePath5 = _T("jump02.mp3");
	isSuccess = soundsManager.AddFile(filePath5, _T("gamePlayerJump2"));
	const TCHAR* filePath6 = _T("jump03.mp3");
	isSuccess = soundsManager.AddFile(filePath6, _T("gamePlayerJump3"));
	const TCHAR* filePath7 = _T("jump04.mp3");
	isSuccess = soundsManager.AddFile(filePath7, _T("gamePlayerJump4"));
	const TCHAR* filePath8 = _T("jump09.mp3");
	isSuccess = soundsManager.AddFile(filePath8, _T("gameTrampoline"));
	const TCHAR* filePath9 = _T("jump09_2.mp3");
	isSuccess = soundsManager.AddFile(filePath9, _T("gameTrampoline2"));
	const TCHAR* filePath10 = _T("clapping1.mp3");
	isSuccess = soundsManager.AddFile(filePath10, _T("clappingSE"));
	const TCHAR* filePath11 = _T("cheers2.mp3");
	isSuccess = soundsManager.AddFile(filePath11, _T("cheersSE"));

}

//メインルーチン
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR lpCmdLine, int nCmdShow)
{
	MSG msg;
	WNDCLASS Wndclass;
	HWND hWnd;

	//Windows情報の設定
	Wndclass.style = CS_HREDRAW | CS_VREDRAW;
	Wndclass.lpfnWndProc = WndProc;
	Wndclass.cbClsExtra = 0;
	Wndclass.cbWndExtra = 0;
	Wndclass.hInstance = hInstance;
	Wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	Wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	Wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	Wndclass.lpszMenuName = NULL;
	Wndclass.lpszClassName = TITLE;	//クラス名
	RegisterClass(&Wndclass);

	//Windowの生成
	hWnd = CreateWindow(
		TITLE,								//ウィンドウのクラス名
		TITLE, 							//ウィンドウのタイトル
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,	//ウィンドウスタイル
		CW_USEDEFAULT,						// ウィンドウの横方向の位置x
		CW_USEDEFAULT,						// ウィンドウの縦方向の位置y
		DISPLAY_WIDTH,							// Width（幅）
		DISPLAY_HIGHT,							// Height（高さ）
		NULL,
		NULL,
		hInstance,							// アプリケーションインスタンスのハンドル
		NULL
	);
	if (!hWnd) return 0;

	if (FAILED(InitDinput(hWnd))) {
		return 0;
	}
	//音声の初期化
	SoundConfiguration();

	//DirectX オブジェクトの生成
	g_pDirect3D = Direct3DCreate9(
		D3D_SDK_VERSION);

	//Display Mode の設定
	g_pDirect3D->GetAdapterDisplayMode(
		D3DADAPTER_DEFAULT,
		&g_D3DdisplayMode);

	ZeroMemory(&g_D3dPresentParameters,
		sizeof(D3DPRESENT_PARAMETERS));
	g_D3dPresentParameters.BackBufferFormat = g_D3DdisplayMode.Format;
	g_D3dPresentParameters.BackBufferCount = 1;
	g_D3dPresentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
	g_D3dPresentParameters.Windowed = TRUE;

	//デバイスを作る
	g_pDirect3D->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&g_D3dPresentParameters, &g_pD3Device);

	//フォントの設定
	D3DXCreateFont(
		g_pD3Device,
		40,					//文字の高さを指定
		40,					//文字の幅を指定
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		"VENUS RISING",	//フォントの種類を指定
		&g_pFont[DEBUG_FONT]//

	);

	//描画設定
	g_pD3Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	g_pD3Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);  //SRCの設定
	g_pD3Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);


	g_pD3Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	g_pD3Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);

	g_pD3Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	//頂点に入れるデータを設定
	g_pD3Device->SetFVF(D3DFVF_CUSTOMVERTEX);

	//マップ、テクスチャの読み込みの処理群
	ReadTexture();
	ReadMapData("BigField.csv", &MapData01[0][0], MAP_01_WIDTH);
	ReadMapData("Book2.csv", &MapData02[0][0], MAP_02_WIDTH);


	DWORD SyncOld = timeGetTime();	//	システム時間を取得
	DWORD SyncNow;

	timeBeginPeriod(1);
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT)
	{
		Sleep(1);
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			SyncNow = timeGetTime();
			if (SyncNow - SyncOld >= 1000 / FRAME) //	1秒間にFRAME回この中に入るはず
			{
				switch (scene) {
				case TEAMLOGO_SCENE:
					TeamlogoControl();
					TeamlogoRender();
					break;
				case TITLE_SCENE:
					TitleControl();
					TitleRender();
					break;
					/*case TUTORIAL_SCENE:
					ControlTutorial();
					RenderTutorial();
					break;*/
				case GAME_SCENE:
					GameControl();
					GameRender();
					break;
				case RESULT_SCENE:
					ResultControl();
					ResultRender();
					break;
					/*case THANKYOU_SCENE:
					ControlThankYou();
					RenderThankYou();*/
				}
				SyncOld = SyncNow;
			}
		}
	}
	timeEndPeriod(1);

	FreeDx();
	return (int)msg.wParam;
}