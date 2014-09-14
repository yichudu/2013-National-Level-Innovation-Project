
// mfc_timerDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// Cmfc_timerDlg �Ի���
class Cmfc_timerDlg : public CDialogEx
{
// ����
public:
	Cmfc_timerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MFC_TIMER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	void OnTimer(UINT_PTR nIDEvent);
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	int m_slider1;
	CSliderCtrl m_slider1_ctr;
	int system_volume;
	CSliderCtrl m_slider2_ctr;
	int m_slider2;
	CSliderCtrl m_slider3_ctr;
	int m_slider3;
	afx_msg void OnNMCustomdrawSlider3(NMHDR *pNMHDR, LRESULT *pResult);

	int current_amplitude;
	int critical_amplitude;
	afx_msg void OnBnClickedButton1();
	CButton m_button1_ctr;
	CButton m_button2_ctr;
	afx_msg void OnBnClickedButton2();

	bool timer_enable_flag;
};
