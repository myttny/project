#pragma once
#include <stdlib.h>
#include <iostream>
#include <unordered_map>
#include <string>
#include <unordered_set>
#include <Windows.h>
#include <assert.h>
#include <fstream>
#include <algorithm>
#include <cppjieba/jieba.hpp>

class TextSimilarity
{
public:
	typedef std::unordered_map<std::string, int> wordFreq;//����һ��map��������Ͷ�Ӧ�Ĵ�Ƶ
	typedef std::unordered_set<std::string> wordSet;//������һ��set�����ͣ�ô�
	TextSimilarity(std::string dict);//���幹�캯��������Ϊ��Դ·��
	double getTextSimilarity(const char* file1, const char* file2);

//private:

	void getStopWordTable(const char* stopWordFile);//��ȡͣ�ôʣ�������ͣ�ô��ļ�·�����ַ���
	wordFreq getWordFreq(const char* file);//���ļ��л�ȡ�ı�
	std::string UTF8ToGBK(std::string str);//ת��
	std::string GBKToUTF8(std::string str);//ת��
	//���Ѿ�ͳ�ƴ�Ƶ�����飬����value�Ӵ�С����
	std::vector<std::pair<std::string, int>> sortByValueReverse(wordFreq& wf);

	//��һ��������ɵ�vector��ѡȡһ���ĵ��ʣ���ŵ�set�У���Ϊset����Ȼȥ��Ч��
	void selectAimWords(std::vector<std::pair<std::string, int>>& wfvec, wordSet& wset);
	//������Ƶ����
	std::vector<double> getOneHot(wordSet& wset, wordFreq& wf);
	//�����������ƶ�
	double cosine(std::vector<double> oneHot1, std::vector<double> oneHot2);

	std::string DICT;
	std::string DICT_PATH;
	std::string HMM_PATH;
	std::string USER_DICT_PATH;
	std::string IDF_PATH;
	std::string STOP_WORD_PATH;
	cppjieba::Jieba _jieba;//����jieba�������ڷִ�

	wordSet _stopWordSet;//���ͣ�ô�
	int _maxWordNumber;//ָ����ȡ�����ʵĸ�����������������vector��ȡ��ǰ50��


};