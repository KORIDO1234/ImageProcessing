// Constant2Dlg.cpp: 구현 파일
//

#include "pch.h"
#include "ColorImageAlpha1.h"
#include "afxdialogex.h"
#include "Constant2Dlg.h"


// CConstant2Dlg 대화 상자

IMPLEMENT_DYNAMIC(CConstant2Dlg, CDialogEx)

CConstant2Dlg::CConstant2Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CONSTANT2, pParent)
	, m_constant2(0)
{

}

CConstant2Dlg::~CConstant2Dlg()
{
}

void CConstant2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_CONSTANT2, m_constant2);
	DDV_MinMaxDouble(pDX, m_constant2, -1.00, 1.00);
}


BEGIN_MESSAGE_MAP(CConstant2Dlg, CDialogEx)
	ON_EN_CHANGE(IDC_CONSTANT2, &CConstant2Dlg::OnEnChangeConstant2)
END_MESSAGE_MAP()


// CConstant2Dlg 메시지 처리기


void CConstant2Dlg::OnEnChangeConstant2()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
