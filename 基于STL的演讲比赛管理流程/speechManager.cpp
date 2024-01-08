#include "speechManager.h"
#include <algorithm>
#include <functional>
#include <numeric>
#include <deque>
#include <iostream>
#include <fstream>
#include <string>

#define FILENAME "record.csv"

//构造函数
SpeechManager::SpeechManager()
{
	//初始化容器和属性
	this->InitSpeech();
	this->CreateSpeaker();
	this->LoadRecord(); //获取往届记录
}

//菜单功能
void SpeechManager::ShowMenu()
{
	cout << "*********************************" << endl;
	cout << "********* 欢迎参加演讲比赛 ********" << endl;
	cout << "********* 1.开始演讲比赛  ********" << endl;
	cout << "********* 2.查看往届记录  ********" << endl;
	cout << "********* 3.清空比赛记录  ********" << endl;
	cout << "********* 0.退出演讲比赛  ********" << endl;
	cout << "*********************************" << endl;
	cout << endl;
}

//退出系统
void SpeechManager::ExitSystem()
{
	cout << "感谢您的使用!" << endl;
	system("pause");
	exit(0);
}

void SpeechManager::InitSpeech()
{
	//容器都置空
	this->vRound1.clear();
	this->vRound2.clear();
	this->vVictory.clear();
	this->mSpeaker.clear();

	this->roundIndex = 1;
	this->mRecord.clear(); //将记录的容器 也清空
}

void SpeechManager::CreateSpeaker()
{
	string nameSeed = "ABCDEFGHIJKL";

	
	for (int i = 0; i < nameSeed.size(); i++)
	{
		string name = "选手";
		name += nameSeed[i];

		Speaker sp;
		sp.m_Name = name;

		for (int j = 0; j < 2; j++)
		{
			sp.m_Score[j] = 0;
		}

		this->vRound1.push_back(i + 10001);
		this->mSpeaker.insert(pair<int, Speaker>(i + 10001, sp));
	}
}

void SpeechManager::StartSpeech()
{
	//第一轮比赛
	//1.抽签
	this->SpeechDraw();
	//2.比赛
	this->SpeechContest();
	//3.显示晋级结果
	this->ShowScore();
	//第二轮比赛
	this->roundIndex++;
	//1.抽签
	this->SpeechDraw();
	//2.比赛
	this->SpeechContest();
	//3.显示最终结果
	this->ShowScore();
	//4.保存分数
	this->SaveRecord();
	//重置比赛 获取记录
	this->InitSpeech();
	this->CreateSpeaker();
	this->LoadRecord();

	cout << "本届比赛完毕！" << endl;
	system("pause");
	system("cls");
}

void SpeechManager::SpeechDraw()
{
	cout << "第" << this->roundIndex << "轮比赛选手正在抽签" << endl;
	cout << "---------------------------------" << endl;
	cout << "抽签后演讲顺序如下：" << endl;

	//用三目运算符
	vector<int>v = this->roundIndex == 1 ? vRound1 : vRound2;

	random_shuffle(v.begin(), v.end());
	for (vector<int>::iterator it = v.begin(); it != v.end(); it++)
	{
		cout << *it << " ";
	}
	cout << endl;
	cout << "---------------------------------" << endl;
	cout << endl;
	system("pause");
}

void SpeechManager::SpeechContest()
{
	cout << "第" << this->roundIndex << "轮比赛开始!" << endl;
	multimap<double, int, greater<double>>groupScore; //临时容器，保存key：分数， value：选手编号
	vector<int>v_Src = this->roundIndex == 1 ? vRound1 : vRound2; //比赛的人员容器

	int num = 0; //记录人数，6个一组
	for (vector<int>::iterator it = v_Src.begin(); it != v_Src.end(); it++)
	{
		num++;

		//评委打分
		deque<double>d;
		for (int i = 0; i < 10; i++)
		{
			double score = (rand() % 401 + 600) / 10.f;//随机出小数的方法
			d.push_back(score);
		}

		sort(d.begin(), d.end(), greater<double>());
		d.pop_back();
		d.pop_front();

		double sum = accumulate(d.begin(), d.end(), 0.0f);
		double avg = sum / d.size();

		this->mSpeaker[*it].m_Score[this->roundIndex - 1] = avg;

		//6个人一组，用临时容器保存
		groupScore.insert(make_pair(avg, *it));

		if (num % 6 == 0)
		{
			cout << "第" << num / 6 << "小组比赛名次：" << endl;
			for (multimap<double, int, greater<double>>::iterator it = groupScore.begin(); 
				it != groupScore.end(); it++)
			{
				cout << "编号：" << it->second << ", 姓名：" << this->mSpeaker[it->second].m_Name <<",  成绩：" << it->first << endl;
			}			
			cout << endl;

			//取前三名
			int count = 0;
			for (multimap<double, int, greater<double>>::iterator it = groupScore.begin();
				it != groupScore.end() && count < 3; it++, count++)
			{
				if (this->roundIndex == 1)
				{
					vRound2.push_back(it->second);
				}
				else
				{
					vVictory.push_back(it->second);
				}
			}
			groupScore.clear();
		}
	}
	cout << "第" << this->roundIndex << "轮比赛完毕" << endl;
	system("pause");
}

void SpeechManager::ShowScore()
{
	cout << "\n第" << this->roundIndex << "轮晋级选手信息如下：" << endl;

	vector<int>v = this->roundIndex == 1 ? vRound2 : vVictory;

	for (vector<int>::iterator it = v.begin(); it != v.end(); it++)
	{
		cout << "编号：" << * it << ", 姓名：" << this->mSpeaker[*it].m_Name << ", 成绩: " << this->mSpeaker[*it].m_Score[this->roundIndex - 1] << endl;
	}
	cout << "---------------------------------" << endl;
	cout << endl;
	system("pause");
	system("cls"); //清屏操作
	this->ShowMenu();
}

void SpeechManager::SaveRecord()
{
	ofstream ofs;
	ofs.open(FILENAME, ios::out | ios::app);//用追加的方式写入 app

	for (vector<int>::iterator it = vVictory.begin(); it != vVictory.end(); it++)
	{
		ofs << *it << ","
			<< this->mSpeaker[*it].m_Name << ","
			<< this->mSpeaker[*it].m_Score[this->roundIndex - 1] << ",";
	}
	ofs << endl; //在文件里换行
	ofs.close();
	
	cout << "记录保存成功！" << endl;
	cout << "---------------------------------" << endl;
	//更改文件不为空状态
	this->fileIsEmpty = false;
}

void SpeechManager::LoadRecord()
{
	//ifstream ifs;
	//ifs.open(FILENAME, ios::in);
	ifstream ifs(FILENAME, ios::in);

	//判断文件是否存在
	if (!ifs.is_open())
	{
		cout << "文件不存在！" << endl;
		this->fileIsEmpty = true;
		ifs.close();
		return;
	}

	//判断文件是否为空
	char ch;
	ifs >> ch;
	if (ifs.eof())
	{
		cout << "文件为空！" << endl;
		this->fileIsEmpty = true;
		ifs.close();
		return;
	}

	this->fileIsEmpty = false;
	ifs.putback(ch);//读取的单个字符放回去
	
	string data;
	int index = 0;

	while (ifs>>data)
	{
		//cout << data << endl;

		int pos = -1;//查到","位置的变量
		int start = 0;
		vector<string>v; //存放9个string字符串

		while (true)
		{
			pos = data.find(",", start);
			if (pos == -1)
			{
				//没有找到
				break;
			}
			string temp = data.substr(start, pos - start);//子串
			//cout << temp << endl;
			v.push_back(temp);
			start = pos + 1;
		}
		this->mRecord.insert(make_pair(index, v));
		index++;
	}

	//string data;
	//while (getline(ifs, data))
	//{
	//	cout << data << endl;
	//}

	//for (map<int, vector<string>>::iterator it = mRecord.begin(); it != mRecord.end(); it++)
	//{
	//	cout << it->first << "冠军编号：" << it->second[0] << " 姓名：" << it->second[1] << " 分数：" << it->second[2] << endl;
	//}

	ifs.close();
}

void SpeechManager::ShowRecord()
{
	if (this->fileIsEmpty)
	{
		cout << "文件不存在，或记录为空！" << endl;
	}
	else
	{
		for (map<int, vector<string>>::iterator it = mRecord.begin(); it != mRecord.end(); it++)
		{
			cout << "第" << (*it).first + 1 << "届" <<
				"冠军编号：" << it->second[0] << " 姓名：" << it->second[1] << " 得分：" << it->second[2] << " "
				"亚军编号：" << it->second[3] << " 姓名：" << it->second[4] << " 得分：" << it->second[5] << " "
				"季军编号：" << it->second[6] << " 姓名：" << it->second[7] << " 得分：" << it->second[8] << endl;
		}
	}

	system("pause");
	system("cls");
}

void SpeechManager::CleanRecord()
{
	cout << "确定要抹掉所有数据?" << endl;
	cout << "y --- 确定" << endl;
	cout << "n --- 返回" << endl;

	char dSelect;
	cin >> dSelect;

	if (this->fileIsEmpty)
	{
		cout << "文件不存在或记录为空" << endl;
	}
	else if (dSelect == 'y')
	{
		ofstream ofs(FILENAME, ios::trunc);
		ofs.close();

		this->InitSpeech();
		this->CreateSpeaker();
		this->LoadRecord();
		cout << "抹除成功！" << endl;
	}

	system("pause");
	system("cls");
}

//析构函数
SpeechManager::~SpeechManager()
{

}