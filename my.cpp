#include <stdio.h> 
#include <stdlib.h>

struct node
{
	int data;
	struct node *next;
};
//�����ڵ� 
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
//������ĩβ��ӽڵ� 
int addNode(struct node **pheadptr,int val)
{
	struct node *p = createNode(val);
	//����ʧ�ܣ�pΪ�գ���������ӣ�ֱ�ӷ���0 
	if(p==NULL)
		return 0;
	//������ֱ�ӽ��׽ڵ��ɵ�ǰ��ӵĽڵ� 
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


// ��������ĳ��� 
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
//��ӡ���еĽڵ㣬��ͷ��β˳���ӡ 
void printAllNode(struct node *headptr)
{
	while(headptr)
	{
		printf("%d ",headptr->data);
		headptr=headptr->next;
	}
	printf("\n");
}
//ɾ����Ӧλ�õĽڵ� 
int deleteNode(struct node **pheadptr, unsigned loc)
{
	unsigned c = countOfNodes(*pheadptr);
	//���ڵ�λ�ó��������ȣ������ڸ�λ�� 
	if(c<loc)
		return 0;
	struct node *p = *pheadptr;
	//���ɾ��ͷ�ڵ� 
	if(loc==1)
	{
		*pheadptr = (*pheadptr)->next;
		free(p);
	}
	else  //ɾ����Ӧλ�õĽڵ� 
	{
		for(int i=2;i<loc;++i)
			p=p->next;
		struct node *pdel = p->next;
		p->next = pdel->next;
		free(pdel);
	}
	return 1;
}
//ɾ�����нڵ� 
void deleteAllNodes(struct node **pheadptr)
{
	while(countOfNodes(*pheadptr))
		deleteNode(pheadptr,1); //�ݹ���ã�ֻҪ���нڵ�ͼ���ɾ�� 
}
//������Ӧλ�õĽڵ㲢���� 
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

//ѡ��λ�ò�������ֵ�Ľڵ� ��ע�����λ��Ϊloc+1 
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
// ��ȡtxt�ĵ��е����ݣ�������������
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
// ��������������ݰ���˳��д��txt�ĵ�
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
// ��ת����
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
//�����������
void insertSort(struct node** pheadptr)
{
	//һ��ʼû������������Ϊ�� 
    struct node* sortedList = NULL;
    struct node* current = *pheadptr;
    while (current != NULL) {
    	//��¼��һ���ڵ��λ�ã���ֹ�Ͽ����Ҳ������ 
        struct node* nextNode = current->next;
        // ���������������ҵ�����λ��
        // �����ǰ��û���������� ���� Ŀǰ�ڵ�ֵС���������нڵ����Сֵ �������������һ���ڵ��ֵ�� 
        if (sortedList == NULL || current->data < sortedList->data) {
        	//��Ŀǰ�Ľڵ�ָ��ԭ����������ĵ�һ���ڵ㣬���ҽ�����������С�ڵ� �滻Ϊ ��ǰ�Ľڵ� 
            current->next = sortedList;
            sortedList = current;
        }
        else { //�����ǰ�Ľڵ�ֵ������������ĵ�һ���ڵ�ֵС 
            struct node* p = sortedList;
            //�������������ҵ�Ӧ�ò����λ�� 
            while (p->next != NULL && p->next->data < current->data) {
                p = p->next;
            }
            current->next = p->next;
            p->next = current;
        }
        current = nextNode;
    }
    //ͷ�ڵ��滻Ϊ��������ĵ�һ���ڵ� 
    *pheadptr = sortedList;
}

//����ð������
//����ͷ�ڵ�ĵ�ַ�ı����ĵ�ַ������ı�ͷ�ڵ��ַ��ָ���ͷ�ڵ��λ�� 
//����ͷ�ڵ�ĵ�ַΪ60010����60010��һ����������ô���ĵ�ַ������24000
//����h = head-refʱ��ʵ��������h�ĵ�ַ�ı����ĵ�ַ���� 60010����*h��*(60010)ָ��ľ���ͷ�ڵ�ĵ�ַ
//������ֻ�Ǹ�ֵ��ͷ�ڵ�ĵ�ַ�ı����ĵ�ַû�иı䣬��ô��ָ���ͷ�ڵ�Ҳ����ı� 
void bubbleSort(struct node **head_ref) 
{
    //ָ��ͷ�ڵ�
    struct node **h;
    int i, j, swapped;
 
    for (i = 0; i <= countOfNodes(*head_ref); i++) 
    {
        h = head_ref;
        swapped = 0;//����Ƿ񽻻��� 
 
        for (j = 0; j < countOfNodes(*head_ref) - i - 1; j++) 
        {
        	//*hΪ����ʱ��ǰһλ 
            struct node *p1 = *h;
            struct node *p2 = p1->next;
 
            if (p1->data > p2->data) 
            {
                /* ���� */
                p1->next = p2->next;
                p2->next = p1;
                *h = p2;//ָ���С�ڵ� ,����Ϊ��ǰ�ڵ� 
 
                swapped = 1;
            }
 			//��ǰ�ڵ������һλ 
            h = &(*h)->next;
        }
 
        /* �����������ѭ�� */
        if (swapped == 0)
            break;
    }
}



// ����ѡ������
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
        // ����
        struct node *temp = *min;
        //��������ɾ��ԭ����min�ڵ� 
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
