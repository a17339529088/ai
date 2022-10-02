#pragma once
#include<string>
#include<iostream>
#include<fstream>
#include<cmath>
#include <windows.h>
#include <Tlhelp32.h>
#include<opencv2/opencv.hpp>
#include<opencv2/imgproc.hpp>

#include "AssistConfig.h"
#include "ImageDetection.h"

using namespace cv;
using namespace std;

/**
* ����Ļ����ʾ׼��
**/
class DrawImage
{
public:
    DrawImage();
    ~DrawImage();
    void ReInit();

    //��׼��
    void drawAim();

private:
    void initImg(); //��ʼ��ͼ����Դ
    void releaseImg(); //�ͷ�ͼ����Դ

private:
    static AssistConfig* m_AssistConfig;

    //ͼ�������������
    HWND m_hWnd;
    HDC m_screenDC;
    HDC m_memDC;
    HBITMAP m_hBitmap;
    HPEN m_hPen;
    BITMAPINFO* m_Bitmapinfo = NULL;

};

