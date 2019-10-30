#ifndef CORE_HPP
#define CORE_HPP

/*
 * The defination of python vm version.
 */
#define VM_VERSION 2.7

/*
 * no exception definations of different cpp version.
 * My inspiration from MSVC source code.
 */
#if __cplusplus < 201103L
    #define NOEXCEPT throw()
#else
    #define NOEXCEPT noexcept
#endif

/*
 * A macron of disable move constructor and operator=
 */
#define DISABLE_COPY(_Ty) \
    private:  \
      _Ty(const _Ty&); \
      _Ty& operator=(const _Ty&);

/*
 * 3rd namespace.
 */
#define FROM_BOOST_BEGIN namespace fromboost {
#define FROM_BOOST_END }



#endif // endif CORE_HPP
