#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>

char IP[50], PORT[20];
#define BUFSIZE 1024

typedef struct
{
    int id;
    char province[50];
    char city[50];
    char county[50];
    char district[50];
}Setting;

Setting setting;



void printHelp()
{
    printf("CovidSystemLinux Usage:\nCovidSystemLinux <command>\nCommands:\nstart\t\t\tStart Working\nsetting\t\t\tSet this Device Config\n");
}

void setConfig()
{
    printf("本设备编号为：%d\n输入省或直辖市名：", setting.id);
    scanf("%s", setting.province);
    printf("输入市名（若为直辖市请再输入一次）：");
    scanf("%s", setting.city);
    printf("输入区县名：");
    scanf("%s", setting.county);
    printf("输入地区名：");
    scanf("%s", setting.district);
    printf("新设置为：\n编号：%d\n省：%s\n市：%s\n区县：%s\n地区：%s\n[y/n]?", setting.id, setting.province, setting.city, setting.county, setting.district);
    char t;
    while (1)
    {
        scanf("%c", &t);
        if (t == 'y')
        {
            FILE* fp;
            fp = fopen("setting.cfg", "w+");
            fprintf(fp, "%d\n%s\n%s\n%s\n%s", setting.id, setting.province, setting.city, setting.county, setting.district);
            fclose(fp);
            break;
        }
        else if (t == 'n')
        {
            break;
        }
    }
}

void postAbnormal()
{
    int sockfd, ret;
    struct sockaddr_in servaddr;
    char str1[4096], str2[4096], buf[BUFSIZE], * str;
    socklen_t len;
    fd_set   t_set;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("创建网络连接失败,本线程即将终止---socket error!\n");
        exit(0);
    };

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(atoi(PORT));
    if (inet_pton(AF_INET, IP, &servaddr.sin_addr) <= 0) {
        printf("创建网络连接失败,本线程即将终止--inet_pton error!\n");
        exit(0);
    };

    if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        printf("连接到服务器失败,connect error!\n");
        exit(0);
    }


    //发送数据
    memset(str2, 0, 4096);

    char ids[10];
    sprintf(ids, "%d", setting.id);

    struct tm* t;
    time_t tt;
    time(&tt);
    t = localtime(&tt);
    char date[30];
    sprintf(date, "%4d-%02d-%02d %02d:%02d:%02d", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);


    //json格式 字符串的拼接
    strcat(str2, "{ \n");
    strcat(str2, "\"Province\":    ");
    strcat(str2, " \""); strcat(str2, setting.province); strcat(str2, "\",\n");
    strcat(str2, "\"City\":       ");
    strcat(str2, "\""); strcat(str2, setting.city); strcat(str2, "\",\n");
    strcat(str2, "\"County\":  ");
    strcat(str2, "\""); strcat(str2, setting.county); strcat(str2, "\",\n");
    strcat(str2, "\"District\":     ");
    strcat(str2, "\""); strcat(str2, setting.district); strcat(str2, "\",\n");
    strcat(str2, "\"Info\":     ");
    strcat(str2, "\""); strcat(str2, "检测到体温异常"); strcat(str2, "\",\n");
    strcat(str2, "\"Time\":     ");
    strcat(str2, "\""); strcat(str2, date); strcat(str2, "\",\n");
    strcat(str2, "\"Device\":     "); strcat(str2, ids); strcat(str2, "\n");
    strcat(str2, "}\n");


    str = (char*)malloc(4096);
    len = strlen(str2);
    sprintf(str, "%d", len);

    memset(str1, 0, 4096);
    strcat(str1, "POST /abnormal HTTP/1.1\n");
    strcat(str1, "Content-Type: application/json\n");
    strcat(str1, "User-Agent: PostmanRuntime/7.28.4\n");
    strcat(str1, "Accept: */*\n");
    strcat(str1, "Postman-Token: 275ebedf-0b79-4543-a218-ef798dad3a6a\n");
    strcat(str1, "Host: ");
    strcat(str1, IP);
    strcat(str1, ":");
    strcat(str1, PORT);
    strcat(str1, "\n");
    strcat(str1, "Accept-Encoding: gzip, deflate, br\n");
    strcat(str1, "Connection: keep-alive\n");
    strcat(str1, "Content-Length: ");
    strcat(str1, str);
    strcat(str1, "\n\n");

    strcat(str1, str2);
    strcat(str1, "\r\n\r\n");

    ret = write(sockfd, str1, strlen(str1));
    if (ret < 0) {
        printf("发送失败！错误代码是%d，错误信息是'%s'\n", errno, strerror(errno));
        exit(0);
    }
    else {
        // printf("%s\n", str1);
    }
    FD_ZERO(&t_set);
    FD_SET(sockfd, &t_set);
    close(sockfd);
    printf("异常信息已汇报\n\n");
}

void startWorking()
{
    float temp;
    printf("编号：%d\n省：%s\n市：%s\n区县：%s\n地区：%s\n开始模拟工作...\n输入模拟温度数据：", setting.id, setting.province, setting.city, setting.county, setting.district);
    while (1)
    {
        scanf("%f", &temp);
        if (temp <= 37.5)
        {
            printf("体温正常\n");
        }
        else
        {
            postAbnormal();
        }
        printf("输入模拟温度数据：");
    }

}

int main(int argc, char** argv)
{
    

    if (argc <= 1)
    {
        printHelp();
        return 0;
    }
    FILE* fp;
    fp = fopen("setting.cfg", "r");
    fscanf(fp, "%d\n", &setting.id);
    fscanf(fp, "%s\n", setting.province);
    fscanf(fp, "%s\n", setting.city);
    fscanf(fp, "%s\n", setting.county);
    fscanf(fp, "%s\n", setting.district);
    fclose(fp);
    fp = fopen("IP.cfg", "r");
    fscanf(fp, "%s\n", IP);
    fscanf(fp, "%s\n", PORT);
    fclose(fp);
    if (strcmp(argv[1], "setting") == 0)
    {
        setConfig();
    }
    else if (strcmp(argv[1], "start") == 0)
    {
        startWorking();
    }
}