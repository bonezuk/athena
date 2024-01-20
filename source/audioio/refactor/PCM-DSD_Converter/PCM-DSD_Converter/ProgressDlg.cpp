// ProgressDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "PCM-DSD_Converter.h"
#include "ProgressDlg.h"
#include "afxdialogex.h"

// ProgressDlg �_�C�A���O

IMPLEMENT_DYNAMIC(ProgressDlg, CDialogEx)

ProgressDlg::ProgressDlg(CWnd* pParent /*=NULL*/)
: CDialogEx(ProgressDlg::IDD, pParent)
, m_evTEXT(_T(""))
{

}

void ProgressDlg::OnSetFocus() {
	::DestroyCaret();
}

ProgressDlg::~ProgressDlg()
{
}
//�q�_�C�A���O�ł̑���͖�������
void ProgressDlg::OnCancel(){
	//DestroyWindow();
}
void ProgressDlg::OnOK(){
	//DestroyWindow();
}
void ProgressDlg::PostNcDestroy()
{
	//delete this;    //BBB�ǉ�  �_�C�A���O�̔j��
}

void ProgressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, m_pProgress);
	DDX_Text(pDX, IDC_PATHTEXT, m_evTEXT);
	DDX_Control(pDX, IDC_PATHTEXT, m_ecTEXT);
	DDX_Control(pDX, IDC_EDIT2, m_ecTimes);
}

//�q�_�C�A���O�ł̃p�X���\��
void ProgressDlg::Start(TCHAR *Path){
	TCHAR tchstr1[512];
	TCHAR *tchstr2 = L"��DSD�ϊ���";
	lstrcpy(tchstr1, Path);
	lstrcat(tchstr1, tchstr2);
	m_ecTEXT.SetWindowTextW(tchstr1);
}

//�v���O���X�o�[�Ǘ�
void ProgressDlg::Process(unsigned int percent, unsigned int position){
	TCHAR *tchstr;
	m_pProgress.SetRange32(0, position);
	m_pProgress.SetPos(percent);
	if (percent == 0){
		tchstr = L"�A�b�v�T���v�����O�̏�����";
		m_ecTimes.SetWindowTextW(tchstr);
	}
	else if (percent == position){
		tchstr = L"�����o����";
		m_ecTimes.SetWindowTextW(tchstr);

	}
	else{
		tchstr = L"DSD�ϊ���";
		if (percent == 1)m_ecTimes.SetWindowTextW(tchstr);
	}
}

BEGIN_MESSAGE_MAP(ProgressDlg, CDialogEx)
	ON_WM_HELPINFO()
	ON_BN_CLICKED(IDC_CancelBottun, &ProgressDlg::OnBnClickedCancelbottun)
END_MESSAGE_MAP()

//F1�w���v������
BOOL ProgressDlg::OnHelpInfo(HELPINFO* pHelpInfo)
{
	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B

	return true;
}

//���~�{�^���������ꂽ
void ProgressDlg::OnBnClickedCancelbottun()
{
	Cancelbottun = false;
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
}
