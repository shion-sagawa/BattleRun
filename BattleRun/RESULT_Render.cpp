#include"main.h"
#include"RESULT_Control.h"
#include"RESULT_Render.h"

int ResultWinner;//Result1PWINΕResult2PWINsπ»f·ιΟ

//`ζ
void ResultRender(void)
{
	CUSTOMVERTEX vertexResultBKG[4]
	{
		{ 0.f,                      0.f, 1.f,1.f, 0xFFFFFFFF, 0.f, 0.f },
		{ DISPLAY_WIDTH,            0.f, 1.f,1.f, 0xFFFFFFFF, 1.f, 0.f },
		{ DISPLAY_WIDTH,  DISPLAY_HIGHT, 1.f,1.f, 0xFFFFFFFF, 1.f, 1.f },
		{ 0.f,            DISPLAY_HIGHT, 1.f,1.f, 0xFFFFFFFF, 0.f, 1.f }
	};

	/*CUSTOMVERTEX vertexResult[4]
	{
	{ g_Player.x - g_Player.scale, g_Player.y - g_Player.scale, 1.f, 1.f, 0xFFFFFFFF, 0.f, 0.f },
	{ g_Player.x + g_Player.scale, g_Player.y - g_Player.scale, 1.f, 1.f, 0xFFFFFFFF, 0.25f, 0.f },
	{ g_Player.x + g_Player.scale, g_Player.y + g_Player.scale, 1.f, 1.f, 0xFFFFFFFF, 0.25f, 0.25f },
	{ g_Player.x - g_Player.scale, g_Player.y + g_Player.scale, 1.f, 1.f, 0xFFFFFFFF, 0.f, 0.25f }
	};*/

	//ζΚΜΑ
	g_pD3Device->Clear(0, NULL,
		D3DCLEAR_TARGET,
		D3DCOLOR_XRGB(0x00, 0x00, 0x00),
		1.0, 0);

	//`ζΜJn
	g_pD3Device->BeginScene();

	if (ResultWinner == Result1PWIN)//PPΜΏ
	{
		g_pD3Device->SetTexture(0, g_pTexture[RESULT_1P_BKG_TEX]);
		g_pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, vertexResultBKG, sizeof(CUSTOMVERTEX));
	}
	if (ResultWinner == Result2PWIN)//QPΜΏ
	{
		g_pD3Device->SetTexture(0, g_pTexture[RESULT_2P_BKG_TEX]);
		g_pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, vertexResultBKG, sizeof(CUSTOMVERTEX));
	}
	//g_pD3Device->SetTexture(0, g_pTexture[RESULT_BKG_TEX]);
	//g_pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, vertexResultBKG, sizeof(CUSTOMVERTEX));

	/*g_pD3Device->SetTexture(0, g_pTexture[GAME_PLAYER_TEX]);
	g_pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, vertexResult, sizeof(CUSTOMVERTEX));*/

	//`ζΜIΉ
	g_pD3Device->EndScene();
	//\¦
	g_pD3Device->Present(NULL, NULL, NULL, NULL);
}