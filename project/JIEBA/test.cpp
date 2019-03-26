#include "TextSimilarity.h"

using namespace std;

void testWordFreq()
{
	TextSimilarity ts1("dict");//����һ�������ڵ�ǰĿ¼�»�ȡ�ļ�
	TextSimilarity ts2("dict");
	//����һ��map��װ������ļ�·���´򿪵��ļ����ִʣ�ͳ�ƴ�Ƶ��Ľ��
	TextSimilarity::wordFreq wf1 = ts1.getWordFreq("test1.txt");
	TextSimilarity::wordFreq wf2 = ts2.getWordFreq("test2.txt");
	//wf����װ���Ƿִʣ�ȥͣ�ôʣ�ͳ�ƴ�Ƶ�Ľ������ӡ����
	/*for (const auto& w : wf)
	{
		cout << ts.UTF8ToGBK(w.first) << ": " << w.second << endl;
	}*/
	//��wf���map������ݰ���valueֵ������������
	//����һ��������������������֮��Ľ��
	vector<pair<string, int>> wfvec1 = ts1.sortByValueReverse(wf1);
	vector<pair<string, int>> wfvec2 = ts2.sortByValueReverse(wf2);
	/*for (const auto& e : wfvec)
	{
		cout << ts.UTF8ToGBK(e.first) << ": " << e.second << endl;
	}*/
	TextSimilarity::wordSet wset;
	ts1.selectAimWords(wfvec1, wset);
	ts2.selectAimWords(wfvec2, wset);
	//����set�д�ŵ���ѡȡ��һ�������ĵ���
	for (const auto& e : wset)
	{
		cout << ts1.UTF8ToGBK(e) << ' ' ;
	}
	cout << endl;
	vector<double> oneHot1 = ts1.getOneHot(wset, wf1);
	vector<double> oneHot2 = ts2.getOneHot(wset, wf2);

    cout << "�ļ����ƶ�Ϊ: " << ts1.cosine(oneHot1, oneHot2) << endl;

}

int main()
{
	testWordFreq();
	//cout << "niho" << endl;
	system("pause");
	return 0;
}