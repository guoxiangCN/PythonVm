#ifndef PY_DICT_HPP__
#define PY_DICT_HPP__

#include "Map.hpp"
#include "Klass.hpp"
#include "PyObject.hpp"

#include <mutex>

class DictKlass : public Klass {
public:
    static DictKlass* get_instance();

    virtual void print(PyObject *self);

private:
    static DictKlass     *m_Instance;
    static std::mutex    m_Mutex;
    DictKlass();
};

class PyDict : public PyObject {
public:
    PyDict();
    PyDict(int init_cap = 8);
    PyDict(const PyDict &rhs) = delete;
    PyDict(PyDict &&rhs);
    ~PyDict();
    PyDict& operator=(const PyDict &rhs) = delete;

    void put(PyObject *first, PyObject *second) { m_InnerMap->put(first, second); }

    int size() const      { return m_InnerMap->size();}
    int capacity() const  { return m_InnerMap->capacity();}
    Map<PyObject*,PyObject*> *innerMap() { return m_InnerMap; }
private:
    Map<PyObject*,PyObject*> *m_InnerMap;
};

namespace pydict {
    typedef ArrayList<PyObject*> *FuncArgs;
    // D.clear()                              #移除D中的所有项  
    // D.copy()                               #返回D的副本  
    // D.fromkeys(seq[,val])                  #返回从seq中获得的键和被设置为val的值的字典。可做类方法调用  
    // D.get(key[,default])                   #如果D[key]存在，将其返回；否则返回给定的默认值None  
    // D.has_key(key)                         #检查D是否有给定键key  
    // D.items()                              #返回表示D项的(键，值)对列表  
    // D.iteritems()                          #从D.items()返回的(键，值)对中返回一个可迭代的对象  
    // D.iterkeys()                           #从D的键中返回一个可迭代对象  
    // D.itervalues()                         #从D的值中返回一个可迭代对象  
    // D.keys()                               #返回D键的列表  
    // D.pop(key[,d])                         #移除并且返回对应给定键key或给定的默认值D的值  
    // D.popitem()                            #从D中移除任意一项，并将其作为(键，值)对返回  
    // D.setdefault(key[,default])            #如果D[key]存在则将其返回；否则返回默认值None  
    // D.update(other)                        #将other中的每一项加入到D中。  
    // D.values()                             #返回D中值的列表
}

#endif
