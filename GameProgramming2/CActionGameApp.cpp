/**
 * @file CActionGameApp.cpp
 * @brief アプリケーションのエントリ ポイントを定義します。
 */

#include "stdafx.h"
#include "CActionGameApp.h"


CActionGameApp::CActionGameApp()
{
}


CActionGameApp::~CActionGameApp()
{
}

/**
 * @fn void CActionGameApp::initCameraInfo()
 * @brief カメラの設定
 * @param	無し
 * @return	無し
 */
void CActionGameApp::initCameraInfo()
{
	float z = -(float)(min(WINDOW_HEIGHT,WINDOW_WIDTH) / 2) / tanf(XM_PI / 8.0f);
	m_vEyePt	= XMFLOAT3{ 0.0f, 0.0f,		z };	//!< カメラ（視点）位置
	m_vLookatPt	= XMFLOAT3{ 0.0f, 0.0f,     0.0f };	//!< 注視位置
	m_vUpVec	= XMFLOAT3{ 0.0f, 1.0f,     0.0f };	//!< 上方位置
	m_angle		= (FLOAT)XM_PI / 4;					//!< 視野角
	m_aspect	= ASPECT;							//!< アスペクト比
	m_near		= 0.1f;								//!< 前方クリップ
	m_far		= abs(z) + 100.0f;					//!< 後方クリップ
}

void CActionGameApp::renderSprite( CPicture* object)
{
	XMFLOAT4X4	matView = m_pCamera->getViewMatrix();		 //!< ビュートランスフォーム（視点座標変換）
	XMFLOAT4X4	matProj = m_pCamera->getProjectionMatrix();	 //!< プロジェクショントランスフォーム（射影変換）
	if( object != NULL)
	{
#if 1
		XMFLOAT4 col;
		col.x = 1.0f;
		col.y = 1.0f;
		col.z = 1.0f;
		col.w = 1.0f;

		object->render(matView, matProj, 0Ui64, col);
#else
		object->render(matView, matProj);
#endif
	}
}
void CActionGameApp::renderSprite(CPicture* object, XMFLOAT3 color)
{
	XMFLOAT4X4	matView = m_pCamera->getViewMatrix();		 //!< ビュートランスフォーム（視点座標変換）
	XMFLOAT4X4	matProj = m_pCamera->getProjectionMatrix();	 //!< プロジェクショントランスフォーム（射影変換）
	if (object != NULL)
	{
		XMFLOAT4 col;
		col.x = color.x;
		col.y = color.y;
		col.z = color.z;

		object->render(matView, matProj, 0Ui64, col);
	}
}
void CActionGameApp::renderSprite(CPicture* object, float r, float g, float b)
{
	renderSprite( object, XMFLOAT3(r, g, b));
}
void CActionGameApp::renderSprite(CPicture* object, XMFLOAT4 color)
{
	XMFLOAT4X4	matView = m_pCamera->getViewMatrix();		 //!< ビュートランスフォーム（視点座標変換）
	XMFLOAT4X4	matProj = m_pCamera->getProjectionMatrix();	 //!< プロジェクショントランスフォーム（射影変換）
	if (object != NULL)
	{
		XMFLOAT4 col;
		col.x = color.x;
		col.y = color.y;
		col.z = color.z;
		col.w = color.w;

		object->render(matView, matProj, 0Ui64, col);
	}
}void CActionGameApp::disposeSprite( CPicture* object)
{
	if( object != NULL)
	{
		object->release();
		delete object;
	}
}
void CActionGameApp::renderFont(CFont* object, int num_)
{
	XMFLOAT4X4	matView = m_pCamera->getViewMatrix();		 //!< ビュートランスフォーム（視点座標変換）
	XMFLOAT4X4	matProj = m_pCamera->getProjectionMatrix();	 //!< プロジェクショントランスフォーム（射影変換）
	if (object != NULL)
	{
		XMFLOAT4 col;
		col.x = 1.0f;
		col.y = 1.0f;
		col.z = 1.0f;
		col.w = 1.0f;

		object->render(matView, matProj, num_, 0Ui64, col);
	}
}
void CActionGameApp::disposeFont(CFont* object)
{
	if (object != NULL)
	{
		object->release();
	}
}

/**
 * @fn bool CActionGameApp::createSpaceship(void)
 * @brief スプライトの生成
 * @param	無し
 * @return	成功・失敗
 */
bool CActionGameApp::createSpaceship(void)
{
	/*
    if (m_pPlayer == NULL)
    {
        m_pPlayer = CPlayer::create(m_pDevice, m_pDeviceContext, m_viewPort, HLSL_PlaneHLSL,
            { 0.0f, -200.0f, 0.0f, 0.0f },	//!< スプライトの位置
            kCharSpriteSize,				//!< スプライトのサイズ
            kTexelPlayerBulletIndex);		//!< テクセルの配列のインデックス（アニメーションのため）
    }
    m_pPlayer->setHP(kMaxHP);                           // <!
    m_pPlayer->setDamage(FALSE);                        // <!
    m_pPlayer->setActive(TRUE);                         // <!
    m_pPlayer->setPos({ 0.0f, -200.0f, 0.0f, 0.0f });   // <!
	m_pPlayer->setState(eMoving);
	m_pPlayer->setScale(1.0f);
    m_pPlayer->resetExplosion();                        // <!
    m_pPlayer->resetAllBullets();                       // <!
    m_pPlayer->setAngleZ(0.0f);                         // <!

	XMFLOAT4 ePos[] = {
		{-120.0f +  0 + 48 * 0, 200.0f, 0.0f, 0.0f},
		{-120.0f +  0 + 48 * 1, 200.0f, 0.0f, 0.0f},
		{-120.0f +  0 + 48 * 2, 200.0f, 0.0f, 0.0f},
		{-120.0f +  0 + 48 * 3, 200.0f, 0.0f, 0.0f},
		{-120.0f +  0 + 48 * 4, 200.0f, 0.0f, 0.0f},
		{-120.0f + 24 + 48 * 0, 160.0f, 0.0f, 0.0f},
		{-120.0f + 24 + 48 * 1, 160.0f, 0.0f, 0.0f},
		{-120.0f + 24 + 48 * 2, 160.0f, 0.0f, 0.0f},
		{-120.0f + 24 + 48 * 3, 160.0f, 0.0f, 0.0f}
	};
	for (size_t i = 0; i < _countof(m_pEnemies); i++)
	{
        if (m_pEnemies[i] == NULL)
        {
            m_pEnemies[i] = CEnemy::create(m_pDevice, m_pDeviceContext, m_viewPort, HLSL_PlaneHLSL,
                ePos[i],                    //!< スプライトの位置
                kCharSpriteSize,			//!< スプライトのサイズ
                kTexelEnemyBulletIndex);	//!< テクセルの配列のインデックス（アニメーションのため）
        }
        m_pEnemies[i]->setHP(kMaxHP);       // !<
        m_pEnemies[i]->setDamage(FALSE);    // <!
        m_pEnemies[i]->setActive(TRUE);     // !<
        m_pEnemies[i]->setPos(ePos[i]);     // !<
		m_pEnemies[i]->setState(eMoving);
		m_pEnemies[i]->setScale(1.0f);
        m_pEnemies[i]->resetExplosion();     //!< 
        m_pEnemies[i]->resetAllBullets();    //!< 
	}
	m_pPlayerTex    = CTexture::create(m_pDevice, Tex_Spaceship);	//!< テクセル
	m_pEnemyTex     = CTexture::create(m_pDevice, Tex_Spaceship);	//!< テクセル
	*/

	return true;
}

/**
 * @fn void CActionGameApp::doOpenning()
 * @brief オープニング
 * @param	無し
 * @return	無し
 */
void CActionGameApp::doOpenning ()
{
	procTitle();
}

/**
 * @fn void CActionGameApp::doPlaying()
 * @brief プレー中
 * @param	無し
 * @return	無し
 */
void CActionGameApp::doPlaying ()
{
#if 0
	// 【ステップ4】
	/**
	* プレイヤーのアニメーション
	*/
	m_frameCounter++;
	m_frameCounter = m_frameCounter > 99 ? 0 : m_frameCounter;

	m_pPlayer->stepAnimation(m_frameCounter % kTexelIntervalFrames == 0);

    m_pBack->scroll(-1.0f);
	m_pMiddle->scroll(-2.0f);
	m_pFront->scroll(-3.0f);
	/**
	* プレーヤーの制御
	*/
	if (m_pDirectInput->isKeyPressed(DIK_LEFT))		m_pPlayer->offsetPosInNormalRange({ -2.0f,  0.0f, 0.0f, 0.0f });
	if (m_pDirectInput->isKeyPressed(DIK_RIGHT))	m_pPlayer->offsetPosInNormalRange({  2.0f,  0.0f, 0.0f, 0.0f });
	if (m_pDirectInput->isKeyPressed(DIK_UP))		m_pPlayer->offsetPosInNormalRange({  0.0f,  2.0f, 0.0f, 0.0f });
	if (m_pDirectInput->isKeyPressed(DIK_DOWN))		m_pPlayer->offsetPosInNormalRange({  0.0f, -2.0f, 0.0f, 0.0f });

	if (m_pDirectInput->isKeyPressed(DIK_V))		m_pPlayer->addAngleZ(-0.5);
	if (m_pDirectInput->isKeyPressed(DIK_N))		m_pPlayer->addAngleZ( 0.5);
	if (m_pDirectInput->isPressedOnce(DIK_B))		m_pPlayer->shot();

	if (m_pDirectInput->isLeftButtonClicked())	    m_pPlayer->shot();

	/**
	* 【ステップ5】敵の操作
	*/
    auto    callback = [&](std::shared_ptr<CEnemy> enemy_, size_t index_) -> BOOL {
        if (enemy_->getActive() == TRUE)
        {
            prepareFrame(enemy_, index_);
        }
        return TRUE; //!< 列挙を続行
    };
	if (isActiveEnemies() == FALSE)
	{
		m_gameStatus = eGameClear;
	}
	
	/**
	* プレーヤーの爆発が完了したらゲームオーバー.
	*/
	if (m_pPlayer->isExplosionCompleted())
	{
		m_gameStatus = eGameOver;
	}
#else
	procGame ();

#endif
}


/**
 * @fn void CActionGameApp::doGameClear()
 * @brief ゲームクリア
 * @param	無し
 * @return	無し
 */
void CActionGameApp::doGameClear()
{
	procClear ();
}

/**
 * @fn void CActionGameApp::doGameOver()
 * @brief ゲームオーバー
 * @param	無し
 * @return	無し
 */
void CActionGameApp::doGameOver()
{
	procGameOver ();
}

/**
 * @fn void CActionGameApp::doEnding()
 * @brief エンディング
 * @param	無し
 * @return	無し
 */
void CActionGameApp::doEnding()
{
}

/**
 * @fn void CActionGameApp::render(XMFLOAT4X4 matView_, XMFLOAT4X4 matProj_)
 * @brief レンダー
 * @param	matView_	ビュー行列
 * @param	matProj_	プロジェクション行列
 * @return				無し
 */
void CActionGameApp::render(XMFLOAT4X4 matView_, XMFLOAT4X4 matProj_)
{
	/*
    auto setUpEnemyTex = [&](ID3D11DeviceContext* pDeviceContext_) -> void {
        m_pEnemyTex->setup(pDeviceContext_);
    };
    auto callback = [&](CEnemy* enemy_, size_t index_) -> BOOL {
		enemy_->render(matView_, matProj_, setUpEnemyTex);
		return TRUE; //!< 列挙を続行	
    };
    auto setUpPlayerTex = [&](ID3D11DeviceContext* pDeviceContext_) -> void {
        m_pPlayerTex->setup(pDeviceContext_);
    };

	switch (m_gameStatus)
	{
	case eOpening:
		m_pTitle->render(matView_, matProj_);
		break;
	case ePlaying:
		// 引数描画する順番と結果を確認すること
		m_pPlayer->render(matView_, matProj_, setUpPlayerTex);

		m_pFront->render(matView_, matProj_);
		m_pMiddle->render(matView_, matProj_);
		m_pBack->render(matView_, matProj_);
		break;
	case eGameOver:
		m_pGameOver->render(matView_, matProj_);
		break;
	default:
		break;
	}
	*/
}


/**
* @fn void CActionGameApp::release()
* @brief 解放
* @param	無し
* @return	無し
*/
void CActionGameApp::release()
{
	super::release();
//	m_pTitle->release();
//	m_pGameOver->release();
}


// --------------------------------------------
// 以下、細田弥卯が記述（2021_09_29）

/**
* @fn void COriginalGameApp::createSound()
* @brief サウンドの作成
* @param	filePath_	wavファイルへのパス
* @param	loop_	音声をループさせるなら、true
* @return	作成した音声を管理するポインタ
*/
CSound* CActionGameApp::createSound(const char* filePath_, const bool loop_)
{
	CSound* object = NULL;

	// wavファイルの作成
	if (!object)
	{
		object = CSound::Create(m_pXaudio2, filePath_, loop_);
	}

	return object;
}

/**
* @fn void COriginalGameApp::PlaySourceSound()
* @brief サウンドの再生
* @param	objects	CSoundのポインタ
* @return	なし
*/
void CActionGameApp::PlaySourceSound(CSound* const objects)
{
	// 作成されていなければ戻す
	if (!objects) { return; }
	objects->PlaySourcevoice();
}

/**
* @fn void COriginalGameApp::disposeSound()
* @brief サウンドの破棄
* @param	objects	CSoundのポインタ
* @return	なし
*/
void CActionGameApp::disposeSound(CSound* const objects)
{
	// 作成されていなければ戻す
	if (!objects) { return; }
	objects->DeleteSourcevoice();
	delete objects;
}