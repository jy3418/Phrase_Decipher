#include "provided.h"
#include <string>
#include <vector>
using namespace std;

class TokenizerImpl
{
public:
    TokenizerImpl(string separators);
    vector<string> tokenize(const std::string& s) const;
private:
    string m_seperators;
    bool isSeperator(char c) const;
};

TokenizerImpl::TokenizerImpl(string separators)
: m_seperators(separators)
{ }

bool TokenizerImpl::isSeperator(char c) const
{
    for (int i = 0; i < m_seperators.size(); i++)
    {
        if (c == m_seperators[i])
        {
            return true;
        }
    }
    return false;
}

vector<string> TokenizerImpl::tokenize(const std::string& s) const
{
    vector<string> words;
    string wordToAdd;
    for (int i = 0; i < s.size(); i++)
    {
        if (isSeperator(s[i]))
        {
            if (wordToAdd.size() > 0)
            {
                words.push_back(wordToAdd);
                wordToAdd = "";
            }
        }
        else
        {
            wordToAdd += s[i];
        }
    }
    if (wordToAdd.size() > 0)
    {
        words.push_back(wordToAdd);
    }
    return words;
}

//******************** Tokenizer functions ************************************

// These functions simply delegate to TokenizerImpl's functions.
// You probably don't want to change any of this code.

Tokenizer::Tokenizer(string separators)
{
    m_impl = new TokenizerImpl(separators);
}

Tokenizer::~Tokenizer()
{
    delete m_impl;
}

vector<string> Tokenizer::tokenize(const std::string& s) const
{
    return m_impl->tokenize(s);
}
