
// ColorImageAlpha1Doc.h: CColorImageAlpha1Doc 클래스의 인터페이스
//


#pragma once


class CColorImageAlpha1Doc : public CDocument
{
protected: // serialization에서만 만들어집니다.
	CColorImageAlpha1Doc() noexcept;
	DECLARE_DYNCREATE(CColorImageAlpha1Doc)

// 특성입니다.
public:

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 구현입니다.
public:
	virtual ~CColorImageAlpha1Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 검색 처리기에 대한 검색 콘텐츠를 설정하는 도우미 함수
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	unsigned char** m_inImageR = NULL;
	unsigned char** m_inImageG = NULL;
	unsigned char** m_inImageB = NULL;
	unsigned char** m_outImageR = NULL;
	unsigned char** m_outImageG = NULL;
	unsigned char** m_outImageB = NULL;
	int m_inH = 0;
	int m_inW = 0;
	int m_outH = 0;
	int m_outW = 0;
	unsigned char** OnMalloc2D(int h, int w);
	template <typename T>
	void OnFree2D(T** memory, int h);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void OnCloseDocument();
	afx_msg void OnEqualImage();
	void OnFreeOutImage();
	void OnGrayscaleImage();
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	void OnAddImage();
	void OnBwImage();
	void OnAvgbwImage();
	void OnRevImage();
	void OnMirrorImage();
	void OnBrparaImage();
	void OnDaparaImage();
	void OnGammaImage();
	void OnChangeSatur();
	double* RGB2HSI(int R, int G, int B);
	unsigned char* HSI2RGB(double H, double S, double I);
	void OnPickOrange();
	void OnRotateImage();
	void AccumulateImage();
	char Acc = 0;
	unsigned char** m_basicImageR = NULL;
	unsigned char** m_basicImageG = NULL;
	unsigned char** m_basicImageB = NULL;
	void OnBasicImage();
	void OnMidZoomout();
	void OnAvgZoomout();
	int m_basicH = 0;
	int m_basicW = 0;
	void OnZoomout();
	void OnEmboss();
	double** OnMalloc2D_double(int h, int w);
	void OnEmbossHsi();
	void OnZoomin();
	void OnBilZoomin();
	void OnStrechImage();
	void OnHiseqImage();
	void OnEndinImage();
	void OnBlurImage();
	void OnSharpImage();
	void OnVeredgeImage();
	void OnPalImage();
	void OnTwistImage();
};
