#include "PyFunction.hpp"
#include "Panic.hpp"
#include "VM.hpp"
#include "PyInteger.hpp"
#include "PyDouble.hpp"
#include <stdio.h>
#include <cmath>
#include <assert.h>

NativeFunctionKlass* NativeFunctionKlass::m_Instance = nullptr;
std::mutex NativeFunctionKlass::m_Mutex;
NativeFunctionKlass::NativeFunctionKlass()
{

}

NativeFunctionKlass* NativeFunctionKlass::get_instance()
{
    if(nullptr == m_Instance)
    {
        std::lock_guard<std::mutex> lock(m_Mutex);
        if(nullptr == m_Instance)
        {
            m_Instance = new NativeFunctionKlass();
        }
    }
    return m_Instance;
}


FunctionKlass* FunctionKlass::m_Instance = nullptr;
std::mutex FunctionKlass::m_Mutex;

FunctionKlass::FunctionKlass(){}

FunctionKlass* FunctionKlass::get_instance()
{
    if(m_Instance == nullptr)
    {
        std::lock_guard<std::mutex> lock(m_Mutex);
        if(m_Instance == nullptr)
        {
            m_Instance = new FunctionKlass();
        }
    }
    return m_Instance;
}

void FunctionKlass::print(PyObject *x)
{
    printf("<function: ");
    PyFunction *fun = dynamic_cast<PyFunction*>(x);
    assert(fun && fun->klass() == static_cast<Klass*>(this));
    //
    printf(">");
}


// PyFunction
PyFunction::PyFunction(CodeObject *codeObj)
{
    m_FuncCode = codeObj;
    m_FuncName = codeObj->m_CoName;
    m_Flags    = codeObj->m_Flag;
    m_Globals  = nullptr;
    m_DefaultArgs = nullptr;
    set_klass(FunctionKlass::get_instance());
}

PyFunction::PyFunction(Klass *klass)
{
    m_FuncCode = nullptr;
    m_FuncName = nullptr;
    m_Globals  = nullptr;
    m_Flags    = 0;
    m_Globals  = nullptr;
    m_DefaultArgs = nullptr;
    set_klass(klass);
}

PyFunction::~PyFunction()
{
    // 尼玛的全是in类型指针 鬼知道要不要delete
    if(m_DefaultArgs != nullptr)
        delete m_DefaultArgs;
}

void PyFunction::set_default_args(ArrayList<PyObject*> *_default_args)
{
    if(_default_args == nullptr)
    {
        m_DefaultArgs = nullptr;
        return;
    }
    m_DefaultArgs = new ArrayList<PyObject*>(_default_args->capacity());
    for(auto i = 0; i < _default_args->size(); i++)
    {
        m_DefaultArgs->set(i, _default_args->get(i));
    }
}

PyFunction::PyFunction(NativeFunctionPtr nativeFuncPtr)
{
    m_FuncName = nullptr;
    m_FuncCode = nullptr;
    m_Globals = nullptr;
    m_Flags = 0;
    m_DefaultArgs = nullptr;
    m_NativeFunctionPtr = nativeFuncPtr;
    set_klass(NativeFunctionKlass::get_instance());
}

PyObject* PyFunction::native_call(ArrayList<PyObject*> *args)
{
    if(this->klass() != static_cast<Klass*>(NativeFunctionKlass::get_instance()))
        __panic("native called by a PyFunction not instance of NativeFunctionKlass.");
    return (*m_NativeFunctionPtr)(args);
}

namespace native {

    /**
     * 获取一个对象的唯一id. 
     * 在本虚拟机中返回的是对象的地址. 
     */
    PyObject* id(FuncArgs args)
    {
        if(args == nullptr || args->size() != 1)
        {
            __panic("TypeError: id() takes exactly one argument(%d given)", args == nullptr ? 0 : args->size());
        }
        return args->get(0)->id();
    }

    /**
     * 对一个整形或者浮点型求绝对值
     */
    PyObject* abs(FuncArgs args)
    {
        PyObject *arg = args->get(0);
        if(arg->klass() == IntegerKlass::get_instance())
        {
            return new PyInteger(std::abs(dynamic_cast<PyInteger*>(arg)->value()));
        }
        else if(arg->klass() == DoubleKlass::get_instance())
        {
            return new PyDouble(std::fabs(dynamic_cast<PyDouble*>(arg)->value()));
        } else 
        {
            __panic("TypeError: bad operand type for abs():'%s' ^_^\n", "?");
        }
        return nullptr;
    }

    PyObject* len(FuncArgs args)
    {
        return args->get(0)->len();
    }

    PyObject* type_of(FuncArgs args)
    {
        return nullptr;
    }

    PyObject* isinstance(FuncArgs args)
    {
        return VM::PyFalse;
    }
}
