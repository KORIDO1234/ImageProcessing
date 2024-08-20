from tkinter import *
from tkinter import messagebox
from tkinter.filedialog import*
from tkinter.simpledialog import*
import math
import os.path

def openImage():
    global window, canvas, paper, inImage, outImage, inH, inW, outH, outW, basicImage, basicH, basicW
   #fullname = 'D:/RAW/Pet_RAW(squre)/Pet_RAW(256x256)/cat01_256.raw'
    fullname = askopenfilename(parent=window, filetypes=(('RAW파일', '*.raw'),('모든파일','*.*')))
    fsize = os.path.getsize(fullname)
    basicH = basicW = inH = inW = int(math.sqrt(fsize))
    inImage = malloc2D(inH, inW)
    rfp = open(fullname, 'rb')
    for i in range(inH):
        for k in range(inW):
            inImage[i][k] = ord(rfp.read(1))
    rfp.close()
    basicImage = inImage
    equalImage()

def malloc2D(h, w, initValue=0):
    memory = [[0 for _ in range(w)] for _ in range(h)]
    return memory

def displayImage():
    global window, canvas, paper, inImage, outImage, inH, inW, outH, outW, fullname
    ramSave()
    if( canvas != None):
        canvas.destroy()
    if outH <512 or outW <512:
        windowH, windowW = 512, 512
    elif outH >= 512 or outW >= 512:
        windowH, windowW = outH, outW
    window.geometry(str(windowH)+'x'+str(windowW))
    canvas = Canvas(window, height=outH, width=outW, bg='yellow')
    paper = PhotoImage(height=outH, width=outW)
    canvas.create_image((outH // 2, outW // 2), image=paper, state='normal')
    rgbString = " "# 전체에 대한 16진수 문자열
    for i in range(outH):
        oneString = " "
        for k in range(outW):
            r = g = b = outImage[i][k]
            oneString += '#%02x%02x%02x ' % (r, g, b)
        rgbString += '{'+oneString+'} '
    paper.put(rgbString)
    canvas.pack()

def saveImage():
    global window, canvas, paper, inImage, outImage, inH, inW, outH, outW, fullname
    if (outImage ==None):
        return
    wfp = asksaveasfile(parent=window, mode ='wb', defaultextension='*.raw',filetypes=(('RAW파일', '*.raw'),('모든파일','*.*')))
    import struct
    for i in range(outH):
        for k in range(outW):
            wfp.write( struct.pack('B', outImage[i][k]))
    wfp.close()
    messagebox.showinfo('성공', wfp.name + '저장완료')


def ramSave(): ##이미지 누적기능
    global window, canvas, paper, inImage, outImage, inH, inW, outH, outW, fullname
    inH, inW = outH, outW
    inImage = malloc2D(outH, outW)
    for i in range(outH):
        for k in range(outW):
            inImage[i][k] = outImage[i][k]

def clearImage(): ## 이미지 초기화
    global window, canvas, paper, inImage, outImage, inH, inW, outH, outW, fullname, basicH, basicW
    inH = outH = basicH
    inW = outW = basicW
    inImage = malloc2D(outH, outW)
    outImage = malloc2D(outH, outW)
    for i in range(basicH):
        for k in range(basicW):
            outImage[i][k] = inImage[i][k] = basicImage[i][k]
    displayImage()


####################
### 영상처리 함수부###
####################


## 화소점처리 함수
def equalImage():
    global inImage, outImage, inH, inW, outH, outW
    # (중요!) 출력 이미지 크기 결정 --> 알고리즘에 의존
    outH = inH
    outW = inW
    ## 출력 이미지 메모리 확보, free는 필요없음
    outImage = malloc2D(outH, outW)
    ## 진짜 영상처리 알고리즘
    for i in range(inH):
        for k in range(inW):
            outImage[i][k]=inImage[i][k]
    displayImage()

def addImage():
    global inImage, outImage, inH, inW, outH, outW
    outH = inH
    outW = inW
    outImage = malloc2D(outH, outW)
    val = askinteger('조절 값 입력', '값을 입력하세요 : (-255~255)', maxvalue=255, minvalue=-255)
    for i in range(outH):
        for k in range(outW):
            px = inImage[i][k] +val
            if (px>255) :
                px = 255
            if (px<0):
                px = 0
            outImage[i][k] = px
    displayImage()

def revImage():
    global inImage, outImage, inH, inW, outH, outW
    outH = inH
    outW = inW
    outImage = malloc2D(outH, outW)
    for i in range(outH):
        for k in range(outW):
            outImage[i][k] = 255 - inImage[i][k]
    displayImage()


def mirror():
    global inImage, outImage, inH, inW, outH, outW
    outH = inH
    outW = inW
    for i in range(outH):
        for k in range(outW):
            outImage[i][outW-k-1] = inImage[i][k]
    displayImage()




def bwImage():
    global inImage, outImage, inH, inW, outH, outW
    outH = inH
    outW = inW
    val = askinteger('흑백 값 입력', '흑백 기준값을 입력하세요 : (0~255)', maxvalue=255, minvalue=0)
    for i in range(outH):
        for k in range(outW):
            if inImage[i][k] >val:
                outImage[i][k] = 255
            else :
                outImage[i][k] = 0
    displayImage()

def avgbwImage():
    global inImage, outImage, inH, inW, outH, outW
    sum = 0
    for i in range(inH):
        for k in range(inW):
            sum += inImage[i][k]
    val = sum/(inH*inW)
    for i in range(outH):
        for k in range(outW):
            if inImage[i][k] > val:
                outImage[i][k] = 255
            else:
                outImage[i][k] = 0
    displayImage()

def midbwImage():
    global inImage, outImage, inH, inW, outH, outW
    dim1 = []
    count = 0
    temp = 0
    for i in range(inH):
        for k in range(inW):
            dim1.append(inImage[i][k])
            count += 1
    for i in range(inH):
        for k in range(0,inW-i-1,1):
            if dim1[k] > dim1[k + 1]:
                temp = dim1[k]
                dim1[k] = dim1[k + 1]
                dim1[k + 1] = temp
    val = dim1[(int)(count / 2)]
    for i in range(outH):
        for k in range(outW):
            if inImage[i][k] > val:
                outImage[i][k] = 255
            else:
                outImage[i][k] = 0
    displayImage()

def brightParaImage():
    global inImage, outImage, inH, inW, outH, outW
    for i in range(inH):
        for k in range(inW):
            outImage[i][k] = int(255.0 * pow(inImage[i][k] / 172 - 1, 2))
    displayImage()
def darkParaImage():
    global inImage, outImage, inH, inW, outH, outW
    for i in range(inH):
        for k in range(inW):
            outImage[i][k] = int(255 - 255.0 * pow(inImage[i][k] / 172 -1, 2))
    displayImage()
def gamma():
    global inImage, outImage, inH, inW, outH, outW, val
    val = askfloat('감마값', '감마값을 입력하세요(0~3)', maxvalue=3, minvalue=0)
    for i in range(inH):
        for k in range(inW):
            outImage[i][k] = int(255.0 * pow(inImage[i][k] /255.0, val))
    displayImage()






## 기하학 처리 함수

def rotateImage():
    global inImage, outImage, inH, inW, outH, outW
    outH = inH
    outW = inW
    outImage = malloc2D(outH, outW)
    degree = askinteger('회전 값 입력', '회전 각도를 입력하세요 : (0~360)', maxvalue=360, minvalue=0)
    radian = degree *math.pi/180.0
    centX = outH // 2
    centY = outW // 2
    for i in range(outH):
        for k in range(outW):
            xs = (int)(math.cos(radian) * (i - centX) - math.sin(radian) * (k - centY) + centX)
            ys = (int)(math.sin(radian) * (i - centX) + math.cos(radian) * (k- centY) + centY)
            if 0 <= xs < inH and 0 <= ys < inW:
                outImage[i][k] = inImage[xs][ys]
    displayImage()

def midZoomOut(): # 중앙값 축소
    global inImage, outImage, inH, inW, outH, outW
    scale = 0
    temp = 0
    scale = askinteger('축소 배율 입력', '축소 배율을 입력하세요(정수)', maxvalue=10, minvalue=1)
    outH = inH//scale
    outW = inW//scale
    outImage = malloc2D(outH, outW)
    for i in range(outH):
        for k in range(outW):
            block = [0] * scale*scale
            count =0
            for p in range(i*scale,(i+1)*scale):
                for l in range(k*scale,(k+1)*scale):
                    block[count] = inImage[p][l]
                    count += 1
            for x in range(count):
                for y in range(count-1-x):
                    if block[y] > block[y+1]:
                        temp = block[y]
                        block[y] = block[y + 1]
                        block[y + 1] = temp
            outImage[i][k] = block[count // 2]
    displayImage()

def zoomIn():
    global inImage, outImage, inH, inW, outH, outW
    scale = 0
    scale = askinteger('확대 배율 입력', '확대 배율을 입력하세요(정수)', maxvalue=10, minvalue=1)
    outH = (int)(inH * scale)
    outW = (int)(inW * scale)
    outImage = malloc2D(outH, outW)
    for i in range(inH):
        for k in range(inW):
            outImage[(int)(i * scale)][(int)(k * scale)] = inImage[i][k]
    displayImage()

def zoomIn2():
    global inImage, outImage, inH, inW, outH, outW
    scale = 0
    scale = askinteger('확대 배율 입력', '확대 배율을 입력하세요(정수)', maxvalue=10, minvalue=1)
    outH = (int)(inH * scale)
    outW = (int)(inW * scale)
    outImage = malloc2D(outH, outW)
    for i in range(outH):
        for k in range(outW):
            outImage[i][k] = inImage[(int)(i / scale)][(int)(k / scale)]
    displayImage()
def biLinear():
    global inImage, outImage, inH, inW, outH, outW
    scale =0
    scale = askinteger('확대 배율 입력', '확대 배율을 입력하세요(정수)', maxvalue=10, minvalue=1)
    outH = (int)(inH * scale)
    outW = (int)(inW * scale)
    outImage = malloc2D(outH, outW)
    for i in range(outH):
        for k in range(outW):
            orig_i = i / scale
            orig_k = k / scale
            i1 = int(orig_i)
            i2 = min(i1 + 1, inH - 1)
            k1 = int(orig_k)
            k2 = min(k1 + 1, inW - 1)
            biY = orig_i - i1
            biX = orig_k - k1
            interpolatedValue = int(((1 - biY) * ((1 - biX) * inImage[i1][k1] + biX * inImage[i1][k2]) +
                                 biY * ((1 - biX) * inImage[i2][k1] + biX * inImage[i2][k2])))
            outImage[i][k] = interpolatedValue
    displayImage()

def zoomOut():
    global inImage, outImage, inH, inW, outH, outW
    scale = 0
    scale = askinteger('축소 배율 입력', '축소 배율을 입력하세요(정수)', maxvalue=10, minvalue=1)
    outH = inH // scale
    outW = inW // scale
    outImage = malloc2D(outH, outW)
    for i in range(inH):
        for k in range(inW):
            outImage[i//scale][k//scale] = inImage[i][k]
    displayImage()

def avgZoomOut():
    global inImage, outImage, inH, inW, outH, outW
    scale = 0
    scale = askinteger('축소 배율 입력', '축소 배율을 입력하세요(정수)', maxvalue=10, minvalue=1)
    outH = inH // scale
    outW = inW // scale
    outImage = malloc2D(outH, outW)
    for i in range(outH):
        for j in range(outW):
            total_sum = 0
            for k in range(i * scale, (i + 1) * scale):
                for p in range(j * scale, (j + 1) * scale):
                    total_sum += inImage[k][p]
            avg = total_sum // (scale * scale)
            outImage[i][j] = avg
    displayImage()

## 히스토그램 함수###

def histoStretch():
    global inImage, outImage, inH, inW, outH, outW
    outH = inH
    outW = inW
    high = inImage[0][0]
    low = inImage[0][0]
    for i in range(inH):
        for k in range(inW):
            if inImage[i][k] < low:
                low = inImage[i][k]
            if inImage[i][k] > high:
                high = inImage[i][k]
    for i in range(outH):
        for k in range(outW):
            old = inImage[i][k]
            new = int((old - low) / (high - low) * 255.0)
            if new > 255:
                new = 255
            if new < 0:
                new = 0
            outImage[i][k] = new
    displayImage()

def endin():
    global inImage, outImage, inH, inW, outH, outW
    outH = inH
    outW = inW
    high = inImage[0][0]
    low = inImage[0][0]
    for i in range(inH):
        for k in range(inW):
            if inImage[i][k] < low:
                low = inImage[i][k]
            if inImage[i][k] > high:
                high = inImage[i][k]
    high -=50
    low +=50
    for i in range(outH):
        for k in range(outW):
            old = inImage[i][k]
            new = int((old - low) / (high - low) * 255.0)
            if new > 255:
                new = 255
            if new < 0:
                new = 0
            outImage[i][k] = new
    displayImage()

def histoEqual():
    global inImage, outImage, inH, inW, outH, outW
    outH = inH
    outW = inW
    high = inImage[0][0]
    low = inImage[0][0]
    histo = [0 for _ in range(256)]
    for i in range(inH):
        for k in range(inW):
            histo[inImage[i][k]]+=1
    sumHisto=[ 0 for _ in range(256)]
    sumHisto[0] = histo[0]
    for i in range(1,256,1):
        sumHisto[i] = sumHisto[i - 1] + histo[i]
    normalHisto = [ 1 for _ in range(256)]
    for i in range(256):
        normalHisto[i] = sumHisto[i] * (1.0 / (inH * inW)) * 255.0

    for i in range(inH):
        for k in range(inW):
            outImage[i][k] = int(normalHisto[inImage[i][k]])
    displayImage()



## 영역처리 함수




def emboss():
    global inImage, outImage, inH, inW, outH, outW, mask, tmpInImage, tmpOutImage
    outH = inH
    outW = inW
    mask= [[-1.0, 0.0, 0.0],
	             [ 0.0, 0.0, 0.0],
	 	          [0.0, 0.0, 1.0]]
    tmpInImage = malloc2D(inH+2, inW+2)
    tmpOutImage = malloc2D(outH, inH)
    for i in range(inH+2):
        for k in range(inW+2):
            tmpInImage[i][k] = 127

    for i in range(inH):
        for k in range(inW):
            tmpInImage[i + 1][k + 1] = inImage[i][k]

    for i in range(inH):
        for k in range(inW):
            S = 0.0
            for m in range(3):
                for n in range(3):
                    S += tmpInImage[i + m][k + n] * mask[m][n]
            tmpOutImage[i][k] = S
    for i in range(outH):
        for k in range(outW):
            tmpOutImage[i][k] += 127.0
            if tmpOutImage[i][k] < 0.0:
                outImage[i][k] = 0
            elif tmpOutImage[i][k] > 255.0:
                outImage[i][k] = 255
            else:
                outImage[i][k] = int(tmpOutImage[i][k])
    displayImage()


def verEdge(): ##수직검출
    global inImage, outImage, inH, inW, outH, outW, mask, tmpInImage, tmpOutImage
    outH = inH
    outW = inW
    mask=[[0.0, 0.0, 0.0],
	    [ -1.0, 1.0, 0.0],
	 	  [0.0, 0.0, 0.0]]
    tmpInImage = malloc2D(inH+2, inW+2)
    tmpOutImage = malloc2D(outH, inH)
    for i in range(inH+2):
        for k in range(inW+2):
            tmpInImage[i][k] = 127

    for i in range(inH):
        for k in range(inW):
            tmpInImage[i + 1][k + 1] = inImage[i][k]

    for i in range(inH):
        for k in range(inW):
            S = 0.0
            for m in range(3):
                for n in range(3):
                    S += tmpInImage[i + m][k + n] * mask[m][n]
            tmpOutImage[i][k] = S
    for i in range(outH):
        for k in range(outW):
            if tmpOutImage[i][k] < 0.0:
                outImage[i][k] = 0
            elif tmpOutImage[i][k] > 255.0:
                outImage[i][k] = 255
            else:
                outImage[i][k] = int(tmpOutImage[i][k])
    displayImage()

def parEdge(): ##수평검출
    global inImage, outImage, inH, inW, outH, outW, mask, tmpInImage, tmpOutImage
    outH = inH
    outW = inW
    mask=[[0.0, -1.0, 0.0],
	    [ 0.0, 1.0, 0.0],
	 	  [0.0, 0.0, 0.0]]
    tmpInImage = malloc2D(inH+2, inW+2)
    tmpOutImage = malloc2D(outH, inH)
    for i in range(inH+2):
        for k in range(inW+2):
            tmpInImage[i][k] = 127

    for i in range(inH):
        for k in range(inW):
            tmpInImage[i + 1][k + 1] = inImage[i][k]

    for i in range(inH):
        for k in range(inW):
            S = 0.0
            for m in range(3):
                for n in range(3):
                    S += tmpInImage[i + m][k + n] * mask[m][n]
            tmpOutImage[i][k] = S
    for i in range(outH):
        for k in range(outW):
            if tmpOutImage[i][k] < 0.0:
                outImage[i][k] = 0
            elif tmpOutImage[i][k] > 255.0:
                outImage[i][k] = 255
            else:
                outImage[i][k] = int(tmpOutImage[i][k])
    displayImage()


def sharp():
    global inImage, outImage, inH, inW, outH, outW, mask, tmpInImage, tmpOutImage
    outH = inH
    outW = inW
    mask=[[-1.0, -1.0, -1.0],
	    [ -1.0, 9.0, -1.0],
	 	  [-1.0, -1.0, -1.0]]
    tmpInImage = malloc2D(inH+2, inW+2)
    tmpOutImage = malloc2D(outH, inH)
    for i in range(inH+2):
        for k in range(inW+2):
            tmpInImage[i][k] = 127

    for i in range(inH):
        for k in range(inW):
            tmpInImage[i + 1][k + 1] = inImage[i][k]

    for i in range(inH):
        for k in range(inW):
            S = 0.0
            for m in range(3):
                for n in range(3):
                    S += tmpInImage[i + m][k + n] * mask[m][n]
            tmpOutImage[i][k] = S
    for i in range(outH):
        for k in range(outW):
            if tmpOutImage[i][k] < 0.0:
                outImage[i][k] = 0
            elif tmpOutImage[i][k] > 255.0:
                outImage[i][k] = 255
            else:
                outImage[i][k] = int(tmpOutImage[i][k])
    displayImage()

def blur():
    global inImage, outImage, inH, inW, outH, outW, mask, tmpInImage, tmpOutImage
    outH = inH
    outW = inW
    mask=[[1.0/9, 1.0/9, 1.0/9],
	    [ 1.0/9, 1.0/9, 1.0/9],
	 	  [1.0/9, 1.0/9, 1.0/9]]
    tmpInImage = malloc2D(inH+2, inW+2)
    tmpOutImage = malloc2D(outH, inH)
    for i in range(inH+2):
        for k in range(inW+2):
            tmpInImage[i][k] = 127

    for i in range(inH):
        for k in range(inW):
            tmpInImage[i + 1][k + 1] = inImage[i][k]

    for i in range(inH):
        for k in range(inW):
            S = 0.0
            for m in range(3):
                for n in range(3):
                    S += tmpInImage[i + m][k + n] * mask[m][n]
            tmpOutImage[i][k] = S
    for i in range(outH):
        for k in range(outW):
            if tmpOutImage[i][k] < 0.0:
                outImage[i][k] = 0
            elif tmpOutImage[i][k] > 255.0:
                outImage[i][k] = 255
            else:
                outImage[i][k] = int(tmpOutImage[i][k])
    displayImage()

window, canvas, paper= None, None, None
inImage, outImage, basicImage = [], [], []
inH, inW, outH, outW, basicH, basicW = [0]*6
fullname = ''

## 메인코드부
window = Tk() ## 벽
window.geometry("400x500")
window.resizable(width =False, height=False)
window.title("영상처리 RC 1")



# 메뉴 만들기
mainMenu = Menu(window)# 메뉴의 틀
window.config(menu=mainMenu)

fileMenu = Menu(mainMenu, tearoff=0) # 상위 메뉴
mainMenu.add_cascade(label='파일', menu=fileMenu)
fileMenu.add_command(label='열기', command=openImage)
fileMenu.add_command(label='저장', command=saveImage)
fileMenu.add_command(label='초기화', command=clearImage)
fileMenu.add_separator()
fileMenu.add_command(label='종료', command=None)

pixelMenu = Menu(mainMenu, tearoff=0) # 화소점처리
mainMenu.add_cascade(label='화소점 처리', menu=pixelMenu)
pixelMenu.add_command(label='동일', command=equalImage)
pixelMenu.add_command(label='밝게/어둡게', command=addImage)
pixelMenu.add_command(label='반전', command=revImage)
pixelMenu.add_command(label='감마', command=gamma)

pixel2Menu = Menu(pixelMenu, tearoff=0) #화소점처리 - 흑백
pixelMenu.add_cascade(label='흑백', menu=pixel2Menu)
pixel2Menu.add_command(label='흑백(지정값)', command=bwImage)
pixel2Menu.add_command(label='흑백(평균값)', command=avgbwImage)
pixel2Menu.add_command(label='흑백(중앙값)', command=midbwImage)

pixel3Menu = Menu(pixelMenu, tearoff=0) #화소점처리 - 흑백
pixelMenu.add_cascade(label='파라볼라', menu=pixel3Menu)
pixel3Menu.add_command(label='파라볼라(밝은 부분)', command=brightParaImage)
pixel3Menu.add_command(label='파라볼라(어두운 부분)', command=darkParaImage)

geoMenu = Menu(mainMenu, tearoff=0) # 기하학처리
mainMenu.add_cascade(label='기하학 처리', menu=geoMenu)
geoMenu.add_command(label='회전', command=rotateImage)
geoMenu.add_command(label='미러(좌우대칭)', command=mirror)

geo3Menu = Menu(pixelMenu, tearoff=0) #기하학 처리 - 축소
geoMenu.add_cascade(label='축소', menu=geo3Menu)
geo3Menu.add_command(label='축소', command=zoomOut)
geo3Menu.add_command(label='축소(평균)', command=avgZoomOut)
geo3Menu.add_command(label='축소(중앙값)', command=midZoomOut)

geo2Menu = Menu(pixelMenu, tearoff=0) #기하학 처리 - 확대
geoMenu.add_cascade(label='확대', menu=geo2Menu)
geo2Menu.add_command(label='확대1', command=zoomIn)
geo2Menu.add_command(label='확대2', command=zoomIn2)
geo2Menu.add_command(label='확대(양선형보간법)', command=biLinear)

histoMenu = Menu(mainMenu, tearoff=0) # 히스토그램
mainMenu.add_cascade(label='히스토그램', menu=histoMenu)
histoMenu.add_command(label='스트래칭', command=histoStretch)
histoMenu.add_command(label='엔드-인', command=endin)
histoMenu.add_command(label='평활화', command=histoEqual)

areaMenu = Menu(mainMenu, tearoff=0) # 히스토그램
mainMenu.add_cascade(label='영역 처리', menu=areaMenu)
areaMenu.add_command(label='블러', command=blur)
areaMenu.add_command(label='샤프닝', command=sharp)
areaMenu.add_command(label='엠보싱', command=emboss)

area1Menu = Menu(pixelMenu, tearoff=0) # 히스토그램 - 엣지검출
areaMenu.add_cascade(label='엣지 검출', menu=area1Menu)
area1Menu.add_command(label='수직엣지검출', command=verEdge)
area1Menu.add_command(label='수평엣지검출', command=parEdge)



window.mainloop()