//
// [How to build]
//
// For Linux:
// make
//
// For windows:
// cl /nologo /utf-8 /DUNICODE /D_UNICODE /Ferobokssay main.c
//
#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <wchar.h>

#ifdef _WIN32
#include <Windows.h>
#include <io.h>
#include <fcntl.h>
#endif

#define MAX_STRARG_NUM 128

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

#if _WIN32
// Custom wcwidth function to estimate character width
int custom_wcwidth(wchar_t ucs) {
    if (ucs == 0)
        return 0;
    if (ucs < 0x20 || (ucs >= 0x7f && ucs < 0xa0))
        return -1;
    if (ucs < 0x7f)
        return 1;
    if (ucs < 0x1100)
        return 1;
    if (ucs >= 0x1100 && (ucs <= 0x115f || // Hangul Jamo init. consonants
                          ucs == 0x2329 || ucs == 0x232a ||
                          (ucs >= 0x2e80 && ucs <= 0xa4cf && ucs != 0x303f) || // CJK ... Yi
                          (ucs >= 0xac00 && ucs <= 0xd7a3) ||                  // Hangul Syllables
                          (ucs >= 0xf900 && ucs <= 0xfaff) || // CJK Compatibility Ideographs
                          (ucs >= 0xfe10 && ucs <= 0xfe19) || // Vertical forms
                          (ucs >= 0xfe30 && ucs <= 0xfe6f) || // CJK Compatibility Forms
                          (ucs >= 0xff00 && ucs <= 0xff60) || // Fullwidth Forms
                          (ucs >= 0xffe0 && ucs <= 0xffe6) || (ucs >= 0x20000 && ucs <= 0x2fffd) ||
                          (ucs >= 0x30000 && ucs <= 0x3fffd))) {
        return 2;
    }
    return 1;
}
#endif

// 文字列の表示幅を計算する関数
int mb_strlen(const wchar_t *str) {
    int len = 0;
    while (*str != L'\0') {
#ifdef _WIN32
        len += custom_wcwidth(*str);
#else
        len += wcwidth(*str);
#endif
        str++;
    }
    return len;
}

// 指定された長さの横棒を表示する関数
void print_bar(int len) {
    wprintf(L"  ");
    for (int i = 0; i < len + 2; i++) {
        wprintf(L"-");
    }
    wprintf(L"\n");
}

// 文字列を指定されたチャンクサイズで分割して表示する関数
void say_bubble(const wchar_t *str, int line_num) {
    const wchar_t *p = str;
    wchar_t *chunk = (wchar_t *)malloc((line_num + 1) * sizeof(wchar_t));
    if (chunk == NULL) {
        wprintf(L"Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    int chunk_len;
    int total_max_len = 0;

    // line_numが指定されている場合の処理
    if (line_num > 0) {
        // 各チャンクの最大長を計算
        while (*p != L'\0') {
            chunk_len = 0;
            while (chunk_len < line_num && *p != L'\0') {
                chunk[chunk_len++] = *p++;
            }
            chunk[chunk_len] = L'\0';

            // チャンクが短い場合は全角スペースで埋める
            if (chunk_len < line_num) {
                for (int i = chunk_len; i < line_num; i++) {
                    chunk[i] = L'　'; // 全角スペース
                }
                chunk[line_num] = L'\0';
            }

            int len = mb_strlen(chunk);
            if (len > total_max_len) {
                total_max_len = len;
            }
        }

        // 上部の横棒を表示
        print_bar(total_max_len);

        // 各チャンクを表示
        p = str;
        while (*p != L'\0') {
            chunk_len = 0;
            while (chunk_len < line_num && *p != L'\0') {
                chunk[chunk_len++] = *p++;
            }
            chunk[chunk_len] = L'\0';

            // チャンクが短い場合は全角スペースで埋める
            if (chunk_len < line_num) {
                for (int i = chunk_len; i < line_num; i++) {
                    chunk[i] = L'　'; // 全角スペース
                }
                chunk[line_num] = L'\0';
            }

            wprintf(L"＜ %ls ＞\n", chunk);
        }

        // 下部の横棒を表示
        print_bar(total_max_len);
    } else {
        // line_numが指定されていない場合の処理
        int len = mb_strlen(str);

        // 上部の横棒を表示
        print_bar(len);

        // 文字列全体を表示
        wprintf(L"＜ %ls ＞\n", str);

        // 下部の横棒を表示
        print_bar(len);
    }

    free(chunk);
}

#ifdef _WIN32
int wmain(int argc, wchar_t **argv) {
#else
int main(int argc, char **argv) {
#endif
    const wchar_t *default_str = L"踏めば助かるのに...";
    wchar_t *str = NULL;
    wchar_t argbuf[MAX_STRARG_NUM];
    int line_num = 0; // デフォルトでは折り返しを行わない

#ifdef _WIN32
    _setmode(_fileno(stdout), _O_U16TEXT);
    // setlocale(LC_ALL, ".UTF-8");
    // setlocale(LC_ALL, "");
    // setlocale(LC_ALL, "japanese");
    // setlocale(LC_CTYPE, ".utf8");
    // SetConsoleCP(CP_UTF8);
    // SetConsoleOutputCP(CP_UTF8);
    // setvbuf(stdout, NULL, _IOFBF, 1024);
#else
    setlocale(LC_ALL, "");
#endif

    // オプションの解析
#ifdef _WIN32
    for (int i = 1; i < argc; i++) {
        if (wcscmp(argv[i], L"-f") == 0 && i + 1 < argc) {
            line_num = _wtoi(argv[++i]);
        } else {
            str = argv[i];
        }
    }
#else
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-f") == 0 && i + 1 < argc) {
            line_num = atoi(argv[++i]);
        } else {
            int ret = mbstowcs(argbuf, argv[i], MAX_STRARG_NUM);
            if (ret == (size_t)-1) {
                wprintf(L"Invalid argument\n");
                exit(1);
            }
            str = argbuf;
        }
    }
#endif

    if (str == NULL) {
        str = (wchar_t *)default_str;
    }

    FILE *out = stdout;

    // 文字色の設定
    fwprintf(out, L"\x1b[37m");
    say_bubble(str, line_num);
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
            fwprintf(out, L"%lc", c);
            break;
        }
    }

    return 0;
}
