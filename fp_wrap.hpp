//////////////////////////////////////////////////////////////////////////////
// fp_wrap.cpp --- FILE pointer wrapper class
// Copyright (C) 2019 Katayama Hirofumi MZ <katayama.hirofumi.mz@gmail.com>
// This file is public domain software.

#ifndef FP_WRAP_HPP_
#define FP_WRAP_HPP_     3   // Version 3

#include <cstdio>
#include <cstdarg>
#include <cassert>
using std::size_t;
using std::va_list;
using std::FILE;

class fp_wrap
{
public:
    fp_wrap();
    fp_wrap(FILE *fp);
    fp_wrap(const char *fname, const char *mode = "rb");
    virtual ~fp_wrap();

    bool open(const char *fname, const char *mode = "rb");
    bool open_tmp();
    bool is_open() const;
    bool is_eof() const;
    int get_error() const;
    FILE*& fp();

    size_t read(void *buffer, size_t size, size_t count);
    size_t write(const void *buffer, size_t size, size_t count);

    int getchr();
    int putchr(int ch);

    char *getstr(char *str, int count);
    int putstr(const char *str);

    int scan(const char *fmt, ...);
    int vscan(const char *fmt, va_list va);
    int print(const char *fmt, ...);
    int vprint(const char *fmt, va_list va);

    long tell();
    int seek(long offset = 0, int origin = SEEK_SET);

    int flush();
    int close();

#ifdef _WIN32
    fp_wrap(const wchar_t *fname, const wchar_t *mode = L"rb");
    bool open(const wchar_t *fname, const wchar_t *mode = L"rb");
#endif

protected:
    FILE *m_fp;

private:
    fp_wrap(const fp_wrap&);
    fp_wrap& operator=(const fp_wrap&);
};

//////////////////////////////////////////////////////////////////////////////

inline fp_wrap::fp_wrap() : m_fp(NULL)
{
}

inline fp_wrap::fp_wrap(FILE *fp) : m_fp(fp)
{
}

inline fp_wrap::fp_wrap(const char *fname, const char *mode)
    : m_fp(std::fopen(fname, mode))
{
}

inline fp_wrap::~fp_wrap()
{
    close();
}

inline bool fp_wrap::open(const char *fname, const char *mode)
{
    using namespace std;
    close();
    m_fp = fopen(fname, mode);
    return is_open();
}

inline bool fp_wrap::open_tmp()
{
    close();
    m_fp = std::tmpfile();
    return is_open();
}

inline bool fp_wrap::is_open() const
{
    return m_fp != NULL;
}

inline bool fp_wrap::is_eof() const
{
    using namespace std;
    assert(m_fp);
    return !!feof(m_fp);
}

inline int fp_wrap::get_error() const
{
    using namespace std;
    assert(m_fp);
    return ferror(m_fp);
}

inline FILE*& fp_wrap::fp()
{
    return m_fp;
}

inline size_t fp_wrap::read(void *buffer, size_t size, size_t count)
{
    using namespace std;
    assert(m_fp);
    return fread(buffer, size, count, m_fp);
}

inline size_t fp_wrap::write(const void *buffer, size_t size, size_t count)
{
    using namespace std;
    assert(m_fp);
    return fwrite(buffer, size, count, m_fp);
}

inline int fp_wrap::getchr()
{
    using namespace std;
    assert(m_fp);
    return fgetc(m_fp);
}

inline int fp_wrap::putchr(int chr)
{
    using std::fputc;
    assert(m_fp);
    return fputc(chr, m_fp);
}

inline char *fp_wrap::getstr(char *str, int count)
{
    using namespace std;
    assert(m_fp);
    return fgets(str, count, m_fp);
}

inline int fp_wrap::putstr(const char *str)
{
    using namespace std;
    assert(m_fp);
    return fputs(str, m_fp);
}

inline int fp_wrap::scan(const char *fmt, ...)
{
    int ret;
    va_list va;
    assert(m_fp);
    va_start(va, fmt);
    ret = vscan(fmt, va);
    va_end(va);
    return ret;
}

inline int fp_wrap::vscan(const char *fmt, va_list va)
{
    using namespace std;
    assert(m_fp);
    return vfscanf(m_fp, fmt, va);
}

inline int fp_wrap::print(const char *fmt, ...)
{
    int ret;
    va_list va;
    assert(m_fp);
    va_start(va, fmt);
    ret = vprint(fmt, va);
    va_end(va);
    return ret;
}

inline int fp_wrap::vprint(const char *fmt, va_list va)
{
    using namespace std;
    assert(m_fp);
    return vfprintf(m_fp, fmt, va);
}

inline long fp_wrap::tell()
{
    using std::ftell;
    assert(m_fp);
    return ftell(m_fp);
}

inline int fp_wrap::seek(long offset, int origin)
{
    using namespace std;
    assert(m_fp);
    return fseek(m_fp, offset, origin);
}

inline int fp_wrap::flush()
{
    using std::fflush;
    assert(m_fp);
    return fflush(m_fp);
}

inline int fp_wrap::close()
{
    using namespace std;
    int ret = EOF;
    if (m_fp)
    {
        ret = fclose(m_fp);
        m_fp = NULL;
    }
    return ret;
}

#ifdef _WIN32
    inline fp_wrap::fp_wrap(const wchar_t *fname, const wchar_t *mode)
        : m_fp(NULL)
    {
        wopen(fname, mode);
    }

    inline bool fp_wrap::wopen(const wchar_t *fname, const wchar_t *mode)
    {
        using namespace std;
        close();
        m_fp = _wfopen(fname, mode);
        return is_open();
    }
#endif

#endif  // ndef FP_WRAP_HPP_
