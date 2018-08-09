#include"main.h"
#include"RESULT_Control.h"
#include"RESULT_Render.h"

//描画処理
void ResultRender(void)
{
	CUSTOMVERTEX vertexResultBKG[4]
	{
		{ 0.f,       0.f, 1.f,1.f, 0xFFFFFFFF, 0.f, 0.f },
		{ 1400.f,    0.f, 1.f,1.f, 0xFFFFFFFF, 1.f, 0.f },
		{ 1400.f,  1000.f, 1.f,1.f, 0xFFFFFFFF, 1.f, 1.f },
		{ 0.f,     1000.f, 1.f,1.f, 0xFFFFFFFF, 0.f, 1.f }
	};

	/*CUSTOMVERTEX vertexResult[4]
	{
		{ g_Player.x - g_Player.scale, g_Player.y - g_Player.scale, 1.f, 1.f, 0xFFFFFFFF, 0.f, 0.f },
	{ g_Player.x + g_Player.scale, g_Player.y - g_Player.scale, 1.f, 1.f, 0xFFFFFFFF, 0.25f, 0.f },
	{ g_Player.x + g_Player.scale, g_Player.y + g_Player.scale, 1.f, 1.f, 0xFFFFFFFF, 0.25f, 0.25f },
	{ g_Player.x - g_Player.scale, g_Player.y + g_Player.scale, 1.f, 1.f, 0xFFFFFFFF, 0.f, 0.25f }
	};*/

	//画面の消去
	g_pD3Device->Clear(0, NULL,
		D3DCLEAR_TARGET,
		D3DCOLOR_XRGB(0x00, 0x00, 0x00),
		1.0, 0);

	//描画の開始
	g_pD3Device->BeginScene();

	g_pD3Device->SetTexture(0, g_pTexture[RESULT_BKG_TEX]);
	g_pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, vertexResultBKG, sizeof(CUSTOMVERTEX));

	/*g_pD3Device->SetTexture(0, g_pTexture[GAME_PLAYER_TEX]);
	g_pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, vertexResult, sizeof(CUSTOMVERTEX));*/

	//描画の終了
	g_pD3Device->EndScene();
	//表示
	g_pD3Device->Present(NULL, NULL, NULL, NULL);
}