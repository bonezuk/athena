
// PCM-DSD_Converter.h : PROJECT_NAME �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C���ł��B
//

#pragma once

#ifndef __AFXWIN_H__
#error "PCH �ɑ΂��Ă��̃t�@�C�����C���N���[�h����O�� 'stdafx.h' ���C���N���[�h���Ă�������"
#endif

#include "resource.h"		// ���C�� �V���{��


// CPCMDSD_ConverterApp:
// ���̃N���X�̎����ɂ��ẮAPCM-DSD_Converter.cpp ���Q�Ƃ��Ă��������B
//

class CPCMDSD_ConverterApp : public CWinApp
{
public:
	CPCMDSD_ConverterApp();

	// �I�[�o�[���C�h
public:
	virtual BOOL InitInstance();

	// ����

	DECLARE_MESSAGE_MAP()
};

extern CPCMDSD_ConverterApp theApp;