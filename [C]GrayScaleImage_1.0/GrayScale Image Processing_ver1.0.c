//m_outImage[i][k] = 255;	#define  _CRT_SECURE_NO_WARNINGS
//	#include <stdio.h>
//	#include <stdbool.h>
//	#include <Windows.h>
//	#include <time.h>
//	#include <math.h>
//
//
//	///////////////
//	//전역변수부//
//	//////////////
//	HWND hwnd; // 윈도 화면(외부, 물리적)
//	HDC hdc; // 윈도 화면 (내부, 논리적)
//
//
//	FILE* rfp, * wfp;
//	char fileName[200];
//	int inH,inW, outH, outW, basicW, basicH;
//	unsigned char** inImage = NULL, ** outImage = NULL, ** basicImage = NULL,**medImage =NULL;
//
//
//	///////////////
//	//함수선언부//
//	//////////////
//	// 
//	// 영상 출입력 및 로드
//	void loadImage(), printImage(), saveImage(), printMenu(), ramSaveCheck(), goFirst(), endIn();
//	double getDoubleValue(); int getIntValue();
//
//	// 영상 처리 
//	void equalImage(), darkImage(), revImage(), bwImage(), avgbwImage(), midbwImage(), histoEqual(), avgZoomOut(), darkParaImage(), brightParaImage(), gammaImage(), addImage();
//	void zoomOut(),zoomIn(),zoomIn2(),rotate(), mirror(), midZoomOut(), biLinear(), emboss(), edge1(), edge2(), histoStretch(), blur(), sharp(), morphImage(),morphEmboss();
//
//	// 메모리 관리
//	void freeInputMemory(), freeOutputMemory(), freeMedMemory(), mallocInputMemory(), mallocMedMemory(), mallocOutputMemory(), mallocBasicMemory(), freeBasicMemory();
//	void freeDoubleMemory(double** memory, int h);
//	double** mallocDoubleMemory(int h, int w);
//
//
//
//
//	// 메인
//	void main() {
//		hwnd = GetForegroundWindow();
//		hdc = GetWindowDC(NULL); // Windows11 :NULL
//
//		char inKey = 0;
//		while (inKey != '9') {
//			printMenu();
//			inKey = _getch();
//			system("cls");
//			switch (inKey) {
//			case '0': loadImage(); break;
//			case '1': saveImage(); break;
//			case '2': goFirst(); break;
//			case 'a':case 'A': equalImage(); break;
//			case 'b':case 'B': addImage(); ramSaveCheck(); break;
//			case 'c':case 'C': darkImage(); ramSaveCheck(); break;
//			case 'd':case 'D': revImage(); ramSaveCheck(); break;
//			case 'e':case 'E': bwImage(); ramSaveCheck(); break;
//			case 'f':case 'F': avgbwImage(); ramSaveCheck(); break;
//			case 'g':case 'G': midbwImage(); ramSaveCheck(); break;
//			case 'h':case 'H': brightParaImage(); ramSaveCheck(); break;
//			case 'i':case 'I': darkParaImage(); ramSaveCheck(); break;
//			case 'j':case 'J': gammaImage(); ramSaveCheck(); break;
//			case 'k':case 'K': zoomOut(); ramSaveCheck(); break;
//			case 'l':case 'L': zoomIn(); ramSaveCheck(); break;
//			case 'm':case 'M': zoomIn2(); ramSaveCheck(); break;
//			case 'p':case 'P': avgZoomOut(); ramSaveCheck(); break;
//			case 'n':case 'N': rotate(); ramSaveCheck(); break;
//			case 'q':case 'Q': mirror(); ramSaveCheck(); break;
//			case 'w':case 'W': midZoomOut(); ramSaveCheck(); break;
//			case 'r':case 'R': histoStretch(); ramSaveCheck(); break;
//			case 't':case 'T': endIn(); ramSaveCheck(); break;
//			case 'y':case 'Y': histoEqual(); ramSaveCheck(); break;
//			case 'z':case 'Z': biLinear(); ramSaveCheck(); break;
//			case 'u':case 'U': emboss(); ramSaveCheck(); break;
//			case 'x':case 'X': blur(); ramSaveCheck(); break;
//			case '5': sharp(); ramSaveCheck(); break;
//			case '6': edge1(); ramSaveCheck(); break;
//			case '7': edge2(); ramSaveCheck(); break; 
//			case '8': morphEmboss(); ramSaveCheck(); break;
//			}
//		}
//		freeOutputMemory();
//		freeInputMemory();
//	}
//
//	///////////////
//	//함수 정의//
//	//////////////	
//
//	//공통함수
//	void printMenu() {
//		system("cls");
//		puts("\n ## GrayScale Image Processing (ver 1.0)##");
//		puts("\n     0. 열기    1. 저장     2.초기화     9.종료");
//		puts(" ======================================================");
//		puts(" = A. 동일                 = B. 밝게                  =");
//		puts(" = C. 어둡게               = D. 반전                  =");
//		puts(" = E. 입력값 흑백          = F. 평균값 흑백           =");
//		puts(" = G. 중앙값 흑백          = H. 파라볼라(bright)      =");
//		puts(" = I. 파라볼라(dark)       = J. 감마(권장 값:0.2~1.8) =");
//		puts(" = K. 축소                 = P. 축소(평균값)          =");
//		puts(" = W. 축소(중앙값)         = L. 확대                  =");
//		puts(" = M. 확대2                = Z. 확대 (선형보간법)     =");
//		puts(" = N. 회전                 = Q. 미러링                =");
//		puts(" = R. 히스토그램 스트레칭  = T. 엔드-인               =");
//		puts(" = Y. 히스토그램 평활화    = U. 엠보싱                =");
//		puts(" = X. 블러링               = 5. 샤프닝                =");
//		puts(" = 6. 수직에지검출         = 7. 수평에지검출          =");
//		puts(" = 8. 엠보싱(+모핑)                                   =");
//		puts(" ======================================================");
//	}
//
//
//	void loadImage() {
//		freeOutputMemory();
//		char fullName[250] = "D:/RAW/Etc_Raw(squre)/";
//		char tmpName[50];
//		printf("파일명-->");
//		scanf("%s", tmpName);
//		strcat(fullName, tmpName);
//		strcat(fullName, ".raw");
//		strcpy(fileName, fullName);
//
//		rfp = fopen(fileName, "rb");
//		if (rfp == NULL) {
//			MessageBox(hwnd, L"해당 파일이 존재하지 않습니다.", L"에러", MB_OK | MB_ICONERROR);
//			return;
//		}
//		fseek(rfp, 0L, SEEK_END);
//		long long fsize = ftell(rfp); 
//		fclose(rfp);
//
//		freeInputMemory();
//		freeBasicMemory();
//
//		inH = inW = basicH = basicW = sqrt(fsize); 
//		mallocInputMemory();
//		mallocBasicMemory();
//		 //파일 -> 메모리
//		rfp = fopen(fileName, "rb");
//		for (int i = 0; i < inH; i++) {
//			fread(inImage[i], sizeof(unsigned char), inW, rfp);
//		}
//	
//
//		fclose(rfp);
//		rfp = fopen(fileName, "rb");
//		for (int i = 0; i < inH; i++) {
//			fread(basicImage[i], sizeof(unsigned char), basicW, rfp);
//		}
//		equalImage();
//	}
//
//	void ramSaveCheck() {
//		printf("효과를 누적하려면 '0'\n취소하고 나가려면 아무키나 누르세요.\n");
//		int inkey = _getch();
//		if (inkey == '0') {
//			freeInputMemory();
//			inH = outH;
//			inW = outW;
//			mallocInputMemory();
//			for (int i = 0; i < inH; i++) {
//				for (int k = 0; k < inW; k++) {
//					inImage[i][k] = outImage[i][k];
//				}
//			}
//		}
//		else {
//			equalImage();
//		}
//	}
//
//	void saveImage() {
//		if (outImage == NULL) {
//			return;
//		}
//		char fullName[200] = "D:/RAW/Etc_Raw(squre)/";
//		char tmpName[50];
//		printf("파일명-->");
//		scanf("%s", tmpName);
//		strcat(fullName, tmpName);
//		strcat(fullName, ".raw");
//		strcpy(fileName, fullName);
//
//		wfp = fopen(fileName, "wb");
//		for (int i = 0; i < outH; i++) {
//			fwrite(outImage[i], sizeof(unsigned char), outW, wfp);
//		}
//		fclose(wfp);
//		MessageBox(hwnd, L"저장완료", L"저장 창", NULL);
//	}
//
//	void goFirst() { //초기화
//	
//		freeInputMemory();
//		freeOutputMemory();
//		inH = outH = basicH;
//		inW = outW = basicW;
//		mallocInputMemory();
//		mallocOutputMemory();
//
//		for (int i = 0; i < basicH; i++) {
//			for (int k = 0; k < basicW; k++) {
//				outImage[i][k] = inImage[i][k] = basicImage[i][k];
//			}
//		}
//	
//		printImage();
//	}
//
//	int getIntValue() {
//		int retValue;
//		printf("정수값--> ");
//		scanf("%d", &retValue);
//		return retValue;
//	}
//
//	double getDoubleValue() {
//		double retValue;
//		printf("실수값--> ");
//		scanf("%lf", &retValue);
//		return retValue;
//	}
//
//	void freeInputMemory() {
//		if (inImage == NULL) {
//			return;
//		}
//		for (int i = 0; i < inH; i++) {
//			free(inImage[i]);
//		}
//		free(inImage);
//		inImage = NULL;
//	}
//
//	void freeBasicMemory() {
//		if (basicImage == NULL) {
//			return;
//		}
//		for (int i = 0; i < inH; i++) {
//			free(basicImage[i]);
//		}
//		free(basicImage);
//		basicImage = NULL;
//	}
//
//	void freeMedMemory() {
//		if (medImage == NULL) {
//			return;
//		}
//		for (int i = 0; i < inH; i++) {
//			free(medImage[i]);
//		}
//		free(medImage);
//		medImage = NULL;
//	}
//
//	void mallocInputMemory(){
//		inImage = (unsigned char**)malloc(sizeof(unsigned char*) * inH);
//		for (int i = 0; i < inH; i++) {
//			inImage[i] = (unsigned char*)malloc(sizeof(unsigned char) * inW);
//		}
//	}
//
//	void mallocMedMemory() {
//		medImage = (unsigned char**)malloc(sizeof(unsigned char*) * inH);
//		for (int i = 0; i < inH; i++) {
//			medImage[i] = (unsigned char*)malloc(sizeof(unsigned char) * inW);
//		}
//	}
//
//	void mallocBasicMemory() {
//		basicImage = (unsigned char**)malloc(sizeof(unsigned char*) * basicH);
//		for (int i = 0; i < basicH; i++) {
//			basicImage[i] = (unsigned char*)malloc(sizeof(unsigned char) * basicW);
//		}
//	}
//
//	void mallocOutputMemory(){
//		outImage = (unsigned char**)malloc(sizeof(unsigned char*) * outH);
//		for (int i = 0; i < outH; i++) {
//			outImage[i] = (unsigned char*)malloc(sizeof(unsigned char) * outW);
//		}
//	}
//
//	double** mallocDoubleMemory(int h, int w) {
//		double** retMemory;
//		retMemory = (double**)malloc(sizeof(double*) * h);
//		for (int i = 0; i < h; i++) {
//			retMemory[i] = (double*)malloc(sizeof(double) * w);
//		
//		}
//		return retMemory;
//	}
//
//	void freeOutputMemory() {
//		if (outImage == NULL) {
//			return;
//		}
//		for (int i = 0; i < outH; i++) {
//			free(outImage[i]);
//		}
//		free(outImage);
//		outImage = NULL;
//	}
//
//	void freeDoubleMemory(double**memory, int h) {
//		if (memory == NULL) {
//			return;
//		}
//		for (int i = 0; i < h; i++) {
//			free(memory[i]);
//		}
//		free(memory);
//		memory = NULL;
//	}
//
//	void printImage() {
//		system("cls");
//		for (int i = 0; i < outH; i++) {
//			for (int k = 0; k < outW; k++) {
//				int px = outImage[i][k];
//				SetPixel(hdc, k+850, i+50, RGB(px, px, px));
//			}
//		}
//	}
//
//	void printMedImage() {
//		system("cls");
//		for (int i = 0; i < outH; i++) {
//			for (int k = 0; k < outW; k++) {
//				int px = medImage[i][k];
//				SetPixel(hdc, k + 850, i + 50, RGB(px, px, px));
//			}
//		}
//	}
//
//	void morphImage() {
//		double alpha;
//		int numFrames = 10;
//		mallocMedMemory();
//		for (int frame = 0; frame < numFrames; frame++) {
//			alpha = (double)frame / (numFrames - 1);
//			for (int i = 0; i < inH; i++) {
//				for (int j = 0; j < outW; j++) {
//					int pixelValue = (1 - alpha) * inImage[i][j] + alpha * outImage[i][j];
//					medImage[i][j] = (unsigned char)pixelValue;
//				}
//			}
//			printMedImage();
//		}
//		freeMedMemory();
//	}
//
//
//	//====영상처리 함수====
//
//	void equalImage() {
//		//매모리 해제
//		freeOutputMemory();
//		//중요!! 출력 이미지의크기를 결정해야 한다. ->알고리즘에 의존
//		outH = inH;
//		outW = inW;
//		mallocOutputMemory();
//		for (int i = 0; i < inH; i++) {
//			for (int k = 0; k < inW; k++) {
//				outImage[i][k] = inImage[i][k];
//			}
//		}
//		printImage();
//	}
//
//	void addImage() {
//		freeOutputMemory();
//		outH = inH;
//		outW = inW;
//		mallocOutputMemory();
//		int val = getIntValue();
//		for (int i = 0; i < inH; i++) {
//			for (int k = 0; k < inW; k++) {
//				if (inImage[i][k] + val > 255) {
//					outImage[i][k] = 255;
//				}
//				else { outImage[i][k] = inImage[i][k]+ val; }
//			}
//		}
//		printImage();
//	}
//
//	void darkImage() {
//		freeOutputMemory();
//		outH = inH;
//		outW = inW;
//		mallocOutputMemory();
//		int val = getIntValue();
//		for (int i = 0; i < inH; i++) {
//			for (int k = 0; k < inW; k++) {
//				if (inImage[i][k] - val < 0) {
//					outImage[i][k] = 0;
//				}
//				else { outImage[i][k] -= val; }
//			}
//		}
//		printImage();
//	}
//
//	void revImage() {
//		freeOutputMemory();
//		outH = inH;
//		outW = inW;
//		mallocOutputMemory();
//		for (int i = 0; i < inH; i++) {
//			for (int k = 0; k < inW; k++) {
//				outImage[i][k] = 255 - inImage[i][k];
//			}
//		}
//		printImage();
//	}
//
//	void bwImage() {
//		freeOutputMemory();
//		outH = inH;
//		outW = inW;
//		mallocOutputMemory();
//		int val = getIntValue();
//		for (int i = 0; i < inH; i++) {
//			for (int k = 0; k < inW; k++) {
//				if (inImage[i][k] >= val) {
//					outImage[i][k] = 255;
//				}
//				else {
//					outImage[i][k] = 0;
//				}
//			}
//		}
//		printImage();
//	}
//
//	void avgbwImage() {
//		freeOutputMemory();
//		outH = inH;
//		outW = inW;
//		mallocOutputMemory();
//		int val = avgImage();
//		for (int i = 0; i < inH; i++) {
//			for (int k = 0; k < inW; k++) {
//				if (inImage[i][k] >= val) {
//					outImage[i][k] = 255;
//				}
//				else {
//					outImage[i][k] = 0;
//				}
//			}
//		}
//		printImage();
//	}
//
//void midbwImage() {
//	freeOutputMemory();
//	outH = inH;
//	outW = inW;
//	mallocOutputMemory();
//	int val = midImage();
//	printf("중앙값 : %d", val);
//	for (int i = 0; i < inH; i++) {
//		for (int k = 0; k < inW; k++) {
//			if (inImage[i][k] >= val) {
//				outImage[i][k] = 255;
//			}
//			else {
//				outImage[i][k] = 0;
//			}
//		}
//	}
//	printImage();
//}
//
//void darkParaImage() {
//	freeOutputMemory();
//	outH = inH;
//	outW = inW;
//	mallocOutputMemory();
//	for (int i = 0; i < inH; i++) {
//		for (int k = 0; k < inW; k++) {
//			outImage[i][k] = 255.0 - 255.0 * pow(inImage[i][k] / 172 - 1, 2);
//		}
//	}
//	printImage();
//}
//
//void brightParaImage() {
//	freeOutputMemory();
//	outH = inH;
//	outW = inW;
//	mallocOutputMemory();
//	for (int i = 0; i < inH; i++) {
//		for (int k = 0; k < inW; k++) {
//			outImage[i][k] = 255.0 * pow(inImage[i][k] / 172 - 1, 2);
//		}
//	}
//	printImage();
//}
//
//void gammaImage() {
//	freeOutputMemory();
//	outH = inH;
//	outW = inW;
//	mallocOutputMemory();
//	double gamma = getDoubleValue();
//	for (int i = 0; i < inH; i++) {
//		for (int k = 0; k < inW; k++) {
//			outImage[i][k] = 255.0 * pow(inImage[i][k] /255.0, gamma);
//		}
//	}
//	printImage();
//}
//
//void zoomOut() {
//	int scale = getIntValue();
//	freeOutputMemory();
//	outH = (int)(inH/scale);
//	outW = (int)(inW/scale);
//	mallocOutputMemory();
//	for (int i = 0; i < inH; i++) {
//		for (int k = 0; k < inW; k++) {
//			outImage[(int)(i/scale)][(int)(k/scale)] = inImage[i][k];
//		}
//	}
//	printImage();
//}
//
//void zoomIn() {
//	int scale = getIntValue();
//	freeOutputMemory();
//	outH = (int)(inH * scale);
//	outW = (int)(inW * scale);
//	mallocOutputMemory();
//	for (int i = 0; i < inH; i++) {
//		for (int k = 0; k < inW; k++) {
//			outImage[(int)(i * scale)][(int)(k * scale)] = inImage[i][k];
//		}
//	}
//	printImage();
//}
//
//void zoomIn2() {
//	int scale = getIntValue();
//	freeOutputMemory();
//	outH = (int)(inH * scale);
//	outW = (int)(inW * scale);
//	mallocOutputMemory();
//	for (int i = 0; i < outH; i++) {
//		for (int k = 0; k < outW; k++) {
//			outImage[i][k] = inImage[(int)(i/scale)][(int)(k/scale)];
//		}
//	}
//	printImage();
//}
//
//void rotate() {
//	freeOutputMemory();
//	outH = inH;
//	outW = inW;
//	mallocOutputMemory();
//	int degree = getIntValue();
//	double radian = degree * 3.1415926 / 180.0;
//	int centX = (int)(outH / 2);
//	int centY = (int)(outW / 2);
//	int avg = avgImage();
//	for (int i = 0; i < outH; i++) {
//		for (int k = 0; k < outW; k++) {
//			int xd = i;
//			int yd = k;
//
//			int xs = (int)(cos(radian) * (xd - centX) - sin(radian) * (yd - centY) + centX);
//			int ys = (int)(sin(radian) * (xd - centX) + cos(radian) * (yd - centY) + centY);
//
//			if (xs >= 0 && xs < inH && ys >= 0 && ys < inW) {
//				outImage[xd][yd] = inImage[xs][ys];
//			}
//			else {
//				outImage[xd][yd] = avg;
//			}
//		}
//	}
//	printImage();
//}
//
//void mirror() {
//	freeOutputMemory();
//	outH = inH;
//	outW = inW;
//	mallocOutputMemory();
//	for (int i = 0; i < outH; i++) {
//		for (int k = 0; k < outW; k++) {
//			outImage[i][outW-k-1] = inImage[i][k];
//		}
//	}
//	printImage();
//}
//
//void midZoomOut() {
//	int scale = getIntValue();
//	freeOutputMemory();
//	outH = inH / scale;
//	outW = inW / scale;
//	mallocOutputMemory(); 
//	unsigned char* block;
//	for (int i = 0; i < outH; i++) {
//		for (int j = 0; j < outW; j++) {
//			block = (unsigned char*)malloc(scale * scale * sizeof(unsigned char));
//			short count = 0;
//			for (int k = i * scale; k < (i + 1) * scale; k++) {
//				for (int p = j * scale; p < (j + 1) * scale; p++) {
//					block[count++] = inImage[k][p];
//				}
//			}
//			int temp;
//			for (int x = 0; x < count; x++) {
//				for (int y = 0; y < count - 1 - x; y++) {
//					if (block[y] > block[y + 1]) {
//						temp = block[y];
//						block[y] = block[y + 1];
//						block[y + 1] = temp;
//					}
//				}
//			}
//			int mid= block[(int)(count / 2)];
//			outImage[i][j] = mid;
//			free(block);
//		}
//	}
//	printImage();
//}
//
//void biLinear() { // 양선형 보간법
//	int scale = getIntValue(); // 이미지 크기를 조절할 배율을 입력 받습니다.
//	freeOutputMemory(); // 출력 이미지 메모리를 해제합니다.
//	outH = (int)(inH * scale); // 출력 이미지의 높이를 조정합니다.
//	outW = (int)(inW * scale); // 출력 이미지의 너비를 조정합니다.
//	mallocOutputMemory(); // 출력 이미지를 위한 메모리를 할당합니다.
//
//	for (int i = 0; i < outH; i++) { // 출력 이미지의 각 픽셀에 대해 반복합니다.
//		for (int k = 0; k < outW; k++) {
//			double orig_i = i / (double)scale; 
//			double orig_k = k / (double)scale; 
//			int i1 = (int)orig_i; 
//			int i2 = i1 + 1;
//			int k1 = (int)orig_k;
//			int k2 = k1 + 1;
//			double biY = orig_i - i1; // 세로 방향의 거리에 대한 가중치를 계산합니다.
//			double biX = orig_k - k1; // 가로 방향의 거리에 대한 가중치를 계산합니다.
//
//			if (i2 >= inH) i2 = inH - 1; 
//			if (k2 >= inW) k2 = inW - 1; 
//
//			double interpolatedValue =
//				(1 - biY) * ((1 - biX) * inImage[i1][k1] + biX * inImage[i1][k2]) +
//				biY * ((1 - biX) * inImage[i2][k1] + biX * inImage[i2][k2]);
//
//			outImage[i][k] = interpolatedValue; // 계산된 값을 출력 이미지에 저장합니다.
//		}
//	}
//
//	printImage(); // 보간된 이미지를 출력합니다.
//}
//
//void avgZoomOut() {
//	int scale = getIntValue();
//	freeOutputMemory();
//	outH = inH / scale;
//	outW = inW / scale;
//	mallocOutputMemory();
//	for (int i = 0; i < outH; i++) {
//		for (int j = 0; j < outW; j++) {
//			int sum = 0;
//			for (int k = i * scale; k < (i + 1) * scale; k++) {
//				for (int p = j * scale; p < (j + 1) * scale; p++) {
//					sum += inImage[k][p];
//				}
//			}
//			int avg = sum / (scale * (double)scale);
//			outImage[i][j] = avg;
//		}
//	}
//	printImage();
//}
//
//void histoStretch(){
//	freeOutputMemory();
//	outH = inH;
//	outW = inW;
//	mallocOutputMemory();
//	// new pixel = (old pixel - low)/(high-low)*255
//	int high=inImage[0][0], low = inImage[0][0];
//	for (int i = 0; i < inH; i++) {
//		for (int k = 0; k < inW; k++) {
//			if (inImage[i][k] < low) {
//				low = inImage[i][k];
//			}
//			if (inImage[i][k] > high) {
//				high = inImage[i][k];
//			}
//		}
//	}
//	for (int i = 0; i < outH; i++) {
//		for (int k = 0; k < outW; k++) {
//			int old = inImage[i][k];
//			int new = (int)((old - low) / (double)(high - low) * 255.0);
//			if (new > 255) {
//				new = 255;
//			} 
//			if (new < 0) {
//				new = 0;
//			}
//			outImage[i][k] = new;
//		}
//	}
//	printImage();
//}
//
//void endIn() {
//	freeOutputMemory();
//	outH = inH;
//	outW = inW;
//	mallocOutputMemory();
//	int high = inImage[0][0], low = inImage[0][0];
//	for (int i = 0; i < inH; i++) {
//		for (int k = 0; k < inW; k++) {
//			if (inImage[i][k] < low) {
//				low = inImage[i][k];
//			}
//			if (inImage[i][k] > high) {
//				high = inImage[i][k];
//			}
//		}
//	}
//
//	high -= 50;
//	low += 50;
//	for (int i = 0; i < outH; i++) {
//		for (int k = 0; k < outW; k++) {
//			int old = inImage[i][k];
//			int new = (int)((old - low) / (double)(high - low) * 255.0);
//			if (new > 255) {
//				new = 255;
//			}
//			if (new < 0) {
//				new = 0;
//			}
//			outImage[i][k] = new;
//		}
//	}
//	printImage();
//}
//
//void histoEqual() {
//	freeOutputMemory();
//	outH = inH;
//	outW = inW;
//	mallocOutputMemory();
//	int histo[256] = { 0, };
//	//1 단계 : 빈도수 세기
//	for (int i = 0; i < inH; i++) {
//		for (int k = 0; k < inW; k++) {
//			histo[inImage[i][k]]++;
//		}
//	}
//	// 2단계 : 누적 히스토그램 생성
//	int sumHisto[256] = { 0, };
//	sumHisto[0] = histo[0];
//	for (int i = 1; i < 256; i++) {
//		sumHisto[i] = sumHisto[i - 1] + histo[i];
//	}
//	// 3단계 : 정규화된 히스토그램 생성
//	double normalHisto[256] = { 1.0, };
//	for (int i = 0; i < 256; i++) {
//		normalHisto[i] = sumHisto[i] * (1.0 / (inH * inW)) * 255.0;
//	}
//	for (int i = 0; i < inH; i++) {
//		for (int k = 0; k < inW; k++) {
//			outImage[i][k] = (unsigned char)(normalHisto[inImage[i][k]]);
//		}
//	}
//	printImage();
//}
//
//void emboss() { //화소영역처리
//	freeOutputMemory();
//	outH = inH;
//	outW = inW;
//	mallocOutputMemory();
//
//	double mask[3][3] = {{-1.0, 0.0, 0.0}, 
//	                   	{ 0.0, 0.0, 0.0 }, 
//	 	                { 0.0, 0.0, 1.0 } }; 
//	double** tmpInImage = mallocDoubleMemory(inH + 2, inW + 2);
//	double** tmpOutImage = mallocDoubleMemory(outH, outW);
//	for (int i = 0; i < inH+2; i++) {
//		for (int k = 0; k < inW+2; k++) {
//			tmpInImage[i][k] = 127;
//		}
//	}
//	for (int i = 0; i < inH; i++) {
//		for (int k = 0; k < inW; k++) {
//			tmpInImage[i + 1][k + 1] = inImage[i][k];
//		}
//	}
//
//	double S;
//	for (int i = 0; i < inH; i++) {
//		for (int k = 0; k < inW; k++) {
//			S = 0.0;
//			for (int m = 0; m < 3; m++) {
//				for (int n = 0; n < 3; n++) {
//					S += tmpInImage[i + m][k + n] * mask[m][n];
//				}
//			}
//			tmpOutImage[i][k] = S;
//		}
//	}
//	for (int i = 0; i < outH; i++) {	
//		for (int k = 0; k < outW; k++) {
//			tmpOutImage[i][k] += 127.0;
//			if (tmpOutImage[i][k] < 0.0) {
//				outImage[i][k] = 0;
//			}
//			else if (tmpOutImage[i][k] > 255.0) {
//				outImage[i][k] = 255;
//			}
//			else {
//				outImage[i][k] = (unsigned char)tmpOutImage[i][k];
//			}
//		}
//	}
//	freeDoubleMemory(tmpInImage, inH + 2);
//	freeDoubleMemory(tmpOutImage, outH);
//	printImage();
//}
//void morphEmboss() { //화소영역처리
//	freeOutputMemory();
//	outH = inH;
//	outW = inW;
//	mallocOutputMemory();
//
//	double mask[3][3] = { {-1.0, 0.0, 0.0},
//						{ 0.0, 0.0, 0.0 },
//						{ 0.0, 0.0, 1.0 } }; // 마스크는 무조건 합계가 0 or 1
//	// 임시 메모리 할당(실수형)
//	double** tmpInImage = mallocDoubleMemory(inH + 2, inW + 2);
//	double** tmpOutImage = mallocDoubleMemory(outH, outW);
//	// 임시입력메모리를 초기화(127): 필요시 평균값
//	for (int i = 0; i < inH + 2; i++) {
//		for (int k = 0; k < inW + 2; k++) {
//			tmpInImage[i][k] = 127;
//		}
//	}
//	// 입력이미지 --> 임시 입력 이미지
//	for (int i = 0; i < inH; i++) {
//		for (int k = 0; k < inW; k++) {
//			tmpInImage[i + 1][k + 1] = inImage[i][k];
//		}
//	}
//	// **회선 연산**
//	double S;
//	for (int i = 0; i < inH; i++) {
//		for (int k = 0; k < inW; k++) {
//			S = 0.0;
//			for (int m = 0; m < 3; m++) {
//				for (int n = 0; n < 3; n++) {
//					S += tmpInImage[i + m][k + n] * mask[m][n];
//				}
//			}
//			tmpOutImage[i][k] = S;
//		}
//	}
//	for (int i = 0; i < outH; i++) {
//		for (int k = 0; k < outW; k++) {
//			tmpOutImage[i][k] += 127.0;
//			if (tmpOutImage[i][k] < 0.0) {
//				outImage[i][k] = 0;
//			}
//			else if (tmpOutImage[i][k] > 255.0) {
//				outImage[i][k] = 255;
//			}
//			else {
//				outImage[i][k] = (unsigned char)tmpOutImage[i][k];
//			}
//		}
//	}
//	freeDoubleMemory(tmpInImage, inH + 2);
//	freeDoubleMemory(tmpOutImage, outH);
//	morphImage();
//}
//
//void blur() {
//	freeOutputMemory();
//	outH = inH;
//	outW = inW;
//	mallocOutputMemory();
//
//	double mask[3][3] = { {1./9, 1. / 9, 1. / 9},
//						{ 1. / 9, 1. / 9,1. / 9 },
//						{ 1. / 9, 1. / 9, 1. / 9 } }; // 마스크는 무조건 합계가 0 or 1
//	// 임시 메모리 할당(실수형)
//	double** tmpInImage = mallocDoubleMemory(inH + 2, inW + 2);
//	double** tmpOutImage = mallocDoubleMemory(outH, outW);
//	// 임시입력메모리를 초기화(127): 필요시 평균값
//	for (int i = 0; i < inH + 2; i++) {
//		for (int k = 0; k < inW + 2; k++) {
//			tmpInImage[i][k] = 127;
//		}
//	}
//	// 입력이미지 --> 임시 입력 이미지
//	for (int i = 0; i < inH; i++) {
//		for (int k = 0; k < inW; k++) {
//			tmpInImage[i + 1][k + 1] = inImage[i][k];
//		}
//	}
//	// **회선 연산**
//	double S;
//	for (int i = 0; i < inH; i++) {
//		for (int k = 0; k < inW; k++) {
//			S = 0.0;
//			for (int m = 0; m < 3; m++) {
//				for (int n = 0; n < 3; n++) {
//					S += tmpInImage[i + m][k + n] * mask[m][n];
//				}
//			}
//			tmpOutImage[i][k] = S;
//		}
//	}
//	for (int i = 0; i < outH; i++) {
//		for (int k = 0; k < outW; k++) {
//			if (tmpOutImage[i][k] < 0.0) {
//				outImage[i][k] = 0;
//			}
//			else if (tmpOutImage[i][k] > 255.0) {
//				outImage[i][k] = 255;
//			}
//			else {
//				outImage[i][k] = (unsigned char)tmpOutImage[i][k];
//			}
//		}
//	}
//	freeDoubleMemory(tmpInImage, inH + 2);
//	freeDoubleMemory(tmpOutImage, outH);
//	printImage();
//}
//
//void sharp() {
//	freeOutputMemory();
//	outH = inH;
//	outW = inW;
//	mallocOutputMemory();
//
//	double mask[3][3] = { {-1,-1, -1},
//						{ -1,9, -1 },
//						{ -1,-1, -1 } }; // 마스크는 무조건 합계가 0 or 1
//	// 임시 메모리 할당(실수형)
//	double** tmpInImage = mallocDoubleMemory(inH + 2, inW + 2);
//	double** tmpOutImage = mallocDoubleMemory(outH, outW);
//	// 임시입력메모리를 초기화(127): 필요시 평균값
//	for (int i = 0; i < inH + 2; i++) {
//		for (int k = 0; k < inW + 2; k++) {
//			tmpInImage[i][k] = 127;
//		}
//	}
//	// 입력이미지 --> 임시 입력 이미지
//	for (int i = 0; i < inH; i++) {
//		for (int k = 0; k < inW; k++) {
//			tmpInImage[i + 1][k + 1] = inImage[i][k];
//		}
//	}
//	// **회선 연산**
//	double S;
//	for (int i = 0; i < inH; i++) {
//		for (int k = 0; k < inW; k++) {
//			S = 0.0;
//			for (int m = 0; m < 3; m++) {
//				for (int n = 0; n < 3; n++) {
//					S += tmpInImage[i + m][k + n] * mask[m][n];
//				}
//			}
//			tmpOutImage[i][k] = S;
//		}
//	}
//	for (int i = 0; i < outH; i++) {
//		for (int k = 0; k < outW; k++) {
//			if (tmpOutImage[i][k] < 0.0) {
//				outImage[i][k] = 0;
//			}
//			else if (tmpOutImage[i][k] > 255.0) {
//				outImage[i][k] = 255;
//			}
//			else {
//				outImage[i][k] = (unsigned char)tmpOutImage[i][k];
//			}
//		}
//	}
//	freeDoubleMemory(tmpInImage, inH + 2);
//	freeDoubleMemory(tmpOutImage, outH);
//	printImage();
//}
//
//void edge1() { // 수직검출 알고리즘
//	freeOutputMemory();
//	outH = inH;
//	outW = inW;
//	mallocOutputMemory();
//
//	double mask[3][3] = { {0.0, 0.0, 0.0},
//						{ -1.0, 1.0,0.0 },
//						{0.0, 0.0, 0.0 } }; // 마스크는 무조건 합계가 0 or 1
//	// 임시 메모리 할당(실수형)
//	double** tmpInImage = mallocDoubleMemory(inH + 2, inW + 2);
//	double** tmpOutImage = mallocDoubleMemory(outH, outW);
//	// 임시입력메모리를 초기화(127): 필요시 평균값
//	for (int i = 0; i < inH + 2; i++) {
//		for (int k = 0; k < inW + 2; k++) {
//			tmpInImage[i][k] = 127;
//		}
//	}
//	// 입력이미지 --> 임시 입력 이미지
//	for (int i = 0; i < inH; i++) {
//		for (int k = 0; k < inW; k++) {
//			tmpInImage[i + 1][k + 1] = inImage[i][k];
//		}
//	}
//	// **회선 연산**
//	double S;
//	for (int i = 0; i < inH; i++) {
//		for (int k = 0; k < inW; k++) {
//			S = 0.0;
//			for (int m = 0; m < 3; m++) {
//				for (int n = 0; n < 3; n++) {
//					S += tmpInImage[i + m][k + n] * mask[m][n];
//				}
//			}
//			tmpOutImage[i][k] = S;
//		}
//	}
//	for (int i = 0; i < outH; i++) {
//		for (int k = 0; k < outW; k++) {
//			if (tmpOutImage[i][k] < 0.0) {
//				outImage[i][k] = 0;
//			}
//			else if (tmpOutImage[i][k] > 255.0) {
//				outImage[i][k] = 255;
//			}
//			else {
//				outImage[i][k] = (unsigned char)tmpOutImage[i][k];
//			}
//		}
//	}
//	freeDoubleMemory(tmpInImage, inH + 2);
//	freeDoubleMemory(tmpOutImage, outH);
//	printImage();
//}
//
//void edge2() { // 수평에지
//	freeOutputMemory();
//	outH = inH;
//	outW = inW;
//	mallocOutputMemory();
//
//	double mask[3][3] = { {0.0, -1.0, 0.0},
//						{ 0.0, 1.0,0.0 },
//						{0.0, 0.0, 0.0 } }; // 마스크는 무조건 합계가 0 or 1
//	// 임시 메모리 할당(실수형)
//	double** tmpInImage = mallocDoubleMemory(inH + 2, inW + 2);
//	double** tmpOutImage = mallocDoubleMemory(outH, outW);
//	// 임시입력메모리를 초기화(127): 필요시 평균값
//	for (int i = 0; i < inH + 2; i++) {
//		for (int k = 0; k < inW + 2; k++) {
//			tmpInImage[i][k] = 127;
//		}
//	}
//	// 입력이미지 --> 임시 입력 이미지
//	for (int i = 0; i < inH; i++) {
//		for (int k = 0; k < inW; k++) {
//			tmpInImage[i + 1][k + 1] = inImage[i][k];
//		}
//	}
//	// **회선 연산**
//	double S;
//	for (int i = 0; i < inH; i++) {
//		for (int k = 0; k < inW; k++) {
//			S = 0.0;
//			for (int m = 0; m < 3; m++) {
//				for (int n = 0; n < 3; n++) {
//					S += tmpInImage[i + m][k + n] * mask[m][n];
//				}
//			}
//			tmpOutImage[i][k] = S;
//		}
//	}
//	for (int i = 0; i < outH; i++) {
//		for (int k = 0; k < outW; k++) {
//			if (tmpOutImage[i][k] < 0.0) {
//				outImage[i][k] = 0;
//			}
//			else if (tmpOutImage[i][k] > 255.0) {
//				outImage[i][k] = 255;
//			}
//			else {
//				outImage[i][k] = (unsigned char)tmpOutImage[i][k];
//			}
//		}
//	}
//	freeDoubleMemory(tmpInImage, inH + 2);
//	freeDoubleMemory(tmpOutImage, outH);
//	printImage();
//}
//
//
//// 리턴 함수
//int avgImage() {
//	int sum = 0;
//	int avgval = 0;
//	for (int i = 0; i < inH; i++) {
//		for (int k = 0; k < inW; k++) {
//			sum += inImage[i][k];
//		}
//	}
//	avgval = (int)sum / (inH * inW);
//	return avgval;
//}
//
//int midImage() {
//	int count = 0;
//	int temp = 0;
//	int midvalue = 0;
//	unsigned char* dim1;
//	dim1 = (unsigned char*)malloc(sizeof(unsigned char) * inH * inW);
//	for (int i = 0; i < inH; i++) {
//		for (int k = 0; k < inW; k++) {
//			dim1[count++] = inImage[i][k];
//		}
//	}
//	for (int i = 0; i < count; i++) {
//		for (int k = 0; k < count - 1 - i; k++) {
//			if (dim1[k] > dim1[k + 1]) {
//				temp = dim1[k];
//				dim1[k] = dim1[k + 1];
//				dim1[k + 1] = temp;
//			}
//		}
//	}
//	midvalue = dim1[(int)(count / 2)];
//	free(dim1);
//	return midvalue;
//}