
// PCM-DSD_ConverterDlg.h : �w�b�_�[ �t�@�C��
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include <string>
#include"ProgressDlg.h"
#include "fftw3.h"
#include <omp.h>
#include <fstream>
#include <complex>
#define _USE_MATH_DEFINES
#include <math.h>
#include <locale.h>

#include <malloc.h>
#include <stdlib.h>

using namespace std;

// CPCMDSD_ConverterDlg �_�C�A���O
class CPCMDSD_ConverterDlg : public CDialogEx
{
	// �R���X�g���N�V����
public:
	CPCMDSD_ConverterDlg(CWnd* pParent = NULL);	// �W���R���X�g���N�^�[
	string flag_Bottun = "";
	// �_�C�A���O �f�[�^
	enum { IDD = IDD_PCMDSD_CONVERTER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �T�|�[�g

private:
	ProgressDlg m_dProgress;
	static UINT __cdecl WorkThread(LPVOID pParam);
	// ����
protected:
	HICON m_hIcon;
	// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
	virtual BOOL OnInitDialog();
	virtual void ListInit();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnCancel();
	CMFCListCtrl m_lFileList;
	CButton m_bAllRun;
	CButton m_bAllListDelete;
	CButton m_bRun;
	CButton m_bListDelete;
	CStatic m_scPrecision;
	CComboBox m_ccPrecision;
	CStatic m_scPath;
	CEdit m_ecPath;
	CButton m_bcPath;
	CComboBox m_cSamplingRate;
	CStatic m_sSamplingRate;
	CString m_evPath;
	//�S�Ď��s
	afx_msg void OnBnClickedAllrun();
	//�S�č폜
	afx_msg void OnBnClickedAlllistdelete();
	//���s
	afx_msg void OnBnClickedRun();
	//�폜
	afx_msg void OnBnClickedListdelete();
	//�Q��
	afx_msg void OnBnClickedPathcheck();
	//�t�@�C��/�f�B���N�g�����h���b�v&�h���b�O
	afx_msg void OnDropFiles(HDROP hDropInfo);
	//����
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	//�T�C�Y�ύX
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//Wav�t�@�C�����h���b�v���ꂽ���̏���
	afx_msg void WAV_FileRead(TCHAR *FileName);
	//�f�B���N�g���̍ċA�I����
	afx_msg void DirectoryFind(TCHAR *DirectoryPath);
	//Wav�t�@�C���`�F�b�N�y�у��^�f�[�^�ǂݎ��
	afx_msg bool WAV_Metadata(TCHAR *filepath, wstring *metadata);
	//PCM-DSD�ϊ��̊Ǘ�
	afx_msg bool WAV_Convert(TCHAR *filepath, int number);
	//DSDIFF�`���ŏ�������
	afx_msg bool DSD_Write(FILE *LData, FILE *RData, FILE *WriteData, int number);
	//�ǂݏ����f�[�^����
	afx_msg bool RequireWriteData(TCHAR *filepath, CString flag, wchar_t *FileMode, FILE **WriteDatadsd);
	//�ꎞ�t�@�C���폜
	afx_msg bool TrushFile(TCHAR *filepath, CString flag);
	//Wav�t�@�C����64bitFloat(double)�����ALR�������Ĉꎞ�t�@�C���Ƃ��ď����o��
	afx_msg bool TmpWriteData(TCHAR *filepath, FILE *tmpl, FILE *tmpr, int Times);
	//PCM-DSD�ϊ�
	afx_msg bool WAV_Filter_Renew(FILE *UpSampleData, FILE *OrigData, unsigned int Times, omp_lock_t *myLock, unsigned int DSDsamplingRate);
	afx_msg bool WAV_FilterLight_Renew(FILE *UpSampleData, FILE *OrigData, unsigned int Times);
	//�t���[�Y�h�~�̂��߂ɃX���b�h�쐬
	void WorkThread();
	//�������̓{�^���Ȃǖ�����
	void Disable();
	//�������I�������{�^���ȂǗL����
	void Enable();
	//F1�w���v������
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//FFT�v����������
	void FFTInit(fftw_plan *plan, unsigned int fftsize, int Times, double *fftin, fftw_complex *ifftout);
	void iFFTInit(fftw_plan *plan, unsigned int fftsize, int Times, fftw_complex *ifftin, double *fftout);
	void PathCheck();
};
