#pragma once
#include<string>
#include <atlstr.h>
#include<iostream>
#include <windows.h>
#include <Tlhelp32.h>
#include<opencv2/opencv.hpp>

#include "AssistConfig.h"

using namespace cv;
using namespace std;


//ϵͳ������
class SystemUtil
{
public:
    // ���ݽ��������Ҵ�������
    static void findProcessWindowRect();
    // WChar����ת�ַ���
    static string WChar2String(WCHAR   szExeFile[MAX_PATH]);

    static string CStringToString(CString cs) {
#ifdef _UNICODE

        //�����unicode����
        USES_CONVERSION;
        std::string str(W2A(cs));
        return str;
#else
        //����Ƕ��ֽڹ��� 
        std::string str(cs.GetBuffer());
        cs.ReleaseBuffer();
        return str;

#endif // _UNICODE 
    }

    static CString StringToCString(string str) {
#ifdef _UNICODE
        //�����unicode����
        USES_CONVERSION;
        CString ans(str.c_str());
        return ans;
#else
        //����Ƕ��ֽڹ��� 
        CString ans;
        ans.Format("%s", str.c_str());
        return ans;
#endif // _UNICODE  
    }

private:
    static AssistConfig* m_AssistConfig;

};

typedef struct tagWNDINFO
{
    DWORD dwProcessId;
    HWND hWnd;
} WNDINFO, * LPWNDINFO;

//ö�ٴ��ڹ���
static BOOL CALLBACK EnumProc(HWND hWnd, LPARAM lParam)
{
    DWORD dwProcessId;
    GetWindowThreadProcessId(hWnd, &dwProcessId);
    LPWNDINFO pInfo = (LPWNDINFO)lParam;

    if (dwProcessId == pInfo->dwProcessId)
    {
        pInfo->hWnd = hWnd;
        return FALSE;
    }
    return TRUE;
}

//��ȡ�����ھ��
static HWND GetProcessMainWnd(DWORD dwProcessId)
{
    WNDINFO wi;
    wi.dwProcessId = dwProcessId;
    wi.hWnd = NULL;
    EnumWindows(EnumProc, (LPARAM)&wi);
    return wi.hWnd;
}