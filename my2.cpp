#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void InsertStudent(char*** students,int*** marks);
void DeleteStudent(char*** students,int*** marks);
void SortClass(char** students,int** marks);
void ShowClass(char** students,int** marks);
void EditMarks(char** students,int** marks);
char* ReadLine();
#define merror(a) {printf("memory allocation error %d\n",a);exit(1);}

/* function main
----------------------------------------------- */
int main()
{

    char** students = NULL;
    int** marks = NULL;
    char line[100];
    int menu;

    while(1)
    {
        printf("Enter (1) to Add a Student to Class List\n"
               "      (2) to Delete a Student from Class List\n"
               "      (3) to show Class List\n"
               "      (4) to Edit marks of a Student\n"
               "      (5) to Quit\n");
        fflush(stdout);
        gets(line);
        if (sscanf(line,"%d",&menu) != 1)
        {
            printf("incorrect entry");
            continue;
        }
        if (menu < 1 || menu > 5)
        {
            printf("incorrect selection");
            continue;
        }

        if (menu == 1)
            InsertStudent(&students,&marks);
        else if (menu == 2)
            DeleteStudent(&students,&marks);
        else if (menu == 3)
            ShowClass(students,marks);
        else if (menu == 4)
            EditMarks(students,marks);
        else
            break;
    }/*endwhile*/

    return 0;

}/*end main*/


/* function InsertStudent
------------------------------------ */
/*This function prompts the user to enter a new student name.
The precise form of the prompt is
  Enter Student Name to be added to Class List:
Then this function reads the name from standard input.
But it must be able to read any name of any size!! Thus you have
to allocate memory (using malloc()) to read the input into, and if
it is not big enough, must use realloc() to extend it. You are not
allowed to allocate more than 10 bytes in one allocation or
extend your segment by more than 10 bytes in one call to realloc().
Do not forget to deallocate the memory before exiting the function!

Once the function has the name to be entered, it traverses the array student
to find out if the name is already there. If so, the function displays
a warning message "student xxx already in the class list" and terminates (
where xxx is the name of the student). If the name is not in the array, the
array students is extended by one item (using realloc()) and a copy
of the string with the name is "placed" there. Also the array marks is
extended by one row (using realloc()) and all five marks in that row are set to
 -1. Then updated arrays students and marks are passed to SortClass()
so they can be sorted alphabetically, as you are required to keep the class
list and the list of marks in alphabetical order.

Note that both students and marks are passed to this function "by reference",
for this function on occasions must be able to modify the value stored in the
pointer student (in main()) - when putting there the very first student or when
realloc() possibly "moves" the array in the memory somewhere else. The same applies
to marks.*/
void InsertStudent(char*** students,int*** marks)
{
    char* buf;
    int found, i, j, size;
    char** students1;
    int** marks1;
    students1 = *students;
    marks1 = *marks;

    char *name;
    printf("Enter Student Name to be added to Class List:\n");
    fflush(stdout);
    name = ReadLine();

    if (students1 == NULL)
    {
        if ((students1 = (char**) malloc(2*sizeof(char*))) == NULL)merror(2);
        if ((students1[0] = (char*) malloc(strlen(name)+1)) == NULL)merror(3);
        strcpy(students1[0],name);

        students1[1] = NULL;
        if ((marks1 = (int**) malloc(2*sizeof(int*))) == NULL)merror(4);
        if ((marks1[0] = (int*) malloc(5*sizeof(int))) == NULL)merror(5);
        marks1[0][0]=marks1[0][1]=marks1[0][2]=marks1[0][3]=marks1[0][4]=-1;
        marks1[1] = NULL;

        *students = students1;
        *marks = marks1;
        return;
    }

    /* do we have the student yet ? */
    for(found = i = 0; students1[i] != NULL; i++)
    {
        if (strcmp(students1[i],name) == 0)
        {
            found = 1;
            break;
        }
    }

    if (found)
    {
        printf("student %s already in the class list\n",name);
        return;
    }


    /* so it is not in the class list yet */

    students1 = (char**) realloc((void*)students1,(i+2)*sizeof(char*));
    if (students1 == NULL)
        merror(6);
    if ((students1[i] = (char*) malloc(strlen(name)+1)) == NULL)
        merror(7);
    strcpy(students1[i],name);

    students1[i+1] = NULL;

    free((void*) name);


    if ((marks1 = (int**) realloc((void*)marks1,sizeof(int*)*(i+2))) == NULL)
        merror(8);
    if ((marks1[i] = (int*) malloc(5*sizeof(int))) == NULL)
        merror(9);
    marks1[i][0]=marks1[i][1]=marks1[i][2]=marks1[i][3]=marks1[i][4]=-1;
    marks1[i+1] = NULL;

    SortClass(students1,marks1);

    *students = students1;
    *marks = marks1;
}




/* function DeleteStudent
------------------------------------ */

/*This function prompts the user to enter a student name
to be deleted.The precise form of the prompt is

  Enter Student Name to be deleted from Class List:
Then this function reads the name from standard input.
But it must be able to read any name of any size!! Thus you have
to allocate memory (using malloc()) to read the input into, and if
it is not big enough, must use realloc() to extend it. You are not
allowed to allocate more than 10 bytes in one allocation or extend
your segment by more than 10 bytes in one call to realloc().
Do not forget to deallocate the memory before exiting the function!
Note that this part of the code is the same as in InsertStudent(),
so you can either copy it or write a function that would do it for both.

Once the function has the name to be deleted, it traverses the array
student to find out if it is there. If not, a warning message is
displayed "student xxx not in the class list" and the function terminates.
If it is found (say its is students[d]), a dynamic one-dimensional array one
item shorter than students is created and the strings from the array students
are "unhooked" from the arrays students and "hooked" to the new array with the
exception of the string to be deleted (don't forget that
the "deleted" string must be deallocated). Now deallocate the array students
and make the pointer student point to the new array. As you can see we have
the "same" array of string as before, but with one string deleted. You must
do the same with the array marks, for you must delete completely d-th row
(so it did correspond to student[d] which we deleted).

Note that both students and marks are passed to this function "by reference",
for this function on must be able to modify the value stored in the pointer
student (in main()) - when a student name is deleted. The same aplies to marks.*/

void DeleteStudent(char*** students,int*** marks)
{
    char* buf;
    int found, i, j, pos;
    char** students1;
    int** marks1;
    students1 = *students;
    marks1 = *marks;
    
    if(students1==NULL)
    {
    	printf("Class List is empty\n");
    	return;
	}
	char *name;
    printf("enter names of students to be deleted separated by commas:\n");
    fflush(stdout);
    name = ReadLine();
    
	for(found = i = 0; students1[i] != NULL; i++)
    {
        if (strcmp(students1[i],name) == 0)
        {
            found = 1;
            pos = i;
            break;
        }
    }
    if (!found)
    {
        printf("student %s not in the class list\n",name);
        return;
    }
    
    /* so it is in the class list yet */
	
    for(i = pos; students1[i] != NULL; i++)
    {
    	students1[i] = students1[i+1];
    	marks1[i] = marks1[i+1];
    }
    free(students1[i]);
	free(students1[i]);
}



/* function SortClass
--------------------------------------------------- */
/*In this function you sort the strings in the array students alphabetically
(lexico-graphically) using a simple bubble sort and the comparison function
for strings strcmp(). Do not forget that as you are changing the order of
strings in students, you must be changing the order of rows in the arry marks.
Note, that when sorting the array students, and when you want to switch
ith- string
with the (i+1)st-string, all you have to do is switch the pointers, you do not
have to touch the strings themselves.

Note that both students and marks are passed to this
function "by value", for this function does not need to modify the
value stored in the pointer student. The same applies to marks.*/

void SortClass(char** students,int** marks)
{
	int i, j, size;
    char** students1;
    char* tmp;
    int** marks1;
    int* tmp1;
    students1 = students;
    marks1 = marks;
    
	for(i = 0; students1 [i+1] !=NULL; i++)
    {
    	for(j = 0; students1 [j+1+i] !=NULL; j++)
    	{
    		if (strcmp(students1[j],students1[j+1]) > 0)
	        {
	        	tmp = students1[j];
				students1[j] = students1[j+1];
				students1[j+1] = tmp;
				tmp1=marks1[j];
				marks1[j]=marks1[j+1];
				marks1[j+1]=tmp1;
	        }
		}
        
    }
}



/* function ShowClass
------------------------------------------------- */
/*In this function you display on the screen each
student in the class and his/her 5 marks on a line.
If the mark is -1 display a space instead. After the
name of student display : and separate the marks by ,

Note that both students and marks are passed to this
function "by value", for this function does not need to modify the
value stored in the pointer student. The same applies to marks.*/
void ShowClass(char** students,int** marks)
{
    int i, j, size;
    char** students1;
    int** marks1;
    students1 = students;
    marks1 = marks;
    
	printf("Class List:\n");
	if(students1 == NULL) return;
	for(i = 0; students1[i] != NULL; i++)
    {
    	int k=0;
        printf("%s:",students1[i]);
        while(k<=4)
        {
        	if(k==0)
	        {
	        	if(marks[i][k]==-1)
	        	{
	        		printf(" ");
				}else{
					printf("%d",marks[i][k]);
				}
			}else{
				if(marks[i][k]==-1)
	        	{
	        		printf(", ");
				}else{
					printf(",%d",marks[i][k]);
				}
			}
			k++;
		}
		printf("\n");
    }
}



/* function EditMarks
----------------------------------------------- */
/*This function prompts the user to enter a student name
whose marks are to be edited. The precise form of the prompt
is  Enter Student Name whose marks are to be edited:

Then this function reads the name from standard input.
But it must be able to read any name of any size!! Thus you have
to allocate memory (using malloc()) to read the input into, and if
it is not big enough, must use realloc() to extend it. You are not
allowed to allocate more than 10 bytes in one allocation or extend
your segment by more than 10 bytes in one call to realloc().
Do not forget to deallocate the memory before exiting the function!
Note that this part of the code is the same as in InsertStudent()
and DeleteStudent(), so you can either copy it or write a function
that would do it for all three.

If the student is not found in the array students, a warning message
"student xxx not in the Class List" is displayed on the screen and
the function terminates. If it is found, the students name followed
by : and his/her five marks are displayed (again, -1 must be shown
as a space, as in ShowClass()). Let us assume that the student in
question has marks 10,-1,20,15,5

Then the function displays a prompt to the user for
the first mark  to leave the 1st mark 10 unchanged press<enter>
otherwise type the new mark and press<enter>  and reads the user's
reply using gets() into a character arrays of size 80. Then it uses
sscanf() on that array to "extract" the response. Then  the function
displays a prompt to the user for the second mark  to leave the 2nd mark
unchanged press<enter>otherwise type the new mark and press<enter>
and reads the user's reply using gets() into a character arrays of size 80.
Then it uses sscanf() on that array to "extract" the response. Etc.
for the third, fourth, and fifth.

Note that both students and marks are passed to thisfunction "by value",
for this function does not need to modify the value stored in the pointer student.
The same applies to marks.*/
void EditMarks(char** students,int** marks)
{

	char* buf;
    int found, i,j, pos;
    char** students1;
    int** marks1;
    char* mark;
    int mark1;
    students1 = students;
    marks1 = marks;
    if(students1 == NULL) 
	{
		printf("Class List is empty\n");
		return;
	}
	char *name;
    printf("Enter Student Name whose marks are to be edited:\n");
    fflush(stdout);
    name = ReadLine();
	
	for(found = i = 0; students1[i] != NULL; i++)
    {
        if (strcmp(students1[i],name) == 0)
        {
            found = 1;
            pos = i;
            break;
        }
    }
    if (!found)
    {
        printf("student %s not in the class list\n",name);
        return;
    }
    
    printf("editing %s:\n",students1[pos]);
    for(i = 0; i < 5 ; i++)
    {
    	printf("to leave the %d mark   unchanged, press <enter>,\n",i+1);
 		printf("otherwise type new mark and press <enter>\n");
 		fflush(stdout);
    	mark = ReadLine();
    	j=0;mark1=0;
    	if(mark[0] == NULL) continue;
    	while(mark[j])
    	{
    		mark1=mark1*10+int(mark[j]-'0'); 	
			j++;	
		}
		if(mark1>100) 
		{
			printf("incorrect mark entered, re-do\n");
			i--;
			continue;
		}
		marks1[pos][i]=mark1;
	}

}

char* ReadLine()
{
    char *buf;
    int i, j, size;
    if ((buf = (char*) malloc(10)) == NULL)
        merror(0);
    size = 10;
    j = -1;
    while(1)
    {
        for(i = 0; i < 10; i++)
        {
            buf[++j] = getchar();
            if (buf[j] == '\n')
            {
                buf[j] = '\0';
                break;
            }
        }/*endfor*/

        if (i == 10 && buf[j] != '\0')
        {
            size += 10;
            if ((buf = (char*) realloc((void*)buf,size)) == NULL)
                merror(1);
            continue;
        }
        break;
    }/*endwhile*/
    return buf;

}/*end ReadLine*/

