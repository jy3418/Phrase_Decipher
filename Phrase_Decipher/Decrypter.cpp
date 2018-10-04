#include "provided.h"
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;

class DecrypterImpl
{
public:
    DecrypterImpl();
    bool load(string filename);
    vector<string> crack(const string& ciphertext);
private:
    vector<string> crack(const string& ciphertext, vector<string>& result);
    WordList m_wl;
    Tokenizer m_tk;
    Translator m_tl;
};

DecrypterImpl::DecrypterImpl()
: m_tk(" ,;:.!()[]{}-\"#$%^&")
{ }

bool DecrypterImpl::load(string filename)
{
    if (m_wl.loadWordList(filename))
        return true;
    return false;
}

vector<string> DecrypterImpl::crack(const string& ciphertext)
{
    vector<string> result;
    vector<string> tokens = m_tk.tokenize(ciphertext);
    int chosenIndex = 0, currNumUnknowns = 0, biggestNumUnknowns = 0;
    for (int i = 0; i < tokens.size(); i++)
    {
        currNumUnknowns = 0;
        string currWord = m_tl.getTranslation(tokens[i]);
        for (int j = 0; j < currWord.size(); j++)
        {
            if (currWord[j] == '?')
                currNumUnknowns++;
        }
        if (currNumUnknowns > biggestNumUnknowns)
        {
            chosenIndex = i;
            biggestNumUnknowns = currNumUnknowns;
        }
    }
    
    string chosenWord = tokens[chosenIndex];
    string chosenTranslation = m_tl.getTranslation(chosenWord);
    
    vector<string> possibleWords = m_wl.findCandidates(chosenWord, chosenTranslation);
    if (possibleWords.empty())
    {
        m_tl.popMapping();
        return result;
    }
    sort(possibleWords.begin(), possibleWords.end());
    
    for (int i = 0; i < possibleWords.size(); i++)
    {
        string currentPossibility = possibleWords[i];
        if (! m_tl.pushMapping(chosenWord, currentPossibility))
            continue;
        string tempResult = m_tl.getTranslation(ciphertext);
        vector<string> resultToken = m_tk.tokenize(tempResult);
        int numCorrect = 0;
        bool shouldSkip = false;
        for (int j = 0; j < resultToken.size(); j++)
        {
            bool hasQuestionMark = false;
            string currentToken = resultToken[j];
            for (int k = 0; k < currentToken.size(); k++)
            {
                if (currentToken[k] == '?')
                {
                    hasQuestionMark = true;
                    break;
                }
            }
            if (hasQuestionMark)
                continue;
            else
            {
                if (m_wl.contains(currentToken))
                    numCorrect++;
                else
                {
                    shouldSkip = true;
                    break;
                }
            }
        }
        if (shouldSkip)
        {
            m_tl.popMapping();
            continue;
        }
        if (numCorrect == resultToken.size())
        {
            result.push_back(tempResult);
            m_tl.popMapping();
        }
        else
        {
            crack(ciphertext, result);
        }
    }
    return result;
}

vector<string> DecrypterImpl::crack(const string& ciphertext, vector<string>& result)
{
    vector<string> tokens = m_tk.tokenize(ciphertext);
    int chosenIndex = 0, currNumUnknowns = 0, biggestNumUnknowns = 0;
    for (int i = 0; i < tokens.size(); i++)
    {
        currNumUnknowns = 0;
        string currWord = m_tl.getTranslation(tokens[i]);
        for (int j = 0; j < currWord.size(); j++)
        {
            if (currWord[j] == '?')
                currNumUnknowns++;
        }
        if (currNumUnknowns > biggestNumUnknowns)
        {
            chosenIndex = i;
            biggestNumUnknowns = currNumUnknowns;
        }
    }
    
    string chosenWord = tokens[chosenIndex];
    string chosenTranslation = m_tl.getTranslation(chosenWord);
    
    vector<string> possibleWords = m_wl.findCandidates(chosenWord, chosenTranslation);
    if (possibleWords.empty())
    {
        m_tl.popMapping();
        return result;
    }
    sort(possibleWords.begin(), possibleWords.end());
    
    for (int i = 0; i < possibleWords.size(); i++)
    {
        string currentPossibility = possibleWords[i];
        if (! m_tl.pushMapping(chosenWord, currentPossibility))
            continue;
        string tempResult = m_tl.getTranslation(ciphertext);
        vector<string> resultToken = m_tk.tokenize(tempResult);
        int numCorrect = 0;
        bool shouldSkip = false;
        for (int j = 0; j < resultToken.size(); j++)
        {
            bool hasQuestionMark = false;
            string currentToken = resultToken[j];
            for (int k = 0; k < currentToken.size(); k++)
            {
                if (currentToken[k] == '?')
                {
                    hasQuestionMark = true;
                    break;
                }
            }
            if (hasQuestionMark)
                continue;
            else
            {
                if (m_wl.contains(currentToken))
                    numCorrect++;
                else
                {
                    shouldSkip = true;
                    break;
                }
            }
        }
        if (shouldSkip)
        {
            m_tl.popMapping();
            continue;
        }
        if (numCorrect == resultToken.size())
        {
            result.push_back(tempResult);
            m_tl.popMapping();
        }
        else
        {
            crack(ciphertext, result);
        }
    }
    m_tl.popMapping();
    return result;
}

//******************** Decrypter functions ************************************

// These functions simply delegate to DecrypterImpl's functions.

Decrypter::Decrypter()
{
    m_impl = new DecrypterImpl;
}

Decrypter::~Decrypter()
{
    delete m_impl;
}

bool Decrypter::load(string filename)
{
    return m_impl->load(filename);
}

vector<string> Decrypter::crack(const string& ciphertext)
{
   return m_impl->crack(ciphertext);
}
