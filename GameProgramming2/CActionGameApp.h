/**
* @file CActionGameApp.h
* @author �D�ˁ@�엲
* @date ���t�i2017.02.14�j
*/
#pragma once

#include "stdafx.h"
#include "CApplication.h"
#include "CPicture.h"
#include "MediaResource.h"
#include "UVAnimation.h"
#include "CFont.h"
#include <math.h>
// --------------------------------------------
// �g���͍דc��K���L�q�i2021_09_29�j
#include "CSound.h"
// --------------------------------------------


class CActionGameApp :
	public CApplication
{
	typedef CApplication super;
private:
	int m_activeEnemies										= 0;

	CPicture* createSprite(LPCWSTR path_, float width_ = WINDOW_WIDTH, float height_ = WINDOW_HEIGHT);
	void renderSprite( CPicture* object);
	void renderSprite(CPicture* object, XMFLOAT3 color);
	void renderSprite(CPicture* object, float r, float g, float b);
	void renderSprite(CPicture* object, XMFLOAT4 color);
	void disposeSprite( CPicture* object);
	CFont* createFont(LPCWSTR path_, int digits_, float width_, float height_);
	void renderFont(CFont* object, int num_);
	void disposeFont(CFont* object);
	bool createSpaceship(void);
	void initCameraInfo();
	void doOpenning();
	void doPlaying();
	void doGameClear();
	void doGameOver();
	void doEnding();

	void procTitle();
	void procGame();
	void procClear();
	void procGameOver();

	// --------------------------------------------
	// �g���͍דc��K���L�q�i2021_09_29�j

	CSound* createSound(const char* filePath_, const bool loop_ = false);
	void PlaySourceSound(CSound* const objects);
	void disposeSound(CSound* const objects);
	// --------------------------------------------


	// �ۑ�p�֐�
	void procTitleBegin();
	void procTitleMain();
	void procTitleEnd();
	void procPlayBegin();
	void procPlayMain();
	void procPlayEnd();
	void procClearBegin();
	void procClearMain();
	void procClearEnd();
	void procOverBegin();
	void procOverMain();
	void procOverEnd();
	void procPlayMain_movePlayer();     // �ۑ�1-3
	void procPlayMain_procEnemy();     // �ۑ�1-3
	void procPlayMain_animatePlayerDeath();     // �ۑ�2-1
	void procPlayMain_shotPlayer();            // �ۑ�2-2
	void procPlayMain_procPlayerBullet();      // �ۑ�2-2
	void procPlayBegin_createBackgroundSprite();
	void procPlayMain_procBackground();
	void procPlayMain_renderBackgroundSprite(); // �ۑ�4-2
	void procPlayEnd_disposeBackgroundSprite();
	void procPlayMain_procEnemyBullet();
	void procPlayMain_shotEnemy();
	void procPlayMain_procPlayer();
	void procPlayMain_animateEnemyDeath();

public:
	CActionGameApp();
	~CActionGameApp();
	void render(XMFLOAT4X4 matView, XMFLOAT4X4 matProj);
	void release();
};
