// ControlDialog.cpp : implementation file
//

#include "stdafx.h"
#include "axAnxOggPlayer.h"
#include "ControlDialog.h"
#include ".\controldialog.h"


// CControlDialog dialog

IMPLEMENT_DYNAMIC(CControlDialog, CDialog)
CControlDialog::CControlDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CControlDialog::IDD, pParent)
	, mBrowser(NULL)
{
	debugLog.open("G:\\logs\\axanx.log", ios_base::out);
}

CControlDialog::~CControlDialog()
{
	debugLog.close();
	if (mBrowser != NULL) {
		mBrowser->Release();
	}
}

void CControlDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CControlDialog, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
END_MESSAGE_MAP()


// CControlDialog message handlers

void CControlDialog::OnBnClickedButton1()
{
	//PLay button.
	debugLog<<"Play button pushed"<<endl;

}

void CControlDialog::setBrowser(IWebBrowser2* inBrowser) {
	debugLog<<"Setting browser instance to "<<(int)inBrowser<<endl;
	mBrowser = inBrowser;

}

void CControlDialog::OnBnClickedButton2()
{
	//Pause button
	debugLog<<"Pause button pushed..."<<endl;
}

void CControlDialog::OnBnClickedButton3()
{
	//Stop button
	debugLog<<"Stop button pushed.."<<endl;
}
