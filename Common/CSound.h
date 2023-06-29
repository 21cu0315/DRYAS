/**
* ファイル名	：CSound.h
* 概要		：サウンドの作成や再生を行う
* 作成日		：2021_08_18_細田弥卯
*/

#pragma once


//using Microsoft::WRL::ComPtr;

class CSound
{
	IXAudio2* m_pXaudio2;	// CXAudio2クラスのm_pXaudio2のポインタを取得する

	IXAudio2SourceVoice* m_pSourcevoice;	// ソースボイス
	__int8 m_iVolume;	// この音声の音量：０～１００で設定

	char* m_pBuffer;	// 波形データのバッファ

	// チャンクデータの基本構造
	struct Chunk
	{
		char id[4];	// チャンク毎のID
		uint32_t size;	// チャンクサイズ
	};
	// RIFFヘッダ
	struct RIFFHeader
	{
		Chunk chunk;	// "RIFF"
		char type[4];	// "WAVE"：WAVEファイルの読み込みをするのでWAVEが入る
	};
	// FMTチャンク
	struct FormatChunk
	{
		Chunk chunk;	// "fmt"
		WAVEFORMAT fmt;	// 波形フォーマット
	};

	uint32_t m_dataSize;	// チャンクサイズ
public:
	CSound();
	~CSound();

	void LoadSound(IXAudio2* pXAudio2_, const char* filePath_, const bool loop_ = false);
	void PlaySourcevoice();
	void StopSourcevoice();
	void DeleteSourcevoice();


	void ExitLoopSource() { m_pSourcevoice->ExitLoop(); }	// ループ音声のとき、現在の再生が終わったらループ再生を停止する
	void ReLoadSource();	// 再度同じ音を流したいとき用の関数

	bool isPlayed();	// この音声が再生中かどうか
	IXAudio2SourceVoice* getSourcevoice()const { if (m_pSourcevoice) { return m_pSourcevoice; } else { return NULL; } }

	void ChangeVolume(const __int8 vol_);	// 音量の変更
	void setVolumeMute() { m_pSourcevoice->SetVolume(0.f); }	// この音声のボリュームを0にする

	static CSound* Create(IXAudio2* pXAudio2_, const char* filePath_, const bool loop_ = false);
};