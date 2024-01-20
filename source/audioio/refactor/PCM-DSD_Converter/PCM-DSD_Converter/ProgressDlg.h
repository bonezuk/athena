#pragma once
#include "afxwin.h"
#include "afxcmn.h"

// ProgressDlg �_�C�A���O

class ProgressDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ProgressDlg)

public:
	ProgressDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~ProgressDlg();

	// �_�C�A���O �f�[�^
	enum { IDD = IDD_PROGRESS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnOK();
	virtual void OnCancel();
	virtual void PostNcDestroy();
	void Start(TCHAR *Path);
	void Process(unsigned int percent, unsigned int position);
	CProgressCtrl m_pProgress;
	CString m_evTEXT;
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	CEdit m_ecTEXT;
	virtual void OnSetFocus();
	CEdit m_ecTimes;
	afx_msg void OnBnClickedCancelbottun();
	bool Cancelbottun = true;
};
