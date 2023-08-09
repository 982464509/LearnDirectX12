//***************************************************************************************
// @author : zxz
// @file : Main.cpp
// @description : �������ڣ�û���κ��߼�
//***************************************************************************************


#include <windows.h>
#include "0Example/1_ClearColor.h"
#include "0Example/2_SimplerBox.h"
#include "0Example/3_Shapes.h"


// Ĭ����Ļ
static int gClientWidth = 1280;
static int gClientHeight = 720;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
	//1. Iint Color
	// auto clearColor = ClearColor(gClientWidth, gClientHeight, L"D3D Color");
	// return WinApplication::Run(&clearColor, hInstance, nCmdShow);

	// 2. Box
	 /*auto simplerbox = SimplerBox(gClientWidth, gClientHeight, L"D3D Box");
	 return WinApplication::Run(&simplerbox, hInstance, nCmdShow);*/

	// 3. Shapes
	auto shapes = Shapes(gClientWidth, gClientHeight, L"D3D Shapes");
	return WinApplication::Run(&shapes, hInstance, nCmdShow);
}