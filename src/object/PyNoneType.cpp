#include "PyNoneType.hpp"

#include <cassert>
#include <iostream>

std::mutex NoneTypeKlass::m_Mutex;
std::unique_ptr<NoneTypeKlass> NoneTypeKlass::m_Instance;

NoneTypeKlass::NoneTypeKlass()
{
    set_name("NoneType");
}

void NoneTypeKlass::print(PyObject *x)
{
    assert(x && x->klass() == this);
    std::cout << "None";
    std::fflush(stdout);
}
