// CConstantDLG.cpp: 구현 파일
//

#include "pch.h"
#include "ColorImageAlpha1.h"
#include "afxdialogex.h"
#include "ConstantDLG.h"


// CConstantDLG 대화 상자

IMPLEMENT_DYNAMIC(CConstantDlg, CDialog)

CConstantDlg::CConstantDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_CONSTANT, pParent)
	, m_constant(0)
{

}

CConstantDlg::~CConstantDlg()
{
}

void CConstantDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_CONSTANT, m_constant);
	DDV_MinMaxDouble(pDX, m_constant, -255, 255);
}


BEGIN_MESSAGE_MAP(CConstantDlg, CDialog)
	ON_EN_CHANGE(IDC_EDIT_CONSTANT, &CConstantDlg::OnEnChangeEditConstant)
END_MESSAGE_MAP()


// CConstantDLG 메시지 처리기
