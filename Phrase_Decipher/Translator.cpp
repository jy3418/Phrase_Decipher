#include "provided.h"
#include <string>
#include <algorithm>
#include <iostream>
using namespace std;

const string ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

class TranslatorImpl
{
public:
    TranslatorImpl();
    ~TranslatorImpl();
    bool pushMapping(string ciphertext, string plaintext);
    bool popMapping();
    string getTranslation(const string& ciphertext) const;
private:
    struct mapNode
    {
        char m_map[26];
        mapNode* m_next;
    };
    mapNode* m_head;
    int charToInt(char ch) const;
    int numPopped;
    int numPushed;
};
TranslatorImpl::TranslatorImpl()
:numPopped(0), numPushed(0)
{
    m_head = new mapNode;
    for (int i = 0; i < 26; i++)
    {
        m_head->m_map[i] = '?';
    }
    m_head->m_next = nullptr;
}

TranslatorImpl::~TranslatorImpl()
{
    mapNode* curr = m_head;
    mapNode* prev;
    while (curr != nullptr)
    {
        prev = curr;
        curr = curr->m_next;
        delete prev;
    }
}

bool TranslatorImpl::pushMapping(string ciphertext, string plaintext)
{
    if (ciphertext.size() != plaintext.size())
        return false;
    mapNode* newMapNode = new mapNode;
    newMapNode->m_next = m_head;
    copy(begin(m_head->m_map), end(m_head->m_map), begin(newMapNode->m_map));
    for (int i = 0; i < ciphertext.size(); i++)
    {
        char ch = ciphertext[i];
        char ch2 = plaintext[i];
        
        if (! isalpha(ch))
        {
            delete newMapNode;
            return false;
        }
        ch = toupper(ch);
        ch2 = toupper(ch2);
        int indexNum = charToInt(ch);
        if (newMapNode->m_map[indexNum] != '?')
        {
            if (newMapNode->m_map[indexNum] == ch2)
                continue;
            delete newMapNode;
            return false;
        }
        newMapNode->m_map[indexNum] = ch2;
    }
    int counter[26] = {0};
    for (int i = 0; i < 26; i++)
    {
        char ch = newMapNode->m_map[i];
        if (! isalpha(ch))
            continue;
        int indexNum = charToInt(ch);
        counter[indexNum]++;
        if (counter[indexNum] > 1)
        {
            delete newMapNode;
            return false;
        }
    }
    m_head = newMapNode;
    numPushed++;
    return true;
}

bool TranslatorImpl::popMapping()
{
    if (numPushed == numPopped)
        return false;
    mapNode* prev = m_head;
    m_head = m_head->m_next;
    delete prev;
    numPopped++;
    return true;
}

string TranslatorImpl::getTranslation(const string& ciphertext) const
{
    string result = "";
    for (int i = 0; i < ciphertext.size(); i++)
    {
        bool wasLower = false;
        char ch = ciphertext[i];
        if (! isalpha(ch))
        {
            result += ch;
            continue;
        }
        if (islower(ch))
        {
            ch = toupper(ch);
            wasLower = true;
        }
        int indexNum = charToInt(ch);
        if (wasLower)
            result += tolower(m_head->m_map[indexNum]);
        else
            result += m_head->m_map[indexNum];
    }
    return result;
}

int TranslatorImpl::charToInt(char ch) const
{
    switch (ch)
    {
        case 'A':
            return 0;
        case 'B':
            return 1;
        case 'C':
            return 2;
        case 'D':
            return 3;
        case 'E':
            return 4;
        case 'F':
            return 5;
        case 'G':
            return 6;
        case 'H':
            return 7;
        case 'I':
            return 8;
        case 'J':
            return 9;
        case 'K':
            return 10;
        case 'L':
            return 11;
        case 'M':
            return 12;
        case 'N':
            return 13;
        case 'O':
            return 14;
        case 'P':
            return 15;
        case 'Q':
            return 16;
        case 'R':
            return 17;
        case 'S':
            return 18;
        case 'T':
            return 19;
        case 'U':
            return 20;
        case 'V':
            return 21;
        case 'W':
            return 22;
        case 'X':
            return 23;
        case 'Y':
            return 24;
        case 'Z':
            return 25;
        default:
            return 0;
    }
}

//******************** Translator functions ************************************

// These functions simply delegate to TranslatorImpl's functions.
// You probably don't want to change any of this code.

Translator::Translator()
{
    m_impl = new TranslatorImpl;
}

Translator::~Translator()
{
    delete m_impl;
}

bool Translator::pushMapping(string ciphertext, string plaintext)
{
    return m_impl->pushMapping(ciphertext, plaintext);
}

bool Translator::popMapping()
{
    return m_impl->popMapping();
}

string Translator::getTranslation(const string& ciphertext) const
{
    return m_impl->getTranslation(ciphertext);
}
