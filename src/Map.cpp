#include "Map.hpp"
#include "PyObject.hpp"
#include "PyInteger.hpp"

// 针对k-v都是PyObject*的特化版本

typedef PyObject *pPyObject;

template<>
void Map<pPyObject,pPyObject>::put(const pPyObject &k, const pPyObject &v)
{
    for(decltype(size()) i = 0; i < m_Size; i++)
    {
        if(m_Entries[i].m_K->equal(k) == VM::PyTrue)
        {
            m_Entries[i].m_V = v;
            return;
        }
    }
    if(m_Size >= m_Capacity)
    {
        this->expand_capacity();
    }
    m_Entries[m_Size++] = MapEntry<pPyObject,pPyObject>(k,v);
}

template<>
int Map<pPyObject,pPyObject>::index(const pPyObject &k)
{
    for(decltype(size()) i = 0; i < m_Size; i++)
    {
        if(m_Entries[i].m_K->equal(k) == VM::PyTrue)
        {
            return i;
        }
    }
    return -1;
}