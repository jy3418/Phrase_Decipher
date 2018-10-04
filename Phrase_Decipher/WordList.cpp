#include "provided.h"
#include "MyHash.h"
#include <string>
#include <vector>
#include <functional>
#include <iostream>
#include <fstream>
#include <algorithm>
using namespace std;

const string ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

class WordListImpl
{
public:
    WordListImpl();
    bool loadWordList(string filename);
    bool contains(string word) const;
    vector<string> findCandidates(string cipherWord, string currTranslation) const;
private:
    MyHash<string, vector<string>> m_wordList;
    string letterPattern(string value) const;
};

WordListImpl::WordListImpl()
:m_wordList()
{ }

string WordListImpl::letterPattern(string value) const
{
    bool shouldSkip = false;
    bool uniqLetter = true;
    string uniqKey = "";
    string key = "";
    for (int i = 0; i < value.size(); i++)
    {
        if (! (islower(value[i]) || isupper(value[i]) || value[i] == '\''))
        {
            shouldSkip = true;
            break;
        }
        int j = 0;
        value[i] = tolower(value[i]);
        while (j < uniqKey.size())
        {
            if (value[i] == uniqKey[j])
            {
                key += ALPHABET[j];
                uniqLetter = false;
                break;
            }
            j++;
        }
        if (uniqLetter)
        {
            key += ALPHABET[j];
            uniqKey += value[i];
        }
        uniqLetter = true;
    }
    return key;
}

bool WordListImpl::loadWordList(string filename)
{
    m_wordList.reset();
    
    ifstream infile(filename);
    if (! infile )
    {
        return false;
    }
    string value;
    while (getline(infile, value))
    {
        bool shouldSkip = false;
        bool uniqLetter = true;
        string uniqKey = "";
        string key = "";
        for (int i = 0; i < value.size(); i++)
        {
            if (! (islower(value[i]) || isupper(value[i]) || value[i] == '\''))
            {
                shouldSkip = true;
                break;
            }
            int j = 0;
            value[i] = tolower(value[i]);
            while (j < uniqKey.size())
            {
                if (value[i] == uniqKey[j])
                {
                    key += ALPHABET[j];
                    uniqLetter = false;
                    break;
                }
                j++;
            }
            if (uniqLetter)
            {
                key += ALPHABET[j];
                uniqKey += value[i];
            }
            uniqLetter = true;
        }
        if (shouldSkip)
        {
            shouldSkip = false;
            continue;
        }
        vector<string>* valueptr = m_wordList.find(key);
        if (valueptr == nullptr)
        {
            vector<string> valueVector;
            valueVector.push_back(value);
            m_wordList.associate(key, valueVector);
        }
        else
        {
            valueptr->push_back(value);
        }
    }
    return true;
}

bool WordListImpl::contains(string word) const
{
    string key = letterPattern(word);
    const vector<string>* valueptr = m_wordList.find(key);
    if (valueptr == nullptr)
        return false;
    for (int i = 0; i < word.size(); i++)
        word[i] = tolower(word[i]);
    for (int i = 0; i < valueptr->size(); i++)
    {
        string temp = (*valueptr)[i];
        if (temp.size() == word.size())
        {
            for (int j = 0; j < temp.size(); j++)
                temp[i] = tolower(temp[i]);
            if (temp == word)
                return true;
        }
    }
    return false;
}

vector<string> WordListImpl::findCandidates(string cipherWord, string currTranslation) const
{
    vector<string> result;
    
    if (cipherWord.size() != currTranslation.size())
        return result;
    for (int i = 0; i < cipherWord.size(); i++)
    {
        if (! (islower(cipherWord[i]) || isupper(cipherWord[i]) || cipherWord[i] == '\''))
            return result;
        if (! (islower(currTranslation[i]) || isupper(currTranslation[i]) || currTranslation[i] == '\'' || currTranslation[i] == '?'))
            return result;
        if (isalpha(currTranslation[i]) && ! isalpha(cipherWord[i]))
            return result;
        if (currTranslation[i] == '\'' && cipherWord[i] != '\'')
            return result;
    }
    
    string key = letterPattern(cipherWord);
    const vector<string>* valueptr = m_wordList.find(key);
    if (valueptr == nullptr)
        return result;
    
    for (int i = 0; i < valueptr->size(); i++)
    {
        string temp = (*valueptr)[i];
        bool toAdd = true;
        for (int j = 0; j < temp.size(); j++)
        {
            temp[j] = tolower(temp[j]);
            currTranslation[j] = tolower(currTranslation[j]);
            if (currTranslation[j] != '?')
            {
                if (temp[j] != currTranslation[j])
                {
                    toAdd = false;
                    break;
                }
            }
        }
        if (toAdd)
            result.push_back((*valueptr)[i]);
    }
    return result;
}

//***** hash functions for string, int, and char *****

unsigned int hash(const std::string& s)
{
    return std::hash<std::string>()(s);
}

unsigned int hash(const int& i)
{
    return std::hash<int>()(i);
}

unsigned int hash(const char& c)
{
    return std::hash<char>()(c);
}

//******************** WordList functions ************************************

// These functions simply delegate to WordListImpl's functions.
// You probably don't want to change any of this code.

WordList::WordList()
{
    m_impl = new WordListImpl;
}

WordList::~WordList()
{
    delete m_impl;
}

bool WordList::loadWordList(string filename)
{
    return m_impl->loadWordList(filename);
}

bool WordList::contains(string word) const
{
    return m_impl->contains(word);
}

vector<string> WordList::findCandidates(string cipherWord, string currTranslation) const
{
   return m_impl->findCandidates(cipherWord, currTranslation);
}
