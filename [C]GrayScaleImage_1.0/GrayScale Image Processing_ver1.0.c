//m_outImage[i][k] = 255;	#define  _CRT_SECURE_NO_WARNINGS
//	#include <stdio.h>
//	#include <stdbool.h>
//	#include <Windows.h>
//	#include <time.h>
//	#include <math.h>
//
//
//	///////////////
//	//����������//
//	//////////////
//	HWND hwnd; // ���� ȭ��(�ܺ�, ������)
//	HDC hdc; // ���� ȭ�� (����, ����)
//
//
//	FILE* rfp, * wfp;
//	char fileName[200];
//	int inH,inW, outH, outW, basicW, basicH;
//	unsigned char** inImage = NULL, ** outImage = NULL, ** basicImage = NULL,**medImage =NULL;
//
//
//	///////////////
//	//�Լ������//
//	//////////////
//	// 
//	// ���� ���Է� �� �ε�
//	void loadImage(), printImage(), saveImage(), printMenu(), ramSaveCheck(), goFirst(), endIn();
//	double getDoubleValue(); int getIntValue();
//
//	// ���� ó�� 
//	void equalImage(), darkImage(), revImage(), bwImage(), avgbwImage(), midbwImage(), histoEqual(), avgZoomOut(), darkParaImage(), brightParaImage(), gammaImage(), addImage();
//	void zoomOut(),zoomIn(),zoomIn2(),rotate(), mirror(), midZoomOut(), biLinear(), emboss(), edge1(), edge2(), histoStretch(), blur(), sharp(), morphImage(),morphEmboss();
//
//	// �޸� ����
//	void freeInputMemory(), freeOutputMemory(), freeMedMemory(), mallocInputMemory(), mallocMedMemory(), mallocOutputMemory(), mallocBasicMemory(), freeBasicMemory();
//	void freeDoubleMemory(double** memory, int h);
//	double** mallocDoubleMemory(int h, int w);
//
//
//
//
//	// ����
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
//	//�Լ� ����//
//	//////////////	
//
//	//�����Լ�
//	void printMenu() {
//		system("cls");
//		puts("\n ## GrayScale Image Processing (ver 1.0)##");
//		puts("\n     0. ����    1. ����     2.�ʱ�ȭ     9.����");
//		puts(" ======================================================");
//		puts(" = A. ����                 = B. ���                  =");
//		puts(" = C. ��Ӱ�               = D. ����                  =");
//		puts(" = E. �Է°� ���          = F. ��հ� ���           =");
//		puts(" = G. �߾Ӱ� ���          = H. �Ķ󺼶�(bright)      =");
//		puts(" = I. �Ķ󺼶�(dark)       = J. ����(���� ��:0.2~1.8) =");
//		puts(" = K. ���                 = P. ���(��հ�)          =");
//		puts(" = W. ���(�߾Ӱ�)         = L. Ȯ��                  =");
//		puts(" = M. Ȯ��2                = Z. Ȯ�� (����������)     =");
//		puts(" = N. ȸ��                 = Q. �̷���                =");
//		puts(" = R. ������׷� ��Ʈ��Ī  = T. ����-��               =");
//		puts(" = Y. ������׷� ��Ȱȭ    = U. ������                =");
//		puts(" = X. ����               = 5. ������                =");
//		puts(" = 6. ������������         = 7. ����������          =");
//		puts(" = 8. ������(+����)                                   =");
//		puts(" ======================================================");
//	}
//
//
//	void loadImage() {
//		freeOutputMemory();
//		char fullName[250] = "D:/RAW/Etc_Raw(squre)/";
//		char tmpName[50];
//		printf("���ϸ�-->");
//		scanf("%s", tmpName);
//		strcat(fullName, tmpName);
//		strcat(fullName, ".raw");
//		strcpy(fileName, fullName);
//
//		rfp = fopen(fileName, "rb");
//		if (rfp == NULL) {
//			MessageBox(hwnd, L"�ش� ������ �������� �ʽ��ϴ�.", L"����", MB_OK | MB_ICONERROR);
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
//		 //���� -> �޸�
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
//		printf("ȿ���� �����Ϸ��� '0'\n����ϰ� �������� �ƹ�Ű�� ��������.\n");
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
//		printf("���ϸ�-->");
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
//		MessageBox(hwnd, L"����Ϸ�", L"���� â", NULL);
//	}
//
//	void goFirst() { //�ʱ�ȭ
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
//		printf("������--> ");
//		scanf("%d", &retValue);
//		return retValue;
//	}
//
//	double getDoubleValue() {
//		double retValue;
//		printf("�Ǽ���--> ");
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
//	//====����ó�� �Լ�====
//
//	void equalImage() {
//		//�Ÿ� ����
//		freeOutputMemory();
//		//�߿�!! ��� �̹�����ũ�⸦ �����ؾ� �Ѵ�. ->�˰��� ����
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
//	printf("�߾Ӱ� : %d", val);
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
//void biLinear() { // �缱�� ������
//	int scale = getIntValue(); // �̹��� ũ�⸦ ������ ������ �Է� �޽��ϴ�.
//	freeOutputMemory(); // ��� �̹��� �޸𸮸� �����մϴ�.
//	outH = (int)(inH * scale); // ��� �̹����� ���̸� �����մϴ�.
//	outW = (int)(inW * scale); // ��� �̹����� �ʺ� �����մϴ�.
//	mallocOutputMemory(); // ��� �̹����� ���� �޸𸮸� �Ҵ��մϴ�.
//
//	for (int i = 0; i < outH; i++) { // ��� �̹����� �� �ȼ��� ���� �ݺ��մϴ�.
//		for (int k = 0; k < outW; k++) {
//			double orig_i = i / (double)scale; 
//			double orig_k = k / (double)scale; 
//			int i1 = (int)orig_i; 
//			int i2 = i1 + 1;
//			int k1 = (int)orig_k;
//			int k2 = k1 + 1;
//			double biY = orig_i - i1; // ���� ������ �Ÿ��� ���� ����ġ�� ����մϴ�.
//			double biX = orig_k - k1; // ���� ������ �Ÿ��� ���� ����ġ�� ����մϴ�.
//
//			if (i2 >= inH) i2 = inH - 1; 
//			if (k2 >= inW) k2 = inW - 1; 
//
//			double interpolatedValue =
//				(1 - biY) * ((1 - biX) * inImage[i1][k1] + biX * inImage[i1][k2]) +
//				biY * ((1 - biX) * inImage[i2][k1] + biX * inImage[i2][k2]);
//
//			outImage[i][k] = interpolatedValue; // ���� ���� ��� �̹����� �����մϴ�.
//		}
//	}
//
//	printImage(); // ������ �̹����� ����մϴ�.
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
//	//1 �ܰ� : �󵵼� ����
//	for (int i = 0; i < inH; i++) {
//		for (int k = 0; k < inW; k++) {
//			histo[inImage[i][k]]++;
//		}
//	}
//	// 2�ܰ� : ���� ������׷� ����
//	int sumHisto[256] = { 0, };
//	sumHisto[0] = histo[0];
//	for (int i = 1; i < 256; i++) {
//		sumHisto[i] = sumHisto[i - 1] + histo[i];
//	}
//	// 3�ܰ� : ����ȭ�� ������׷� ����
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
//void emboss() { //ȭ�ҿ���ó��
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
//void morphEmboss() { //ȭ�ҿ���ó��
//	freeOutputMemory();
//	outH = inH;
//	outW = inW;
//	mallocOutputMemory();
//
//	double mask[3][3] = { {-1.0, 0.0, 0.0},
//						{ 0.0, 0.0, 0.0 },
//						{ 0.0, 0.0, 1.0 } }; // ����ũ�� ������ �հ谡 0 or 1
//	// �ӽ� �޸� �Ҵ�(�Ǽ���)
//	double** tmpInImage = mallocDoubleMemory(inH + 2, inW + 2);
//	double** tmpOutImage = mallocDoubleMemory(outH, outW);
//	// �ӽ��Է¸޸𸮸� �ʱ�ȭ(127): �ʿ�� ��հ�
//	for (int i = 0; i < inH + 2; i++) {
//		for (int k = 0; k < inW + 2; k++) {
//			tmpInImage[i][k] = 127;
//		}
//	}
//	// �Է��̹��� --> �ӽ� �Է� �̹���
//	for (int i = 0; i < inH; i++) {
//		for (int k = 0; k < inW; k++) {
//			tmpInImage[i + 1][k + 1] = inImage[i][k];
//		}
//	}
//	// **ȸ�� ����**
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
//						{ 1. / 9, 1. / 9, 1. / 9 } }; // ����ũ�� ������ �հ谡 0 or 1
//	// �ӽ� �޸� �Ҵ�(�Ǽ���)
//	double** tmpInImage = mallocDoubleMemory(inH + 2, inW + 2);
//	double** tmpOutImage = mallocDoubleMemory(outH, outW);
//	// �ӽ��Է¸޸𸮸� �ʱ�ȭ(127): �ʿ�� ��հ�
//	for (int i = 0; i < inH + 2; i++) {
//		for (int k = 0; k < inW + 2; k++) {
//			tmpInImage[i][k] = 127;
//		}
//	}
//	// �Է��̹��� --> �ӽ� �Է� �̹���
//	for (int i = 0; i < inH; i++) {
//		for (int k = 0; k < inW; k++) {
//			tmpInImage[i + 1][k + 1] = inImage[i][k];
//		}
//	}
//	// **ȸ�� ����**
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
//						{ -1,-1, -1 } }; // ����ũ�� ������ �հ谡 0 or 1
//	// �ӽ� �޸� �Ҵ�(�Ǽ���)
//	double** tmpInImage = mallocDoubleMemory(inH + 2, inW + 2);
//	double** tmpOutImage = mallocDoubleMemory(outH, outW);
//	// �ӽ��Է¸޸𸮸� �ʱ�ȭ(127): �ʿ�� ��հ�
//	for (int i = 0; i < inH + 2; i++) {
//		for (int k = 0; k < inW + 2; k++) {
//			tmpInImage[i][k] = 127;
//		}
//	}
//	// �Է��̹��� --> �ӽ� �Է� �̹���
//	for (int i = 0; i < inH; i++) {
//		for (int k = 0; k < inW; k++) {
//			tmpInImage[i + 1][k + 1] = inImage[i][k];
//		}
//	}
//	// **ȸ�� ����**
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
//void edge1() { // �������� �˰���
//	freeOutputMemory();
//	outH = inH;
//	outW = inW;
//	mallocOutputMemory();
//
//	double mask[3][3] = { {0.0, 0.0, 0.0},
//						{ -1.0, 1.0,0.0 },
//						{0.0, 0.0, 0.0 } }; // ����ũ�� ������ �հ谡 0 or 1
//	// �ӽ� �޸� �Ҵ�(�Ǽ���)
//	double** tmpInImage = mallocDoubleMemory(inH + 2, inW + 2);
//	double** tmpOutImage = mallocDoubleMemory(outH, outW);
//	// �ӽ��Է¸޸𸮸� �ʱ�ȭ(127): �ʿ�� ��հ�
//	for (int i = 0; i < inH + 2; i++) {
//		for (int k = 0; k < inW + 2; k++) {
//			tmpInImage[i][k] = 127;
//		}
//	}
//	// �Է��̹��� --> �ӽ� �Է� �̹���
//	for (int i = 0; i < inH; i++) {
//		for (int k = 0; k < inW; k++) {
//			tmpInImage[i + 1][k + 1] = inImage[i][k];
//		}
//	}
//	// **ȸ�� ����**
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
//void edge2() { // ������
//	freeOutputMemory();
//	outH = inH;
//	outW = inW;
//	mallocOutputMemory();
//
//	double mask[3][3] = { {0.0, -1.0, 0.0},
//						{ 0.0, 1.0,0.0 },
//						{0.0, 0.0, 0.0 } }; // ����ũ�� ������ �հ谡 0 or 1
//	// �ӽ� �޸� �Ҵ�(�Ǽ���)
//	double** tmpInImage = mallocDoubleMemory(inH + 2, inW + 2);
//	double** tmpOutImage = mallocDoubleMemory(outH, outW);
//	// �ӽ��Է¸޸𸮸� �ʱ�ȭ(127): �ʿ�� ��հ�
//	for (int i = 0; i < inH + 2; i++) {
//		for (int k = 0; k < inW + 2; k++) {
//			tmpInImage[i][k] = 127;
//		}
//	}
//	// �Է��̹��� --> �ӽ� �Է� �̹���
//	for (int i = 0; i < inH; i++) {
//		for (int k = 0; k < inW; k++) {
//			tmpInImage[i + 1][k + 1] = inImage[i][k];
//		}
//	}
//	// **ȸ�� ����**
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
//// ���� �Լ�
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