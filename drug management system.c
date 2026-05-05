#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define drug_informationCode_len 8
#define name_len 16

typedef struct Date{
	int year;
	int month;
	int day;
}Date;
typedef struct Drug{
	int ID;//编号
	char shelfCode [drug_informationCode_len];//货架编号
	char genericName [name_len];// 通用名
	char tradeName [name_len];//商品名
	char alias [name_len];//别名
	int stock;//库存
	int threshold;//阈值
	Date expirationDate;//有效期
	int level;//药品分级，12345对应处方药ABCDX级，67对应非处方药甲类和乙类
	Drug *next;
}Drug;
Drug *head=NULL;
int next_ID=1;

//该部分用以计算日期差
Date GetCurrentDate(){
    time_t t=time(NULL);
	struct tm *tm=localtime(&t);
	Date d;
	d.year=tm->tm_year+1900;
	d.month=tm->tm_mon+1;
	d.day=tm->tm_mday;
	return d;	
}
int LeapYear(int year){
	if ((year%4==0&&year%100!=0)||year%400==0)
	return 1;
	else
	return 0;
}
long CEdays(Date date){//计算一个日期到公元1年1月1日有多少天                                             
	int dayofmonth[12]={31,28,31,30,31,30,31,31,30,31,30,31};
	long days=0;
	for(int i=1;i<date.year;i++){
		if (LeapYear(i)==1)
		days+=366;
		else days+=365;
	}
	for(int i=0;i<date.month-1;i++){
		if (LeapYear(date.year)==1&&i==1)
		days+=dayofmonth[i]+1;
		else days+=dayofmonth[i];
	}
	days+=date.day;
	return days;
}
long DateDifference(Date date1,Date date2){//计算两个日期的差
	if (CEdays(date1)>CEdays(date2))
	return CEdays(date1)-CEdays(date2);
	else return CEdays(date2)-CEdays(date1);
}
int IfDateValid(int year,int month,int day){
	int dayofmonth[12]={31,28,31,30,31,30,31,31,30,31,30,31};
	if (year<2026) return 0;
	if (month<1||month>12) return 0;
	if (day<1) return 0;
	if (month==2)
		if(day>28+LeapYear(year))
		return 0;
	else if(day>dayofmonth[month-1])
	return 0;
	return 1;	
}


void addDrug(){
   Drug *newDrug=(Drug*)malloc(sizeof(Drug));  
   if (!newDrug){
   	printf("内存分配失败！\n");
   	return; 
   }
   newDrug->ID =next_ID++;
   printf(" 请输入药品通用名：");
   fgets(newDrug->genericName,name_len,stdin);
   newDrug->genericName[strcspn(newDrug->genericName,"\n")]='\0';
    printf("请输入药品商品名：");
   fgets(newDrug->tradeName,name_len,stdin);
   newDrug->tradeName[strcspn(newDrug->tradeName,"\n")]='\0';
    printf(" 请输入药品别名：");
   fgets(newDrug->alias,name_len,stdin);
   newDrug->alias[strcspn(newDrug->alias,"\n")]='\0';
   printf(" 请输入药品货架编号：");
   fgets(newDrug->shelfCode,drug_informationCode_len,stdin);
   newDrug->shelfCode[strcspn(newDrug->shelfCode,"\n")]='\0';
   printf("请输入药品分级：\n12345对应处方药ABCDX级，67对应非处方药甲类和乙类\n");
   scanf("%d",&newDrug->level);
   getchar();
   printf("请输入有效期(年月日，空格分隔): ");
   scanf("%d %d %d",&newDrug->expirationDate.year,&newDrug->expirationDate.month,&newDrug->expirationDate.day);
   getchar();
   if (IfDateValid(newDrug->expirationDate.year,newDrug->expirationDate.month,newDrug->expirationDate.day)==0){
   	printf("输入有效期无效");
   	free(newDrug);
   	return;
   }
   printf("请输入库存阈值：");
   scanf("%d",&newDrug->threshold);
   getchar();
   printf("请输入入库数量：");
   scanf("%d",&newDrug->stock);
   getchar();
   newDrug->next=head;
   head=newDrug;
   printf("药品录入成功，ID: %d\n",newDrug->ID);
}

void searchDrug(Drug *head,const char *keyword){
	if (head == NULL) {
        printf("药品链表为空，没有数据可搜索\n");
        return;
    }
    if (keyword == NULL || strlen(keyword) == 0) {
    	printf("关键字为空，显示所有药品：\n");
    	Drug *current=head;
    	 int index=1;
    	 while (current){
    	 	printf("--- 药品 %d ---\n", index++);
    	 	printf("药品ID：%d\n",current->ID);
    	 	printf("货架编号：%s\n",current->shelfCode);
    	 	printf("药品分级：%d\n12345对应处方药ABCDX级，67对应非处方药甲类和乙类\n",current->level);
    	 	printf("商品名: %s\n", current->tradeName);
            printf("通用名: %s\n", current->genericName);
            printf("别  名: %s\n", current->alias);
            printf("库存：%d\n",current->stock);
            printf("有效期：%d %d %d\n",current->expirationDate.year,current->expirationDate.month,current->expirationDate.day);
            current=current->next;
		 }
		 return;
	}      
    int found=0;
    int index=1;
    Drug *current=head;
    while (current!=NULL){
    	int match=0;
    	if (current->tradeName&&strstr(current->tradeName,keyword))
            match = 1;
        if (!match&&current->genericName&&strstr(current->genericName,keyword))
            match = 1;
        if (!match&&current->alias&&strstr(current->alias,keyword))
            match = 1;
        if(match){
        	printf("--- 匹配结果 %d ---\n", index++);
        	printf("药品ID：%d\n",current->ID);
    	 	printf("货架编号：%s\n",current->shelfCode);
    	 	printf("药品分级：%d\n12345对应处方药ABCDX级，67对应非处方药甲类和乙类\n",current->level);
            printf("商品名: %s\n", current->tradeName);
            printf("通用名: %s\n", current->genericName);
            printf("别  名: %s\n", current->alias);
            printf("库存：%d\n",current->stock);
            printf("有效期：%d %d %d\n",current->expirationDate.year,current->expirationDate.month,current->expirationDate.day);
            found = 1;
		}
		current=current->next;
	}
	if (!found) {
        printf("未找到包含 \"%s\" 的药品\n",keyword);
    }
}    


void modifyStock(){
	int id;
    char keyword [name_len];
    printf("请输入关键词: ");
    fgets(keyword,name_len,stdin);
    keyword[strcspn(keyword,"\n")]='\0';
    searchDrug(head,keyword);
    printf("相关药品信息已给出，请输入药品ID: ");
	scanf("%d", &id);
    getchar();
    Drug *q=head;
    while (q&&q->ID!= id)q=q->next;
    if (!q) {
        printf("未找到ID为%d的药品！\n", id);
        return;
    }
    int newStock;
    printf("请输入新的库存数量: ");
    scanf("%d", &newStock);
    getchar();
    if (newStock< 0) {
        printf("库存不能为负数！\n");
        return;
    }
    q->stock=newStock;
    printf("库存修改成功！\n");
} 

void modifyShelfCode(){
	int id;
    char keyword [name_len];
    printf("请输入关键词: ");
    fgets(keyword,name_len,stdin);
    keyword[strcspn(keyword,"\n")]='\0';
    searchDrug(head,keyword);
    printf("相关药品信息已给出，请输入药品ID: ");
	scanf("%d", &id);
    getchar();
    Drug *q=head;
    while (q&&q->ID!= id)q=q->next;
    if (!q) {
        printf("未找到ID为%d的药品！\n", id);
        return;
    }
    char newShelf [drug_informationCode_len];
    printf("请输入新的货架编号: ");
    fgets(newShelf,drug_informationCode_len,stdin);
    newShelf[strcspn(newShelf,"\n")]='\0';
    strcpy(q->shelfCode,newShelf);
    printf("货架编号修改成功！\n");
}

void stockWarning(){
	Drug *current=head;
	int found=0;
	while (current){
		if (current->stock<=current->threshold){
        	printf("药品ID：%d\n",current->ID);
    	 	printf("货架编号：%s\n",current->shelfCode);
    	 	printf("药品分级：%d\n12345对应处方药ABCDX级，67对应非处方药甲类和乙类\n",current->level);
            printf("商品名: %s\n", current->tradeName);
            printf("通用名: %s\n", current->genericName);
            printf("别  名: %s\n", current->alias);
            printf("库存：%d\n",current->stock);
            printf("有效期：%d %d %d\n",current->expirationDate.year,current->expirationDate.month,current->expirationDate.day);
            found = 1;
		}
		current=current->next;
	}
	if (!found) {
        printf("未找到库存低于阈值的药品\n");
    }
}

void expirationWarning(){
   Date today=GetCurrentDate();
   int expired=0;
   int warning=0;
   Drug *current=head;
   while (current){
   	if (CEdays(current->expirationDate)<=CEdays(today))
   		expired=1; 
	if (CEdays(current->expirationDate)>CEdays(today)&&DateDifference(current->expirationDate,today)<=7)
	    warning=1;
	current=current->next;
   }
   current=head;
   if (expired==1){
   	printf("以下是已过期药品，请及时处理\n");
   	while (current){
   	if (CEdays(current->expirationDate)<=CEdays(today)){
   		    printf("药品ID：%d\n",current->ID);
    	 	printf("货架编号：%s\n",current->shelfCode);
    	 	printf("药品分级：%d\n12345对应处方药ABCDX级，67对应非处方药甲类和乙类\n",current->level);
            printf("商品名: %s\n", current->tradeName);
            printf("通用名: %s\n", current->genericName);
            printf("别  名: %s\n", current->alias);
            printf("库存：%d\n",current->stock);
            printf("有效期：%d %d %d\n",current->expirationDate.year,current->expirationDate.month,current->expirationDate.day);
	   }
	   current=current->next;
   }
   }
   else printf("无过期药品\n");
   current=head;
   if (warning==1){
   	printf("以下是保质期不超过一周的药品，请及时处理\n");
   	while (current){
   	if (CEdays(current->expirationDate)>CEdays(today)&&DateDifference(current->expirationDate,today)<=7){
   		    printf("药品ID：%d\n",current->ID);
    	 	printf("货架编号：%s\n",current->shelfCode);
    	 	printf("药品分级：%d\n12345对应处方药ABCDX级，67对应非处方药甲类和乙类\n",current->level);
            printf("商品名: %s\n", current->tradeName);
            printf("通用名: %s\n", current->genericName);
            printf("别  名: %s\n", current->alias);
            printf("库存：%d\n",current->stock);
            printf("有效期：%d %d %d\n",current->expirationDate.year,current->expirationDate.month,current->expirationDate.day);
	   }
	   current=current->next;
   }
   }
   
}

void deleteDrug(){
	Drug *current=head;
	Drug *prev=NULL;
	int id;
	printf("请输入要删除的药品的id:\n");
	scanf("%d",&id);
	getchar();
	while(current!=NULL&&current->ID!=id){
		prev=current;
		current=current->next;
	}
	if (current==NULL){
		printf("未找到ID为%d的药品\n");
		return;
	}
	if (prev==NULL)
	head=current->next;
	else prev->next=current->next;
	free (current);
	printf("药品删除成功\n");
}

// 一键删除所有已过期药品
void deleteAllExpiredDrugs() {
    Date today = GetCurrentDate();
    long todayDays = CEdays(today);
    Drug *current = head;
    Drug *prev = NULL;
    int count = 0;
    if (head == NULL) {
        printf("无药品可删\n");
        return;
    }
    while (current != NULL) {
        if (CEdays(current->expirationDate) <= todayDays) {
            Drug *toDelete = current;
            if (prev == NULL) {
                head = current->next;
                current = head;
            } else {
                prev->next = current->next;
                current = prev->next;
            }
            free(toDelete);
            count++;
        } else {
            prev = current;
            current = current->next;
        }
    }
    if (count == 0)
        printf("没有过期药品\n");
    else
        printf("已删除 %d 个过期药品\n", count);
}

int main()
{
    int choice;
    char key[name_len];
    while(1)
    {
        printf("\n========== 药品管理系统 ==========\n");
        printf("1. 添加药品\n");
        printf("2. 搜索药品\n");
        printf("3. 修改药品库存\n");
        printf("4. 修改货架编号\n");
        printf("5. 库存不足预警\n");
        printf("6. 过期与临期预警\n");
        printf("7. 按ID删除药品\n");
        printf("8. 一键删除所有过期药品\n");
        printf("0. 退出系统\n");
        printf("==================================\n");
        printf("请输入功能选项：");
        scanf("%d",&choice);
        getchar(); 
        switch(choice)
        {
            case 1:
                addDrug();
                break;
            case 2:
                printf("请输入搜索关键词(直接回车查看全部)：");
                fgets(key,name_len,stdin);
                key[strcspn(key,"\n")] = '\0';
                searchDrug(head,key);
                break;
            case 3:
                modifyStock();
                break;
            case 4:
                modifyShelfCode();
                break;
            case 5:
                stockWarning();
                break;
            case 6:
                expirationWarning();
                break;
            case 7:
                deleteDrug();
                break;
            case 8:
                deleteAllExpiredDrugs();
                break;
            case 0:
                printf("退出系统成功！\n");
                return 0;
            default:
                printf("输入选项无效，请重新选择！\n");
                break;
        }
    }
    return 0;
}



















