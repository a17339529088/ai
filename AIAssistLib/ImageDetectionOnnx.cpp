#include "pch.h"
#include "ImageDetectionOnnx.h"
using cv::Mat;

//��̬��Ա��ʼ��
AssistConfig* ImageDetectionOnnx::m_AssistConfig = AssistConfig::GetInstance();


//�෽��ʵ��
ImageDetectionOnnx::ImageDetectionOnnx()
{
    initImg();

    //����AIģ��
    yolov6 = new onnxlib::YOLOv6(ModelFile, 4);
}

ImageDetectionOnnx::~ImageDetectionOnnx()
{
    //ͼ����Դ�ͷ�
    releaseImg();

    //�ͷ���Դ����
    try {
        if (yolov6 != NULL) {
            delete yolov6;
            yolov6 = NULL;
        }
    }
    catch (Exception ex) {
        string msg = "";
    }
}

//�޸����ú���Ҫ���³�ʼ��һЩ����
void ImageDetectionOnnx::ReInit() {
    releaseImg();
    initImg();
}

//��ʼ��ͼ����Դ
void ImageDetectionOnnx::initImg(){
    //ע����Ļ���ź�����
    //ע��ץȡ��Ļ��ʱ��ʹ�����ź�������������꣬ץȡ��������ʵ�����߼��ֱ�������
    cv::Rect detectRect = m_AssistConfig->detectRect;
    cv::Rect detectZoomRect = m_AssistConfig->detectZoomRect;


    // ��ȡ��Ļ DC
    m_screenDC = GetDC(HWND_DESKTOP);
    m_memDC = CreateCompatibleDC(m_screenDC);
    // ����λͼ
    m_hBitmap = CreateCompatibleBitmap(m_screenDC, detectRect.width, detectRect.height);
    SelectObject(m_memDC, m_hBitmap);

    //����λͼ��Ϣͷ
    int iBits = GetDeviceCaps(m_memDC, BITSPIXEL) * GetDeviceCaps(m_memDC, PLANES);
    WORD wBitCount;
    if (iBits <= 1)
        wBitCount = 1;
    else if (iBits <= 4)
        wBitCount = 4;
    else if (iBits <= 8)
        wBitCount = 8;
    else if (iBits <= 24)
        wBitCount = 24;
    else
        wBitCount = 32;
    m_Bitmapinfo = new BITMAPINFO{ {sizeof(BITMAPINFOHEADER), detectRect.width, -detectRect.height, 1, wBitCount, BI_RGB },{0,0,0,0} };

    //�������ͼ�����ݵ�mat
    //m_mat.create(detectRect.height, detectRect.width, CV_8UC4);
    //m_mat3.create(detectRect.height, detectRect.width, CV_8UC3);
}

//�ͷ�ͼ����Դ
void ImageDetectionOnnx::releaseImg() {

    //��Դ�ͷ�
    try {
        m_mat_src.release();
        m_mat_3.release();

        if (m_Bitmapinfo != NULL)
            delete m_Bitmapinfo;
        DeleteObject(m_hBitmap);
        DeleteDC(m_memDC);
        ReleaseDC(HWND_DESKTOP, m_screenDC);
    }
    catch (Exception ex) {
        string msg = "";
    }

    m_Bitmapinfo = NULL;
    m_hBitmap = NULL;
    m_memDC = NULL;
    m_screenDC = NULL;
}


/* ��ȡ���������Ļ��ͼ */
void ImageDetectionOnnx::getScreenshot()
{
    //������Ļ���ź�ģ��ü����ʵ��ͼ��������
    //ע��ץȡ��Ļ��ʱ��ʹ�����ź�������������꣬ץȡ��������ʵ�����߼��ֱ�������
    cv::Rect detectRect = m_AssistConfig->detectRect;
    cv::Rect detectZoomRect = m_AssistConfig->detectZoomRect;
   

    // �õ�λͼ������
    // ʹ��BitBlt��ͼ�����ܽϵͣ������޸�ΪDXGI
    //Windows8�Ժ�΢��������һ���µĽӿڣ��С�Desktop Duplication API����Ӧ�ó��򣬿���ͨ������API�����������ݡ�
    //����Desktop Duplication API��ͨ��Microsoft DirectX Graphics Infrastructure (DXGI)���ṩ����ͼ��ģ��ٶȷǳ��졣
    bool opt = BitBlt(m_memDC, 0, 0, detectRect.width, detectRect.height, m_screenDC, detectZoomRect.x, detectZoomRect.y, SRCCOPY);

    //ע���ڷ�opencv������ʹ��matʱ����Ҫ�ֶ�����create�����ڴ洴�����飻�ظ�ִ��create�������ᵼ���ظ��������ݴ�����飻
    m_mat_src.create(detectRect.height, detectRect.width, CV_8UC4);
    //int rows = GetDIBits(m_screenDC, m_hBitmap, 0, detectRect.height, m_mat.data, (BITMAPINFO*)&bi, DIB_RGB_COLORS);
    int rows = GetDIBits(m_memDC, m_hBitmap, 0, detectRect.height, m_mat_src.data, (BITMAPINFO*)m_Bitmapinfo, DIB_RGB_COLORS);

    //cv::namedWindow("opencvwindows", WINDOW_AUTOSIZE);
    //cv::imshow("opencvwindows", m_mat_src);
    //waitKey(2000);

    //��Ļ��ͼ����Ƶ��ͼ��ʽ��һ������Ҫ����ͼ���ʽת��
    //ȥ��alpha ֵ(͸����)ͨ����ת��ΪCV_8UC3��ʽ
    cv::cvtColor(m_mat_src, m_mat_3, COLOR_RGBA2RGB);

    //���ݸ�ʽת��
    //m_mat_3.convertTo(img_mat, CV_8UC3, 1.0);

    //���ݲ�ͬ����Ϸ��һ�������⴦���������Ϸ�߸���ͼ��
    //super people �� pubg������Ϸ�����⴦��
    //Ӱ��ʶ��Ч�����ȹر�����߼�
    /*
    if (m_AssistConfig->gameIndex == 0 || m_AssistConfig->gameIndex == 1) {
        int y = m_mat_3.rows * 3 / 4;
        Mat mask(m_mat_3, Rect(0, y, m_mat_3.cols/2, m_mat_3.rows- y));
        mask = Scalar(0, 0, 0);
    }*/

    return;
}


/* AI���� */
DETECTRESULTS ImageDetectionOnnx::detectImg()
{
    //������Ļ���ź�ģ��ü����ʵ��ͼ��������
    //ע��ץȡ��Ļ��ʱ��ʹ�����ź�������������꣬ץȡ��������ʵ�����߼��ֱ�������
    cv::Rect detectRect = m_AssistConfig->detectRect;
    cv::Rect detectZoomRect = m_AssistConfig->detectZoomRect;
    int gameIndex = m_AssistConfig->gameIndex;
    int playerCentX = m_AssistConfig->playerCentX;

    std::vector< int > classIds;
    std::vector< float > confidences;
    std::vector< Rect > boxes;
    DETECTRESULTS out;

    std::vector<lite::types::Boxf> detected_boxes;

    try
    {
        //ִ��ģ������
        yolov6->detect(m_mat_3, detected_boxes, MinConfidence);

        //����������
        for (int i = 0; i < detected_boxes.size(); i++) {
        
            //���������⵽�Ķ���
            out.classIds.push_back(detected_boxes[i].label);
            out.confidences.push_back(detected_boxes[i].score);
            out.boxes.push_back(detected_boxes[i].rect());

            //�������Ŷ�������Ա��λ��
            if (i == 0) {
                out.maxPersonConfidencePos = 0;
            }
              
        }
    }
    catch (Exception ex) {
        string msg = "";
    }

    return out;
}


cv::Mat ImageDetectionOnnx::getImg() {
    //��¡mat���ݽ��ⲿ����ʹ�ã����������ֻ��������mat�������ǵ������ڴ���
    Mat mat = m_mat_3.clone();
    return mat;
}