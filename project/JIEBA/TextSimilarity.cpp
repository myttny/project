#include "TextSimilarity.h"

using namespace std;

TextSimilarity::TextSimilarity(string dict)//���幹�캯��������Ϊ��Դ·��
	: DICT(dict)
	, DICT_PATH(dict + "/jieba.dict.utf8")
	, HMM_PATH(dict + "/hmm_model.utf8")	
	, USER_DICT_PATH(dict + "/user.dict.utf8")
	, IDF_PATH(dict + "/idf.utf8")
	, STOP_WORD_PATH(dict + "/stop_words.utf8")
	, _jieba(DICT_PATH,
		HMM_PATH,
		USER_DICT_PATH,
		IDF_PATH,
		STOP_WORD_PATH)
	, _maxWordNumber(20)
{
	getStopWordTable(STOP_WORD_PATH.c_str());//��ʼ��ͣ�ôʱ�
}

string TextSimilarity::GBKToUTF8(string s)
{
	int len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), -1, NULL, 0);
	WCHAR* str1 = new WCHAR[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), -1, str1, len);

	len = WideCharToMultiByte(CP_UTF8, 0, str1, -1, NULL, 0, NULL, NULL);
	char* str2 = new char[len];
	WideCharToMultiByte(CP_UTF8, 0, str1, -1, str2, len, NULL, NULL);
	string strOutUTF8(str2);
	delete[]str1;
	str1 = NULL;
	delete[]str2;
	str2 = NULL;
	return strOutUTF8;
}

string TextSimilarity::UTF8ToGBK(string s)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, NULL, 0);
	WCHAR* str1 = new WCHAR[len];
	MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, str1, len);

	len = WideCharToMultiByte(CP_ACP, 0, str1, -1, NULL, 0, NULL, NULL);
	char* str2 = new char[len];
	WideCharToMultiByte(CP_ACP, 0, str1, -1, str2, len, NULL, NULL);
	string strOutGBK(str2);
	delete[]str1;
	str1 = NULL;
	delete[]str2;
	str2 = NULL;
	return strOutGBK;
}

TextSimilarity::wordFreq TextSimilarity::getWordFreq(const char* filename)//���ļ��л�ȡ�ı�
{
	ifstream fin(filename);//���ļ�
	if (!fin.is_open())//�ж��ļ��Ƿ��
	{
		cout << "open file " << filename << " fail" << endl;
		return wordFreq();//����һ���յ�map
	}

	string line;//����һ�������������ÿһ�е��ַ���
	wordFreq wf;//����һ��map�������һ�����ʺ����Ӧ�Ĵ�Ƶ
	while (!fin.eof())
	{
		//�����ı�
		getline(fin, line);//�ڴ򿪵��ļ��ж���һ�е�line��
		//����������GBK����ģ�����Ҫת��UTF8
		line = GBKToUTF8(line);
		//��ʼ�ִ�
		vector<string> words;//����һ�����飬����װ�ִ�֮����ַ���
		_jieba.Cut(line, words, true);//�ִ�
		//ͳ�ƴ�Ƶ
		for (const auto& e : words)//��������ȡ�����ʣ�ͳ�ƴ�Ƶ
		{
			//ȥ��ͣ�ô�
			if (_stopWordSet.count(e) > 0)//�жϸĴ��Ƿ���ͣ�ôʱ��У��ڵĻ�������
				continue;//��ͳ��
			else
			{
				if (wf.count(e) > 0)//�ж��Ƿ���map�У��ڵĻ���++ count����0��ʾ����
					wf[e]++;
				else
					wf[e] = 1;//���ڵĻ��͸�ֵΪ1
			}
		}
	}
	fin.close();
	return wf;
}

void TextSimilarity::getStopWordTable(const char* stopWordFile)//��ȡͣ�ô�
{
	ifstream fin(stopWordFile);//��������·���¶����ļ�
	if (!fin.is_open())
	{
		cout << "open file" << stopWordFile << "fail" << endl;
		return;
	}

	string line;
	while (!fin.eof())
	{
		getline(fin, line);//����ǰ�е��ַ�����ȡ��line��
		_stopWordSet.insert(line);
	}
	fin.close();
}

//������
bool cmpReverse(pair<string, int> lp, pair<string, int> rp)
{
	return lp.second > rp.second;//��value��ֵ��������
}

//���Ѿ�ͳ�ƴ�Ƶ�����飬����value�Ӵ�С����
vector<pair<string, int>> TextSimilarity::sortByValueReverse(TextSimilarity::wordFreq& wf)
{
	vector<pair<string, int>> wfvector(wf.begin(), wf.end());
	sort(wfvector.begin(), wfvector.end(), cmpReverse);
	return wfvector;
}

//��һ��������ɵ�vector��ѡȡһ���ĵ��ʣ���ŵ�set�У���Ϊset����Ȼȥ��Ч��
void TextSimilarity::selectAimWords(vector<pair<string, int>>& wfvec, TextSimilarity::wordSet& wset)
{
	int length = wfvec.size();
	int sz = length > _maxWordNumber ? _maxWordNumber : length;
	for (int i = 0; i < sz; i++)
	{
		wset.insert(wfvec[i].first);
	}
	//�Ѿ���ɴ������ı�ȡ�����Ĵ�Ƶ����
}

//������Ƶ����
vector<double> TextSimilarity::getOneHot(TextSimilarity::wordSet& wset, TextSimilarity::wordFreq& wf)
{
	//����wset��ÿһ���ʣ�Ȼ����wf���map�в��Ҵ�Ƶ������vector�з���
	vector<double> OneHot;
	for (const auto& e : wset)
	{
		if (wf.count(e))
			//OneHot���value
			OneHot.push_back(wf[e]);
		else
			OneHot.push_back(0);
	}
	return OneHot;
}

//�����������ƶ�
double TextSimilarity::cosine(vector<double> oneHot1, vector<double> oneHot2)
{
	double modelOneHot1 = 0;
	double modelOneHot2 = 0;
	double dotProduct = 0;
	unsigned int i = 0;
	assert(oneHot1.size() == oneHot2.size());
	for (i = 0; i < oneHot1.size(); i++)
	{
		dotProduct += (oneHot1[i] * oneHot2[i]);
	}
	for (i = 0; i < oneHot1.size(); i++)
	{
		modelOneHot1 += pow(oneHot1[i], 2);
		modelOneHot2 += pow(oneHot2[i], 2);
	}
	modelOneHot1 = pow(modelOneHot1, 0.5);
	modelOneHot2 = pow(modelOneHot2, 0.5);

	return dotProduct / (modelOneHot1 * modelOneHot2);
}