#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <wchar.h>

#define MAX_STRARG_NUM 32

wchar_t roboks[] = L"    w\\\n"
                   L"     \\          y>/一<\\w\n"
                   L"      \\         y/ヽ_ノヽw\n"
                   L"       \\           yﾊw\n"
                   L"        \\       c———————w\n"
                   L"         \\.  c／         ＼w\n"
                   L"           c／  w●c       w●c  ＼w\n"
                   L"          c/     w❘ニニニ❘c     c\\w\n"
                   L"        c／|—————————————————|ヽw\n"
                   L"       c/ /b|    g〇 y〇 m〇     b|ヽヽw\n"
                   L"     r(一) b|                 | r(一)w\n"
                   L"          b|     y／一＼      b|\n"
                   L"          b|     y| ?  |      b|\n"
                   L"          b|     y＼一／      b|\n"
                   L"          b|＿  ＿＿＿＿  ＿.|\n"
                   L"             cΠ         Π   \n"
                   L"        r(ニニ|         |ニニ)d)\n";

int mb_strlen(const wchar_t *str) {
    int len = 0;
    while (*str != L'\0') {
        len += wcwidth(*str);
        str++;
    }
    return len;
}

void print_bar(int len) {
    wprintf(L"  ");
    for (int i = 0; i < len + 2; i++) {
        wprintf(L"-");
    }
    wprintf(L"\n");
}

void say_bubble(const wchar_t *str) {
    int len = mb_strlen(str);

    print_bar(len + 2);

    wprintf(L"＜ %ls ＞\n", str);

    print_bar(len + 2);
}

int main(int argc, char **argv) {
    const wchar_t *default_str = L"踏めば助かるのに...";
    wchar_t *str;
    wchar_t argbuf[MAX_STRARG_NUM];

    setlocale(LC_ALL, "");

    if (argc == 2) {
        int ret = mbstowcs(argbuf, argv[1], MAX_STRARG_NUM);
        if (ret == (size_t)-1) {
            wprintf(L"Invalid argument\n");
            exit(1);
        }
        str = argbuf;

    } else {
        str = (wchar_t *)default_str;
    }

    FILE *out = stdout;

    fwprintf(out, L"\x1b[37m");
    say_bubble(str);
    wchar_t *roboks_ptr = roboks;

    while (*(roboks_ptr++) != L'\0') {
        wchar_t c = *roboks_ptr;
        switch (c) {
        case L'c':
            fwprintf(out, L"\x1b[36m");
            break;
        case L'y':
            fwprintf(out, L"\x1b[33m");
            break;
        case L'w':
            fwprintf(out, L"\x1b[37m");
            break;
        case L'r':
            fwprintf(out, L"\x1b[31m");
            break;
        case L'g':
            fwprintf(out, L"\x1b[32m");
            break;
        case L'b':
            fwprintf(out, L"\x1b[34m");
            break;
        case L'd':
            fwprintf(out, L"\x1b[39m");
            break;
        case L'm':
            fwprintf(out, L"\x1b[35m");
            break;
        default:
            putwc(c, out);
        }
    }

    return 0;
}
