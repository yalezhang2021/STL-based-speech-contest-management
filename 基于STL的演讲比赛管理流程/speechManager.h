#pragma once
#include <iostream>
using namespace std;
#include <vector>
#include <map>
#include "speaker.h"

//设计演讲管理类
class SpeechManager
{
public:
	//构造函数
	SpeechManager();

	//功能
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

	//比赛属性
	vector<int>vRound1; //存放选手编号
	vector<int>vRound2; //存放选手编号
	vector<int>vVictory; //前三名选手编号
	map<int, Speaker> mSpeaker;
	map<int, vector<string>> mRecord; //存放往届记录的容器。key代表第几届， value记录具体的信息

	int roundIndex;
	bool fileIsEmpty; //判断文件是否为空

	//析构函数
	~SpeechManager();

};