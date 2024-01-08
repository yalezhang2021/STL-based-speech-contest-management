#include "speechManager.h"

int main()
{
	//创建管理类对象
	SpeechManager sm;

	//test CreateSpeaker()
	//for (map<int, Speaker>::iterator it = sm.mSpeaker.begin(); it != sm.mSpeaker.end(); it++)
	//{
	//	cout << it->first << " " << it->second.m_Name << " " << it->second.m_Score[0] << " " << it->second.m_Score[1] << endl;
	//}

	int select = 0;
	srand((unsigned int)time(NULL));

	while (true)
	{
		sm.ShowMenu();

		cout << "请输入您的选择：" << endl;
		cin >> select;

		//状态机器
		switch (select)
		{
		case 0: //退出系统
			sm.ExitSystem();
			break;
		case 1: //开始比赛
			sm.StartSpeech();
			break;
		case 2: //查看记录
			sm.ShowRecord();
			break;
		case 3: //清空记录
			sm.CleanRecord();
			break;

		default:
			system("cls");
			break;
		}
	}

	system("pause");
	return 0;
}