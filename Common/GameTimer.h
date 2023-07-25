//***************************************************************************************
// GameTimer.h by Frank Luna (C) 2011 All Rights Reserved.
//***************************************************************************************

#ifndef GAMETIMER_H	//����ʽ���
#define GAMETIMER_H

class GameTimer
{
public:
	GameTimer();

	float TotalTime()const; // in seconds
	float DeltaTime()const; // in seconds

	// �ڿ�ʼ��Ϣѭ��֮ǰ����
	void Reset(); 
	// �����ʱ����ͣʱ����
	void Start(); 
	// ��ͣ��ʱ��ʱ����
	void Stop();  
	// ÿ֡��Ҫ����
	void Tick();  

private:
	//��/����
	double mSecondsPerCount;
	//��֮֡���ʱ����
	double mDeltaTime;
	//Ӧ�ó���Ŀ�ʼʱ��
	__int64 mBaseTime;
	//�洢����������ͣʱ��֮��
	__int64 mPausedTime;
	//��ʱ��ֹͣ����ͣ����ʱ�̣���˼��ɼ�¼��ͣ��ʱ��
	__int64 mStopTime;

	__int64 mPrevTime;

	__int64 mCurrTime;

	bool mStopped;
};

#endif