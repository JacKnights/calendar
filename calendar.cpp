#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <map>

using std::map;

typedef map< int, map< int, map<int, int> > > datedata;

int zellerCongruence(int q, int m, int K, int J) {
    int h = (q + (13 * (m + 1)) / 5 + K + K / 4 + J / 4 - 2 * J) % 7;
    return h;
}

int getWeekday(int day, int month, int year) {
    if (month < 3) {
        year--;
        month += 12;
    }
    // 年份最后两位
    int K = year % 100;
    // 公元后年份的前两位数
    int J = year / 100;
    int h = zellerCongruence(day, month, K, J);
    return (h + 6) % 7;
}

int getDaysInMonth(int year, int month) {
    switch (month) {
    case 2: // 2月特殊处理
        return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0) ? 29 : 28;
    case 4:
    case 6:
    case 9:
    case 11:
        return 30;
    }
    return 31;
}

void printMonthCalender(int year, int month) {
    int daysInMonth = getDaysInMonth(year, month);

    // 输出日历头部
    printf("%23d年%2d月\n", year, month);
    printf(" Sun Mon Tue Wed Thu Fri Sat\n");
    
    // 计算1号是周几，并打印前面的空格
    int dayOfWeek = getWeekday(1, month, year);
    for (int i = 0; i < dayOfWeek; ++i) {
        printf("    ");
    }
    
    for (int day = 1; day <= daysInMonth; ++day) {
        printf("%4d", day);
        if (dayOfWeek == 6) { // 如果是周六，换行
            printf("\n");
        }
        dayOfWeek = (dayOfWeek + 1) % 7;
    }
    printf("\n");
}

void printMonthCalenderWithData(int year, int month, map<int, int> data) {
    int daysInMonth = getDaysInMonth(year, month);

    // 输出日历头部
    printf("%28d年%2d月\n", year, month);
    printf("     Sun     Mon     Tue     Wed     Thu     Fri     Sat\n");
    
    // 计算1号是周几，并打印前面的空格
    int dayOfWeek = getWeekday(1, month, year);
    for (int i = 0; i < dayOfWeek; ++i) {
        printf("        ");
    }
    
    for (int day = 1; day <= daysInMonth; ++day) {
        int val = data[day];
        if (val <= 0) {
            printf("%4d(\033[30m%2d\033[0m)", day, val);
        } else if (val < 10) {
            printf("%4d(\033[32m%2d\033[0m)", day, val);
        } else if (val < 20) {
            printf("%4d(\033[34m%2d\033[0m)", day, val);
        } else if (val < 30) {
            printf("%4d(\033[33m%2d\033[0m)", day, val);
        } else {
            printf("%4d(\033[31m%2d\033[0m)", day, val);
        }
        // printf("%4d(%2d)", day, val);
        if (dayOfWeek == 6) { // 如果是周六，换行
            printf("\n");
        }
        dayOfWeek = (dayOfWeek + 1) % 7;
    }
    printf("\n");
}

int main(int argc, char* argv[]) {
    if (argc > 1) {
        int year, month;
        int opt;
        while ((opt = getopt(argc, argv, "y:m:")) != -1) {
            switch (opt) {
                case 'y':
                    year = atoi(optarg); // optarg是由 getopt 设置的，指向当前选项参数的字符串
                    if (year < 1) {
                        printf("年份输入非法: %d\n", year);
                        return 1;
                    }
                    break;
                case 'm':
                    month = atoi(optarg);
                    if (month < 1 || month > 12) {
                        printf("月份输入应在1-12之间: %d\n", month);
                        return 1;
                    }
                    break;
                case '?': // 处理未知的选项
                    printf("错误: 未知的选项\n");
                    return 1;
            }
        }
        printMonthCalender(year, month);
        return 0;
    }

    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    datedata data;
    while ((read = getline(&line, &len, stdin)) != -1) {
        // read 是实际读取的字符数，包括换行符
        if (read <= 1) { // 排除只输入换行的情况
            continue;
        }
        int year, month, day;
        int val;
        sscanf(line, "%d-%d-%d %d", &year, &month, &day, &val);
        data[year][month][day] = val;
    }
    free(line);
    // fflush(stdin);

    for (auto it1 = data.begin(); it1 != data.end(); it1++) {
        int year = it1->first;
        for (auto it2 = it1->second.begin(); it2 != it1->second.end(); it2++) {
            int month = it2->first;
            printMonthCalenderWithData(year, month, it2->second);
        }
    }

    return 0;
}