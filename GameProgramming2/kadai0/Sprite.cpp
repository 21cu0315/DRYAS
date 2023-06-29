#include "stdafx.h"
#include "CActionGameApp.h"
#include "GameResource.h"
#include "UV.h"

/**
 * 関数名 : createSprite()
 * 処理内容 : 指定されたパスにある画像を、指定されたサイズで作成する
 * 引数 :	path_		画像のあるパス
 *			width_		作成する画像の幅
 *			height_		作成する画像の高さ
 * 戻り値 : 作成した画像を管理するオブジェクト
 */
CPicture* CActionGameApp::createSprite (LPCWSTR path_, float width_, float height_)
{
	vector <QuadrangleTexel>  singleTexels = {
	QuadrangleTexel{ { 0, 1.0f },{ 0, 0.0f },{ 1, 1.0f },{ 1, 0.0f } }	// 指定するUV座標が一つの配列
	};
	XMFLOAT2 spriteSize = { width_, height_};
	CPicture* object = NULL;

	//PlayerDeathの場合、指定するUV座標を複数配列用意
	if (path_ == Tex_PlayerDeath)
	{
		object = CPicture::create(m_pDevice, m_pDeviceContext, m_viewPort, HLSL_AlphaHLSL, path_,
			{ 0.0f, 0.0f, 0.0f, 0.0f },	// スプライトの位置
			spriteSize,					// スプライトのサイズ
			kTexelPlayerDeath);				// 指定するUV座標の配列
	}
	//EnemyDeathの場合、指定するUV座標を複数配列用意
	else if (path_ == Tex_EnemyDeath)
	{
		object = CPicture::create(m_pDevice, m_pDeviceContext, m_viewPort, HLSL_AlphaHLSL, path_,
			{ 0.0f, 0.0f, 0.0f, 0.0f },	// スプライトの位置
			spriteSize,					// スプライトのサイズ
			kTexelEnemyDeath);				// 指定するUV座標の配列
	}
	else if (path_ == Tex_Player)
	{
		object = CPicture::create(m_pDevice, m_pDeviceContext, m_viewPort, HLSL_AlphaHLSL, path_,
			{ 0.0f, 0.0f, 0.0f, 0.0f },	// スプライトの位置
			spriteSize,					// スプライトのサイズ
			kTexelPlayer);				// 指定するUV座標の配列
	}
	else
	{
		object = CPicture::create(m_pDevice, m_pDeviceContext, m_viewPort, HLSL_AlphaHLSL, path_,
			{ 0.0f, 0.0f, 0.0f, 0.0f },	// スプライトの位置
			spriteSize,					// スプライトのサイズ
			singleTexels);				// 指定するUV座標の配列
	}
	return object;
}

CFont* CActionGameApp::createFont(LPCWSTR path_, int digits_, float width_, float height_)
{
	XMFLOAT2 spriteSize = { width_, height_ };
	CFont* objects = NULL;

	return objects;
}
