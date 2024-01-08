#pragma once
#include <iostream>
using namespace std;
#include <vector>
#include <map>
#include "speaker.h"

//����ݽ�������
class SpeechManager
{
public:
	//���캯��
	SpeechManager();

	//����
	void ShowMenu();
	void ExitSystem();
	void InitSpeech();
	void CreateSpeaker();
	void StartSpeech();
	void SpeechDraw();
	void SpeechContest();
	void ShowScore();
	void SaveRecord();
	void LoadRecord();
	void ShowRecord();
	void CleanRecord();

	//��������
	vector<int>vRound1; //���ѡ�ֱ��
	vector<int>vRound2; //���ѡ�ֱ��
	vector<int>vVictory; //ǰ����ѡ�ֱ��
	map<int, Speaker> mSpeaker;
	map<int, vector<string>> mRecord; //��������¼��������key����ڼ��죬 value��¼�������Ϣ

	int roundIndex;
	bool fileIsEmpty; //�ж��ļ��Ƿ�Ϊ��

	//��������
	~SpeechManager();

};