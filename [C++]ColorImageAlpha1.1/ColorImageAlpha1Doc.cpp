
// ColorImageAlpha1Doc.cpp: CColorImageAlpha1Doc 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "ColorImageAlpha1.h"
#endif

#include "ColorImageAlpha1Doc.h"

#include <propkey.h>
#include "ConstantDlg.h"
#include "Constant1Dlg.h"
#include "Constant2Dlg.h"
#include "Constant3Dlg.h"
#include <iostream>
#include <cmath>
#include <vector>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CColorImageAlpha1Doc

IMPLEMENT_DYNCREATE(CColorImageAlpha1Doc, CDocument)

BEGIN_MESSAGE_MAP(CColorImageAlpha1Doc, CDocument)
	ON_COMMAND(IDM_EQUAL_IMAGE, &CColorImageAlpha1Doc::OnEqualImage)
END_MESSAGE_MAP()


// CColorImageAlpha1Doc 생성/소멸

CColorImageAlpha1Doc::CColorImageAlpha1Doc() noexcept
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.

}

CColorImageAlpha1Doc::~CColorImageAlpha1Doc()
{
}

BOOL CColorImageAlpha1Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}




// CColorImageAlpha1Doc serialization

void CColorImageAlpha1Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}

#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void CColorImageAlpha1Doc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 문서의 데이터를 그리려면 이 코드를 수정하십시오.
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 검색 처리기를 지원합니다.
void CColorImageAlpha1Doc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CColorImageAlpha1Doc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CColorImageAlpha1Doc 진단

#ifdef _DEBUG
void CColorImageAlpha1Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CColorImageAlpha1Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CColorImageAlpha1Doc 명령


unsigned char** CColorImageAlpha1Doc::OnMalloc2D(int h, int w)
{
	// TODO: 여기에 구현 코드 추가.
	unsigned char** memory;
	memory = new unsigned char* [h];
	for (int i = 0; i < h; i++)
		memory[i] = new unsigned char[w];
	return memory;
}

double** CColorImageAlpha1Doc::OnMalloc2D_double(int h, int w)
{
	double** memory;
	memory = new double* [h];
	for (int i = 0; i < h; i++)
		memory[i] = new double[w];
	return memory;
}

template <typename T>
void CColorImageAlpha1Doc::OnFree2D(T** memory, int h)
{
	if (memory == NULL)
		return;
	for (int i = 0; i < h; i++)
		delete[] memory[i];
	delete[] memory;

}


BOOL CColorImageAlpha1Doc::OnOpenDocument(LPCTSTR lpszPathName)
{
	Acc = 0;
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// 기존 메모리 해제

	if (m_inImageR != NULL) {
		OnFree2D(m_inImageR, m_inH);
		OnFree2D(m_inImageG, m_inH);
		OnFree2D(m_inImageB, m_inH);
		m_inImageR = m_inImageG = m_inImageB = NULL;
		m_inH = m_inW = 0;

		OnFree2D(m_basicImageR, m_inH);
		OnFree2D(m_basicImageG, m_inH);
		OnFree2D(m_basicImageB, m_inH);
		m_basicImageR = m_basicImageG = m_basicImageB = NULL;
		m_basicH = m_basicW = 0;

		OnFree2D(m_outImageR, m_outH);
		OnFree2D(m_outImageG, m_outH);
		OnFree2D(m_outImageB, m_outH);
		m_outImageR = m_outImageG = m_outImageB = NULL;
		m_outH = m_outW = 0;
		
	}

	CImage image;
	image.Load(lpszPathName);
	m_basicH = m_inH = image.GetHeight();
	m_basicW = m_inW = image.GetWidth();

	// 메모리 할당

	m_inImageR = OnMalloc2D(m_inH, m_inW);
	m_inImageG = OnMalloc2D(m_inH, m_inW);
	m_inImageB = OnMalloc2D(m_inH, m_inW);

	m_basicImageR = OnMalloc2D(m_basicH, m_basicW);
	m_basicImageG = OnMalloc2D(m_basicH, m_basicW);
	m_basicImageB = OnMalloc2D(m_basicH, m_basicW);

	// CImage의 객체값 --> 메모리로옮기기
	COLORREF px;
	for (int i =0; i<m_inH; i++)
		for (int k = 0; k < m_inW; k++) {
			px = image.GetPixel(k, i);
			m_basicImageR[i][k] = m_inImageR[i][k] = GetRValue(px);
			m_basicImageG[i][k] = m_inImageG[i][k] = GetGValue(px);
			m_basicImageB[i][k] = m_inImageB[i][k] = GetBValue(px);
		}
	return TRUE;
}


void CColorImageAlpha1Doc::OnCloseDocument()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	OnFree2D(m_inImageR, m_inH);
	OnFree2D(m_inImageG, m_inH);
	OnFree2D(m_inImageB, m_inH);

	OnFree2D(m_outImageR, m_outH);
	OnFree2D(m_outImageG, m_outH);
	OnFree2D(m_outImageB, m_outH);


	CDocument::OnCloseDocument();
}

void CColorImageAlpha1Doc::OnFreeOutImage()
{

	if (m_outImageR != NULL) {
		OnFree2D(m_outImageR, m_outH);
		OnFree2D(m_outImageG, m_outH);
		OnFree2D(m_outImageB, m_outH);
		m_outImageR = m_outImageG = m_outImageB = NULL;
		m_outH = m_outW = 0;
	}
}



BOOL CColorImageAlpha1Doc::OnSaveDocument(LPCTSTR lpszPathName)
{
	if (m_outImageR == NULL) {
		return FALSE;
	}

	CImage image;
	image.Create(m_outW, m_outH, 32);

	unsigned char R, G, B;
	COLORREF px;

	for (int i = 0; i < m_outW; i++) {
		for (int k = 0; k < m_outH; k++) {
			R = m_outImageR[k][i];
			G = m_outImageG[k][i];
			B = m_outImageB[k][i];
			px = RGB(R, G, B);
			image.SetPixel(i, k, px);
		}
	}
	image.Save(lpszPathName, Gdiplus::ImageFormatPNG);
	MessageBox(NULL, L"저장", L"성공", NULL);
		
		return TRUE;
}

double* CColorImageAlpha1Doc::RGB2HSI(int R, int G, int B)
{
	// TODO: 여기에 구현 코드 추가.
	double H, S, I;
	double* HSI = new double[3];
	double min_value, angle;
	I = (R + G + B) / 3.0; // 밝기
	if ((R == G) && (G == B)) { // 그레이
		S = 0.0;
		H = 0.0;
	}
	else {

		min_value = min(min(R, G), B); 
		angle = (R - 0.5 * G - 0.5 * B) / (double)sqrt((R - G) * (R - G) + (R - B) * (G - B));

		H = (double)acos(angle) * 57.29577951;
		S = 1.0f - (3.0 / (R + G + B)) * min_value;
	}
	if (B > G) H = 360. - H;

	HSI[0] = H;
	HSI[1] = S;
	HSI[2] = I;

	return HSI;
}


unsigned char* CColorImageAlpha1Doc::HSI2RGB(double H, double S, double I)
{
	double R, G, B;
	unsigned char* RGB = new unsigned char[3];
	double angle1, angle2, scale;

	if (I == 0.0) { // Black
		RGB[0] = 0;
		RGB[1] = 0;
		RGB[2] = 0;
		return RGB;
	}

	if (H <= 0.0) H += 360.0f;

	scale = 3.0 * I;
	if (H <= 120.0)
	{
		angle1 = H * 0.017453293;
		angle2 = (60.0 - H) * 0.017453293;
		B = (1.0 - S) / 3.0f;
		R = (double)(1.0 + (S * cos(angle1) / cos(angle2))) / 3.0;
		G = 1.0 - R - B;
		B *= scale;
		R *= scale;
		G *= scale;
	}


	else if ((H > 120.0) && (H <= 240.0)) {
		H -= 120.0;
		angle1 = H * 0.017453293;

		angle2 = (60.0 - H) * 0.017453293;
		R = (1.0 - S) / 3.0;
		G = (double)(1.0f + (S * cos(angle1) / cos(angle2))) / 3.0;
		B = 1.0 - R - G;
		R *= scale;
		G *= scale;
		B *= scale;
	}
	else {
		H -= 240.0;
		angle1 = H * 0.017453293;
		angle2 = (60.0 - H) * 0.017453293;
		G = (1.0f - S) / 3.0;
		B = (double)(1.0 + (S * cos(angle1) / cos(angle2))) / 3.0;
		R = 1.0 - G - B;

		R *= scale;
		G *= scale;
		B *= scale;
	}

	RGB[0] = (unsigned char)R;
	RGB[1] = (unsigned char)G;
	RGB[2] = (unsigned char)B;
	return RGB;
}

void CColorImageAlpha1Doc::AccumulateImage() // 이미지 누적
{
	if (Acc == 1) {
		CString message;
		message.Format(_T("변경된 이미지를 누적하시겠습니까?"));
		int result = AfxMessageBox(message, MB_YESNO | MB_ICONQUESTION);
		if (result == IDYES) {
			OnFree2D(m_inImageR, m_inH);
			OnFree2D(m_inImageG, m_inH);
			OnFree2D(m_inImageB, m_inH);
			m_inImageR = m_inImageG = m_inImageB = NULL;
			m_inH = m_outH;
			m_inW = m_outW;
			m_inImageR = OnMalloc2D(m_inH, m_inW);
			m_inImageG = OnMalloc2D(m_inH, m_inW);
			m_inImageB = OnMalloc2D(m_inH, m_inW);
			for (int i = 0; i < m_inH; i++) {
				for (int j = 0; j < m_inW; j++) {
					m_inImageR[i][j] = m_outImageR[i][j];
					m_inImageG[i][j] = m_outImageG[i][j];
					m_inImageB[i][j] = m_outImageB[i][j];
				}
			}
		}
		else {
			AfxMessageBox(_T("변경된 이미지가 누적되지 않았습니다."), MB_OK | MB_ICONINFORMATION);
		}
	}
	Acc = 1;
}

/// <summary>
// 화소점 처리 함수
/// </summary>

void CColorImageAlpha1Doc::OnBasicImage()
{
	OnFreeOutImage();
	m_outH = m_basicH;
	m_outW = m_basicW;
	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);
	for (int i = 0; i < m_basicH; i++)
		for (int k = 0; k < m_basicW; k++) {
			m_outImageR[i][k] = m_basicImageR[i][k];
			m_outImageG[i][k] = m_basicImageG[i][k];
			m_outImageB[i][k] = m_basicImageB[i][k];

		}


}
void CColorImageAlpha1Doc::OnEqualImage()
{
	AccumulateImage();
	OnFreeOutImage();
	m_outH = m_inH;
	m_outW = m_inW;
	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	for (int i = 0; i < m_inH; i++)
		for (int k = 0; k < m_inW; k++) {
			m_outImageR[i][k] = m_inImageR[i][k];
			m_outImageG[i][k] = m_inImageG[i][k];
			m_outImageB[i][k] = m_inImageB[i][k];

		}

}

void CColorImageAlpha1Doc::OnGrayscaleImage()
{
	AccumulateImage();
	OnFreeOutImage();

	m_outH = m_inH;
	m_outW = m_inW;
	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);
	double avg;
	for (int i = 0; i < m_inH; i++)
		for (int k = 0; k < m_inW; k++) {
			avg = (m_inImageR[i][k] + m_inImageG[i][k] + m_inImageB[i][k]) / 3.0;
			m_outImageR[i][k] = m_outImageG[i][k] = m_outImageB[i][k] = (int)avg;

		}

}

void CColorImageAlpha1Doc::OnAddImage()
{
	AccumulateImage();
	CConstantDlg dlg;
	if (dlg.DoModal() == IDOK) {
		OnFreeOutImage();
		m_outH = m_inH;
		m_outW = m_inW;
		m_outImageR = OnMalloc2D(m_outH, m_outW);
		m_outImageG = OnMalloc2D(m_outH, m_outW);
		m_outImageB = OnMalloc2D(m_outH, m_outW);
		int value = (int)dlg.m_constant;

		for (int i = 0; i < m_outH; i++) {
			for (int k = 0; k < m_outW; k++) {
				if (m_inImageR[i][k] + value > 255)
					m_outImageR[i][k] = 255;
				else if (m_inImageR[i][k] + value < 0)
					m_outImageR[i][k] = 0;
				else
					m_outImageR[i][k] = m_inImageR[i][k] + value;

				if (m_inImageG[i][k] + value > 255)
					m_outImageG[i][k] = 255;
				else if (m_inImageG[i][k] + value < 0)
					m_outImageG[i][k] = 0;
				else
					m_outImageG[i][k] = m_inImageG[i][k] + value;

				if (m_inImageB[i][k] + value > 255)
					m_outImageB[i][k] = 255;
				else if (m_inImageB[i][k] + value < 0)
					m_outImageB[i][k] = 0;
				else
					m_outImageB[i][k] = m_inImageB[i][k] + value;
			}
		}
	}
}

void CColorImageAlpha1Doc::OnBwImage()
{
	AccumulateImage();
	CConstantDlg dlg;
	if (dlg.DoModal() == IDOK) {
		OnFreeOutImage();
		m_outH = m_inH;
		m_outW = m_inW;
		m_outImageR = OnMalloc2D(m_outH, m_outW);
		m_outImageG = OnMalloc2D(m_outH, m_outW);
		m_outImageB = OnMalloc2D(m_outH, m_outW);
		int value = (int)dlg.m_constant;

		for (int i = 0; i < m_inH; i++) {
			for (int k = 0; k < m_inW; k++) {
				// Convert to grayscale
				int avg = (m_inImageR[i][k] + m_inImageG[i][k] + m_inImageB[i][k]) / 3;
				if (avg > value) {
					m_outImageR[i][k] = 255;
					m_outImageG[i][k] = 255;
					m_outImageB[i][k] = 255;
				}
				else {
					m_outImageR[i][k] = 0;
					m_outImageG[i][k] = 0;
					m_outImageB[i][k] = 0;
				}
			}
		}
	}
}

void CColorImageAlpha1Doc::OnAvgbwImage()
{
	    AccumulateImage();
		OnFreeOutImage();
		m_outH = m_inH;
		m_outW = m_inW;
		m_outImageR = OnMalloc2D(m_outH, m_outW);
		m_outImageG = OnMalloc2D(m_outH, m_outW);
		m_outImageB = OnMalloc2D(m_outH, m_outW);
		int sumR = 0, sumG= 0, sumB = 0;
		for (int i = 0; i < m_inH; i++) {
			for (int k = 0; k < m_inW; k++) {
				sumR += m_inImageR[i][k];
				sumG += m_inImageG[i][k];
				sumB += m_inImageB[i][k];
			}
		}
		int value = (int)(sumR+sumG+sumB) / (3*m_inH*m_inW);
		for (int i = 0; i < m_inH; i++) {
			for (int k = 0; k < m_inW; k++) {
				int avg = (m_inImageR[i][k] + m_inImageG[i][k] + m_inImageB[i][k]) / 3;
				if (avg > value) {
					m_outImageR[i][k] = 255;
					m_outImageG[i][k] = 255;
					m_outImageB[i][k] = 255;
				}
				else {
					m_outImageR[i][k] = 0;
					m_outImageG[i][k] = 0;
					m_outImageB[i][k] = 0;
				}
			}
		}
	}


void CColorImageAlpha1Doc::OnRevImage()
{
	AccumulateImage();
	OnFreeOutImage();
	m_outH = m_inH;
	m_outW = m_inW;
	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			m_outImageR[i][k] = 255 - m_inImageR[i][k];
			m_outImageG[i][k] = 255 - m_inImageG[i][k];
			m_outImageB[i][k] = 255 - m_inImageB[i][k];
		}
	}
}


void CColorImageAlpha1Doc::OnMirrorImage()
{
	AccumulateImage();
	OnFreeOutImage();
	m_outH = m_inH;
	m_outW = m_inW;
	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			m_outImageR[i][m_outW - k - 1] = m_inImageR[i][k];
			m_outImageG[i][m_outW - k - 1] = m_inImageG[i][k];
			m_outImageB[i][m_outW - k - 1] = m_inImageB[i][k];
		}
	}
}


void CColorImageAlpha1Doc::OnBrparaImage()
{
	AccumulateImage();
	OnFreeOutImage();
	m_outH = m_inH;
	m_outW = m_inW;
	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			m_outImageR[i][k] = 255.0 * pow((m_inImageR[i][k] / 127.0) - 1, 2);
			m_outImageG[i][k] = 255.0 * pow((m_inImageG[i][k] / 127.0) - 1, 2);
			m_outImageB[i][k] = 255.0 * pow((m_inImageG[i][k] / 127.0) - 1, 2);
		}
	}
}


void CColorImageAlpha1Doc::OnDaparaImage()
{
	AccumulateImage();
	OnFreeOutImage();
	m_outH = m_inH;
	m_outW = m_inW;
	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			m_outImageR[i][k] = 255 - 255.0 * pow((m_inImageR[i][k] / 127.0) - 1, 2);
			m_outImageG[i][k] = 255 - 255.0 * pow((m_inImageG[i][k] / 127.0) - 1, 2);
			m_outImageB[i][k] = 255 - 255.0 * pow((m_inImageG[i][k] / 127.0) - 1, 2);
		}
	}
}


void CColorImageAlpha1Doc::OnGammaImage()
{
	AccumulateImage();
	CConstantDlg dlg;
	if (dlg.DoModal() == IDOK) {
		OnFreeOutImage();
		m_outH = m_inH;
		m_outW = m_inW;
		m_outImageR = OnMalloc2D(m_outH, m_outW);
		m_outImageG = OnMalloc2D(m_outH, m_outW);
		m_outImageB = OnMalloc2D(m_outH, m_outW);
		double value = (double)dlg.m_constant;

		for (int i = 0; i < m_inH; i++) {
			for (int k = 0; k < m_inW; k++) {
				m_outImageR[i][k] = (int)255.0 * pow(m_inImageR[i][k] / 255.0, value);
				m_outImageG[i][k] = (int)255.0 * pow(m_inImageG[i][k] / 255.0, value);
				m_outImageB[i][k] = (int)255.0 * pow(m_inImageB[i][k] / 255.0, value);
			}
		}
	}
}

/// <summary>X
/// 기하학 처리 함수
/// </summary>

void CColorImageAlpha1Doc::OnRotateImage()
{
	AccumulateImage();
	CConstant1Dlg dlg;
	if (dlg.DoModal() == IDOK) {
		OnFreeOutImage();
		m_outH = m_inH;
		m_outW = m_inW;
		m_outImageR = OnMalloc2D(m_outH, m_outW);
		m_outImageG = OnMalloc2D(m_outH, m_outW);
		m_outImageB = OnMalloc2D(m_outH, m_outW);
        double degree = (double)dlg.m_degree;
		double radian = degree * 3.1415926 / 180.0;
		int centX = (int)(m_outH / 2);
		int centY = (int)(m_outW / 2);
		for (int i = 0; i < m_outH; i++) {
			for (int k = 0; k < m_outW; k++) {
				int xd = i;
				int yd = k;

				int xs = (int)(cos(radian) * (xd - centX) - sin(radian) * (yd - centY) + centX);
				int ys = (int)(sin(radian) * (xd - centX) + cos(radian) * (yd - centY) + centY);

				if (xs >= 0 && xs < m_inH && ys >= 0 && ys < m_inW) {
					m_outImageR[xd][yd] = m_inImageR[xs][ys];
					m_outImageG[xd][yd] = m_inImageG[xs][ys];
					m_outImageB[xd][yd] = m_inImageB[xs][ys];
				}
			}
		}
	}
}

void CColorImageAlpha1Doc::OnMidZoomout()
{
	AccumulateImage();
	CConstantDlg dlg;
	if (dlg.DoModal() == IDOK) {
		OnFreeOutImage();
		int scale = (int)dlg.m_constant;
		m_outH = (int)m_inH/scale;
		m_outW = (int)m_inW/scale;
		m_outImageR = OnMalloc2D(m_outH, m_outW);
		m_outImageG = OnMalloc2D(m_outH, m_outW);
		m_outImageB = OnMalloc2D(m_outH, m_outW);
		unsigned char* blockR;
		unsigned char* blockG;
		unsigned char* blockB;

		for (int i = 0; i < m_outH; i++) {
			for (int j = 0; j < m_outW; j++) {
				blockR = (unsigned char*)malloc(scale * scale * sizeof(unsigned char));
				blockG = (unsigned char*)malloc(scale * scale * sizeof(unsigned char));
				blockB = (unsigned char*)malloc(scale * scale * sizeof(unsigned char));
				short count = 0;
				for (int k = i * scale; k < (i + 1) * scale; k++) {
					for (int p = j * scale; p < (j + 1) * scale; p++) {
						blockR[count] = m_inImageR[k][p];
						blockG[count] = m_inImageG[k][p];
						blockB[count] = m_inImageB[k][p];
						count++;
					}
				}
				unsigned char tempR; unsigned char tempG; unsigned char tempB;
				for (int x = 0; x < count; x++) {
					for (int y = 0; y < count - 1 - x; y++) {
						if (blockR[y] > blockR[y + 1]) {
							tempR = blockR[y];
							blockR[y] = blockR[y + 1];
							blockR[y + 1] = tempR;
						}
						if (blockG[y] > blockG[y + 1]) {
							tempG = blockG[y];
							blockG[y] = blockG[y + 1];
							blockG[y + 1] = tempG;
						}
						if (blockB[y] > blockB[y + 1]) {
							tempB = blockB[y];
							blockB[y] = blockB[y + 1];
							blockB[y + 1] = tempB;
						}
					}
				}
				m_outImageR[i][j] = (int)blockR[count / 2];
				m_outImageG[i][j] = (int)blockG[count/ 2];
				m_outImageB[i][j] = (int)blockB[count / 2];
				free(blockR); free(blockG); free(blockB);
			}
		}
	}
}

void CColorImageAlpha1Doc::OnAvgZoomout()
{
	AccumulateImage();
	CConstantDlg dlg;
	if (dlg.DoModal() == IDOK) {
		OnFreeOutImage();
		int scale = (int)dlg.m_constant;
		m_outH = (int)m_inH / scale;
		m_outW = (int)m_inW / scale;
		m_outImageR = OnMalloc2D(m_outH, m_outW);
		m_outImageG = OnMalloc2D(m_outH, m_outW);
		m_outImageB = OnMalloc2D(m_outH, m_outW);
		for (int i = 0; i < m_outH; i++) {
			for (int j = 0; j < m_outW; j++) {
				int sumR = 0 , sumG = 0, sumB = 0;
				for (int k = i * scale; k < (i + 1) * scale; k++) {
					for (int p = j * scale; p < (j + 1) * scale; p++) {
						sumR += m_inImageR[k][p];
						sumG += m_inImageG[k][p];
						sumB += m_inImageB[k][p];
					}
				}
				m_outImageR[i][j] = sumR / (scale * (double)scale);
				m_outImageG[i][j] = sumG / (scale * (double)scale);
				m_outImageB[i][j] = sumB / (scale * (double)scale);
				
			}
		}
	}
}

void CColorImageAlpha1Doc::OnZoomout()
{
	AccumulateImage();
	CConstantDlg dlg;
	if (dlg.DoModal() == IDOK) {
		OnFreeOutImage();
		int scale = (int)dlg.m_constant;
		m_outH = (int)m_inH / scale;
		m_outW = (int)m_inW / scale;
		m_outImageR = OnMalloc2D(m_outH, m_outW);
		m_outImageG = OnMalloc2D(m_outH, m_outW);
		m_outImageB = OnMalloc2D(m_outH, m_outW);
		for (int i = 0; i < m_inH; i++) {
			for (int k = 0; k < m_inW; k++) {
				m_outImageR[(int)(i / scale)][(int)(k / scale)] = m_inImageR[i][k];
				m_outImageG[(int)(i / scale)][(int)(k / scale)] = m_inImageG[i][k];
				m_outImageB[(int)(i / scale)][(int)(k / scale)] = m_inImageB[i][k];
			}
		}
	}
}

void CColorImageAlpha1Doc::OnZoomin()
{
	AccumulateImage();
	CConstantDlg dlg;
	if (dlg.DoModal() == IDOK) {
		OnFreeOutImage();
		int scale = (int)dlg.m_constant;
		m_outH = (int)m_inH * scale;
		m_outW = (int)m_inW * scale;
		m_outImageR = OnMalloc2D(m_outH, m_outW);
		m_outImageG = OnMalloc2D(m_outH, m_outW);
		m_outImageB = OnMalloc2D(m_outH, m_outW);
		for (int i = 0; i < m_outH; i++) {
			for (int k = 0; k < m_outW; k++) {
				m_outImageR[i][k] = m_inImageR[(int)(i / scale)][(int)(k / scale)];
				m_outImageG[i][k] = m_inImageG[(int)(i / scale)][(int)(k / scale)];
				m_outImageB[i][k] = m_inImageB[(int)(i / scale)][(int)(k / scale)];
			}
		}
	}
}

void CColorImageAlpha1Doc::OnBilZoomin()
{
	AccumulateImage();
	CConstantDlg dlg;
	if (dlg.DoModal() == IDOK) {
		OnFreeOutImage();
		int scale = (int)dlg.m_constant;
		m_outH = (int)m_inH * scale;
		m_outW = (int)m_inW * scale;
		m_outImageR = OnMalloc2D(m_outH, m_outW);
		m_outImageG = OnMalloc2D(m_outH, m_outW);
		m_outImageB = OnMalloc2D(m_outH, m_outW);
		for (int i = 0; i < m_outH; i++) { 
			for (int k = 0; k < m_outW; k++) {
				double orig_i = i / (double)scale;
				double orig_k = k / (double)scale;

				int i1 = (int)orig_i;
				int i2 = i1 + 1;
				int k1 = (int)orig_k;
				int k2 = k1 + 1;

				double biY = orig_i - i1;
				double biX = orig_k - k1;

				if (i2 >= m_inH) i2 = m_inH - 1;
				if (k2 >= m_inW) k2 = m_inW - 1;

				m_outImageR[i][k] = (int)
					(1 - biY) * ((1 - biX) * m_inImageR[i1][k1] + biX * m_inImageR[i1][k2]) +
					biY * ((1 - biX) * m_inImageR[i2][k1] + biX * m_inImageR[i2][k2]);
				m_outImageG[i][k] = (int)
					(1 - biY) * ((1 - biX) * m_inImageG[i1][k1] + biX * m_inImageG[i1][k2]) +
					biY * ((1 - biX) * m_inImageG[i2][k1] + biX * m_inImageG[i2][k2]);
				m_outImageB[i][k] = (int)
					(1 - biY) * ((1 - biX) * m_inImageB[i1][k1] + biX * m_inImageB[i1][k2]) +
					biY * ((1 - biX) * m_inImageB[i2][k1] + biX * m_inImageB[i2][k2]);

				
			}
		}
	}
}

void CColorImageAlpha1Doc::OnTwistImage()
{
	AccumulateImage();
	CConstant3Dlg dlg;
	if (dlg.DoModal() == IDOK) {
		OnFreeOutImage();
		double twistAmount = (double) dlg.m_constant3/100;
		m_outH = m_inH;
		m_outW = m_inW;
		m_outImageR = OnMalloc2D(m_outH, m_outW);
		m_outImageG = OnMalloc2D(m_outH, m_outW);
		m_outImageB = OnMalloc2D(m_outH, m_outW);

		// 이미지의 중심 좌표 계산
		int centerX = m_outW / 2;
		int centerY = m_outH / 2;

		// 최대 거리 계산
		double maxDistance = sqrt(centerX * centerX + centerY * centerY);

		for (int y = 0; y < m_outH; ++y)
		{
			for (int x = 0; x < m_outW; ++x)
			{
				double dx = x - centerX;
				double dy = y - centerY;
				double distance = sqrt(dx * dx + dy * dy);
				// 회전 강도를 거리에 비례하여 조절
				double normalizedDistance = distance / maxDistance; // 0부터 1까지의 값으로 정규화
				double adjustedTwistAmount = twistAmount * normalizedDistance;
				double angle = atan2(dy, dx);
				double twistAngle = adjustedTwistAmount * distance;
				int newX = centerX + distance * cos(angle + twistAngle);
				int newY = centerY + distance * sin(angle + twistAngle);

				if (newX >= 0 && newX < m_outW && newY >= 0 && newY < m_outH)
				{
					m_outImageR[newY][newX] = m_inImageR[y][x];
					m_outImageG[newY][newX] = m_inImageG[y][x];
					m_outImageB[newY][newX] = m_inImageB[y][x];
				}
			}
		}
	}
}



/////////////////////
//히스토그램 처리///
/////////////////////




void CColorImageAlpha1Doc::OnStrechImage()
{
	AccumulateImage();
	OnFreeOutImage();
	m_outH = m_inH;
	m_outW = m_inW;
	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);
	int highR = m_inImageR[0][0], lowR = m_inImageR[0][0];
	int highG = m_inImageR[0][0], lowG = m_inImageR[0][0];
	int highB = m_inImageR[0][0], lowB = m_inImageR[0][0];
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			if (m_inImageR[i][k] < lowR) {
				lowR = m_inImageR[i][k];
			}
			if (m_inImageR[i][k] > highR) {
				highR = m_inImageR[i][k];
			}

			if (m_inImageG[i][k] < lowG) {
				lowG = m_inImageG[i][k];
			}
			if (m_inImageG[i][k] > highG) {
				highG = m_inImageG[i][k];
			}

			if (m_inImageB[i][k] < lowB) {
				lowB = m_inImageB[i][k];
			}
			if (m_inImageB[i][k] > highB) {
				highB = m_inImageB[i][k];
			}
		}
	}
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			int oldR = m_inImageR[i][k];
			int oldG = m_inImageG[i][k];
			int oldB = m_inImageB[i][k];
			int newR = (int)((oldR - lowR) / (double)(highR - lowR) * 255.0);
			int newG = (int)((oldG - lowG) / (double)(highG - lowG) * 255.0);
			int newB = (int)((oldB - lowB) / (double)(highB - lowB) * 255.0);

			if (newR > 255) {
				newR = 255;
			}
			if (newR < 0) {
				newR = 0;
			}

			if (newG > 255) {
				newG = 255;
			}
			if (newG < 0) {
				newG = 0;
			}

			if (newB > 255) {
				newB = 255;
			}
			if (newB < 0) {
				newB = 0;
			}
			m_outImageR[i][k] = newR;
			m_outImageG[i][k] = newG;
			m_outImageB[i][k] = newB;
		}
	}
}


void CColorImageAlpha1Doc::OnHiseqImage()
{
	AccumulateImage();
	OnFreeOutImage();
	m_outH = m_inH;
	m_outW = m_inW;
	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);
	int highR = m_inImageR[0][0], lowR = m_inImageR[0][0];
	int highG = m_inImageR[0][0], lowG = m_inImageR[0][0];
	int highB = m_inImageR[0][0], lowB = m_inImageR[0][0];
	int histoR[256] = { 0, };
	int histoG[256] = { 0, };
	int histoB[256] = { 0, };
	//1 단계 : 빈도수 세기
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			histoR[m_inImageR[i][k]]++;
			histoG[m_inImageG[i][k]]++;
			histoB[m_inImageB[i][k]]++;
		}
	}
	// 2단계 : 누적 히스토그램 생성
	int sumHistoR[256] = { 0, };
	int sumHistoG[256] = { 0, };
	int sumHistoB[256] = { 0, };
	sumHistoR[0] = histoR[0];
	sumHistoG[0] = histoG[0];
	sumHistoB[0] = histoB[0];
	for (int i = 1; i < 256; i++) {
		sumHistoR[i] = sumHistoR[i - 1] + histoR[i];
		sumHistoG[i] = sumHistoG[i - 1] + histoG[i];
		sumHistoB[i] = sumHistoB[i - 1] + histoB[i];
	}
	// 3단계 : 정규화된 히스토그램 생성
	double normalHistoR[256] = { 1.0, };
	double normalHistoG[256] = { 1.0, };
	double normalHistoB[256] = { 1.0, };
	for (int i = 0; i < 256; i++) {
		normalHistoR[i] = sumHistoR[i] * (1.0 / (m_inH * m_inW)) * 255.0;
		normalHistoG[i] = sumHistoG[i] * (1.0 / (m_inH * m_inW)) * 255.0;
		normalHistoB[i] = sumHistoB[i] * (1.0 / (m_inH * m_inW)) * 255.0;
	}
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			m_outImageR[i][k] = (unsigned char)(normalHistoR[m_inImageR[i][k]]);
			m_outImageG[i][k] = (unsigned char)(normalHistoG[m_inImageG[i][k]]);
			m_outImageB[i][k] = (unsigned char)(normalHistoB[m_inImageB[i][k]]);
		}
	}
}


void CColorImageAlpha1Doc::OnEndinImage()
{
	AccumulateImage();
	OnFreeOutImage();
	m_outH = m_inH;
	m_outW = m_inW;
	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);
	int highR = m_inImageR[0][0], lowR = m_inImageR[0][0];
	int highG = m_inImageR[0][0], lowG = m_inImageR[0][0];
	int highB = m_inImageR[0][0], lowB = m_inImageR[0][0];
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			if (m_inImageR[i][k] < lowR) {
				lowR = m_inImageR[i][k];
			}
			if (m_inImageR[i][k] > highR) {
				highR = m_inImageR[i][k];
			}

			if (m_inImageG[i][k] < lowG) {
				lowG = m_inImageG[i][k];
			}
			if (m_inImageG[i][k] > highG) {
				highG = m_inImageG[i][k];
			}

			if (m_inImageB[i][k] < lowB) {
				lowB = m_inImageB[i][k];
			}
			if (m_inImageB[i][k] > highB) {
				highB = m_inImageB[i][k];
			}
		}
	}
	highR -= 50;
	lowR += 50;
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			int oldR = m_inImageR[i][k];
			int oldG = m_inImageG[i][k];
			int oldB = m_inImageB[i][k];
			int newR = (int)((oldR - lowR) / (double)(highR - lowR) * 255.0);
			int newG = (int)((oldG - lowG) / (double)(highG - lowG) * 255.0);
			int newB = (int)((oldB - lowB) / (double)(highB - lowB) * 255.0);

			if (newR > 255) {
				newR = 255;
			}
			if (newR < 0) {
				newR = 0;
			}

			if (newG > 255) {
				newG = 255;
			}
			if (newG < 0) {
				newG = 0;
			}

			if (newB > 255) {
				newB = 255;
			}
			if (newB < 0) {
				newB = 0;
			}
			m_outImageR[i][k] = newR;
			m_outImageG[i][k] = newG;
			m_outImageB[i][k] = newB;
		}
	}
}

/////////////////
//영역 처리 함수/
////////////////


void CColorImageAlpha1Doc::OnEmboss()
{
	AccumulateImage();
	OnFreeOutImage();
	m_outH = m_inH;
	m_outW = m_inW;
	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);
	const int MSIZE = 3;
	double mask[MSIZE][MSIZE] = {  
		{ -1.0, 0.0, 0.0 },
		{  0.0, 0.0, 0.0 },
		{  0.0, 0.0, 1.0 } };
	double** tmpInImageR, ** tmpInImageG, ** tmpInImageB, ** tmpOutImageR, ** tmpOutImageG, ** tmpOutImageB;
	tmpInImageR = OnMalloc2D_double(m_inH + 2, m_inW + 2);
	tmpInImageG = OnMalloc2D_double(m_inH + 2, m_inW + 2);
	tmpInImageB = OnMalloc2D_double(m_inH + 2, m_inW + 2);
	tmpOutImageR = OnMalloc2D_double(m_outH, m_outW);
	tmpOutImageG = OnMalloc2D_double(m_outH, m_outW);
	tmpOutImageB = OnMalloc2D_double(m_outH, m_outW);

	for (int i = 0; i < m_inH + 2; i++)
		for (int k = 0; k < m_inW + 2; k++)
			tmpInImageR[i][k] = tmpInImageG[i][k] = tmpInImageB[i][k] = 127.0;

	for (int i = 0; i < m_inH; i++)
		for (int k = 0; k < m_inW; k++) {
			tmpInImageR[i + 1][k + 1] = m_inImageR[i][k];
			tmpInImageG[i + 1][k + 1] = m_inImageG[i][k];
			tmpInImageB[i + 1][k + 1] = m_inImageB[i][k];
		}


	double S;
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			S = 0.0;
			for (int m = 0; m < MSIZE; m++)
				for (int n = 0; n < MSIZE; n++)
					S += tmpInImageR[i + m][k + n] * mask[m][n];
			tmpOutImageR[i][k] = S;

			S = 0.0;
			for (int m = 0; m < MSIZE; m++)
				for (int n = 0; n < MSIZE; n++)
					S += tmpInImageG[i + m][k + n] * mask[m][n];
			tmpOutImageG[i][k] = S;

			S = 0.0;
			for (int m = 0; m < MSIZE; m++)
				for (int n = 0; n < MSIZE; n++)
					S += tmpInImageB[i + m][k + n] * mask[m][n];
			tmpOutImageB[i][k] = S;
		}
	}
	for (int i = 0; i < m_inH; i++)
		for (int k = 0; k < m_inW; k++) {
			tmpOutImageR[i][k] += 127;
			tmpOutImageG[i][k] += 127;
			tmpOutImageB[i][k] += 127;
		}

	for (int i = 0; i < m_outH; i++)
		for (int k = 0; k < m_outW; k++) {
			if (tmpOutImageR[i][k] < 0.0)
				m_outImageR[i][k] = 0;
			else if (tmpOutImageR[i][k] > 255.0)
				m_outImageR[i][k] = 255;
			else
				m_outImageR[i][k] = (unsigned char)tmpOutImageR[i][k];

			if (tmpOutImageG[i][k] < 0.0)
				m_outImageG[i][k] = 0;
			else if (tmpOutImageG[i][k] > 255.0)
				m_outImageG[i][k] = 255;
			else
				m_outImageG[i][k] = (unsigned char)tmpOutImageG[i][k];

			if (tmpOutImageB[i][k] < 0.0)
				m_outImageB[i][k] = 0;
			else if (tmpOutImageB[i][k] > 255.0)
				m_outImageB[i][k] = 255;
			else
				m_outImageB[i][k] = (unsigned char)tmpOutImageB[i][k];

		}
	OnFree2D(tmpInImageR, m_inH + 2);
	OnFree2D(tmpInImageG, m_inH + 2);
	OnFree2D(tmpInImageB, m_inH + 2);
	OnFree2D(tmpOutImageR, m_outH);
	OnFree2D(tmpOutImageG, m_outH);
	OnFree2D(tmpOutImageB, m_outH);

}






void CColorImageAlpha1Doc::OnEmbossHsi()
{
	AccumulateImage();
	OnFreeOutImage();
	m_outH = m_inH;
	m_outW = m_inW;
	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);
	const int MSIZE = 3;
	double mask[MSIZE][MSIZE] = { 
		{ -1.0, 0.0, 0.0 },
		{  0.0, 0.0, 0.0 },
		{  0.0, 0.0, 1.0 } };

	double** tmpInImageR, ** tmpInImageG, ** tmpInImageB, ** tmpOutImageR, ** tmpOutImageG, ** tmpOutImageB;
	double** tmpInImageH, ** tmpInImageS, ** tmpInImageI;
	tmpInImageR = OnMalloc2D_double(m_inH + 2, m_inW + 2);
	tmpInImageG = OnMalloc2D_double(m_inH + 2, m_inW + 2);
	tmpInImageB = OnMalloc2D_double(m_inH + 2, m_inW + 2);

	tmpInImageH = OnMalloc2D_double(m_inH + 2, m_inW + 2);
	tmpInImageS = OnMalloc2D_double(m_inH + 2, m_inW + 2);
	tmpInImageI = OnMalloc2D_double(m_inH + 2, m_inW + 2);

	tmpOutImageR = OnMalloc2D_double(m_outH, m_outW);
	tmpOutImageG = OnMalloc2D_double(m_outH, m_outW);
	tmpOutImageB = OnMalloc2D_double(m_outH, m_outW);

	for (int i = 0; i < m_inH + 2; i++)
		for (int k = 0; k < m_inW + 2; k++)
			tmpInImageR[i][k] = tmpInImageG[i][k] = tmpInImageB[i][k] = 127.0;

	for (int i = 0; i < m_inH; i++)
		for (int k = 0; k < m_inW; k++) {
			tmpInImageR[i + 1][k + 1] = m_inImageR[i][k];
			tmpInImageG[i + 1][k + 1] = m_inImageG[i][k];
			tmpInImageB[i + 1][k + 1] = m_inImageB[i][k];
		}

	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			double* hsi;
			unsigned char R, G, B;
			R = tmpInImageR[i][k]; G = tmpInImageG[i][k]; B = tmpInImageB[i][k];
			hsi = RGB2HSI(R, G, B);

			double H, S, I;
			H = hsi[0]; S = hsi[1]; I = hsi[2];
			tmpInImageH[i][k] = H; tmpInImageS[i][k] = S; tmpInImageI[i][k] = I;
		}
	}
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			double S_VALUE = 0.0;
			for (int m = 0; m < MSIZE; m++)
				for (int n = 0; n < MSIZE; n++)
					S_VALUE += tmpInImageI[i + m][k + n] * mask[m][n];
			tmpInImageI[i][k] = S_VALUE;
		}
	}

	for (int i = 0; i < m_inH; i++)
		for (int k = 0; k < m_inW; k++) {
			tmpInImageI[i][k] += 127;
		}

	for (int i = 0; i < m_inH; i++)
		for (int k = 0; k < m_inW; k++) {
			unsigned char* rgb;
			double H, S, I;

			H = tmpInImageH[i][k]; S = tmpInImageS[i][k]; I = tmpInImageI[i][k];

			rgb = HSI2RGB(H, S, I);
			tmpOutImageR[i][k] = rgb[0]; tmpOutImageG[i][k] = rgb[1]; tmpOutImageB[i][k] = rgb[2];
		}

	for (int i = 0; i < m_outH; i++)
		for (int k = 0; k < m_outW; k++) {
			if (tmpOutImageR[i][k] < 0.0)
				m_outImageR[i][k] = 0;
			else if (tmpOutImageR[i][k] > 255.0)
				m_outImageR[i][k] = 255;
			else
				m_outImageR[i][k] = (unsigned char)tmpOutImageR[i][k];

			if (tmpOutImageG[i][k] < 0.0)
				m_outImageG[i][k] = 0;
			else if (tmpOutImageG[i][k] > 255.0)
				m_outImageG[i][k] = 255;
			else
				m_outImageG[i][k] = (unsigned char)tmpOutImageG[i][k];

			if (tmpOutImageB[i][k] < 0.0)
				m_outImageB[i][k] = 0;
			else if (tmpOutImageB[i][k] > 255.0)
				m_outImageB[i][k] = 255;
			else
				m_outImageB[i][k] = (unsigned char)tmpOutImageB[i][k];

		}

	OnFree2D(tmpInImageR, m_inH + 2);
	OnFree2D(tmpInImageG, m_inH + 2);
	OnFree2D(tmpInImageB, m_inH + 2);
	OnFree2D(tmpOutImageR, m_outH);
	OnFree2D(tmpOutImageG, m_outH);
	OnFree2D(tmpOutImageB, m_outH);
	OnFree2D(tmpInImageH, m_inH + 2);
	OnFree2D(tmpInImageS, m_inH + 2);
	OnFree2D(tmpInImageI, m_inH + 2);
}


void CColorImageAlpha1Doc::OnBlurImage()
{
	AccumulateImage();
	OnFreeOutImage();
	m_outH = m_inH;
	m_outW = m_inW;
	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);
	const int MSIZE = 3;
	double mask[MSIZE][MSIZE] = { {1. / 9, 1. / 9, 1. / 9},
						{ 1. / 9, 1. / 9,1. / 9 },
						{ 1. / 9, 1. / 9, 1. / 9 } };

	double** tmpInImageR, ** tmpInImageG, ** tmpInImageB, ** tmpOutImageR, ** tmpOutImageG, ** tmpOutImageB;
	double** tmpInImageH, ** tmpInImageS, ** tmpInImageI;
	tmpInImageR = OnMalloc2D_double(m_inH + 2, m_inW + 2);
	tmpInImageG = OnMalloc2D_double(m_inH + 2, m_inW + 2);
	tmpInImageB = OnMalloc2D_double(m_inH + 2, m_inW + 2);

	tmpInImageH = OnMalloc2D_double(m_inH + 2, m_inW + 2);
	tmpInImageS = OnMalloc2D_double(m_inH + 2, m_inW + 2);	
	tmpInImageI = OnMalloc2D_double(m_inH + 2, m_inW + 2);

	tmpOutImageR = OnMalloc2D_double(m_outH, m_outW);
	tmpOutImageG = OnMalloc2D_double(m_outH, m_outW);
	tmpOutImageB = OnMalloc2D_double(m_outH, m_outW);

	for (int i = 0; i < m_inH + 2; i++)
		for (int k = 0; k < m_inW + 2; k++)
			tmpInImageR[i][k] = tmpInImageG[i][k] = tmpInImageB[i][k] = 127.0;

	for (int i = 0; i < m_inH; i++)
		for (int k = 0; k < m_inW; k++) {
			tmpInImageR[i + 1][k + 1] = m_inImageR[i][k];
			tmpInImageG[i + 1][k + 1] = m_inImageG[i][k];
			tmpInImageB[i + 1][k + 1] = m_inImageB[i][k];
		}

	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			double* hsi;
			unsigned char R, G, B;
			R = tmpInImageR[i][k]; G = tmpInImageG[i][k]; B = tmpInImageB[i][k];
			hsi = RGB2HSI(R, G, B);

			double H, S, I;
			H = hsi[0]; S = hsi[1]; I = hsi[2];
			tmpInImageH[i][k] = H; tmpInImageS[i][k] = S; tmpInImageI[i][k] = I;
		}
	}
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			double S_VALUE = 0.0;
			for (int m = 0; m < MSIZE; m++)
				for (int n = 0; n < MSIZE; n++)
					S_VALUE += tmpInImageI[i + m][k + n] * mask[m][n];
			tmpInImageI[i][k] = S_VALUE;
		}
	}
	for (int i = 0; i < m_inH; i++)
		for (int k = 0; k < m_inW; k++) {
			unsigned char* rgb;
			double H, S, I;

			H = tmpInImageH[i][k]; S = tmpInImageS[i][k]; I = tmpInImageI[i][k];

			rgb = HSI2RGB(H, S, I);
			tmpOutImageR[i][k] = rgb[0]; tmpOutImageG[i][k] = rgb[1]; tmpOutImageB[i][k] = rgb[2];
		}
	for (int i = 0; i < m_outH; i++)
		for (int k = 0; k < m_outW; k++) {
			if (tmpOutImageR[i][k] < 0.0)
				m_outImageR[i][k] = 0;
			else if (tmpOutImageR[i][k] > 255.0)
				m_outImageR[i][k] = 255;
			else
				m_outImageR[i][k] = (unsigned char)tmpOutImageR[i][k];

			if (tmpOutImageG[i][k] < 0.0)
				m_outImageG[i][k] = 0;
			else if (tmpOutImageG[i][k] > 255.0)
				m_outImageG[i][k] = 255;
			else
				m_outImageG[i][k] = (unsigned char)tmpOutImageG[i][k];

			if (tmpOutImageB[i][k] < 0.0)
				m_outImageB[i][k] = 0;
			else if (tmpOutImageB[i][k] > 255.0)
				m_outImageB[i][k] = 255;
			else
				m_outImageB[i][k] = (unsigned char)tmpOutImageB[i][k];

		}

	OnFree2D(tmpInImageR, m_inH + 2);
	OnFree2D(tmpInImageG, m_inH + 2);
	OnFree2D(tmpInImageB, m_inH + 2);
	OnFree2D(tmpOutImageR, m_outH);
	OnFree2D(tmpOutImageG, m_outH);
	OnFree2D(tmpOutImageB, m_outH);
	OnFree2D(tmpInImageH, m_inH + 2);
	OnFree2D(tmpInImageS, m_inH + 2);
	OnFree2D(tmpInImageI, m_inH + 2);
}


void CColorImageAlpha1Doc::OnSharpImage()
{
	AccumulateImage();
	OnFreeOutImage();
	m_outH = m_inH;
	m_outW = m_inW;
	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);
	const int MSIZE = 3;
	double mask[MSIZE][MSIZE] = { {-1,-1, -1},
						{ -1,9, -1 },
						{ -1,-1, -1 } };
	double** tmpInImageR, ** tmpInImageG, ** tmpInImageB, ** tmpOutImageR, ** tmpOutImageG, ** tmpOutImageB;
	tmpInImageR = OnMalloc2D_double(m_inH + 2, m_inW + 2);
	tmpInImageG = OnMalloc2D_double(m_inH + 2, m_inW + 2);
	tmpInImageB = OnMalloc2D_double(m_inH + 2, m_inW + 2);
	tmpOutImageR = OnMalloc2D_double(m_outH, m_outW);
	tmpOutImageG = OnMalloc2D_double(m_outH, m_outW);
	tmpOutImageB = OnMalloc2D_double(m_outH, m_outW);

	for (int i = 0; i < m_inH + 2; i++)
		for (int k = 0; k < m_inW + 2; k++)
			tmpInImageR[i][k] = tmpInImageG[i][k] = tmpInImageB[i][k] = 127.0;

	for (int i = 0; i < m_inH; i++)
		for (int k = 0; k < m_inW; k++) {
			tmpInImageR[i + 1][k + 1] = m_inImageR[i][k];
			tmpInImageG[i + 1][k + 1] = m_inImageG[i][k];
			tmpInImageB[i + 1][k + 1] = m_inImageB[i][k];
		}


	double S;
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			S = 0.0;
			for (int m = 0; m < MSIZE; m++)
				for (int n = 0; n < MSIZE; n++)
					S += tmpInImageR[i + m][k + n] * mask[m][n];
			tmpOutImageR[i][k] = S;

			S = 0.0;
			for (int m = 0; m < MSIZE; m++)
				for (int n = 0; n < MSIZE; n++)
					S += tmpInImageG[i + m][k + n] * mask[m][n];
			tmpOutImageG[i][k] = S;

			S = 0.0;
			for (int m = 0; m < MSIZE; m++)
				for (int n = 0; n < MSIZE; n++)
					S += tmpInImageB[i + m][k + n] * mask[m][n];
			tmpOutImageB[i][k] = S;
		}
	}

	for (int i = 0; i < m_outH; i++)
		for (int k = 0; k < m_outW; k++) {
			if (tmpOutImageR[i][k] < 0.0)
				m_outImageR[i][k] = 0;
			else if (tmpOutImageR[i][k] > 255.0)
				m_outImageR[i][k] = 255;
			else
				m_outImageR[i][k] = (unsigned char)tmpOutImageR[i][k];

			if (tmpOutImageG[i][k] < 0.0)
				m_outImageG[i][k] = 0;
			else if (tmpOutImageG[i][k] > 255.0)
				m_outImageG[i][k] = 255;
			else
				m_outImageG[i][k] = (unsigned char)tmpOutImageG[i][k];

			if (tmpOutImageB[i][k] < 0.0)
				m_outImageB[i][k] = 0;
			else if (tmpOutImageB[i][k] > 255.0)
				m_outImageB[i][k] = 255;
			else
				m_outImageB[i][k] = (unsigned char)tmpOutImageB[i][k];

		}
	OnFree2D(tmpInImageR, m_inH + 2);
	OnFree2D(tmpInImageG, m_inH + 2);
	OnFree2D(tmpInImageB, m_inH + 2);
	OnFree2D(tmpOutImageR, m_outH);
	OnFree2D(tmpOutImageG, m_outH);
	OnFree2D(tmpOutImageB, m_outH);
}



void CColorImageAlpha1Doc::OnVeredgeImage()
{
	AccumulateImage();
	OnFreeOutImage();
	m_outH = m_inH;
	m_outW = m_inW;
	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);
	const int MSIZE = 3;
	double mask[MSIZE][MSIZE] = {
		{  0.0, 0.0, 0.0 },
		{  -1.0, 1.0, 0.0 },
		{  0.0, 0.0, 0.0 } };
	double** tmpInImageR, ** tmpInImageG, ** tmpInImageB, ** tmpOutImageR, ** tmpOutImageG, ** tmpOutImageB;
	tmpInImageR = OnMalloc2D_double(m_inH + 2, m_inW + 2);
	tmpInImageG = OnMalloc2D_double(m_inH + 2, m_inW + 2);
	tmpInImageB = OnMalloc2D_double(m_inH + 2, m_inW + 2);
	tmpOutImageR = OnMalloc2D_double(m_outH, m_outW);
	tmpOutImageG = OnMalloc2D_double(m_outH, m_outW);
	tmpOutImageB = OnMalloc2D_double(m_outH, m_outW);

	for (int i = 0; i < m_inH + 2; i++)
		for (int k = 0; k < m_inW + 2; k++)
			tmpInImageR[i][k] = tmpInImageG[i][k] = tmpInImageB[i][k] = 127.0;

	for (int i = 0; i < m_inH; i++)
		for (int k = 0; k < m_inW; k++) {
			tmpInImageR[i + 1][k + 1] = m_inImageR[i][k];
			tmpInImageG[i + 1][k + 1] = m_inImageG[i][k];
			tmpInImageB[i + 1][k + 1] = m_inImageB[i][k];
		}


	double S;
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			S = 0.0;
			for (int m = 0; m < MSIZE; m++)
				for (int n = 0; n < MSIZE; n++)
					S += tmpInImageR[i + m][k + n] * mask[m][n];
			tmpOutImageR[i][k] = S;

			S = 0.0;
			for (int m = 0; m < MSIZE; m++)
				for (int n = 0; n < MSIZE; n++)
					S += tmpInImageG[i + m][k + n] * mask[m][n];
			tmpOutImageG[i][k] = S;

			S = 0.0;
			for (int m = 0; m < MSIZE; m++)
				for (int n = 0; n < MSIZE; n++)
					S += tmpInImageB[i + m][k + n] * mask[m][n];
			tmpOutImageB[i][k] = S;
		}
	}

	for (int i = 0; i < m_outH; i++)
		for (int k = 0; k < m_outW; k++) {
			if (tmpOutImageR[i][k] < 0.0)
				m_outImageR[i][k] = 0;
			else if (tmpOutImageR[i][k] > 255.0)
				m_outImageR[i][k] = 255;
			else
				m_outImageR[i][k] = (unsigned char)tmpOutImageR[i][k];

			if (tmpOutImageG[i][k] < 0.0)
				m_outImageG[i][k] = 0;
			else if (tmpOutImageG[i][k] > 255.0)
				m_outImageG[i][k] = 255;
			else
				m_outImageG[i][k] = (unsigned char)tmpOutImageG[i][k];

			if (tmpOutImageB[i][k] < 0.0)
				m_outImageB[i][k] = 0;
			else if (tmpOutImageB[i][k] > 255.0)
				m_outImageB[i][k] = 255;
			else
				m_outImageB[i][k] = (unsigned char)tmpOutImageB[i][k];

		}
	OnFree2D(tmpInImageR, m_inH + 2);
	OnFree2D(tmpInImageG, m_inH + 2);
	OnFree2D(tmpInImageB, m_inH + 2);
	OnFree2D(tmpOutImageR, m_outH);
	OnFree2D(tmpOutImageG, m_outH);
	OnFree2D(tmpOutImageB, m_outH);
}


void CColorImageAlpha1Doc::OnPalImage()
{
	AccumulateImage();
	OnFreeOutImage();
	m_outH = m_inH;
	m_outW = m_inW;
	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);
	const int MSIZE = 3;
	double mask[MSIZE][MSIZE] = {
		{  0.0, -1.0, 0.0 },
		{  0.0, 1.0, 0.0 },
		{  0.0, 0.0, 0.0 } };
	double** tmpInImageR, ** tmpInImageG, ** tmpInImageB, ** tmpOutImageR, ** tmpOutImageG, ** tmpOutImageB;
	tmpInImageR = OnMalloc2D_double(m_inH + 2, m_inW + 2);
	tmpInImageG = OnMalloc2D_double(m_inH + 2, m_inW + 2);
	tmpInImageB = OnMalloc2D_double(m_inH + 2, m_inW + 2);
	tmpOutImageR = OnMalloc2D_double(m_outH, m_outW);
	tmpOutImageG = OnMalloc2D_double(m_outH, m_outW);
	tmpOutImageB = OnMalloc2D_double(m_outH, m_outW);

	for (int i = 0; i < m_inH + 2; i++)
		for (int k = 0; k < m_inW + 2; k++)
			tmpInImageR[i][k] = tmpInImageG[i][k] = tmpInImageB[i][k] = 127.0;

	for (int i = 0; i < m_inH; i++)
		for (int k = 0; k < m_inW; k++) {
			tmpInImageR[i + 1][k + 1] = m_inImageR[i][k];
			tmpInImageG[i + 1][k + 1] = m_inImageG[i][k];
			tmpInImageB[i + 1][k + 1] = m_inImageB[i][k];
		}


	double S;
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			S = 0.0;
			for (int m = 0; m < MSIZE; m++)
				for (int n = 0; n < MSIZE; n++)
					S += tmpInImageR[i + m][k + n] * mask[m][n];
			tmpOutImageR[i][k] = S;

			S = 0.0;
			for (int m = 0; m < MSIZE; m++)
				for (int n = 0; n < MSIZE; n++)
					S += tmpInImageG[i + m][k + n] * mask[m][n];
			tmpOutImageG[i][k] = S;

			S = 0.0;
			for (int m = 0; m < MSIZE; m++)
				for (int n = 0; n < MSIZE; n++)
					S += tmpInImageB[i + m][k + n] * mask[m][n];
			tmpOutImageB[i][k] = S;
		}
	}

	for (int i = 0; i < m_outH; i++)
		for (int k = 0; k < m_outW; k++) {
			if (tmpOutImageR[i][k] < 0.0)
				m_outImageR[i][k] = 0;
			else if (tmpOutImageR[i][k] > 255.0)
				m_outImageR[i][k] = 255;
			else
				m_outImageR[i][k] = (unsigned char)tmpOutImageR[i][k];

			if (tmpOutImageG[i][k] < 0.0)
				m_outImageG[i][k] = 0;
			else if (tmpOutImageG[i][k] > 255.0)
				m_outImageG[i][k] = 255;
			else
				m_outImageG[i][k] = (unsigned char)tmpOutImageG[i][k];

			if (tmpOutImageB[i][k] < 0.0)
				m_outImageB[i][k] = 0;
			else if (tmpOutImageB[i][k] > 255.0)
				m_outImageB[i][k] = 255;
			else
				m_outImageB[i][k] = (unsigned char)tmpOutImageB[i][k];

		}
	OnFree2D(tmpInImageR, m_inH + 2);
	OnFree2D(tmpInImageG, m_inH + 2);
	OnFree2D(tmpInImageB, m_inH + 2);
	OnFree2D(tmpOutImageR, m_outH);
	OnFree2D(tmpOutImageG, m_outH);
	OnFree2D(tmpOutImageB, m_outH);
}


////////////////
// 칼라 채도 효과
/////////////////
void CColorImageAlpha1Doc::OnChangeSatur()
{
	AccumulateImage();
	CConstant2Dlg dlg;
	if (dlg.DoModal() == IDOK) {
		OnFreeOutImage();
		double val =(double)dlg.m_constant2;
		m_outH = m_inH;
		m_outW = m_inW;
		m_outImageR = OnMalloc2D(m_outH, m_outW);
		m_outImageG = OnMalloc2D(m_outH, m_outW);
		m_outImageB = OnMalloc2D(m_outH, m_outW);

		for (int i = 0; i < m_inH; i++)
			for (int k = 0; k < m_inW; k++) {
				// H(색상) : 0~ 360
				// S(채도) :  0.0~ 1.0
				// I(명도) : 0~255

				// RGB --> HSI
				double H, S, I;
				unsigned char R, G, B;
				R = m_inImageR[i][k];
				G = m_inImageG[i][k];
				B = m_inImageB[i][k];
				double* hsi = RGB2HSI(R, G, B);
				H = hsi[0]; S = hsi[1]; I = hsi[2];

				S = S + val;
				if (S < 0) {
					S = 0.0;
				}

				unsigned char* rgb = HSI2RGB(H, S, I);
				R = rgb[0]; G = rgb[1]; B = rgb[2];

				m_outImageR[i][k] = R;
				m_outImageG[i][k] = G;
				m_outImageB[i][k] = B;
			}
		
		}
	}

	

void CColorImageAlpha1Doc::OnPickOrange()
{
	AccumulateImage();
	OnFreeOutImage();
	m_outH = m_inH;
	m_outW = m_inW;
	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			// H(색상) : 0~ 360
			// S(채도) :  0.0~ 1.0
			// I(명도) : 0~255

			// RGB --> HSI
			double H, S, I;
			unsigned char R, G, B;
			R = m_inImageR[i][k];
			G = m_inImageG[i][k];
			B = m_inImageB[i][k];
			double* hsi = RGB2HSI(R, G, B);
			H = hsi[0]; S = hsi[1]; I = hsi[2];

			//오렌지 추출 (H : 8~20)
			if (8 <= H && H <= 20) {
				m_outImageR[i][k] = m_inImageR[i][k];
				m_outImageG[i][k] = m_inImageG[i][k];
				m_outImageB[i][k] = m_inImageB[i][k];
			}
			else {
				double avg = (m_inImageR[i][k] + m_inImageG[i][k] + m_inImageB[i][k]) / 3.0;
				m_outImageR[i][k] = m_outImageG[i][k] = m_outImageB[i][k] = (int)avg;
			}
		}
	}
}













