//***************************************************************************************
// @author : zxz
// @file : Main.cpp
// @description : 程序的入口，没有任何逻辑
//***************************************************************************************


#include <windows.h>
#include "ClearColor.h"


// 默认屏幕
static int gClientWidth = 1280;
static int gClientHeight = 720;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
	auto clearColor = ClearColor(gClientWidth, gClientHeight, L"D3D Color");
	return WinApplication::Run(&clearColor, hInstance, nCmdShow);
}