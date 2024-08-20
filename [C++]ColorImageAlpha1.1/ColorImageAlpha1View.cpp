
// ColorImageAlpha1View.cpp: CColorImageAlpha1View 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "ColorImageAlpha1.h"
#endif

#include "ColorImageAlpha1Doc.h"
#include "ColorImageAlpha1View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CColorImageAlpha1View

IMPLEMENT_DYNCREATE(CColorImageAlpha1View, CView)

BEGIN_MESSAGE_MAP(CColorImageAlpha1View, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(IDM_EQUAL_IMAGE, &CColorImageAlpha1View::OnEqualImage)
	ON_COMMAND(IDM_GRAYSCALE_IMAGE, &CColorImageAlpha1View::OnGrayscaleImage)
	ON_COMMAND(IDM_ADD_IMAGE, &CColorImageAlpha1View::OnAddImage)
	ON_COMMAND(IDC_BW_IMAGE, &CColorImageAlpha1View::OnBwImage)
	ON_COMMAND(IDC_AVGBW_IMAGE, &CColorImageAlpha1View::OnAvgbwImage)
	ON_COMMAND(IDC_REV_IMAGE, &CColorImageAlpha1View::OnRevImage)
	ON_COMMAND(IDC_MIRROR_IMAGE, &CColorImageAlpha1View::OnMirrorImage)
	ON_COMMAND(IDC_BRPARA_IMAGE, &CColorImageAlpha1View::OnBrparaImage)
	ON_COMMAND(IDC_DAPARA_IMAGE, &CColorImageAlpha1View::OnDaparaImage)
	ON_COMMAND(IDC_GAMMA_IMAGE, &CColorImageAlpha1View::OnGammaImage)
	ON_COMMAND(IDM_CHANGE_SATUR, &CColorImageAlpha1View::OnChangeSatur)
	ON_COMMAND(IDM_PICK_ORANGE, &CColorImageAlpha1View::OnPickOrange)
	ON_COMMAND(IDM_ROTATE_IMAGE, &CColorImageAlpha1View::OnRotateImage)
	ON_COMMAND(ID_BASIC_IMAGE, &CColorImageAlpha1View::OnBasicImage)
	ON_COMMAND(IDM_MID_ZOOMOUT, &CColorImageAlpha1View::OnMidZoomout)
	ON_COMMAND(IDM_AVG_ZOOMOUT, &CColorImageAlpha1View::OnAvgZoomout)
	ON_COMMAND(IDM_ZOOMOUT, &CColorImageAlpha1View::OnZoomout)
	ON_COMMAND(IDM_EMBOSS, &CColorImageAlpha1View::OnEmboss)
	ON_COMMAND(IDM_EMBOSS_HSI, &CColorImageAlpha1View::OnEmbossHsi)
	ON_COMMAND(IDM_ZOOMIN, &CColorImageAlpha1View::OnZoomin)
	ON_COMMAND(IDM_BIL_ZOOMIN, &CColorImageAlpha1View::OnBilZoomin)
	ON_COMMAND(IDM_STRECH_IMAGE, &CColorImageAlpha1View::OnStrechImage)

	ON_COMMAND(IDM_HISEQ_IMAGE, &CColorImageAlpha1View::OnHiseqImage)
	ON_COMMAND(IDM_ENDIN_IMAGE, &CColorImageAlpha1View::OnEndinImage)
	ON_COMMAND(IDM_BLUR_IMAGE, &CColorImageAlpha1View::OnBlurImage)
	ON_COMMAND(IDM_SHARP_IMAGE, &CColorImageAlpha1View::OnSharpImage)
	ON_COMMAND(IDM_VEREDGE_IMAGE, &CColorImageAlpha1View::OnVeredgeImage)
	ON_COMMAND(IDM_PAL_IMAGE, &CColorImageAlpha1View::OnPalImage)
	ON_COMMAND(IDM_TWIST_IMAGE, &CColorImageAlpha1View::OnTwistImage)
END_MESSAGE_MAP()

// CColorImageAlpha1View 생성/소멸

CColorImageAlpha1View::CColorImageAlpha1View() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CColorImageAlpha1View::~CColorImageAlpha1View()
{
}

BOOL CColorImageAlpha1View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CColorImageAlpha1View 그리기

void CColorImageAlpha1View::OnDraw(CDC* pDC)
{
	CColorImageAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	int i, k;
	unsigned char R, G, B;
	// 메모리 DC 선언
	CDC memDC;
	CBitmap* pOldBitmap, bitmap;

	// 화면 DC와 호환되는 메모리 DC 객체를 생성
	memDC.CreateCompatibleDC(pDC);

	// 마찬가지로 화면 DC와 호환되는 Bitmap 생성
	bitmap.CreateCompatibleBitmap(pDC, pDoc->m_inW, pDoc->m_inH);

	pOldBitmap = memDC.SelectObject(&bitmap);
	memDC.PatBlt(0, 0, pDoc->m_inW, pDoc->m_inH, WHITENESS); // 흰색으로 초기화

	// 출력 영상의 크기를 자동 조절

	double MAXSIZE = 800; // 필요시 실 모니터 또는 화면의 해상도에 따라서 변경 가능 
	int inH = pDoc->m_inH;
	int inW = pDoc->m_inW;
	double hop = 1.0;

	if (inH > MAXSIZE || inW > MAXSIZE) {
		if (inW > inH) {
			hop = (inW / MAXSIZE);
		}
		else {
			hop = (inH / MAXSIZE);
		}

		inW = (int)(inW / hop);
		inH = (int)(inH / hop);
	}

	// 메모리 DC에 그리기
	for (i = 0; i < inH; i++) {
		for (k = 0; k < inW; k++) {
			R = pDoc->m_inImageR[(int)(i * hop)][(int)(k * hop)];
			G = pDoc->m_inImageG[(int)(i * hop)][(int)(k * hop)];
			B = pDoc->m_inImageB[(int)(i * hop)][(int)(k * hop)];
			memDC.SetPixel(k, i, RGB(R, G, B));
		}
	}
	// 메모리 DC를 화면 DC에 고속 복사
	pDC->BitBlt(5, 5, pDoc->m_inW, pDoc->m_inH, &memDC, 0, 0, SRCCOPY);

	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
	bitmap.DeleteObject();

	// 화면 DC와 호환되는 메모리 DC 객체를 생성
	memDC.CreateCompatibleDC(pDC);

	// 마찬가지로 화면 DC와 호환되는 Bitmap 생성
	bitmap.CreateCompatibleBitmap(pDC, pDoc->m_outW, pDoc->m_outH);

	pOldBitmap = memDC.SelectObject(&bitmap);
	memDC.PatBlt(0, 0, pDoc->m_outW, pDoc->m_outH, WHITENESS); // 흰색으로 초기화

	int outH = pDoc->m_outH;
	int outW = pDoc->m_outW;
	hop = 1.0;

	if (outH > MAXSIZE || outW > MAXSIZE) {
		if (outW > outH) {
			hop = (outW / MAXSIZE);
		}
		else {
			hop = (outH / MAXSIZE);
		}

		outW = (int)(outW / hop);
		outH = (int)(outH / hop);
	}


	// 메모리 DC에 그리기
	for (i = 0; i < outH; i++) {
		for (k = 0; k < outW; k++) {
			R = pDoc->m_outImageR[(int)(i * hop)][(int)(k * hop)];
			G = pDoc->m_outImageG[(int)(i * hop)][(int)(k * hop)];
			B = pDoc->m_outImageB[(int)(i * hop)][(int)(k * hop)];
			memDC.SetPixel(k, i, RGB(R, G, B));
		}
	}
	// 메모리 DC를 화면 DC에 고속 복사
	pDC->BitBlt(inW + 10, 5, pDoc->m_outW, pDoc->m_outH, &memDC, 0, 0, SRCCOPY);

	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
	bitmap.DeleteObject();
}


// CColorImageAlpha1View 인쇄

BOOL CColorImageAlpha1View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CColorImageAlpha1View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CColorImageAlpha1View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CColorImageAlpha1View 진단

#ifdef _DEBUG
void CColorImageAlpha1View::AssertValid() const
{
	CView::AssertValid();
}

void CColorImageAlpha1View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CColorImageAlpha1Doc* CColorImageAlpha1View::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CColorImageAlpha1Doc)));
	return (CColorImageAlpha1Doc*)m_pDocument;
}
#endif //_DEBUG


// CColorImageAlpha1View 메시지 처리기

// 초기화 함수

void CColorImageAlpha1View::OnBasicImage()
{
	CColorImageAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnBasicImage();
	Invalidate(TRUE);
	
}


/// <summary>
/// 화소값 조정 함수
/// </summary>

void CColorImageAlpha1View::OnEqualImage()
{
	CColorImageAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnEqualImage();
	Invalidate(TRUE);
}


void CColorImageAlpha1View::OnGrayscaleImage()
{
	CColorImageAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnGrayscaleImage();
	Invalidate(TRUE);
}


void CColorImageAlpha1View::OnAddImage()
{
	CColorImageAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnAddImage();
	Invalidate(TRUE);
}


void CColorImageAlpha1View::OnBwImage()
{
	CColorImageAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnBwImage();
	Invalidate(TRUE);
}


void CColorImageAlpha1View::OnAvgbwImage()
{
	CColorImageAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnAvgbwImage();
	Invalidate(TRUE);
}


void CColorImageAlpha1View::OnRevImage()
{
	CColorImageAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnRevImage();
	Invalidate(TRUE);
}


void CColorImageAlpha1View::OnMirrorImage()
{
	CColorImageAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnMirrorImage();
	Invalidate(TRUE);
}


void CColorImageAlpha1View::OnBrparaImage()
{
	CColorImageAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnBrparaImage();
	Invalidate(TRUE);
}


void CColorImageAlpha1View::OnDaparaImage()
{
	CColorImageAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnDaparaImage();
	Invalidate(TRUE);
}


void CColorImageAlpha1View::OnGammaImage()
{
	CColorImageAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnGammaImage();
	Invalidate(TRUE);
}

////////////////////////
/// 기하학 처리 함수//////
////////////////////////


void CColorImageAlpha1View::OnRotateImage()
{
	CColorImageAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnRotateImage();
	Invalidate(TRUE);
}

void CColorImageAlpha1View::OnMidZoomout()
{
	CColorImageAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnMidZoomout();
	Invalidate(TRUE);
}

void CColorImageAlpha1View::OnAvgZoomout()
{
	CColorImageAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnAvgZoomout();
	Invalidate(TRUE);
}

void CColorImageAlpha1View::OnZoomout()
{
	CColorImageAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnZoomout();
	Invalidate(TRUE);
}

void CColorImageAlpha1View::OnZoomin()
{
	CColorImageAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnZoomin();
	Invalidate(TRUE);
}



void CColorImageAlpha1View::OnBilZoomin()
{
	CColorImageAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnBilZoomin();
	Invalidate(TRUE);
}

void CColorImageAlpha1View::OnTwistImage()
{
	CColorImageAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnTwistImage();
	Invalidate(TRUE);
}

// 히스토그램 처리

void CColorImageAlpha1View::OnStrechImage()
{
	CColorImageAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnStrechImage();
	Invalidate(TRUE);
}

void CColorImageAlpha1View::OnHiseqImage()
{
	CColorImageAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnHiseqImage();
	Invalidate(TRUE);
}
void CColorImageAlpha1View::OnEndinImage()
{
	CColorImageAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnEndinImage();
	Invalidate(TRUE);
}

//화소 영역 처리 함수


void CColorImageAlpha1View::OnEmboss()
{
	CColorImageAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnEmboss();
	Invalidate(TRUE);
}


void CColorImageAlpha1View::OnEmbossHsi()
{
	CColorImageAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnEmbossHsi();
	Invalidate(TRUE);
}

void CColorImageAlpha1View::OnBlurImage()
{
	CColorImageAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnBlurImage();
	Invalidate(TRUE);
}

void CColorImageAlpha1View::OnSharpImage()
{
	CColorImageAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnSharpImage();
	Invalidate(TRUE);
}



void CColorImageAlpha1View::OnVeredgeImage()
{
	CColorImageAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnVeredgeImage();
	Invalidate(TRUE);
}

void CColorImageAlpha1View::OnPalImage()
{
	CColorImageAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnPalImage();
	Invalidate(TRUE);
}


// 칼라 채도 변경 함수

void CColorImageAlpha1View::OnChangeSatur()
{
	CColorImageAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnChangeSatur();
	Invalidate(TRUE);
}


void CColorImageAlpha1View::OnPickOrange()
{
	CColorImageAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnPickOrange();
	Invalidate(TRUE);
}























