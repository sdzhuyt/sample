#include <stdio.h>
#include <string.h>

static char g_cwd[8][32] = {"000", "../", "111/", "222", "../333", "444/555/../", "./666"};


static inline int dir_up(char *src_dir, int src_len)
{
    char *str_end = src_dir + src_len;

    while (str_end > src_dir) {
        if (*(--str_end) == '/') {
            // 跳过最后一个字符为'/'的情况
            if (src_len - 1 == str_end - src_dir) {
                src_len--;
                continue;
            }
            break;
        }
    }

    src_len = str_end - src_dir;
    return src_len;
}

static int dir_jion(char *src_dir, int src_len, char *in_dir, int in_len, int max_len)
{
    if (in_len > max_len) {
        return -1;
    }
    if (in_len == 0) {
        return 0;
    }

    if (*in_dir == '/' || *in_dir == '~') {
        *src_dir = *in_dir;
        src_dir[in_len] = '\0';
        return 0;
    }

    char *in = in_dir;
    char *in_end = in_dir + in_len;

    while (in < in_end) {
        if (strncmp(in, "..", 2) == 0) {
            src_len = dir_up(src_dir, src_len);
            in += 2;
            continue;
        } else if (*in == '.') {
            in++;
            continue;
        } else if (*in == '/') {
            if (src_len > 0 && src_dir[src_len - 1] != '/') {
                *(src_dir + src_len) = '/';
                src_len++;
            }
            in++;
            continue;
        } else {
            *(src_dir + src_len) = *in;
            src_len++;
            in++;
            continue;
        }
    }

    if (src_len > 0) {
        if (src_dir[src_len - 1] != '/') {
            src_dir[src_len] = '/';
            src_len++;
        }
    }
    src_dir[src_len] = '\0';
    return 0;
}

int main(void)
{
    int i;
    char src_dir[128] = {0};

    for (i = 0; i < sizeof(g_cwd) / sizeof(g_cwd[0]); i++) {
        if (g_cwd[i][0] == '\0') {
            break;
        }
        if (dir_jion(src_dir, strlen(src_dir), g_cwd[i], strlen(g_cwd[i]), sizeof(src_dir)) < 0) {
            printf("dir_jion error: src_dir:%s, in_dir:%s\n", src_dir, g_cwd[i]);
        }
    }

    printf("src_dir:%s\n", src_dir);

    return 0;
}