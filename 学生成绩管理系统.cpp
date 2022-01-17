/**
 * 学生管理系统
 *
 * 学生有学号，姓名，性别，年龄，籍贯，所属学院，电话号码，email, qq 等属性。
 * 课程有语文，数学，英语，专业课 4 门课程
 * 可以查看全班人数
 * 可以输入某课程成绩
 * 可以查看某个学号学生的详细信息，包括成绩
 * 可以对某课程按照成绩排序
 * 可以查看某课程或者综合（总成绩）排名情况
 * 可以查看某课程或者综合排名的第i名的学生信息
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 * 学生定义
 */
typedef struct student{
    char id[14];        // 学号：13 位数字
    char name[50];      // 姓名
    char gender;        // 性别: 1男，2女
    int  age;           // 年龄
    char college[50];   // 学院：学院名
    int  sch;           // 语文
    int  sma;           // 数学
    int  sen;           // 英语
    int  spr;           // 专业课
} Student;

/// 学生保存文件。
#define StuFile  "student.dat"

/// 主菜单操作。
#define ADD        1    //增加
#define DELETE     2    //删除
#define MODIFY     3    //修改
#define SEARCH     4    //查找
#define SORT       5    //排序
#define LIST       6    //查看
#define QUIT       0    //退出

//函数声明
int readInt(const char * hint, int min, int max);
int readStr(const char * hint, char * result, int min, int max);

void head();
void print(Student stu);

int appendStudent(Student stu);
int searchStudent(Student * stu, char * id);
int modifyStudent(Student stu, int p);
int readAllStudents(Student **stu);

int getChoice();

int score(Student stu, int subj);

void addS();
int addS_STU(Student *stu);
void listS();
void info(int val, int num, Student * stu, int subj, int type);
void searchS();
void modifyS();
int modifyS_edit(Student *stu);
void deleteS();
void sortS();
void change(Student * stu);
void sort(Student * array,int n,int key);


///主框架
int main() {
    while(1) {
        int num = getChoice();
        if (num == QUIT) { break; }
        system("cls");
        switch (num) {
            case ADD:
                addS();
                break;
            case DELETE:
                deleteS();
                break;
            case MODIFY:
                modifyS();
                break;
            case SEARCH:
                searchS();
                break;
            case SORT:
                sortS();
                break;
            case LIST:
                listS();
                break;
            default:
                continue;
        }
    }
    printf("\n已退出\n");
    return 0;
}


//选择操作
int getChoice(){
    system("cls");
    printf("   Welcome To Student Management System\n");
    printf("\t\t 功能菜单\n");
    printf("==========================================\n");
    printf("\t    %d: 录入学生信息\n", ADD);
    printf("\t    %d: 删除学生信息\n", DELETE);
    printf("\t    %d: 修改学生信息\n", MODIFY);
    printf("\t    %d: 查找学生信息\n", SEARCH);
    printf("\t    %d: 学生成绩排名\n", SORT);
    printf("\t    %d: 查看全班信息\n", LIST);
    printf("\t    %d: 退出系统\n", QUIT);
    printf("==========================================\n");
    return readInt("请输入功能序号：", 0, 6);
}


// 录入相关函数 ========================================================================================================

/**
 * 读入一个整数。
 *
 * 指定提示字符串，允许的最小值和最大值，如果超出范围或不是整数则重新输入，最后返回读入的整数。
 *
 * 使用：int sch = readInt("请输入成绩：", 0, 100);  // 0 是允许的最小值，100 是允许的最大值。
 */
int readInt(const char * hint, int min, int max) {
    int result;
    printf("%s", hint);   // 输出提示。
    fflush(stdin);        // 清除以前输入剩下的内容。
    while (!scanf("%d", &result) || result < min || result > max) {   // 读取输入整数，如果整数范围不在 min 和 max 之间则循环再次输入。
        fflush(stdin);    //  清除以前输入剩下的内容。
        printf("请重新输入：");
    }
    fflush(stdin);        //  清除以前输入剩下的内容。
    return result;        //  返回输入的整数。
}


/**
 * 读入一个字符串，如果不符合要求则重新输入。
 *
 * 指定提示字符串，保存字符串，允许的最小长度和最大长度，如果超出范围则重新输入。
 * 如果输入正确返回 0, 如果输入退出字符 * 返回 -1。
 *
 * 使用：
 *  char name[20];
 *  int x = readInt("请输入姓名：", name, 1, 19);  // 1 是最短输入 1 个字符，最长输入 19 个字符。
 */
int readStr(const char * hint, char * result, int min, int max) {
    printf("%s", hint);  // 输出提示
    fflush(stdin);       // 清除以前输入剩下的内容。
    char format[10];
    sprintf(format, "%%%ds", max);   // 使用最大长度 max 创建格式字符串： 比如 max = 9 则 format 为 "%9s"
    char cache[100];
    while (!scanf(format, cache) || ((strlen(cache) < min) && strcmp(cache, "*"))) {  //将字符串读入 cache，不符合条件则循环，重新读入。
        fflush(stdin);    //  清除以前输入剩下的内容。
        printf("请重新输入：");
    }
    fflush(stdin);  //  清除以前输入剩下的内容。
    if (cache[0] == '*') {
        return -1;    // 输入 * 则返回 -1。
    } else {
        strcpy(result, cache);   // 将输入放入需要输入的变量。
        return 0;
    }
}

// 文件操作相关函数 ====================================================================================================

/**
 * 追加学生信息。
 *
 * 指定学生结构，将其追加至文件末尾，保存成功返回 0，否则返回 -1。
 */
int appendStudent(Student stu) {
    FILE * fp = fopen(StuFile, "ab");
    if (fp == NULL) {
        return 0;
    }
    int a = fwrite(&stu, sizeof(Student), 1, fp);
    fclose(fp);
    return a == 1 ? 0 : -1;
}


/**
 * 查找学生信息。
 *
 * 指定学生结构（用来保存找到的学生）和学号，返回学生在文件中的序号，找不到则返回 -1.
 */
int searchStudent(Student * stu, char * id) {
    int p = 0;
    FILE * fp = fopen(StuFile, "rb");
    if(fp == NULL){
        return -1;
    }
    while (!feof(fp)) {
        fread(stu, sizeof(Student), 1, fp);
        if (strcmp(stu->id, id) == 0 && (strcmp(stu->id, "DELETED") != 0)) {
            fclose(fp);
            return p;
        }
        p++;
    }
    fclose(fp);
    return -1;
}


/**
 * 保存指定位置的学生信息。
 *
 * 指定学生结构和学生信息在文件中的序号，修改文件中该学生的信息。修改成功返回 0，否则返回 -1。
 */
int modifyStudent(Student stu, int p) {
    FILE * fp = fopen(StuFile, "rb+");
    if(fp == NULL) { return 0; }
    fseek(fp, sizeof(Student) * p, SEEK_SET);
    int a = fwrite(&stu, sizeof(Student), 1, fp);
    fclose(fp);
    return a == 1 ? 0 : -1;
}


/**
 * 从文件读取所有学生信息。
 *
 * 指定一个指针的地址，用来返回所有学生数据，返回值为读取学生个数。
 */
int readAllStudents(Student **stu){
    FILE * fp = fopen(StuFile, "rb");
    if (fp == NULL) { return 0; }
    fseek(fp, 0, SEEK_END);
    int size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    *stu = (Student *)malloc((size / sizeof(Student) + 1) * sizeof(Student));
    Student * cur = *stu;
    int n = 0;
    while (!feof(fp)) {
        if (fread(cur, sizeof(Student), 1, fp) == 1 && (strcmp((*cur).id, "DELETED") != 0)) {
            n++;
            cur++;
        }
    }
    fclose(fp);
    return n;
}


// 录入相关函数 ========================================================================================================

/**
 * 录入学生信息。
 */
void addS(){
    Student stu;
    printf("请录入学生信息：\n");
    printf("----------------\n");
    if (addS_STU(&stu)) { return; };                     //**输入学生信息
    if (!appendStudent(stu)) {                           //**保存成功
        printf("\n已录入学生信息：\n");
        printf("----------------\n");
        head();//**
        print(stu);                     //**输出刚刚输入的学生信息
    } else {
        printf("\n保存失败！\n");
    }
    system("pause");
}


/**
 * 输入学生信息。
 *
 * 输入的学生信息保存在首个参数，输入成功返回 0，中途退出返回 -1。
 */
int addS_STU(Student *stu) {
    if (readStr("学号 [13 位，输入 * 退出]：", stu->id, 13, 13) == -1) { return -1; };
    if (readStr("姓名 [输入 * 退出]：", stu->name, 1, 49) == -1) { return -1; };
    switch (readInt("性别 [1:男, 2:女, 0:退出]：", 0, 2)) {
        case 0: return -1;
        case 1: stu->gender = '1'; break;
        case 2: stu->gender = '2'; break;
    }
    stu->age = readInt("年龄 [0 - 150]：", 0, 150);
    if (readStr("学院 [输入 * 退出]：", stu->college, 1, 49) == -1) { return 1; };
    stu->sch = readInt("语文成绩 [0 - 100]：", 0, 100);
    stu->sma = readInt("数学成绩 [0 - 100]：", 0, 100);
    stu->sen = readInt("英语成绩 [0 - 100]：", 0, 100);
    stu->spr = readInt("专业课成绩 [0 - 100]：", 0, 100);
    return 0;
}


// 输出学生列表相关函数 ================================================================================================

/**
 * 输出学生列表标题。
 */
void head() {
    printf("%14s%10s%5s%5s%14s%6s%6s%6s%6s%6s%6s\n",
        "学号", "姓名", "性别", "年龄", "学院", "语文", "数学", "英语", "专业", "合计", "平均");
    for(int i = 1; i < 90; i++) { printf("-"); }
    printf("\n");
}


/**
 * 输出一个学生信息。
 */
void print(Student stu) {
    printf("%14s%10s%5s%5d%14s%6d%6d%6d%6d%6d%6d\n",
        stu.id, stu.name, (stu.gender == '1') ? "男" : "女", stu.age, stu.college, stu.sch, stu.sma, stu.sen, stu.spr,
        stu.sch + stu.sma + stu.sen + stu.spr, (stu.sch + stu.sma + stu.sen + stu.spr) / 4);
}


/**
 * 输出所有学生信息。
 */
void listS() {
    Student *stu;
    int mins[6];
    int maxs[6];
    int tols[6];
    int n = readAllStudents(&stu);
    if (n > 0) {
        head();
        for (int s = 0; s < 6; s++) {
            mins[s] = score(stu[0], s);
            maxs[s] = score(stu[0], s);
            tols[s] = 0;
        }
        for (int i = 0; i < n; i++){
            print(stu[i]);
            for (int s = 0; s < 6; s++) {
                tols[s] += score(stu[i], s);
                if (mins[s] > score(stu[i], s)) { mins[s] = score(stu[i], s); }
                if (maxs[s] < score(stu[i], s)) { maxs[s] = score(stu[i], s); }
            }
        }
        for(int i = 1; i < 90; i++) { printf("-"); }
        printf("\n%48s%6d%6d%6d%6d%6d%6d\n", "最低分", mins[0], mins[1], mins[2], mins[3], mins[4], mins[5]);
        printf("%48s%6d%6d%6d%6d%6d%6d\n", "最高分", maxs[0], maxs[1], maxs[2], maxs[3], maxs[4], maxs[5]);
        printf("%48s%6d%6d%6d%6d%6d%6d\n", "平均分", tols[0] / n, tols[1] / n, tols[2] / n, tols[3] / n, tols[4] / n, tols[5] / n);
        for(int i = 1; i < 90; i++) { printf("="); }
        printf("\n当前有 %d 个学生信息\n", n);
        for (int s = 0; s < 5; s++) {
            info(maxs[s], n, stu, s, 0);
            info(mins[s], n, stu, s, 1);
        }
        free(stu);
    } else {
        printf("当前没有学生信息\n");
    }
    system("pause");
}


/**
 * 显示某一科最高或最低分学生。
 *
 * 参数依次是该科最高或最低分数，学生总数，学生数组，科目编号，最高或最低分。
 */
void info(int val, int num, Student * stu, int subj, int type) {
    const char * types[2] = { "高", "低" };
    const char * subjs[5] = { "语文", "数学", "英语", "专业课", "总分" };
    printf("\n%s 最%s分：%d\n", subjs[subj], types[type], val);
    head();
    for (int i = 0; i < num; i++) {
        if (score(stu[i], subj) == val) { print(stu[i]); }
    }
}


// 查询学生信息相关函数 ================================================================================================

/**
 * 输入学号查询一个学生信息。
 */
void searchS(){
    Student stu;
    char id[14];
    if (readStr("请输入待查找学生的学号[13位, * 退出]：", id, 13, 13)) { return; }
    int p = searchStudent(&stu, id);
    if (p == -1) {
        printf("没有找到该学生信息。\n");
    } else {
        printf("查找结果：\n\n");
        head();
        print(stu);    
    }
    system("pause");
}

// 删除学生信息相关函数 ================================================================================================

/**
 * 输入学号删除一个学生信息。
 */
void deleteS() {
    Student stu;
    char id[14];
    if (readStr("请输入待删除学生的学号[13位, * 退出]：", id, 13, 13)) { return; }
    int p = searchStudent(&stu, id);
    if(p == -1) {
        printf("没有找到该学生信息。\n");
    } else {
        head();
        print(stu);
        int op = readInt("\n是否删除该学生信息[1: 删除，0: 放弃]：", 0, 1);
        if (op == 1) {
            strcpy(stu.id, "DELETED");
            if (!modifyStudent(stu, p)) {
                printf("已成功删除该学生的信息\n");
            } else{
                printf("删除失败\n");
            }     
        }
    }
    system("pause");
}


// 修改学生信息相关函数 ================================================================================================

/**
 * 修改学生信息。
 *
 * 输入学生学号后修改学生信息。
 */
void modifyS() {
    Student stu;
    char id[14];
    if (readStr("请输入待修改学生的学号[13位, * 退出]：", id, 13, 13)) { return; }
    int p = searchStudent(&stu, id);
    if (p == -1) {
        printf("\n没有找到该学生信息。\n");
        system("pause");
    }else{
        while (1) {
            system("cls");
            printf("学生的原信息如下：\n\n");
            head();
            print(stu);
            int a = modifyS_edit(&stu);
            if (a == 0) { return; }
            if (a >= 1 && a <= 9){
                modifyStudent(stu, p);
                printf("已成功修改学生的信息如下：\n");
                head();
                print(stu);
                system("pause");
            }
        }
     }
}

/**
 * 选择学生信息中的一项进行修改。
 *
 * 修改成功返回修改项序号，放弃修改该项返回 -1，返回主菜单则返回 0。
 */
int modifyS_edit(Student *stu){
    const char *items[10] = {
        "",
        "学号 [13位, * 退出]",
        "姓名 [输入 * 退出]",
        "性别 [1:男, 2:女, 0:退出]",
        "年龄 [0 - 150]",
        "学院 [输入 * 退出]",
        "语文成绩 [0 - 100]",
        "数学成绩 [0 - 100]",
        "英语成绩 [0 - 100]",
        "专业课成绩 [0 - 100]"
    };
    int op = readInt("\n请选择要修改的学生信息(1.学号, 2.姓名, 3.性别, 4.年龄, 5.学院, 6.语文, "
                     "7.数学, 8.英语, 9.专业课, 0.返回主菜单):", 0, 9);
    if (op == 0) { return 0; }
    char hint[100];
    sprintf(hint, "请修改学生的%s：", items[op]);
    switch (op) {
        case 1:
            if (readStr(hint, stu->id, 13, 13) == -1) { return -1; };
            break;
        case 2:
            if (readStr(hint, stu->name, 1, 50) == -1) { return -1; };
            break;
        case 3:
            switch (readInt(hint, 0, 2)) {
                case 0: return -1;
                case 1: stu->gender = '1'; break;
                case 2: stu->gender = '2'; break;
            }
            break;
        case 4:
            stu->age = readInt(hint, 0, 150);
            break;
        case 5:
            if (readStr(hint, stu->college, 1, 50) == -1) { return -1; };
            break;
        case 6:
            stu->sch = readInt(hint, 0, 100);
            break;
        case 7:
            stu->sma = readInt(hint, 0, 100);
            break;
        case 8:
            stu->sen = readInt(hint, 0, 100);
            break;
        case 9:
            stu->spr = readInt(hint, 0, 100);
            break;
    }
    return op;
}


// 排序统计相关函数 ====================================================================================================


/**
 * 输出学生名次列表标题。
 */
void headIdx() {
    printf("%14s%10s%5s%5s%14s%6s%6s%6s%6s%6s%6s%6s\n",
        "学号", "姓名", "性别", "年龄", "学院", "语文", "数学", "英语", "专业", "合计", "平均", "名次");
    for(int i = 1; i < 120; i++) { printf("="); }
    printf("\n");
}


/**
 * 输出一个学生信息。
 *
 * 指定学生信息和名次进行输出。
 */
void printIdx(Student stu, int index) {
    printf("%14s%10s%5s%5d%14s%6d%6d%6d%6d%6d%6d%6d\n",
        stu.id, stu.name, (stu.gender == '1') ? "男" : "女", stu.age, stu.college, stu.sch, stu.sma, stu.sen, stu.spr,
        stu.sch + stu.sma + stu.sen + stu.spr, (stu.sch + stu.sma + stu.sen + stu.spr) / 4, index);
}


/**
 * 获取一个学生信息中指定科目的成绩。
 *
 * 指定学生信息和科目编号（0.语文, 1.数学, 2.英语, 3.专业课, 4.总成绩, 5.平均）。
 */
int score(Student stu, int subj) {
    switch (subj) {
        case 0: return stu.sch;
        case 1: return stu.sma;
        case 2: return stu.sen;
        case 3: return stu.spr;
        case 4: return stu.sch + stu.sma + stu.sen + stu.spr;
        case 5: return (stu.sch + stu.sma + stu.sen + stu.spr) / 4;
    }
    return 0;
}


/**
 * 选择排序项排序学生信息。然后执行选中的操作。
 *
 */
void sortS() {
    const char *hint[5] = {
        "语文",
        "数学",
        "英语",
        "专业课",
        "总成绩"
    };
    Student *stu;
    char id[20];
    while (1) {
        system("cls");
        int subj = readInt("\n请选择要排序的项 (1.语文, 2.数学, 3.英语, 4.专业课, 5.总成绩/平均成绩 0.返回主菜单):", 0, 5);
        if (subj == 0) { break; }
        int n = readAllStudents(&stu);
        sort(stu, n, subj - 1);
        while (1) {
            int op = readInt("\n请选择操作 (1.显示所有同学排名, 2.查看某一名次同学, 3.查看某一同学名次 0.返回主菜单):", 0, 3);
            if (op == 0) { break; }
            switch (op) {
                case 1:
                    {
                        printf("%s排名如下：\n", hint[subj - 1]);
                        headIdx();
                        int idx = 0;
                        int sco = -1;
                        for(int i = 0; i < n; i++){
                            if (score(stu[i], subj - 1) != sco) {
                                sco = score(stu[i], subj - 1);
                                idx = i + 1;
                            }
                            printIdx(stu[i], idx);
                        }
                    }
                    break;
                case 2:
                    {
                        int p = readInt("请输入要查看的名次：", 1, n);
                        printf("\n%s第 %d 名学生信息如下：\n",  hint[subj - 1], p);
                        headIdx();
                        int sco = score(stu[p - 1], subj - 1);
                        for (int i = p - 1; i < n; i++) {
                            if (score(stu[i], subj - 1) == sco) {
                                printIdx(stu[i], p);
                            }
                        }
                    }
                    break;
                case 3:
                    {
                        char id[14];
                        if (readStr("请输入待查名次学生的学号[13位, * 退出]：", id, 13, 13)) { break; }
                        int idx = 0;
                        int sco = -1;
                        int found = 0;
                        for (int i = 0; i < n; i++){
                            if (score(stu[i], subj - 1) != sco) {
                                sco = score(stu[i], subj - 1);
                                idx = i + 1;
                            }
                            if (!strcmp(stu[i].id, id)) {
                                headIdx();
                                printIdx(stu[i], idx);
                                found = 1;
                                break;
                            }
                        }
                        if (found == 0) { printf("没有这个同学\n"); }
                    }
                    break;
                default:
                    break;
            }
        }
        free(stu);
    }
}


/**
 * 交换当前学生和下一个学生的学生信息。
 *
 * 指定当前学生，与下一个学生相交换。
 */
void change(Student * stu){
    if (stu) {
        Student tmp;
        tmp = *stu;
        *stu = *(stu + 1);
        *(stu + 1) = tmp;
    } else {
        printf("很遗憾，系统错误。");
        return;
    }
}

//成绩排序
void sort(Student *array, int n, int subj) {
    if (array && (n > 0)) {
        Student *p = array;
        int i, j;
        for(j = 0; j < n; j++){
            for(i = 0; i < n - j - 1; i++) {
                if (score(*(array + 1), subj) > score(*array, subj)) { change(array); }
                array++;
            }
            array = p;
        }
    }else{
        printf("很遗憾，系统错误。");
        return;
    }
}


