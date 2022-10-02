#include "pch.h"
#include "DrawImage.h"

//��̬��Ա��ʼ��
AssistConfig* DrawImage::m_AssistConfig = AssistConfig::GetInstance();


//�෽��ʵ��
DrawImage::DrawImage()
{
    initImg();
}

DrawImage::~DrawImage()
{
    //ͼ����Դ�ͷ�
    releaseImg();

}

//�޸����ú���Ҫ���³�ʼ��һЩ����
void DrawImage::ReInit() {
    releaseImg();
    initImg();
}

//��ʼ��ͼ����Դ
void DrawImage::initImg() {
    //׼�������СΪ20
    int width = 20;
    int height = 20;

    // ��ȡ��Ļ DC
    HWND m_hWnd = GetDesktopWindow();
    //m_screenDC = GetDC(HWND_DESKTOP);
    m_screenDC = GetDC(m_hWnd);
    m_memDC = CreateCompatibleDC(m_screenDC);
    // ����λͼ
    m_hBitmap = CreateCompatibleBitmap(m_screenDC, width, height);
    SelectObject(m_memDC, m_hBitmap);

    //���ñ���͸��
    //SetBkMode(m_memDC, TRANSPARENT);
    //SetBkColor(m_memDC, RGB(255, 255, 255));
    SetBkColor(m_memDC, RGB(0, 0, 0));

    //�������󶨻���
    m_hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
    SelectObject(m_memDC, m_hPen);

    //��԰
    //Arc(m_memDC, 1, 1, width - 1, height - 1, 1, 1, 1, 1);
    //Arc(m_memDC, width/2 -2, height/2 - 2, width / 2 + 2, height / 2 + 2, width / 2 - 2, height / 2 - 2, width / 2 - 2, height / 2 - 2);
    MoveToEx(m_memDC, 0, height/2, NULL);
    LineTo(m_memDC,  width, height/2);
    MoveToEx(m_memDC, width/2, 0, NULL);
    LineTo(m_memDC, width/2, height);
}

//�ͷ�ͼ����Դ
void DrawImage::releaseImg() {

    //��Դ�ͷ�
    try {

        DeleteObject(m_hPen);
        DeleteObject(m_hBitmap);
        DeleteDC(m_memDC);
        ReleaseDC(m_hWnd, m_screenDC);
    }
    catch (Exception ex) {
        string msg = "";
    }

    m_hPen = NULL;
    m_Bitmapinfo = NULL;
    m_hBitmap = NULL;
    m_memDC = NULL;
    m_screenDC = NULL;
}


/* ��ȡ���������Ļ��ͼ */
void DrawImage::drawAim()
{
    //׼�������СΪ20
    int width = 20;
    int height = 20;

    int centerx = m_AssistConfig->detectCentX;
    int centery = m_AssistConfig->detectCentY;

    RECT rect = { centerx - width / 2, centery - height / 2, centerx + width / 2, centery + height / 2 };

    //��ͼ����TransparentBlt������λͼ��ʱ��ʹ������ɫΪ͸��ɫ
    //TransparentBlt(m_screenDC, centerx - width/2, centery - height/2, width,height, m_memDC,0,0, width, height, RGB(0, 0, 0));
    //BitBlt(m_screenDC, centerx - width / 2, centery - height / 2, width, height, m_memDC, 0, 0, SRCPAINT);

    //InvalidateRect(m_hWnd, &rect, FALSE);
    //UpdateWindow(m_hWnd);


    return;
}
