#include "stdafx.h"
#include "CActionGameApp.h"
#include "GameResource.h"
#include "UV.h"

/**
 * �֐��� : createSprite()
 * �������e : �w�肳�ꂽ�p�X�ɂ���摜���A�w�肳�ꂽ�T�C�Y�ō쐬����
 * ���� :	path_		�摜�̂���p�X
 *			width_		�쐬����摜�̕�
 *			height_		�쐬����摜�̍���
 * �߂�l : �쐬�����摜���Ǘ�����I�u�W�F�N�g
 */
CPicture* CActionGameApp::createSprite (LPCWSTR path_, float width_, float height_)
{
	vector <QuadrangleTexel>  singleTexels = {
	QuadrangleTexel{ { 0, 1.0f },{ 0, 0.0f },{ 1, 1.0f },{ 1, 0.0f } }	// �w�肷��UV���W����̔z��
	};
	XMFLOAT2 spriteSize = { width_, height_};
	CPicture* object = NULL;

	//PlayerDeath�̏ꍇ�A�w�肷��UV���W�𕡐��z��p��
	if (path_ == Tex_PlayerDeath)
	{
		object = CPicture::create(m_pDevice, m_pDeviceContext, m_viewPort, HLSL_AlphaHLSL, path_,
			{ 0.0f, 0.0f, 0.0f, 0.0f },	// �X�v���C�g�̈ʒu
			spriteSize,					// �X�v���C�g�̃T�C�Y
			kTexelPlayerDeath);				// �w�肷��UV���W�̔z��
	}
	//EnemyDeath�̏ꍇ�A�w�肷��UV���W�𕡐��z��p��
	else if (path_ == Tex_EnemyDeath)
	{
		object = CPicture::create(m_pDevice, m_pDeviceContext, m_viewPort, HLSL_AlphaHLSL, path_,
			{ 0.0f, 0.0f, 0.0f, 0.0f },	// �X�v���C�g�̈ʒu
			spriteSize,					// �X�v���C�g�̃T�C�Y
			kTexelEnemyDeath);				// �w�肷��UV���W�̔z��
	}
	else if (path_ == Tex_Player)
	{
		object = CPicture::create(m_pDevice, m_pDeviceContext, m_viewPort, HLSL_AlphaHLSL, path_,
			{ 0.0f, 0.0f, 0.0f, 0.0f },	// �X�v���C�g�̈ʒu
			spriteSize,					// �X�v���C�g�̃T�C�Y
			kTexelPlayer);				// �w�肷��UV���W�̔z��
	}
	else
	{
		object = CPicture::create(m_pDevice, m_pDeviceContext, m_viewPort, HLSL_AlphaHLSL, path_,
			{ 0.0f, 0.0f, 0.0f, 0.0f },	// �X�v���C�g�̈ʒu
			spriteSize,					// �X�v���C�g�̃T�C�Y
			singleTexels);				// �w�肷��UV���W�̔z��
	}
	return object;
}

CFont* CActionGameApp::createFont(LPCWSTR path_, int digits_, float width_, float height_)
{
	XMFLOAT2 spriteSize = { width_, height_ };
	CFont* objects = NULL;

	return objects;
}
