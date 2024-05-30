#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <wchar.h>

const char roboks[] =
    R"(    \
     \          >/一\<
      \         /ヽ_ノヽ
       \           ﾊ
        \       ———————
         \.  ／         ＼
           ／  ●       ●  ＼
          /     ❘ニニニ❘     \
        ／|—————————————————|ヽ
       / /|    〇 〇 〇     |ヽヽ
     (一) |                 | (一)
          |     ／一＼      |
          |     | ?  |      |
          |     ＼一／      |
          |＿  ＿＿＿＿  ＿.|
             Π         Π   
         (ニニ|        |ニニ))";

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

    say_bubble( str );
    puts( roboks );

    return 0;
}