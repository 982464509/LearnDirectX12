//***************************************************************************************
// @author : zxz
// @file : Main.cpp
// @description : �������ڣ�û���κ��߼�
//***************************************************************************************


#include <windows.h>
#include "Box.h"


// Ĭ����Ļ
static int gClientWidth = 1280;
static int gClientHeight = 720;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
	auto box = Box(gClientWidth, gClientHeight, L"D3D Box");
	return WinApplication::Run(&box, hInstance, nCmdShow);
}