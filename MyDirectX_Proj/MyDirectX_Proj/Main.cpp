//***************************************************************************************
// @author : zxz
// @file : Main.cpp
// @description : �������ڣ�û���κ��߼�
//***************************************************************************************


#include <windows.h>
#include "ClearColor.h"


// Ĭ����Ļ
static int gClientWidth = 1280;
static int gClientHeight = 720;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
	auto clearColor = ClearColor(gClientWidth, gClientHeight, L"D3D Color");
	return WinApplication::Run(&clearColor, hInstance, nCmdShow);
}