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

using namespace cv;
using namespace std;


//���������ṹ��
/*
typedef struct DetectResults
{
    std::vector< int > classIds;
    std::vector< float > confidences;
    std::vector< Rect > boxes;
    int maxPersonConfidencePos; //���� ���ִ�ж�
} DETECTRESULTS; */

// ����opencv dnn��ͼ������
// ͼ��������װ�࣬ʹ��AIģ�ͽ�ȡ��Ļ������ж�����
class ImageDetection
{
public:
    ImageDetection();
    ~ImageDetection();
    void ReInit();

    void getScreenshot();
    DETECTRESULTS detectImg();
    cv::Mat getImg();

private:
    void initImg(); //��ʼ��ͼ����Դ
    void releaseImg(); //�ͷ�ͼ����Դ

    void initDnn(); //��ʼ��DNN��Դ

private:
    static AssistConfig* m_AssistConfig;

    //����ͼ�������������
    HDC m_screenDC;
    HDC m_memDC;
    HBITMAP m_hBitmap;
    BITMAPINFO* m_Bitmapinfo = NULL;

    //���ͼ�����ݵ�mat����
    //ע��opencv�ĺ���ʹ���������ʱ���Զ�����create�����������ݴ������
    //�������ط�ʹ��ʱ����Ҫ�ֶ�����create�����ڴ洴�����飻�ظ�ִ��create�������ᵼ���ظ��������ݴ�����飻
    cv::Mat m_mat_src; //���bitmap�л�ȡ��Դͼ
    cv::Mat m_mat_3; //���ת��Ϊ3ͨ����ͼ��


    //AI�����������
    /*
    const string ConfigFile = "../../Data/model/mobilenet/ssd_mobilenet_v3_small.pbtxt";
    const string ModelFile = "../../Data/model/mobilenet/ssd_mobilenet_v3_small.pb";
    const string LabelFile = "../../Data/model/mobilenet/coco.names";
    */

    const string ConfigFile = "../../Data/model/mobilenet/ssd_mobilenet_v3_large.pbtxt";
    const string ModelFile = "../../Data/model/mobilenet/ssd_mobilenet_v3_large.pb";
    const string LabelFile = "../../Data/model/mobilenet/coco.names";
    
    /*
    const string ConfigFile = "../../Data/model/efficientdet/d0.pbtxt";
    const string ModelFile = "../../Data/model/efficientdet/d0.pb";
    const string LabelFile = "../../Data/model/efficientdet/coco.names";
    */

    vector<string> m_classLabels; //���ǩ
    const float MinConfidence = 0.60; //��С���Ŷ�
    const int PersonClassId = 1; //�����ǩ�б��� ���� ��classid
    //cv::dnn::Net m_net;
    //ʹ��ר�ŵĶ�����ģ����
    cv::dnn::DetectionModel* m_net = NULL ;
    //cv::dnn::Net m_net;

};






