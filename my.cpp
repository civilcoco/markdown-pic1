#include <stdio.h> 
#include <stdlib.h>

struct node
{
	int data;
	struct node *next;
};
//创建节点 
struct node* createNode(int val)
{
	struct node *pnode = (struct node*)malloc(sizeof(struct node));
	if(pnode!=NULL)
	{
		pnode->data=val;
		pnode->next=NULL;
	}
	return pnode;
}
//在链表末尾添加节点 
int addNode(struct node **pheadptr,int val)
{
	struct node *p = createNode(val);
	//创建失败，p为空，不可再添加，直接返回0 
	if(p==NULL)
		return 0;
	//空链表，直接将首节点变成当前添加的节点 
	if(*pheadptr==NULL)
		*pheadptr=p;
	else
	{
		struct node *ptmp=*pheadptr;
		while(ptmp->next)
			ptmp=ptmp->next;
		ptmp->next=p;
	}
	return 1;
}


// 返回链表的长度 
unsigned countOfNodes(struct node *headptr)
{
	unsigned c=0;
	while(headptr)
	{
		++c;
		headptr=headptr->next;
	}
	return c;
}
//打印所有的节点，从头到尾顺序打印 
void printAllNode(struct node *headptr)
{
	while(headptr)
	{
		printf("%d ",headptr->data);
		headptr=headptr->next;
	}
	printf("\n");
}
//删除对应位置的节点 
int deleteNode(struct node **pheadptr, unsigned loc)
{
	unsigned c = countOfNodes(*pheadptr);
	//所在的位置超过链表长度，不存在该位置 
	if(c<loc)
		return 0;
	struct node *p = *pheadptr;
	//如果删除头节点 
	if(loc==1)
	{
		*pheadptr = (*pheadptr)->next;
		free(p);
	}
	else  //删除对应位置的节点 
	{
		for(int i=2;i<loc;++i)
			p=p->next;
		struct node *pdel = p->next;
		p->next = pdel->next;
		free(pdel);
	}
	return 1;
}
//删除所有节点 
void deleteAllNodes(struct node **pheadptr)
{
	while(countOfNodes(*pheadptr))
		deleteNode(pheadptr,1); //递归调用，只要还有节点就继续删除 
}
//查找相应位置的节点并返回 
struct node *search(struct node **pheadptr,int loc)
{
	unsigned c = countOfNodes(*pheadptr);
	struct node *p = *pheadptr;
	if(c<loc)
	{
		return *pheadptr;
	}
	else{
		while(--loc)
		{
			p = p -> next; 
		}
		return p;
	}	
			
} 

//选定位置插入已有值的节点 ，注意插入位置为loc+1 
int insertNode(struct node **pheadptr,int val,int loc)
{
	struct node *q=createNode(val);
	struct node *p=search(pheadptr,loc);
	if(loc!=1 && p==*pheadptr){
		free(q);
		return 0;
	}
	else
	{
		q->next=p->next;
		p->next=q;
		return 1; 
	}
	
}
// 读取txt文档中的数据，构建单向链表
struct node* readFileToList(const char* filename)
{
    FILE* f = fopen(filename, "r");
    if(f == NULL)
    {
        printf("Failed to open the file.\n");
        return NULL;
    }
    struct node *head = NULL;
    int val;
	char s;	
	while(!feof(f))
	{
		s=fgetc(f);val=0;
		while(s!=',')
		{
			if(feof(f)) break;
			val=val*10+int(s-'0');
			s=fgetc(f);	
		}
		if(!addNode(&head, val))
        {
            printf("Failed to add node.\n");
            fclose(f);
            return NULL;
        }
	}
	fclose(f);
    return head;
}
// 将单向链表的数据按照顺序写入txt文档
int writeListToFile(const char* filename, struct node *head)
{
    FILE* file = fopen(filename, "w");
    if(file == NULL)
    {
        printf("Failed to open the file.\n");
        return 0;
    }
    
    struct node *current = head;
    
    while(current != NULL)
    {
        fprintf(file, "%d ", current->data);
        current = current->next;
    }
    
    fclose(file);
    return 1;
}
// 逆转链表
void reverse(struct node** pheadptr) {
    struct node *p = *pheadptr;
    struct node *q;
    *pheadptr =NULL;
    while(p)
    {
    	q=p;
    	p=p->next;
    	q->next=*pheadptr;
    	*pheadptr=q;
	}
	
}
//链表插入排序
void insertSort(struct node** pheadptr)
{
	//一开始没有有序链表，设为空 
    struct node* sortedList = NULL;
    struct node* current = *pheadptr;
    while (current != NULL) {
    	//记录下一个节点的位置，防止断开后找不到入口 
        struct node* nextNode = current->next;
        // 在已排序链表中找到插入位置
        // 如果当前还没有有序链表 或者 目前节点值小于有序序列节点的最小值 （即有序链表第一个节点的值） 
        if (sortedList == NULL || current->data < sortedList->data) {
        	//将目前的节点指向原本有序链表的第一个节点，并且将有序序列最小节点 替换为 当前的节点 
            current->next = sortedList;
            sortedList = current;
        }
        else { //如果当前的节点值不比有序链表的第一个节点值小 
            struct node* p = sortedList;
            //遍历有序链表，找到应该插入的位置 
            while (p->next != NULL && p->next->data < current->data) {
                p = p->next;
            }
            current->next = p->next;
            p->next = current;
        }
        current = nextNode;
    }
    //头节点替换为有序链表的第一个节点 
    *pheadptr = sortedList;
}

//链表冒泡排序
//引用头节点的地址的变量的地址，不会改变头节点地址所指向的头节点的位置 
//假设头节点的地址为60010，则60010是一个变量，那么它的地址可能是24000
//当令h = head-ref时，实际上是令h的地址的变量的地址等于 60010，则*h即*(60010)指向的就是头节点的地址
//但是这只是赋值，头节点的地址的变量的地址没有改变，那么所指向的头节点也不会改变 
void bubbleSort(struct node **head_ref) 
{
    //指向头节点
    struct node **h;
    int i, j, swapped;
 
    for (i = 0; i <= countOfNodes(*head_ref); i++) 
    {
        h = head_ref;
        swapped = 0;//标记是否交换过 
 
        for (j = 0; j < countOfNodes(*head_ref) - i - 1; j++) 
        {
        	//*h为交换时的前一位 
            struct node *p1 = *h;
            struct node *p2 = p1->next;
 
            if (p1->data > p2->data) 
            {
                /* 交换 */
                p1->next = p2->next;
                p2->next = p1;
                *h = p2;//指向较小节点 ,设置为当前节点 
 
                swapped = 1;
            }
 			//当前节点向后移一位 
            h = &(*h)->next;
        }
 
        /* 不交换则结束循环 */
        if (swapped == 0)
            break;
    }
}



// 链表选择排序
void selectSort(struct node **head_ref)
{
    struct node **h;
    for (h = head_ref; *h; h = &(*h)->next)
    {
        struct node **min = h;
        for (struct node **p = &(*h)->next; *p; p = &(*p)->next)
        {
            if ((*p)->data < (*min)->data)
            {
                min = p;
            }
        }
        // 交换
        struct node *temp = *min;
        //在链表中删除原来的min节点 
        *min = (*min)->next;
        temp->next = *h;
        *h = temp;
    }
}
int main()
{
    struct node *list = readFileToList("linklist.txt");
    printAllNode(list);
    reverse(&list);
    printAllNode(list);
    selectSort(&list);
    printAllNode(list);
    writeListToFile("output.txt",list);
	struct node *headPtr=NULL;
	for(int i=1;i<5;++i)
		addNode(&headPtr, i * 10);
	printf("The number of linked list nodes is:%u\n",countOfNodes(headPtr));
	struct node *p=search(&headPtr,4);
	printf("%d\n",p->data);
	insertNode(&headPtr,123,3);
	printAllNode(headPtr);
	printf("Delete the node at location 2.\n");
	deleteNode(&headPtr, 2);
	printf("The number of linked list nodes is:%u\n",countOfNodes(headPtr));
	printAllNode(headPtr);
	deleteAllNodes(&headPtr);
	return 0;
}
