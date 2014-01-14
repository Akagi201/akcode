/** 
 * siconv.cpp - A simple way to demostrate the usage of iconv calling
 *
 * Report bugs to marchday2004@yahoo.com.cn
 * July 15th, 2006
 */
#include <iconv.h>
#include <stdio.h>
#include <string>
#include <stdarg.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>

#ifdef DEBUG
#define TRACE(fmt, args...) fprintf(stderr, "%s:%s:%d:"fmt, \
    __FILE__, __FUNCTION__, __LINE__, ##args)
#else
#define TRACE(fmt, args...)
#endif

#define CONVBUF_SIZE 32767

extern int errno;

void print_err(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
}

int print_out(const char* buf, size_t num)
{
    if (num != fwrite(buf, 1, num, stdout)) {
        return -1;
    }

    return 0;
}

void print_usage() {

    print_err("Usage: siconv -f encoding -t encoding [-c] "
        "input-file\n");
}

int iconv_string(const char *from, const char *to, 
    const char *src, size_t len, 
    ::std::string& result, 
    int c = 0, size_t buf_size = 512)
{
    iconv_t cd;

    char *pinbuf = const_cast< char* >(src);
    size_t inbytesleft = len;
    char *poutbuf = NULL;
    size_t outbytesleft = buf_size;

    char *dst = NULL;
    size_t retbytes = 0;
    int done = 0;
    int errno_save = 0;

    if ((iconv_t)-1 == (cd = iconv_open(to, from))) {
        return -1;
    }

    dst = new char[buf_size];

    while(inbytesleft > 0 && !done)  {
        poutbuf = dst;
        outbytesleft = buf_size;

        TRACE("TARGET - in:%p pin:%p left:%d\n", src, pinbuf, inbytesleft);
        retbytes = iconv(cd, &pinbuf, &inbytesleft, &poutbuf, &outbytesleft);
        errno_save = errno;

        if (dst != poutbuf)  {// we have something to write
            TRACE("OK - in:%p pin:%p left:%d done:%d buf:%d\n", 
                src, pinbuf, inbytesleft, pinbuf-src, poutbuf-dst);
            result.append(dst, poutbuf-dst);
        } 

        if (retbytes != (size_t)-1) {
            poutbuf = dst;
            outbytesleft = buf_size;
            (void)iconv(cd, NULL, NULL, &poutbuf, &outbytesleft);

            if (dst != poutbuf) {// we have something to write
                TRACE("OK - in:%p pin:%p left:%d done:%d buf:%d\n", 
                    src, pinbuf, inbytesleft, pinbuf-src, poutbuf-dst);
                result.append(dst, poutbuf-dst);
            }

            errno_save = 0;
            break;
        }


        TRACE("FAIL - in:%p pin:%p left:%d done:%d buf:%d\n", 
            src, pinbuf, inbytesleft, pinbuf-src, poutbuf-dst);

        switch(errno_save)  {
        case E2BIG:
            TRACE("E E2BIG\n");
            break;
        case EILSEQ:
            TRACE("E EILSEQ\n");
            if (c) {
                errno_save = 0;
                inbytesleft = len-(pinbuf-src); // forward one illegal byte
                inbytesleft--;
                pinbuf++;
                break;
            }

            done = 1;
            break;
        case EINVAL:
            TRACE("E EINVAL\n");
            done = 1;
            break;
        default:
            TRACE("E Unknown:[%d]%s\n", errno, strerror(errno));
            done = 1;
            break; 
        }

    }

    delete[] dst;
    iconv_close(cd);

    errno = errno_save;
    return (errno_save) ? -1 : 0;
}

int conv_file_fd(const char* from, const char *to, int fd, 
    ::std::string& result, int c)
{
    struct stat st;
    void *start;

    if (0 != fstat(fd, &st)) {
        return -1;
    }

    start = mmap(NULL, st.st_size, PROT_READ, MAP_SHARED, fd, 0);

    if (MAP_FAILED == start) {
        return -1;
    } 

    if (iconv_string(from, to, (char*)start, 
            st.st_size, result, c, CONVBUF_SIZE) < 0) {
        int errno_save = errno;
        munmap(start, st.st_size);
        TRACE("\n");
        errno = errno_save;
        return -1;
    }

    munmap(start, st.st_size);
    return 0;
}

int conv_file(const char* from, const char* to, 
    const char* filename, int c)
{
    ::std::string result;
    FILE *fp;

    if (NULL == (fp=fopen(filename, "rb"))) {
        print_err("open file %s:[%d]%s\n", filename, 
            errno, strerror(errno));
        return -1;
    }

    if (conv_file_fd(from, to, fileno(fp), result, c) < 0)  {
        print_err("conv file fd:[%d]%s\n", errno, strerror(errno));
        fclose(fp);
        return -1;
    }

    print_out(result.data(), result.size());
    fclose(fp);
    return 0;
}

int main(int argc, char *argv[])
{
#ifdef TESTCASE
    ::std::string strA = "欢迎(welcome ^_^)来到(to)首都北京。";
    ::std::string strB = "大喊一声：We are chinese <=> 我们都是中国人。";

    ::std::string strC = strA.substr(0, 20) + strB.substr(0, 41);
    ::std::string result;
    if (iconv_string("GBK", "UTF-8", strC.data(), strC.size(), result, 1) < 0)
    {
        TRACE("ERROR [%d]%s\n", errno, strerror(errno));
    }

    TRACE("CONVERSION RESULT:");
    result.append("\n");
    print_out(result.data(), result.size());

    return 0;
#else
    ::std::string from, to;
    ::std::string input_file;
    int o;
    int c = 0;

    while (-1 != (c = getopt(argc, argv, "f:t:c")))
    {
        switch(c) {
        case 'f':
            from = optarg;
            break;
        case 't':
            to = optarg;
            break;
        case 'c':
            c = 1;
            break;
        default:
            return -1;
        }
    }

    if (from.empty() || to.empty() || optind != (argc-1))
    {
        print_usage();
        return -1;
    }

    input_file = argv[optind++];

    return conv_file(from.c_str(), to.c_str(), 
        input_file.c_str(), c);
#endif
}



