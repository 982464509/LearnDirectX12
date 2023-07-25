//***************************************************************************************
// GameTimer.cpp by Frank Luna (C) 2011 All Rights Reserved.
//***************************************************************************************

#include <windows.h>
#include "GameTimer.h"

GameTimer::GameTimer() :
	mSecondsPerCount(0.0),
	mDeltaTime(-1.0),
	mBaseTime(0),
	mPausedTime(0),
	mPrevTime(0),
	mCurrTime(0),	
	mStopped(false)
{
	__int64 countsPerSec;	//���ܼ�ʱ����Ƶ�ʣ���λ������/�룩
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
	mSecondsPerCount = 1.0 / (double)countsPerSec;	
}

//�����Ե���Reset������ʼ������ͣʱ�����ʱ��
float GameTimer::TotalTime()const
{	
	if( mStopped )
	{
		return (float)(((mStopTime - mPausedTime)-mBaseTime)*mSecondsPerCount);
	}	
	else
	{
		return (float)(((mCurrTime-mPausedTime)-mBaseTime)*mSecondsPerCount);
	}
}

float GameTimer::DeltaTime()const
{
	return (float)mDeltaTime;
}

void GameTimer::Reset()
{
	__int64 currTime;	
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);//���ܼ�ʱ����ǰʱ��ֵ���Լ���Ϊ��λ��

	mBaseTime = currTime;
	mPrevTime = currTime;
	mStopTime = 0;
	mStopped  = false;
}

void GameTimer::Start()
{
	__int64 startTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&startTime);

	// �ۼӵ���stop��start��Է���֮�����ͣʱ�̼��
	//                     |<-------d------->|
	// ----*---------------*-----------------*------------> time
	//  mBaseTime       mStopTime        startTime     
	if( mStopped )
	{
		mPausedTime += (startTime - mStopTime);	// �ۼ���ͣʱ��
		// �����¿�����ʱ��ʱ��ǰһ֡��ʱ��mPrevTime����Ч�ģ�������Ϊ���洢������ͣʱǰһ
		// ֡�Ŀ�ʼʱ�̣������Ҫ��������Ϊ��ǰʱ��
		mPrevTime = startTime;
		// �Ѳ�����ֹͣ״̬����
		mStopTime = 0;
		mStopped  = false;
	}
}

void GameTimer::Stop()
{
	if( !mStopped )
	{
		__int64 currTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&currTime);		
		mStopTime = currTime;
		mStopped  = true;
	}
}

void GameTimer::Tick()
{
	if( mStopped )
	{
		mDeltaTime = 0.0;
		return;
	}
	// ��ñ�֡��ʼ��ʾ��ʱ��
	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
	mCurrTime = currTime;	
	// ��֡��ǰһ֡��ʱ���
	mDeltaTime = (mCurrTime - mPrevTime)*mSecondsPerCount;
	// ׼�����㱾֡����һ֡��ʱ���
	mPrevTime = mCurrTime;
	// ʹʱ���Ϊ�Ǹ�ֵ��DXSDK�е�CDXUTTimerʾ��ע�����ᵽ��������������ڽ���ģʽ��������
	// ������֡��ʱ���Ĺ������л�����һ��������ʱ����QueryPerformanceCounter���������ε�
	// �ò�����ͬһ�������ϣ�����mDeltaTime�п��ܻ��Ϊ��ֵ
	if(mDeltaTime < 0.0)
	{
		mDeltaTime = 0.0;
	}
}