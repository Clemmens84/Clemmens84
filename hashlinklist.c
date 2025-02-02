#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<math.h>
#include<pthread.h>
#include<semaphore.h>

#define DIRNUM 15
#define MAXNAME 255
#define DELETED_NODE (LIST *) (0xFFFFFFFFFFFFFFFUL)

//Open addressing VS external chaining (linked lists at hash id site)
//Open addressing seeks other open spots in array when hashes clash (clashing should be avoided, but depends on hash function)
//open addressing can do double hash or, just look in nearby spots (linear probing)
//and also keeps track of deleted nodes so it doesnt have to search everywhere
//
//External chaining makes each spot a linked list, so identical hashes go to same list

typedef struct
{
    char name[MAXNAME];
    unsigned int age;
    struct list * next;

}LIST;

LIST * directory[DIRNUM]; //pointers vs struct saves space. easy to set to NULL for empty location.

unsigned int hashingFunc(char *);
void initHashTable();
void printHashTable();
void insertEntry(LIST *);
void * interRupt(void *, void *);
LIST *findPerson(char *);
LIST *deletePerson(char *);

int huihull;
char* oi;

int main(int argc, char *argv[])
{
    pthread_t interrupt;
    /*thread didnt work. pthread_create(&interrupt, NULL, &interRupt, NULL);
    */
    int j = 0;

    for (j=1; j<=150; j++)
    {
        printf("~~%d-- ", ((j%10)));
        if (j%10 == 0)
            printf("\n");
    }
    printf("\n");
    initHashTable();
    printHashTable();
    printf("Helen's hash value is %u\n", hashingFunc("Helen"));
    printf("Joe's hash value is %u\n", hashingFunc("Joe"));
    printf("David's hash value is %u\n", hashingFunc("David"));
    printf("Laura's hash value is %u\n", hashingFunc("Laura"));
    printf("Stacy's hash value is %u\n", hashingFunc("Stacy"));

    LIST greg={"Gregorich", NULL}; //wrong to use pointer?//works as STRUCT
    printf("Gregorich's hash value is %u\n", hashingFunc(greg.name));//does NOT WORK with POINTER

     //ignored and DID NOT WORK with pointer//works with struct

    LIST jaime={.name="Jaime",.age=55,  .next=NULL};//other way to write struct
    LIST patrick={"Patrick", 35, NULL};
    LIST terrence={"Terrence",22, NULL};
    LIST amy={"Amy",43, NULL};
    LIST matt={"Matt",16, NULL};
    LIST upsha={"Upsha",74, NULL};
    insertEntry(&greg);
    insertEntry(&jaime);
    insertEntry(&patrick);
    insertEntry(&terrence);
    insertEntry(&amy);
    insertEntry(&matt);
    insertEntry(&upsha);

    //got duplicate hash values SO TWO 2 OPTIONS.
    //open address vs external chaining

    printHashTable();
    char * lookup="Haley";
    LIST *tmp=findPerson(lookup);
    if (tmp==NULL)
            printf("%s not found.\n", lookup);
    else
        printf("Found %s in the array.", tmp->name);
    tmp=findPerson("Jaime");
    if (tmp==NULL)
            printf("Not found.\n");
    else
        printf("Found %s in the array.\n", tmp->name);

    deletePerson("Matt");
    printHashTable();

    char * userdelete, userfind;
    printf("Person to find?\n");
    scanf("%s", &userfind);
    if(findPerson(&userfind)==NULL)
    {
        printf("Not found.\n");
    }//this does not seem to work
    else
        printf("Found.\n");
    printf("Person to delete?\n");
    scanf("%s", &userdelete);
    printf("Passed scanf.");
    deletePerson(&userdelete); //this does not work it seems
    printHashTable();
    //ABOVE USER DELETE DOES NOT WORK. WEIRD bEHAVIOR. UPSHA is replaced by last few letters in deleted person.
    pthread_join(interrupt, NULL);
    return 0;
}

void * interRupt(void *a, void *b)  //thread function for added fun
{
    int i=0;
    for (i=0;i<5;i++)
        printf("Hello from thread 'interrupt'.\n");

    pthread_exit(NULL);
    return;
}
void initHashTable()
{
    int i;
    for (i=0;i<DIRNUM; i++)
        directory[i]=NULL;

    return;
}

void printHashTable()
{
    int i;
    printf("===========================================\n");
    for (i=0;i<DIRNUM;i++)
    {
        if (directory[i]==NULL)
        {
            printf("\t%i\t...\n", i);
        }
        else
        {
            printf("\t%i\t", i);
            LIST * tmp=directory[i];
            while (tmp!=NULL)
            {
                printf("%s - %d - ", tmp->name, tmp->age);
                tmp=tmp->next;
            }
            printf("\n");
        }
    }
    printf("===========================================\n");

    return;
}

void insertEntryOrig(LIST * p)
{
    if (p==NULL)
        return NULL;
    int index;
    index=hashingFunc(p->name);
    if (directory[index]!=NULL)   //no entry if hashing function index taken
        return NULL;
    directory[index]=p;

    return;
}

void insertEntry(LIST * p)
{
    int i;
    int index;
    LIST * tmp;
    if (p==NULL)
        return NULL;
    index=hashingFunc(p->name);
    p->next=directory[index];
    directory[index]=p;
  /***alex solution; add name to end of list
    if (directory[index]==NULL)
    {
            directory[index]=p;
            return;
    }
    else
    {
        tmp=directory[index];
        while (tmp->next!=NULL)
            tmp=tmp->next;
        tmp->next=p;
    }
    *** other solution; add to beginning*/

    return;
}

unsigned int hashingFunc(char * s)
{
    int hashValue=0;
    int strleng =strlen(s);
    int i=0;
    for (i=0;i<strleng;i++)
    {
        hashValue +=s[i];//basic hash adding all char values
        hashValue = (hashValue * s[i])%DIRNUM;//make values more spread, and HUGE-not good //%DIRNUM ensures values in table array
    }
    return hashValue;
}

LIST *findPerson(char * nametofind) //WORKS! as seen in main
{
    int index=hashingFunc(nametofind);
    int i;
    LIST * tmp=directory[index];
    while (tmp!=NULL && (strcmp(tmp->name, nametofind))!=0)
        tmp=tmp->next;
    return tmp;

}




LIST *deletePerson(char *nametodelete)
{
    int index=hashingFunc(nametodelete);
    int i;
    LIST * pointdir=directory[index];
    LIST * previous=NULL;//to save previous spot and link up after deletion
    //doubly linked list requires more effort
    while ((pointdir!=NULL) &&
        (strcmp(pointdir->name, nametodelete))!=0)
    {
        previous=pointdir;
        pointdir=pointdir->next;
    }
    if (pointdir==NULL)
    {
        printf("%s not found. No deletions made.", nametodelete);
        return NULL;
    }
    if (previous==NULL)//head of list case
        directory[index]=pointdir->next;
    else
        previous->next=pointdir->next;
    return pointdir;
}

