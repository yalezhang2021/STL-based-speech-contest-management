#include "speechManager.h"

int main()
{
	//�������������
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

		cout << "����������ѡ��" << endl;
		cin >> select;

		//״̬����
		switch (select)
		{
		case 0: //�˳�ϵͳ
			sm.ExitSystem();
			break;
		case 1: //��ʼ����
			sm.StartSpeech();
			break;
		case 2: //�鿴��¼
			sm.ShowRecord();
			break;
		case 3: //��ռ�¼
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