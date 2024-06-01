#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <wchar.h>

char roboks[] =
    "    w\\\n"
    "     \\          y>/一<\\w\n"
    "      \\         y/ヽ_ノヽw\n"
    "       \\           yﾊw\n"
    "        \\       c———————w\n"
    "         \\.  c／         ＼w\n"
    "           c／  w●c       w●c  ＼w\n"
    "          c/     w❘ニニニ❘c     c\\w\n"
    "        c／|—————————————————|ヽw\n"
    "       c/ /b|    g〇 y〇 m〇     b|ヽヽw\n"
    "     r(一) b|                 | r(一)w\n"
    "          b|     y／一＼      b|\n"
    "          b|     y| ?  |      b|\n"
    "          b|     y＼一／      b|\n"
    "          b|＿  ＿＿＿＿  ＿.|\n"
    "             cΠ         Π   \n"
    "        r(ニニ|         |ニニ)d)";

int mb_strlen( const char* str ) {
    setlocale( LC_ALL, "" );
    int len = 0;
    wchar_t wc;
    mbstate_t mbs;
    memset( &mbs, 0, sizeof( mbs ) );
    while ( *str != '\0' ) {
        size_t bytes = mbrtowc( &wc, str, MB_CUR_MAX, &mbs );
        if ( bytes == (size_t)-1 || bytes == (size_t)-2 )
            break;
        str += bytes;
        len += wcwidth( wc );
    }
    return len;
}

void say_bubble( const char* str ) {
    int len = mb_strlen( str );

    printf( "  " );
    for ( int i = 0; i < len + 2; i++ ) {
        printf( "-" );
    }
    puts( "" );
    printf( "＜" );
    printf( " %s ＞\n", str );

    printf( "  " );
    for ( int i = 0; i < len + 2; i++ ) {
        printf( "-" );
    }
    puts( "" );
}

int main( int argc, char** argv ) {
    const char* default_str = "踏めば助かるのに...";
    char* str = ( argc == 2 ) ? argv[1] : (char*)default_str;

    FILE* out = stdout;

    fprintf(out, "\x1b[37m");
    say_bubble( str );
    char* roboks_ptr = roboks;

    while (*(roboks_ptr++) != '\0') {
        int c = (int)*roboks_ptr;
        switch(c){
            case 'c':
                fprintf(out, "\x1b[36m");
                break;
            case 'y':
                fprintf(out, "\x1b[33m");
                break;
            case 'w':
                fprintf(out, "\x1b[37m");
                break;
            case 'r':
                fprintf(out, "\x1b[31m");
                break;
            case 'g':
                fprintf(out, "\x1b[32m");
                break;
            case 'b':
                fprintf(out, "\x1b[34m");
                break;
            case 'd':
                fprintf(out, "\x1b[39m");
                break;
            case 'm':
                fprintf(out, "\x1b[35m");
                break;
            default:
                putc(c, out);
        }
    }

    return 0;
}
