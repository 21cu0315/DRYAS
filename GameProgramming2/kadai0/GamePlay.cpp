/*
* 作業内容：ゲームフローを完成
* 作成日：2021/11/15
*/

/*
* 作業内容：プレイヤーと敵の画像を表示する
*			プレイヤーと敵を移動させる
*			プレイヤーと敵の衝突判定の実装	
*			プレイヤーと壁の衝突判定の実装
* 作成日：2021/11/22
*/

/*
* 作業内容：プレイヤーの弾の処理
* 作成日：2021/11/29・2021/12/06・2021/12/13
*/

/*
* 作業内容：プレイヤーの弾の処理（終わり）
*			アイテムを取得させる（speed up 終わり）
* 作成日：2021/12/20
*/

#include "CActionGameApp.h"
#include "GameResource.h"


#define NOPERPWALL 5
#define NOPARATHINWALL 5
#define NOPARATHICKWALL 3
#define pi 3.141592653589793238463
#define noOfBullet 20
#define noOfEnemyBullet 10
#define bulletCDTime 15		//frames needed between shots(player)
#define enemyBulletCDTime 60 

//変数定義
//ゲーム画面のフェーズを管理する変数
ePhase g_GamePhase = eBegin;
//背景の画像を管理する変数
CPicture* g_pBackground = NULL;
//ゲーム画面から次へ進むときのフローを管理する変数
eGameStatus g_NextStatusFromGame = eNone;

//地面と垂直の壁の画像を管理する変数
CPicture* g_pPerpWall[NOPERPWALL] = { NULL,NULL };
//地面と平行の細い壁の画像を管理する変数
CPicture* g_pParaThinWall[NOPARATHINWALL] = { NULL,NULL };
//地面と平行の厚い壁の画像を管理する変数
CPicture* g_pParaThickWall[NOPARATHICKWALL] = { NULL,NULL };

//プレイヤーの画像を管理する変数
CPicture* g_pPlayer = NULL;
//プレイヤーの表示サイズ
const float PLAYER_SIZE_X = 90.0f;
const float PLAYER_SIZE_Y = 90.0f;
//プレイヤーの移動速度
float g_playerMoveSpeed = 6.25f;
//プレイヤーの弾の画像を管理する変数
CPicture* g_pPlayerBullet[noOfBullet];
//プレイヤーの弾の表示サイズ
const float PLAYER_BULLET_SIZE_X = 20.0f;
const float PLAYER_BULLET_SIZE_Y = 3.0f;
//プレイヤーのHP
int playerHP = 150;
//プレイヤー移動アニメーション用のカウンター
int g_animCounter = 9;

//プレイヤーの弾の処理
double dMaxRange = sqrt(pow(WINDOW_WIDTH, 2) + pow(WINDOW_HEIGHT, 2));
//弾が撃たれた時プレイヤーの位置
XMFLOAT2 playerPosShot[noOfBullet];
//弾が撃たれた時弾の方向
XMFLOAT2 shotPos[noOfBullet];
bool bulletIsMoving[noOfBullet] = { false,false,false,false,false };
int bulletCount = 0;
bool bMouseLeftClicked = false;
int bulletCD = 0;
bool cdEnded = true;



//プレイヤーが死ぬときのエフェクトを管理する変数
CPicture* g_pPlayerDeath = NULL;
//死亡フラグ
bool g_isPlayerDead = false;
//プレイヤーが死ぬときのエフェクト終了フラグ
bool g_endPlayerDeathEffect = false;
//プレイヤーが死ぬときのエフェクトのアニメーション番号
int g_PlayerDeathAnimIndex = 0;
//プレイヤーが死ぬときのエフェクトのアニメーション用カウンター
int g_PlayerDeathAnimCounter = 0;
//プレイヤーが死ぬときの向き
XMFLOAT2 playerDeathMousePos;

//敵の情報をまとめた構造体
struct Enemy
{
	CPicture* obj;			//画像管理用
	CPicture* speedUp;		//speed upアイテムの画像用
	XMFLOAT2 beginPos;		//初期座標
	int Hp;					//体力
	CPicture* Shot[noOfEnemyBullet];			//弾の画像用
	XMFLOAT2 shotPos[noOfEnemyBullet];		//弾の方向
	bool gotItem;
};
const float ENEMY_BULLET_SIZE_X = 20.0f;
const float ENEMY_BULLET_SIZE_Y = 3.0f;
bool inStage = false;
int enemyBulletCD = 0;
int enemyBulletCount = 0;


//敵の数
const int ENEMY_OBJECT_COUNT = 3;
//敵の表示サイズ
const int ENEMY_SIZE_X = 75.0f;
const int ENEMY_SIZE_Y = 128.0f;
//敵の情報を管理する変数
Enemy g_enemyObjs[ENEMY_OBJECT_COUNT] =
{
	{NULL,NULL,XMFLOAT2(-400.0f + (200.0f * 0.0f),100.0f),100,NULL},
	{NULL,NULL,XMFLOAT2(-400.0f + (200.0f * 1.0f),100.0f),100,NULL},
	{NULL,NULL,XMFLOAT2(-400.0f + (200.0f * 2.0f),100.0f),100,NULL},
};
//敵の移動速度
float g_enemyMoveSpeed = 3.0f;
//speed upアイテムの表示サイズ
const float SPEEDUP_SIZE_X = 39.0f;
const float SPEEDUP_SIZE_Y = 64.0f;
//死んだ敵の数
int deadEnemy = 0;
//敵の移動方向のフラグ
bool enemyIsMovingRight[ENEMY_OBJECT_COUNT] = { true,true };
bool enemyIsMovingUp[ENEMY_OBJECT_COUNT] = { true,true };

struct EnemyDeathAnime
{
	//敵が死ぬときのエフェクトを管理する変数
	CPicture* Death = NULL;
	//死亡フラグ
	bool isDead = false;
	//敵が死ぬときのエフェクト終了フラグ
	bool endDeathEffect = false;
	//敵が死ぬときのエフェクトのアニメーション番号
	int DeathAnimIndex = 0;
	//敵が死ぬときのエフェクトのアニメーション用カウンター
	int DeathAnimCounter = 0;
	//敵が死ぬときの向き
	XMFLOAT2 DeathPlayerPos;
};

//ステージカウンター
int stageNo = 1;
//ローディング画面1
CPicture* g_pStage1Start = NULL;
CPicture* g_pStage2Start = NULL;
CPicture* g_pStage3Start = NULL;


/*
 * ゲーム本編の処理を行う部分
 */
void CActionGameApp::procGame()
{
	//フェーズの進行状態によって処理を切り替える
	switch (g_GamePhase)
	{
		//開始時
	case eBegin:
	{
		if (stageNo == 1) 
		{
			g_pStage1Start = createSprite(Tex_Stage1Start, WINDOW_WIDTH, WINDOW_HEIGHT);
			g_pStage1Start->setPos(0.0f, 0.0f);
			renderSprite(g_pStage1Start);
			if (m_pDirectInput->isPressedOnce(DIK_X))
			{
				disposeSprite(g_pStage1Start);
				//ゲーム画面の開始処理
				procPlayBegin();
				//進行状態をメインに切り替える
				g_GamePhase = eMain;
			}
		}
		else
		{
			//ゲーム画面の開始処理
			procPlayBegin();
			//進行状態をメインに切り替える
			g_GamePhase = eMain;
		}
	}
	break;
	//メイン
	case eMain:
	{
		//ゲーム画面のメイン処理
		procPlayMain();
	}
	break;
	//終了時
	case eEnd:
	{
		//ゲーム画面の終了処理
		procPlayEnd();
		//再度ゲーム画面に来た時に開始から処理を行うために
		//フェーズを戻しておく
		g_GamePhase = eBegin;
		
		//各種フラグをリセット
		g_isPlayerDead = false;
		g_endPlayerDeathEffect = false;
		for (int i = 0; i < ENEMY_OBJECT_COUNT; ++i)
		{
			g_enemyObjs[i].Hp = 100;
			g_enemyObjs[i].beginPos = XMFLOAT2(-600.0f + (100.0f * (i + 0.0f)), 100.0f);
		}
		g_playerMoveSpeed = 6.0f;
		playerHP = 150;
		stageNo = 1;
		deadEnemy = 0;
		g_enemyObjs[0].gotItem = false;

		//次のステータスを設定する
		SetNextGameStatus(g_NextStatusFromGame);
	}
	break;
	}
}

/*
* 関数名：procPlayBegin()
* 処理内容：ゲーム画面の開始時に行う処理
* 引数：なし
* 戻り値：なし
*/
void CActionGameApp::procPlayBegin()
{
	//g_pBackground が　NULLの時のみ画像を作成する
	if (g_pBackground == NULL)
	{
		//背景の画像を作成する
		g_pBackground = createSprite(Tex_BackBack);
	}
	//g_pPlayer が NULL の時のみ画像を作成する
	if (g_pPlayer == NULL)
	{
		//プレイヤーの画像の作成・配置する
		g_pPlayer = createSprite(Tex_Player, PLAYER_SIZE_X, PLAYER_SIZE_Y);
		g_pPlayer->setPos(0.0f, -400.0f);
	}
	//敵の画像を作成する
	for (int i = 0; i < ENEMY_OBJECT_COUNT; ++i)
	{
		//それぞれの obj が NULL の時のみ画像を作成する
		if (g_enemyObjs[i].obj == NULL)
		{
			//敵の画像を作成・配置する
			g_enemyObjs[i].obj = createSprite(Tex_Enemy, ENEMY_SIZE_X, ENEMY_SIZE_Y);
			g_enemyObjs[i].obj->setPos(g_enemyObjs[i].beginPos);
		}
	}
	
}

/*
* 関数名：procPlayMain
* 処理内容：ゲーム画面のメイン処理
* 引数：なし
* 戻り値：なし
*/
void CActionGameApp::procPlayMain()
{
	//押されたキーによってステータスを変更する
	if (deadEnemy == ENEMY_OBJECT_COUNT && g_enemyObjs[0].speedUp == NULL && stageNo == 3)
	{
		g_NextStatusFromGame = eGameClear;
		g_GamePhase = eEnd;
	}
	else if (deadEnemy == ENEMY_OBJECT_COUNT && g_enemyObjs[0].speedUp == NULL && stageNo != 3)
	{
		if (stageNo == 1)
		{
			if (g_pStage2Start == NULL)
			{
				//ゲーム画面の終了処理
				procPlayEnd();
				g_pStage2Start = createSprite(Tex_Stage2Start, WINDOW_WIDTH, WINDOW_HEIGHT);
				g_pStage2Start->setPos(0.0f, 0.0f);
			}
			if (g_pStage2Start != NULL)
			{
				renderSprite(g_pStage2Start);
			}
		}		
		if (stageNo == 2)
		{
			if (g_pStage3Start == NULL)
			{
				//ゲーム画面の終了処理
				procPlayEnd();
				g_pStage3Start = createSprite(Tex_Stage3Start, WINDOW_WIDTH, WINDOW_HEIGHT);
				g_pStage3Start->setPos(0.0f, 0.0f);
			}
			if (g_pStage3Start != NULL)
			{
				renderSprite(g_pStage3Start);
			}
		}

		if (m_pDirectInput->isPressedOnce(DIK_X))
		{
			stageNo++;
			g_GamePhase = eBegin;
			disposeSprite(g_pStage2Start);
			g_pStage2Start = NULL;
			//各種フラグをリセット
			deadEnemy = 0;
			g_enemyObjs[0].gotItem = false;
			for (int i = 0; i < ENEMY_OBJECT_COUNT; ++i)
			{
				if (stageNo == 2)
				{
					g_enemyObjs[i].beginPos = XMFLOAT2(400.0f - (200.0f * (i + 0.0f)), 300.0f * (i - 0.5f));
					g_enemyObjs[i].Hp = 150;
				}
				else if (stageNo == 3)
				{
					g_enemyObjs[i].beginPos = XMFLOAT2(600.0f - (500.0f * (i + 0.0f)), 200.0f * (i - 0.5f));
					g_enemyObjs[i].Hp = 250;
				}
			}
		}
	}
	//プレイヤーが死ぬときのエフェクトが再生し終わったとき
	//ゲームオーバー画面に移行
	if (g_endPlayerDeathEffect != false)
	{
		g_NextStatusFromGame = eGameOver;
		g_GamePhase = eEnd;
	}

	//プレイヤーの操作
	if (g_pPlayer != NULL)
	{
		//プレイヤーの移動処理
		procPlayMain_movePlayer();
		//プレイヤーの攻撃処理
		procPlayMain_shotPlayer();
	}
	//プレイヤーが存在しない場合は死亡しているので、
	//プレイヤーが死ぬときのエフェクトをアニメーションさせる
	else
	{
		//プレイヤーが死ぬときのエフェクト処理
		procPlayMain_animatePlayerDeath();
	}
	

	//背景画像が作成されていたら描画する
	if (g_pBackground != NULL)
	{
		renderSprite(g_pBackground);
	}

	//敵の処理
	procPlayMain_procEnemy();

	procPlayMain_shotEnemy();

	
	
	

	//プレイヤーの弾の処理
	procPlayMain_procPlayerBullet();

	procPlayMain_procEnemyBullet();

	//プレイヤーの弾の画像が作成されていたら描画する
	for (int i = 0; i < noOfBullet; ++i) 
	{
		if (g_pPlayerBullet[i] != NULL)
		{
			renderSprite(g_pPlayerBullet[i]);
		}
	}
	//敵の弾の画像が作成されていたら描画する
	for (int i = 0; i < ENEMY_OBJECT_COUNT; ++i)
	{
		for (int j = 0; j < noOfEnemyBullet; ++j)
		{
			if (g_enemyObjs[i].Shot[j] != NULL)
			{
				renderSprite(g_enemyObjs[i].Shot[j]);
			}
		}
	}
	

	//プレイヤーが画像が作成されていたら描画する
	if (g_pPlayer != NULL)
	{
		renderSprite(g_pPlayer);
	}

	//プレイヤーが死ぬときのエフェクトが存在しているときだけ描画する
	if (g_pPlayerDeath != NULL)
	{
		renderSprite(g_pPlayerDeath);
	}

	//ローディング画面が存在しているときだけ描画する
	if (g_pStage2Start != NULL)
	{
		renderSprite(g_pStage2Start);
	}
	
}

/*
* 関数名：procPlayMain_movePlayer()
* 処理内容：プレイヤーの移動処理
* 引数：なし
* 戻り値：なし
*/
void CActionGameApp::procPlayMain_movePlayer()
{
	//プレイヤーの移動
	XMFLOAT4 pos = g_pPlayer->getPos();
	if (m_pDirectInput->isKeyPressed(DIK_D))
	{
		
		if (pos.x >= (WINDOW_WIDTH / 2 - 100.0f - 45.0f))
		{
			pos.x = WINDOW_WIDTH / 2 - 100.0f - 45.0f;

		}
		else if (m_pDirectInput->isKeyPressed(DIK_W) || m_pDirectInput->isKeyPressed(DIK_S))
		{
			pos.x += sqrt(pow(g_playerMoveSpeed, 2) / 2);
			//移動アニメーション
			g_animCounter++;
			if (g_animCounter % 10 == 0)
			{
				g_pPlayer->stepAnimation();
				//カウンターを0に戻す
				g_animCounter = 0;
			}
		}
		else
		{
			pos.x += g_playerMoveSpeed;
			//移動アニメーション
			g_animCounter++;
			if (g_animCounter % 10 == 0)
			{
				g_pPlayer->stepAnimation();
				//カウンターを0に戻す
				g_animCounter = 0;
			}
		}
	}
	
	if (m_pDirectInput->isKeyPressed(DIK_A))
	{
		
		if (pos.x <= (-WINDOW_WIDTH / 2 + 100.0f + 45.0f))
		{
			pos.x = -WINDOW_WIDTH / 2 + 100.0f + 45.0f;
			
		}
		else if (m_pDirectInput->isKeyPressed(DIK_W) || m_pDirectInput->isKeyPressed(DIK_S))
		{
			pos.x -= sqrt(pow(g_playerMoveSpeed, 2) / 2);
			//移動アニメーション
			g_animCounter++;
			if (g_animCounter % 10 == 0)
			{
				g_pPlayer->stepAnimation();
				//カウンターを0に戻す
				g_animCounter = 0;
			}
	
		}
		else
		{
			pos.x -= g_playerMoveSpeed;
			//移動アニメーション
			g_animCounter++;
			if (g_animCounter % 10 == 0)
			{
				g_pPlayer->stepAnimation();
				//カウンターを0に戻す
				g_animCounter = 0;
			}
		}
	}
	
	if (m_pDirectInput->isKeyPressed(DIK_W))
	{
		
		if (pos.y >= (WINDOW_HEIGHT / 2 - 100.0f - 45.0f))
		{
			pos.y = WINDOW_HEIGHT / 2 - 100.0f - 45.0f;
			
		}
		else if (m_pDirectInput->isKeyPressed(DIK_A) || m_pDirectInput->isKeyPressed(DIK_D))
		{
			pos.y += sqrt(pow(g_playerMoveSpeed, 2) / 2);
		}
		else
		{
			pos.y += g_playerMoveSpeed;
			//移動アニメーション
			g_animCounter++;
			if (g_animCounter % 10 == 0)
			{
				g_pPlayer->stepAnimation();
				//カウンターを0に戻す
				g_animCounter = 0;
			}
		}
	}
	
	if (m_pDirectInput->isKeyPressed(DIK_S))
	{
		
		if (pos.y <= (-WINDOW_HEIGHT / 2 + 100.0f + 45.0f))
		{
			pos.y = -WINDOW_HEIGHT / 2 + 100.0f + 45.0f;
			
		}
		else if (m_pDirectInput->isKeyPressed(DIK_A) || m_pDirectInput->isKeyPressed(DIK_D))
		{
			pos.y -= sqrt(pow(g_playerMoveSpeed, 2) / 2);
		}
		else
		{
			pos.y -= g_playerMoveSpeed;
			//移動アニメーション
			g_animCounter++;
			if (g_animCounter % 10 == 0)
			{
				g_pPlayer->stepAnimation();
				//カウンターを0に戻す
				g_animCounter = 0;
			}
		}
	}
	
	g_pPlayer->setPos(pos);

	

	
	
	//プレイヤーの向き
	if (g_pPlayer != NULL) 
	{
		double playerFacingDirectionRad = atan2((m_pDirectInput->getMousePos().y - g_pPlayer->getPos().y),
		(m_pDirectInput->getMousePos().x - g_pPlayer->getPos().x));
		double playerFacingDirectionDeg = playerFacingDirectionRad * 180.0 / pi;
		g_pPlayer->setAngle(XMFLOAT3(0.0f, 0.0f, playerFacingDirectionDeg * 1.0f - 90.0f));
	}
	
}



/*
* 関数名：procPlayMain_shotPlayer
* 処理内容：プレイヤーの攻撃処理
* 引数：なし
* 戻り値：なし
*/
void CActionGameApp::procPlayMain_shotPlayer()
{
	if (cdEnded != true)
	{
		bulletCD++;
		
	}
	if (bulletCD >= bulletCDTime)
	{
		cdEnded = true;
	}
	
	//マウスの左クリックが1回だけ押された時、攻撃をする
	if (bMouseLeftClicked == false && m_pDirectInput->isLeftButtonClicked() 
		&& g_pPlayerBullet[bulletCount] == NULL && cdEnded == true)
	{
		
		//弾を発射するため、オブジェクトを作成
		
		g_pPlayerBullet[bulletCount] = createSprite(Tex_PlayerBullet, PLAYER_BULLET_SIZE_X, PLAYER_BULLET_SIZE_Y);
		g_pPlayerBullet[bulletCount]->setPos(g_pPlayer->getPos());
		shotPos[bulletCount] = { m_pDirectInput->getMousePos().x , m_pDirectInput->getMousePos().y };
		playerPosShot[bulletCount] = { g_pPlayer->getPos().x,g_pPlayer->getPos().y };

		//左クリックした時
		bMouseLeftClicked = true;
		//弾の数のカウンター
		bulletCount++;
		//弾の数を補正する
		if (bulletCount >= noOfBullet)
		{
			bulletCount = 0;
		}
		bulletCD = 0;
		cdEnded = false;
	}
	//左クリックに離した時
	if (m_pDirectInput->isLeftButtonReleased())
	{
		bMouseLeftClicked = false;
	}
	
}





/*
* 関数名：procPlayMain_shotEnemy
* 処理内容：敵の攻撃処理
* 引数：なし
* 戻り値：なし
*/
void CActionGameApp::procPlayMain_shotEnemy()
{

	enemyBulletCD++;
	for (int i = 0; i < ENEMY_OBJECT_COUNT; ++i)
	{
		if (g_pPlayer != NULL && g_enemyObjs[i].obj != NULL)
		{
			if (enemyBulletCD % 15 == 0 && g_enemyObjs[i].Shot[enemyBulletCount] == NULL)
			{
				g_enemyObjs[i].Shot[enemyBulletCount] = createSprite(Tex_EnemyBullet, ENEMY_BULLET_SIZE_X, ENEMY_BULLET_SIZE_Y);
				g_enemyObjs[i].Shot[enemyBulletCount]->setPos(g_enemyObjs[i].obj->getPos());
				g_enemyObjs[i].shotPos[enemyBulletCount] = { g_pPlayer->getPos().x , g_pPlayer->getPos().y };
			}
		}
	}
}


	
/*
* 関数名：procPlayMain_procEnemyBullet()
* 処理内容：敵の弾の処理
* 引数：なし
* 戻り値：なし
*/
void CActionGameApp::procPlayMain_procEnemyBullet()
{
	//プレイヤーの弾のオブジェクトが存在しているときだけ処理を行う
	for (int i = 0; i < ENEMY_OBJECT_COUNT; ++i)
	{
		for (int j = 0; j < noOfEnemyBullet; ++j)
		{
			if (g_enemyObjs[i].Shot[j] != NULL && g_enemyObjs[i].obj != NULL && g_pPlayer != NULL)
			{
				//敵の弾の方向(degree)
				double enemyBulletDirectionRad = atan2(((g_enemyObjs[i].shotPos[j]).y - g_enemyObjs[i].obj->getPos().y),
					((g_enemyObjs[i].shotPos[j]).x - g_enemyObjs[i].obj->getPos().x));
				double enemyBulletDirectionDeg = enemyBulletDirectionRad * 180.0 / pi;
				//移動
				XMFLOAT4 enemyBulletPos = g_enemyObjs[i].Shot[j]->getPos();
				enemyBulletPos.x += dMaxRange * cos(enemyBulletDirectionRad) * 1.0f / 150.0f;
				enemyBulletPos.y += dMaxRange * sin(enemyBulletDirectionRad) * 1.0f / 150.0f;
				g_enemyObjs[i].Shot[j]->setPos(enemyBulletPos);
				g_enemyObjs[i].Shot[j]->setAngle(XMFLOAT3(0.0f, 0.0f, enemyBulletDirectionDeg * 1.0f));
				

				//弾が最大範囲に超えたらオブジェクトを破棄する
				if (enemyBulletPos.x > WINDOW_WIDTH / 2.0f + ENEMY_BULLET_SIZE_X ||
					enemyBulletPos.x < -WINDOW_WIDTH / 2.0f - ENEMY_BULLET_SIZE_X)
				{
					disposeSprite(g_enemyObjs[i].Shot[j]);
					g_enemyObjs[i].Shot[j] = NULL;
				}
				else if (enemyBulletPos.y > WINDOW_HEIGHT / 2.0f + ENEMY_BULLET_SIZE_Y ||
					enemyBulletPos.y < -WINDOW_HEIGHT / 2.0f - ENEMY_BULLET_SIZE_Y)
				{
					disposeSprite(g_enemyObjs[i].Shot[j]);
					g_enemyObjs[i].Shot[j] = NULL;
				}
				else if (g_enemyObjs[i].Shot[j]->collision(g_pPlayer))
				{
					disposeSprite(g_enemyObjs[i].Shot[j]);
					g_enemyObjs[i].Shot[j] = NULL;
					if (playerHP > 0)
					{
						//敵のHPを減る
						playerHP -= 10;
					}
					else if (playerHP <= 0)
					{
						//プレイヤーが死ぬときのエフェクトの画像を作成する
						g_pPlayerDeath = createSprite(Tex_PlayerDeath, PLAYER_SIZE_X, PLAYER_SIZE_Y);
						//プレイヤーが死ぬときのエフェクトをプレイヤーの位置と同じ座標に設定する
						g_pPlayerDeath->setPos(g_pPlayer->getPos());

						//プレイヤーが死ぬときマウスの座標を取る
						playerDeathMousePos.x = m_pDirectInput->getMousePos().x;
						playerDeathMousePos.y = m_pDirectInput->getMousePos().y;

						//プレイヤーの向き(degree)
						double playerDeathDirectionRad = atan2((playerDeathMousePos.y - g_pPlayer->getPos().y),
							(playerDeathMousePos.x - g_pPlayer->getPos().x));
						double playerDeathDirectionDeg = playerDeathDirectionRad * 180.0 / pi;
						g_pPlayerDeath->setAngle(XMFLOAT3(0.0f, 0.0f, playerDeathDirectionDeg * 1.0f - 90.0f));
						//死亡フラグを立てる
						g_isPlayerDead = true;

						//プレイヤーを削除
						disposeSprite(g_pPlayer);
						g_pPlayer = NULL;
					}
				}
			}
			else if (g_enemyObjs[i].Shot[j] != NULL && g_enemyObjs[i].obj == NULL)
			{
				disposeSprite(g_enemyObjs[i].Shot[j]);
				g_enemyObjs[i].Shot[j] = NULL;
			}
			else if (g_enemyObjs[i].Shot[j] != NULL && g_pPlayer == NULL)
			{
				disposeSprite(g_enemyObjs[i].Shot[j]);
				g_enemyObjs[i].Shot[j] = NULL;
			}
		}
		
	}
}


/*
* 関数名：procPlayMain_animatePlayerDeath()
* 処理内容：プレイヤーが死ぬときのエフェクト処理
* 引数：なし
* 戻り値：なし
*/
void CActionGameApp::procPlayMain_animatePlayerDeath()
{
	//4フレームに１回、アニメーションを切り替える
	g_PlayerDeathAnimCounter++;
	if (g_PlayerDeathAnimCounter % 8 == 0 && g_pPlayerDeath != NULL)
	{
		g_pPlayerDeath->stepAnimation();
		g_PlayerDeathAnimCounter = 0;
		//何枚目のアニメーションなのかをカウントする
		g_PlayerDeathAnimIndex++;
	}
	//アニメーションが５枚なので、
	//再生し終えたら値をリセットしてフラグを立てる
	if (g_PlayerDeathAnimIndex == 5)
	{
		g_PlayerDeathAnimIndex = 0;
		g_endPlayerDeathEffect = true;
	}
}




/*
* 関数名：procPlayMain_procEnemy()
* 処理内容：敵の処理
* 引数：なし
* 戻り値：なし
*/
void CActionGameApp::procPlayMain_procEnemy()
{
	//敵の処理
	for (int i = 0; i < ENEMY_OBJECT_COUNT; ++i)
	{
		//敵画像が作成されていた時のみ以下の処理を行う
		if (g_enemyObjs[i].obj != NULL)
		{	
			//描画する
			renderSprite(g_enemyObjs[i].obj);
			
			//移動する
			//左右
			if (enemyIsMovingRight[i] ==false && g_enemyObjs[i].obj->getPos().x < (-WINDOW_WIDTH / 2 + 100.0f + 64.0f))
			{
				enemyIsMovingRight[i] = true;
			}
			else if (enemyIsMovingRight[i] == true && g_enemyObjs[i].obj->getPos().x > (WINDOW_WIDTH / 2 - 100.0f - 64.0f))
			{
				enemyIsMovingRight[i] = false;
			}	
			//上下
			if (enemyIsMovingUp[i] == false && g_enemyObjs[i].obj->getPos().y < (-WINDOW_HEIGHT / 2 + 100.0f + 64.0f))
			{
				enemyIsMovingUp[i] = true;
			}
			else if (enemyIsMovingUp[i] == true && g_enemyObjs[i].obj->getPos().y > (WINDOW_HEIGHT / 2 - 100.0f - 64.0f))
			{
				enemyIsMovingUp[i] = false;
			}

			if (enemyIsMovingRight[i] == false && enemyIsMovingUp[i] == false)
			{
				g_enemyObjs[i].obj->setPos(g_enemyObjs[i].obj->getPos().x - g_enemyMoveSpeed,
					g_enemyObjs[i].obj->getPos().y - (g_enemyMoveSpeed + 2.0));
			}
			else if(enemyIsMovingRight[i] == true && enemyIsMovingUp[i] == true)
			{
				g_enemyObjs[i].obj->setPos(g_enemyObjs[i].obj->getPos().x + g_enemyMoveSpeed,
					g_enemyObjs[i].obj->getPos().y + (g_enemyMoveSpeed + 2.0));
			}
			else if (enemyIsMovingRight[i] == false && enemyIsMovingUp[i] == true)
			{
				g_enemyObjs[i].obj->setPos(g_enemyObjs[i].obj->getPos().x - g_enemyMoveSpeed,
					g_enemyObjs[i].obj->getPos().y + (g_enemyMoveSpeed + 2.0));
			}
			else if (enemyIsMovingRight[i] == true && enemyIsMovingUp[i] == false)
			{
				g_enemyObjs[i].obj->setPos(g_enemyObjs[i].obj->getPos().x + g_enemyMoveSpeed,
					g_enemyObjs[i].obj->getPos().y - (g_enemyMoveSpeed + 2.0));
			}

			

			
			//敵の向き
			for (int i = 0; i < ENEMY_OBJECT_COUNT; ++i)
			{
				if (g_pPlayer != NULL && g_enemyObjs[i].obj != NULL)
				{
					double enemyFacingDirectionRad = atan2((g_pPlayer->getPos().y - g_enemyObjs[i].obj->getPos().y),
						(g_pPlayer->getPos().x - g_enemyObjs[i].obj->getPos().x));
					double enemyFacingDirectionDeg = enemyFacingDirectionRad * 180.0 / pi;
					g_enemyObjs[i].obj->setAngle(XMFLOAT3(0.0f, 0.0f, enemyFacingDirectionDeg * 1.0f - 90.0f));
				}
			}
			
		
			
			//プレイヤーとの当たり判定をするにあたって、
			//プレイヤー画像が存在するかどうかをチェック
			if (g_pPlayer != NULL)
			{
				//プレイヤーとの衝突判定
				if (g_pPlayer->collision(g_enemyObjs[i].obj) != false)
				{

					//プレイヤーが死ぬときのエフェクトの画像を作成する
					g_pPlayerDeath = createSprite(Tex_PlayerDeath, PLAYER_SIZE_X, PLAYER_SIZE_Y);
					//プレイヤーが死ぬときのエフェクトをプレイヤーの位置と同じ座標に設定する
					g_pPlayerDeath->setPos(g_pPlayer->getPos());
					//プレイヤーが死ぬときマウスの座標を取る
					playerDeathMousePos.x = m_pDirectInput->getMousePos().x;
					playerDeathMousePos.y = m_pDirectInput->getMousePos().y;

					//プレイヤーの向き(degree)
					double playerDeathDirectionRad = atan2((playerDeathMousePos.y - g_pPlayer->getPos().y),
						(playerDeathMousePos.x - g_pPlayer->getPos().x));
					double playerDeathDirectionDeg = playerDeathDirectionRad * 180.0 / pi;
					g_pPlayerDeath->setAngle(XMFLOAT3(0.0f, 0.0f, playerDeathDirectionDeg * 1.0f - 90.0f));
					//死亡フラグを立てる
					g_isPlayerDead = true;
					
					//プレイヤーを削除
					disposeSprite(g_pPlayer);
					g_pPlayer = NULL;
				}
			}
			
			//プレイヤーの弾と衝突したとき
			for (int j = 0; j < noOfBullet; ++j)
			{
				if (g_pPlayerBullet[j] != NULL && g_enemyObjs[i].obj != NULL)
				{
					if (g_pPlayerBullet[j]->collision(g_enemyObjs[i].obj) != false)
					{
						//プレイヤーの弾を破棄する
						disposeSprite(g_pPlayerBullet[j]);
						g_pPlayerBullet[j] = NULL;
						if (g_enemyObjs[i].Hp > 0)
						{
							//敵のHPを減る
							g_enemyObjs[i].Hp -= 20;
						}
						else if(g_enemyObjs[i].Hp <= 0)
						{
							//敵を破棄する
							disposeSprite(g_enemyObjs[i].obj);
							g_enemyObjs[i].obj = NULL;
							//死んだ敵の数をカウントする
							++deadEnemy;
						}	
					}	
				}	
			}
		}
		if (deadEnemy == ENEMY_OBJECT_COUNT && g_enemyObjs[0].gotItem != true && stageNo != 3)
		{
			//アイテムを出現させる
			g_enemyObjs[0].speedUp = createSprite(Tex_SpeedUp, SPEEDUP_SIZE_X, SPEEDUP_SIZE_Y);
		}
		if (g_enemyObjs[0].speedUp != NULL)
		{
			//アイテムを敵の位置と同じ座標に設定する
			g_enemyObjs[0].speedUp->setPos(0.0f, 0.0f);
			//アイテムを描画する
			renderSprite(g_enemyObjs[0].speedUp);
			//プレイヤーとアイテムが衝突したとき、アイテムを消えて、プレイヤーの移動速度を上げる
			if (g_pPlayer != NULL)
			{
				if (g_enemyObjs[0].speedUp->collision(g_pPlayer) == true)
				{
					g_playerMoveSpeed++;
					g_enemyObjs[0].gotItem = true;
					//deadEnemy = 0;
					//アイテムの画像を破棄する
					disposeSprite(g_enemyObjs[0].speedUp);
					g_enemyObjs[0].speedUp = NULL;
				}
			}
		}
	}
}

/*
* 関数名：procPlayMain_procPlayerBullet()
* 処理内容：プレイヤーの弾の処理
* 引数：なし
* 戻り値：なし
*/
void CActionGameApp::procPlayMain_procPlayerBullet()
{
	//プレイヤーの弾のオブジェクトが存在しているときだけ処理を行う
	for (int i = 0; i < noOfBullet; ++i)
	{
		if (g_pPlayerBullet[i] != NULL && g_pPlayer != NULL)
		{
			//プレイヤーの弾の方向(degree)
			double bulletDirectionRad = atan2((shotPos[i].y - playerPosShot[i].y), (shotPos[i].x - playerPosShot[i].x));
			double bulletDirectionDeg = bulletDirectionRad * 180.0 / pi ;
			//移動
			XMFLOAT4 playerBulletPos = g_pPlayerBullet[i]->getPos();
			playerBulletPos.x += dMaxRange * cos(bulletDirectionRad) * 1.0f / 250.0f;
			playerBulletPos.y += dMaxRange * sin(bulletDirectionRad) * 1.0f / 250.0f;
			g_pPlayerBullet[i]->setPos(playerBulletPos);
			g_pPlayerBullet[i]->setAngle(XMFLOAT3(0.0f, 0.0f , bulletDirectionDeg * 1.0f));
	
			//弾が最大範囲に超えたらオブジェクトを破棄する
			if (playerBulletPos.x > WINDOW_WIDTH / 2.0f + PLAYER_BULLET_SIZE_X || playerBulletPos.x < -WINDOW_WIDTH / 2.0f - PLAYER_BULLET_SIZE_X)
			{
				disposeSprite(g_pPlayerBullet[i]);
				g_pPlayerBullet[i] = NULL;
			}
			else if (playerBulletPos.y > WINDOW_HEIGHT / 2.0f + PLAYER_BULLET_SIZE_Y || playerBulletPos.y < -WINDOW_HEIGHT / 2.0f - PLAYER_BULLET_SIZE_Y)
			{
				disposeSprite(g_pPlayerBullet[i]);
				g_pPlayerBullet[i] = NULL;
			}
		}	
		if (g_pPlayer == NULL && g_pPlayerBullet != NULL)
		{
			disposeSprite(g_pPlayerBullet[i]);
			g_pPlayerBullet[i] = NULL;
		}
	}
}



/*
* 関数名：procPlayEnd()
* 処理内容：ゲーム画面の終了時に行う処理
* 引数：なし
* 戻り値：なし
*/
void CActionGameApp::procPlayEnd()
{
	//g_pPlayerDeath が作成されているときのみ破棄する
	if (g_pPlayerDeath != NULL)
	{
		disposeSprite(g_pPlayerDeath);
		g_pPlayerDeath = NULL;
	}

	//敵画像を破棄する
	for (int i = 0; i < ENEMY_OBJECT_COUNT; ++i)
	{
		//それぞれの obj が作成されている場合のみ破棄する
		if (g_enemyObjs[i].obj != NULL)
		{
			disposeSprite(g_enemyObjs[i].obj);
			g_enemyObjs[i].obj = NULL;
		}
		//それぞれのspeedUp が作成されている場合のみ破棄する
		if (g_enemyObjs[i].speedUp != NULL)
		{
			disposeSprite(g_enemyObjs[i].speedUp);
			g_enemyObjs[i].speedUp = NULL;
		}
		//それぞれの敵の弾が作成されている場合のみ破棄する
		for (int j = 0; j < noOfEnemyBullet; ++j)
		{
			if (g_enemyObjs[i].Shot[j] != NULL)
			{
				disposeSprite(g_enemyObjs[i].Shot[j]);
				g_enemyObjs[i].Shot[j] = NULL;
			}
		}
	}
	//g_pPlayer が作成されている場合のみ破棄する
	if (g_pPlayer != NULL)
	{
		disposeSprite(g_pPlayer);
		g_pPlayer = NULL;
	}
	//g_pPlayerBullet が作成されている場合のみ破棄する
	for (int i = 0; i < noOfBullet; ++i)
	{
		if (g_pPlayerBullet[i] != NULL)
		{
			disposeSprite(g_pPlayerBullet[i]);
			g_pPlayerBullet[i] = NULL;
		}
	}
	
	

	//g_pBackgroundが作成されている場合のみ破棄する
	if (g_pBackground != NULL)
	{
		//背景画像を破棄する
		disposeSprite(g_pBackground);
		g_pBackground = NULL;
	}

	//ステージ画面が存在している場合のみ破棄する
	//if (g_pStage1Start != NULL)
	//{
	//	disposeSprite(g_pStage1Start);
	//	g_pStage1Start = NULL;
	//}
	if (g_pStage2Start != NULL)
	{
		disposeSprite(g_pStage2Start);
		g_pStage2Start = NULL;
	}
	if (g_pStage3Start != NULL)
	{
		disposeSprite(g_pStage3Start);
		g_pStage3Start = NULL;
	}
}