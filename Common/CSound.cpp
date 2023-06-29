/**
* ファイル名	：CSound.cpp
* 概要		：サウンドの作成や再生を行う
* 作成日		：2021_08_18_細田弥卯
*/

#include "stdafx.h"


#include "CSound.h"

CSound::CSound()
	: m_pXaudio2(NULL)
	, m_pSourcevoice(NULL)
	, m_iVolume(100)
	, m_pBuffer(NULL)
	, m_dataSize()
{}



CSound::~CSound()
{}




/// <summary>
/// wavファイルから波形データを読み込む
/// </summary>
/// <param name="filePath_">：wavファイルへのパス</param>
/// <param name="loop_">：ループするかどうかのフラグ（デフォルトはfalseなので、ループするときだけtrueを追加で渡す）</param>
void CSound::LoadSound(IXAudio2* pXAudio2_, const char* filePath_, const bool loop_ )
{
	m_pXaudio2 = pXAudio2_;	// メンバ変数として保持

	// オーディオファイルの読み込み
	FILE* pFile = NULL;

	// errno_t：成功なら0が返ってくる
	// 0が返ってこなければ、失敗しているので戻す
	if (fopen_s(&pFile, filePath_, "rb") != 0) { throw; }

	// RIFFヘッダの読み込み
	RIFFHeader riff;
	fread(&riff, sizeof(riff), 1, pFile);

	// Formatチャンクの読み込み
	FormatChunk format;
	fread(&format, sizeof(format), 1, pFile);

	// Dataチャンクの読み込み
	Chunk data;
	fread(&data, sizeof(data), 1, pFile);
	// Dataチャンクのデータ（波形データ）部分を読み込む
	m_pBuffer = (char*)malloc(data.size);
	if (!m_pBuffer) { return; }
	fread(m_pBuffer, data.size, 1, pFile);
	m_dataSize = data.size;
	fclose(pFile);

	WAVEFORMATEX wfex = {};
	// 波形フォーマットの設定
	memcpy(&wfex, &format.fmt, sizeof(format.fmt));
	wfex.wBitsPerSample = format.fmt.nBlockAlign * 8 / format.fmt.nChannels;

	// 波形フォーマットを元にソースボイスの作成
	HRESULT result = m_pXaudio2->CreateSourceVoice(&m_pSourcevoice, &wfex);
	if (FAILED(result))
	{
		free(m_pBuffer);
		return;
	}

	// 再生する波形データの設定
	XAUDIO2_BUFFER buf = {};
	buf.Flags = XAUDIO2_END_OF_STREAM;
	buf.AudioBytes = m_dataSize;
	buf.pAudioData = (BYTE*)m_pBuffer;
	// ループがtrueなら、無限にループする設定をつけくわえる
	// IXAudio2SourceVoice::ExitLoopを呼び出すまでは再生され続ける
	if (loop_ == true)
	{
		buf.LoopBegin = 0;
		buf.LoopLength = 0;
		buf.LoopCount = XAUDIO2_LOOP_INFINITE;
	}

	// 設定した波形データのセット
	m_pSourcevoice->SubmitSourceBuffer(&buf);
}



/// <summary>
/// この音声を再生する
/// </summary>
void CSound::PlaySourcevoice()
{
	m_pSourcevoice->Start();
}



/// <summary>
/// この音声の再生を止める
/// </summary>
void CSound::StopSourcevoice()
{
	m_pSourcevoice->Stop();
}



/// <summary>
/// この音声が再生中かどうか
/// </summary>
/// <returns>再生中ならture、再生中でなければfalse</returns>
bool CSound::isPlayed()
{
	if (!m_pSourcevoice) { return false; }

	XAUDIO2_VOICE_STATE state;
	m_pSourcevoice->GetState(&state); 
	// BuffersQueued：再生が停止されているなら次に処理予定のバッファへのポインタが入る
	// 次に処理予定のバッファがなければNULL
	// NULLなら再生が停止していることを示す
	if (!state.BuffersQueued)	
	{
		return false;
	}

	return true;
}



/// <summary>
/// サウンドの破棄
/// </summary>
void CSound::DeleteSourcevoice()
{
	if (!m_pSourcevoice) { return; }
	m_pSourcevoice->Stop();
	free(m_pBuffer);
	m_pSourcevoice->FlushSourceBuffers();
	m_pSourcevoice->DestroyVoice();
	m_pSourcevoice = NULL;
}




void CSound::ChangeVolume(const __int8 vol_)
{
	// 音量設定の最大値
	constexpr float MAX_VOLUME = 100.f;
	m_iVolume = vol_;
	// 0.f～1.fで渡す必要がるので、割り算をする
	m_pSourcevoice->SetVolume(static_cast<float>(vol_) / MAX_VOLUME);
}



/// <summary>
/// 同じ音を再度再生したいときに使う
/// </summary>
void CSound::ReLoadSource()
{
	if (!m_pSourcevoice) { return; }
	if (!m_pBuffer) { return; }
	// もし再生中なら停止
	m_pSourcevoice->Stop();
	// ソースボイスのバッファを綺麗に
	m_pSourcevoice->FlushSourceBuffers();

	// ソースボイスに再度保存されているバッファからデータをセットする
	// 再生する波形データの設定
	XAUDIO2_BUFFER buf = {};
	buf.Flags = XAUDIO2_END_OF_STREAM;
	buf.AudioBytes = m_dataSize;
	buf.pAudioData = (BYTE*)m_pBuffer;

	// 設定した波形データのセット
	m_pSourcevoice->SubmitSourceBuffer(&buf);
}





CSound* CSound::Create(IXAudio2* pXAudio2_, const char* filePath_, const bool loop_)
{

	CSound* object = new CSound();
	if (object != NULL)
	{
		object->LoadSound(pXAudio2_, filePath_, loop_);
	}
	return object;
}