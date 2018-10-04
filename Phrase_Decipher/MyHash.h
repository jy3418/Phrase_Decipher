// MyHash.h


template<typename KeyType, typename ValueType>
class MyHash
{
public:
    MyHash(double maxLoadFactor = 0.5);
    ~MyHash();
    void reset();
    void associate(const KeyType& key, const ValueType& value);
    int getNumItems() const;
    double getLoadFactor() const;

      // for a map that can't be modified, return a pointer to const ValueType
    const ValueType* find(const KeyType& key) const;

      // for a modifiable map, return a pointer to modifiable ValueType
    ValueType* find(const KeyType& key)
    {
        return const_cast<ValueType*>(const_cast<const MyHash*>(this)->find(key));
    }

      // C++11 syntax for preventing copying and assignment
    MyHash(const MyHash&) = delete;
    MyHash& operator=(const MyHash&) = delete;

private:
    struct Node
    {
        Node(KeyType key, ValueType value)
        : m_key(key), m_value(value), m_next(nullptr)
        { }
        Node* m_next;
        KeyType m_key;
        ValueType m_value;
    };
    unsigned int getBucketNumber(const KeyType& key) const;
    double m_maxLoad;
    int m_numBuckets;
    double m_numItems;
    Node** m_table;
};

template <typename KeyType, typename ValueType>
MyHash<KeyType, ValueType>::MyHash(double maxLoadFactor)
: m_maxLoad(maxLoadFactor), m_numBuckets(100), m_numItems(0)
{
    if (maxLoadFactor <= 0)
        m_maxLoad = 0.5;
    else if (maxLoadFactor > 2)
        m_maxLoad = 2.0;
    
    m_table = new Node*[m_numBuckets];
    for (int i = 0; i < m_numBuckets; i++)
        m_table[i] = nullptr;
}

template <typename KeyType, typename ValueType>
MyHash<KeyType, ValueType>::~MyHash()
{
    for (int i = 0; i < m_numBuckets; i++)
    {
        Node* temp = m_table[i];
        while (temp != nullptr)
        {
            Node* prev = temp;
            temp = temp->m_next;
            delete prev;
        }
    }
    delete[] m_table;
}

template <typename KeyType, typename ValueType>
void MyHash<KeyType, ValueType>::reset()
{
    for (int i = 0; i < m_numBuckets; i++)
    {
        Node* temp = m_table[i];
        while (temp != nullptr)
        {
            Node* prev = temp;
            temp = temp->m_next;
            delete prev;
        }
    }
    delete[] m_table;
    
    m_numBuckets = 100;
    m_table = new Node*[m_numBuckets];
    for (int i = 0; i < m_numBuckets; i++)
        m_table[i] = nullptr;
    m_numItems = 0;
}

template <typename KeyType, typename ValueType>
unsigned int MyHash<KeyType, ValueType>::getBucketNumber(const KeyType& key) const
{
    unsigned int hash(const KeyType& k); // prototype
    unsigned int h = hash(key);
    return h % m_numBuckets;
}

template <typename KeyType, typename ValueType>
void MyHash<KeyType, ValueType>::associate(const KeyType& key, const ValueType& value)
{
    int bucket = getBucketNumber(key);
    Node* current = m_table[bucket];
    Node* prev = nullptr;
    while (current != nullptr)
    {
        if (current->m_key == key)
        {
            current->m_value = value;
            return;
        }
        prev = current;
        current = current->m_next;
    }
    // Since no same key was found, insert a new one
    current = new Node(key, value);
    if (prev == nullptr)
        m_table[bucket] = current;
    else
        prev->m_next = current;
    m_numItems++;
    
    //Check if current load factor exceeds max load factor
    if (getLoadFactor() > m_maxLoad)
    {
        int oldBucketSize = m_numBuckets;
        m_numBuckets *= 2;
        
        Node** oldTable = m_table;
        m_table = new Node*[m_numBuckets];
        for (int i = 0; i < m_numBuckets; i++)
        {
            m_table[i] = nullptr;
            // Merge the next for loop with this one
        }
        
        for (int i = 0; i < oldBucketSize; i++)
        {
            if (oldTable[i] != nullptr)
            {
                Node* prev = nullptr;
                Node* current = oldTable[i];
                while (current != nullptr)
                {
                    int newBucketNum = getBucketNumber(current->m_key);
                    Node* entry = m_table[newBucketNum];
                    Node* prevEntry = nullptr;
                    while (entry != nullptr)
                    {
                        prevEntry = entry;
                        entry = entry->m_next;
                    }
                    entry = new Node(current->m_key, current->m_value);
                    if (prevEntry == nullptr)
                        m_table[newBucketNum] = entry;
                    else
                        prevEntry->m_next = entry;
                    
                    prev = current;
                    current = current->m_next;
                    delete prev;
                }
            }
        }
        delete[] oldTable;
    }
    return;
}

template <typename KeyType, typename ValueType>
const ValueType* MyHash<KeyType, ValueType>::find(const KeyType& key) const
{
    int bucketNum = getBucketNumber(key);
    Node* current = m_table[bucketNum];
    while (current != nullptr)
    {
        if (current->m_key == key)
        {
            ValueType* vtp = &(current->m_value);
            return vtp;
        }
        current = current->m_next;
    }
    return nullptr;
}

template <typename KeyType, typename ValueType>
int MyHash<KeyType, ValueType>::getNumItems() const
{
    return m_numItems;
}

template <typename KeyType, typename ValueType>
double MyHash<KeyType, ValueType>::getLoadFactor() const
{
    return m_numItems / m_numBuckets;
    
}

