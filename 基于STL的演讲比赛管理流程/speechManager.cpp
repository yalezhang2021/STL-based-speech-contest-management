#include "speechManager.h"
#include <algorithm>
#include <functional>
#include <numeric>
#include <deque>
#include <iostream>
#include <fstream>
#include <string>

#define FILENAME "record.csv"

//���캯��
SpeechManager::SpeechManager()
{
	//��ʼ������������
	this->InitSpeech();
	this->CreateSpeaker();
	this->LoadRecord(); //��ȡ�����¼
}

//�˵�����
void SpeechManager::ShowMenu()
{
	cout << "*********************************" << endl;
	cout << "********* ��ӭ�μ��ݽ����� ********" << endl;
	cout << "********* 1.��ʼ�ݽ�����  ********" << endl;
	cout << "********* 2.�鿴�����¼  ********" << endl;
	cout << "********* 3.��ձ�����¼  ********" << endl;
	cout << "********* 0.�˳��ݽ�����  ********" << endl;
	cout << "*********************************" << endl;
	cout << endl;
}

//�˳�ϵͳ
void SpeechManager::ExitSystem()
{
	cout << "��л����ʹ��!" << endl;
	system("pause");
	exit(0);
}

void SpeechManager::InitSpeech()
{
	//�������ÿ�
	this->vRound1.clear();
	this->vRound2.clear();
	this->vVictory.clear();
	this->mSpeaker.clear();

	this->roundIndex = 1;
	this->mRecord.clear(); //����¼������ Ҳ���
}

void SpeechManager::CreateSpeaker()
{
	string nameSeed = "ABCDEFGHIJKL";

	
	for (int i = 0; i < nameSeed.size(); i++)
	{
		string name = "ѡ��";
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
	//��һ�ֱ���
	//1.��ǩ
	this->SpeechDraw();
	//2.����
	this->SpeechContest();
	//3.��ʾ�������
	this->ShowScore();
	//�ڶ��ֱ���
	this->roundIndex++;
	//1.��ǩ
	this->SpeechDraw();
	//2.����
	this->SpeechContest();
	//3.��ʾ���ս��
	this->ShowScore();
	//4.�������
	this->SaveRecord();
	//���ñ��� ��ȡ��¼
	this->InitSpeech();
	this->CreateSpeaker();
	this->LoadRecord();

	cout << "���������ϣ�" << endl;
	system("pause");
	system("cls");
}

void SpeechManager::SpeechDraw()
{
	cout << "��" << this->roundIndex << "�ֱ���ѡ�����ڳ�ǩ" << endl;
	cout << "---------------------------------" << endl;
	cout << "��ǩ���ݽ�˳�����£�" << endl;

	//����Ŀ�����
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
	cout << "��" << this->roundIndex << "�ֱ�����ʼ!" << endl;
	multimap<double, int, greater<double>>groupScore; //��ʱ����������key�������� value��ѡ�ֱ��
	vector<int>v_Src = this->roundIndex == 1 ? vRound1 : vRound2; //��������Ա����

	int num = 0; //��¼������6��һ��
	for (vector<int>::iterator it = v_Src.begin(); it != v_Src.end(); it++)
	{
		num++;

		//��ί���
		deque<double>d;
		for (int i = 0; i < 10; i++)
		{
			double score = (rand() % 401 + 600) / 10.f;//�����С���ķ���
			d.push_back(score);
		}

		sort(d.begin(), d.end(), greater<double>());
		d.pop_back();
		d.pop_front();

		double sum = accumulate(d.begin(), d.end(), 0.0f);
		double avg = sum / d.size();

		this->mSpeaker[*it].m_Score[this->roundIndex - 1] = avg;

		//6����һ�飬����ʱ��������
		groupScore.insert(make_pair(avg, *it));

		if (num % 6 == 0)
		{
			cout << "��" << num / 6 << "С��������Σ�" << endl;
			for (multimap<double, int, greater<double>>::iterator it = groupScore.begin(); 
				it != groupScore.end(); it++)
			{
				cout << "��ţ�" << it->second << ", ������" << this->mSpeaker[it->second].m_Name <<",  �ɼ���" << it->first << endl;
			}			
			cout << endl;

			//ȡǰ����
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
	cout << "��" << this->roundIndex << "�ֱ������" << endl;
	system("pause");
}

void SpeechManager::ShowScore()
{
	cout << "\n��" << this->roundIndex << "�ֽ���ѡ����Ϣ���£�" << endl;

	vector<int>v = this->roundIndex == 1 ? vRound2 : vVictory;

	for (vector<int>::iterator it = v.begin(); it != v.end(); it++)
	{
		cout << "��ţ�" << * it << ", ������" << this->mSpeaker[*it].m_Name << ", �ɼ�: " << this->mSpeaker[*it].m_Score[this->roundIndex - 1] << endl;
	}
	cout << "---------------------------------" << endl;
	cout << endl;
	system("pause");
	system("cls"); //��������
	this->ShowMenu();
}

void SpeechManager::SaveRecord()
{
	ofstream ofs;
	ofs.open(FILENAME, ios::out | ios::app);//��׷�ӵķ�ʽд�� app

	for (vector<int>::iterator it = vVictory.begin(); it != vVictory.end(); it++)
	{
		ofs << *it << ","
			<< this->mSpeaker[*it].m_Name << ","
			<< this->mSpeaker[*it].m_Score[this->roundIndex - 1] << ",";
	}
	ofs << endl; //���ļ��ﻻ��
	ofs.close();
	
	cout << "��¼����ɹ���" << endl;
	cout << "---------------------------------" << endl;
	//�����ļ���Ϊ��״̬
	this->fileIsEmpty = false;
}

void SpeechManager::LoadRecord()
{
	//ifstream ifs;
	//ifs.open(FILENAME, ios::in);
	ifstream ifs(FILENAME, ios::in);

	//�ж��ļ��Ƿ����
	if (!ifs.is_open())
	{
		cout << "�ļ������ڣ�" << endl;
		this->fileIsEmpty = true;
		ifs.close();
		return;
	}

	//�ж��ļ��Ƿ�Ϊ��
	char ch;
	ifs >> ch;
	if (ifs.eof())
	{
		cout << "�ļ�Ϊ�գ�" << endl;
		this->fileIsEmpty = true;
		ifs.close();
		return;
	}

	this->fileIsEmpty = false;
	ifs.putback(ch);//��ȡ�ĵ����ַ��Ż�ȥ
	
	string data;
	int index = 0;

	while (ifs>>data)
	{
		//cout << data << endl;

		int pos = -1;//�鵽","λ�õı���
		int start = 0;
		vector<string>v; //���9��string�ַ���

		while (true)
		{
			pos = data.find(",", start);
			if (pos == -1)
			{
				//û���ҵ�
				break;
			}
			string temp = data.substr(start, pos - start);//�Ӵ�
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
	//	cout << it->first << "�ھ���ţ�" << it->second[0] << " ������" << it->second[1] << " ������" << it->second[2] << endl;
	//}

	ifs.close();
}

void SpeechManager::ShowRecord()
{
	if (this->fileIsEmpty)
	{
		cout << "�ļ������ڣ����¼Ϊ�գ�" << endl;
	}
	else
	{
		for (map<int, vector<string>>::iterator it = mRecord.begin(); it != mRecord.end(); it++)
		{
			cout << "��" << (*it).first + 1 << "��" <<
				"�ھ���ţ�" << it->second[0] << " ������" << it->second[1] << " �÷֣�" << it->second[2] << " "
				"�Ǿ���ţ�" << it->second[3] << " ������" << it->second[4] << " �÷֣�" << it->second[5] << " "
				"������ţ�" << it->second[6] << " ������" << it->second[7] << " �÷֣�" << it->second[8] << endl;
		}
	}

	system("pause");
	system("cls");
}

void SpeechManager::CleanRecord()
{
	cout << "ȷ��ҪĨ����������?" << endl;
	cout << "y --- ȷ��" << endl;
	cout << "n --- ����" << endl;

	char dSelect;
	cin >> dSelect;

	if (this->fileIsEmpty)
	{
		cout << "�ļ������ڻ��¼Ϊ��" << endl;
	}
	else if (dSelect == 'y')
	{
		ofstream ofs(FILENAME, ios::trunc);
		ofs.close();

		this->InitSpeech();
		this->CreateSpeaker();
		this->LoadRecord();
		cout << "Ĩ���ɹ���" << endl;
	}

	system("pause");
	system("cls");
}

//��������
SpeechManager::~SpeechManager()
{

}