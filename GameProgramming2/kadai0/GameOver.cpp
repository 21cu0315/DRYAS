/*
* 作業内容：ゲームフローを完成
* 作成日：2021/11/15
*/

#include "CActionGameApp.h"
#include "GameResource.h"

//変数定義
//ゲームオーバー画面のフェーズを管理する変数
ePhase g_GameOverPhase = eBegin;
//ゲームオーバー画面の画像を管理する変数
CPicture* g_pGameOver = NULL;
//ゲームオーバー画面から次へ進むときのフローを管理する変数
eGameStatus g_NextStatusFromGameOver = eNone;

/*
 * ゲームオーバー画面の処理を行う部分
 */
void CActionGameApp::procGameOver()
{
	//フェーズの進行状態によってしょりを切り替える
	switch (g_GameOverPhase)
	{
	case eBegin:
	{
		//ゲームオーバー画面の開始処理
		procOverBegin();
		//進行状態をメインに切り替える
		g_GameOverPhase = eMain;
	}
	break;
	//メイン
	case eMain:
	{
		//ゲームオーバー画面のメイン処理
		procOverMain();
	}
	break;
	//終了時
	case eEnd:
	{
		//ゲームオーバー画面の終了処理
		procOverEnd();
		//再度クリア画面に来た時に開始から処理を行うためにフェーズを戻しておく
		g_GameOverPhase = eBegin;
		//次のステータスを設定する
		SetNextGameStatus(g_NextStatusFromGameOver);
	}
	break;
	}
}

/*
* 関数名：procOverBegin()
* 処理内容：ゲームオーバー画面の開始時に行う処理
* 引数：なし
* 戻り値：なし
*/
void CActionGameApp::procOverBegin()
{
	//g_pGameOver が NULL の時のみ画像を作成する
	if (g_pGameOver == NULL)
	{
		//ゲームオーバー画面の画像を作成する
		g_pGameOver = createSprite(Tex_Over);
	}
}

/*
* 関数名：procOverMain()
* 処理内容：ゲーム画面のメイン処理
* 引数：なし
* 戻り値：なし
*/
void CActionGameApp::procOverMain()
{
	//Xが入力された時はタイトルに移行
	if (getInput()->isPressedOnce(DIK_X))
	{
		//タイトルにステータスを移行する準備をする
		g_GameOverPhase = eEnd;
		g_NextStatusFromGameOver = eOpening;
	}
	//ゲームオーバー画面の画像が作成されていたら描画する
	if (g_pGameOver != NULL)
	{
		renderSprite(g_pGameOver);
	}
}

/*
* 関数名：procOverEnd()
* 処理内容：ゲーム画面の終了時に行う処理
* 引数：なし
* 戻り値：なし
*/
void CActionGameApp::procOverEnd()
{
	//g_pGameOver が作成されている場合のみ破棄する
	if (g_pGameOver != NULL)
	{
		//ゲームオーバー画面の画像を破棄する
		disposeSprite(g_pGameOver);
		g_pGameOver = NULL;
	}
}