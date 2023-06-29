/*
* 作業内容：ゲームフローを完成
* 作成日：2021/11/15
*/

#include "CActionGameApp.h"
#include "GameResource.h"

//変数定義
//タイトル画面のフェーズを管理する変数
ePhase g_TitlePhase = eBegin;
//タイトル画面の画像を管理する変数
CPicture* g_pTitle = NULL;
//タイトルから次へ進むときのフローを管理する変数
eGameStatus g_NextStatusFromTitle = eNone;
// サウンド
CSound* g_pSound = NULL;

/*
* タイトルの処理を行う部分
*/
void CActionGameApp::procTitle()
{

#if 1
	// ---------------------------------------------------
	// サウンド再生テストコード	
	// 必要に応じて活用してください。

	// 1が押されたら生成
	if (m_pDirectInput->isKeyPressed(DIK_1))
	{
		// 作成されていなければ、作成
		if (!g_pSound)
		{
			// 第2引数：ループ設定
			// ループしないので、falseを設定
			g_pSound = createSound(Snd_TEST, false);
		}
	}

	// 作成されていたら再生
	if (g_pSound)
	{
		PlaySourceSound(g_pSound);
	}

	// ボリューム変更（0～100）
	//if (g_pSound)
	//{
	//	g_pSound->ChangeVolume(50);
	//}

	// 2が押されたら再生しなおす
	if (m_pDirectInput->isKeyPressed(DIK_2))
	{
		// 作成されていなければ、作成
		if (g_pSound)
		{			
			g_pSound->ReLoadSource();
		}
	}

	// 0が押されたら強制終了
	if (m_pDirectInput->isKeyPressed(DIK_0))
	{
		// 作成されていたら、削除
		if (g_pSound)
		{
			disposeSound(g_pSound);
			g_pSound = NULL;
		}
	}

	// 再生し終わったら削除
	//（ループ設定trueの場合は入らない）
	if (g_pSound && (g_pSound->isPlayed() == false))
	{
		disposeSound(g_pSound);
		g_pSound = NULL;
	}
	// ---------------------------------------------------
#endif


	//フェーズの進行状態によって処理を切り替える
	switch (g_TitlePhase)
	{
		//開始時
	case eBegin:
	{
		//タイトル画面の開始時の処理を行う
		procTitleBegin();
		//進行状態をメインに切り替える
		g_TitlePhase = eMain;
	}
	break;
	//メイン
	case eMain:
	{
		//タイトル画面のメイン処理を行う
		procTitleMain();
	}
	break;
	//終了時
	case eEnd:
	{
		//タイトル画面の終了処理を行う
		procTitleEnd();
		//再度タイトル画面に来た時に開始から処理を行うために
		//フェーズを戻しておく
		g_TitlePhase = eBegin;
		//次のステータスを設定する
		SetNextGameStatus(g_NextStatusFromTitle);
	}
	break;
	}
}

/*
* 関数名：procTitleBegin()
* 処理内容：タイトル画面の開始時に行う処理
* 引数：なし
* 戻り値：なし
*/
void CActionGameApp::procTitleBegin()
{
	//g_pTitle が NULLの時のみ画像を作成する
	if (g_pTitle == NULL)
	{
		//タイトルの画像を作成する
		g_pTitle = createSprite(Tex_Title);
	}
}

/*
* 関数名：procTitleMain()
* 処理内容：タイトル画面のメイン処理
* 引数：なし
* 戻り値：なし
*/
void CActionGameApp::procTitleMain()
{
	//Xキーを押されたらフェーズを終了に変更し、
	//ゲーム本編にステータスを移行する準備をする
	if (getInput()->isPressedOnce(DIK_X))
	{
		g_TitlePhase = eEnd;
		g_NextStatusFromTitle = ePlaying;
	}
	//画像が作成されていたら描画する
	if (g_pTitle != NULL)
	{
		renderSprite(g_pTitle);
	}
}

/*
* 関数名：procTitleEnd()
* 処理内容：タイトル画面の終了時に行う処理
* 引数：なし
* 戻り値：なし
*/
void CActionGameApp::procTitleEnd()
{
	//g_pTitleが作成されている時のみ破棄する
	if (g_pTitle != NULL)
	{
		//タイトル画面の画像を破棄する
		disposeSprite(g_pTitle);
		g_pTitle = NULL;
	}
}