#include <stdio.h>
#include <string.h>
#include<stdlib.h>


typedef struct ruleList
{
	char lhs;
	char rhs[10];
	int num;
	struct ruleList *nextRule;
}Rule;


typedef struct firstAndFollow{
    char lhs;
    char set[10];
    int len;
    struct firstAndFollow *next;
}FirstAndFollow;


typedef struct varList{
    char element;
    struct varList *next;
}VarList;


void printList(Rule *head){
    Rule *curr = head;
    while (curr != NULL){
        printf("(%d): %c -> %s\n",curr->num,curr->lhs,curr->rhs);
        curr = curr->nextRule;
    }
}

void printVarList(VarList *head){
    VarList *curr = head;
    while (curr != NULL){
        printf("%c ",curr->element);
        curr = curr->next;
    }
    printf("\n");
}


void printFirstAndFollowList(FirstAndFollow *head){
    FirstAndFollow *curr = head;
    int length;
    printf("\n");
    while (curr != NULL){
        printf("%c : { ",curr->lhs);
        length = curr->len;
        for(int i=0;i<length;i++){
            printf("%c",curr->set[i]);
            if(i+1<length){
                printf(",");
            }
        }
        printf(" } length: %d\n",length);
        curr = curr->next;
    }
}

int checkInList(VarList *head,char element){
    VarList *curr = head;
    while (curr != NULL){
        if (curr->element == element){
            return 1;
        }
        curr = curr->next;
    }
    return 0;
}

void pushRule(Rule **node,char LHS,char RHS[10],int N){

    Rule *curr = *node;
    Rule *newnode = (Rule *)malloc(sizeof(Rule));
    newnode->lhs = LHS;
    strcpy(newnode->rhs,RHS);
    newnode->num = N;
    newnode->nextRule = NULL;

    if (curr != NULL){
        while(curr->nextRule!=NULL){
            curr = curr->nextRule;
        }
        curr->nextRule = newnode;
    }
    else{
        *node = newnode;
    }
}



void appendToList(VarList **node,char NT){

    VarList *curr = *node;
    VarList *newnode = (VarList *)malloc(sizeof(VarList));
    newnode->element = NT;
    newnode->next = NULL;
    if (curr != NULL){
        while(curr->next!=NULL){
            if(curr->element == NT){
            return;
            }
            curr = curr->next;
        }
        if(curr->element == NT){
            return;
        }
        curr->next = newnode;
    }
    else{
        *node = newnode;
    }
}



void createNonTermList(Rule *head,VarList **node){

    Rule *curr = head;

    while(curr!=NULL){
        appendToList(node,curr->lhs);
        curr = curr->nextRule;
    }
}





void createTermList(Rule *head,VarList *nonTerms,VarList **node){

    Rule *curr = head;
    char temp[10];
    while(curr!=NULL){
        strcpy(temp,curr->rhs);
        for(int i=0;i<strlen(temp);i++){
            if(!checkInList(nonTerms,temp[i])){
                    appendToList(node,temp[i]);
            }
        }
        curr = curr->nextRule;
    }
}


void copyFirst(Rule *node,FirstAndFollow **head,VarList *nonTerms,char LHS,char current){

    Rule *curr = node;
    if(!checkInList(nonTerms,current)){
        pushFirstAndFollow(head,LHS,current);
    }
    else{
        while(curr!=NULL){
            if(curr->lhs == current){
                copyFirst(node,head,nonTerms,LHS,curr->rhs[0]);
            }
            curr = curr->nextRule;
        }
    }
}



void createFirstSet(Rule *node,FirstAndFollow **head,VarList *nonTerms){

    Rule *curr = node;
    while (curr != NULL){
        copyFirst(node,head,nonTerms,curr->lhs,curr->rhs[0]);
        curr = curr->nextRule;
    }
}
void pushFirstAndFollow(FirstAndFollow **node,char LHS,char element){

    FirstAndFollow *curr = *node;
    FirstAndFollow *newnode = (FirstAndFollow *)malloc(sizeof(FirstAndFollow));
    newnode->lhs = LHS;
    newnode->set[0]=element;
    newnode->len = 1;
    newnode->next = NULL;
    if (curr != NULL){
        while(curr->next!=NULL){
            if(curr->lhs == LHS){
                for(int i=0;i<curr->len;i++){
                    if(curr->set[i]==element){
                        return;
                    }
                }
                curr->set[curr->len] = element;
                curr->len = curr->len+1;
                return;
            }
            curr = curr->next;
        }
        if(curr->lhs == LHS){
            for(int i=0;i<curr->len;i++){
                if(curr->set[i]==element){
                    return;
                }
            }
            curr->set[curr->len] = element;
            curr->len = curr->len+1;
            return;
        }
        else{
            curr->next = newnode;
        }

    }
    else{
        *node = newnode;
    }
}



FirstAndFollow *getFirstAndFollow(FirstAndFollow *node,char element){
    FirstAndFollow *curr = node;
    while(curr!=NULL){
        if(curr->lhs == element){
            return curr;
        }
        curr = curr->next;
    }
}
void copyFollow(Rule *rule,Rule *curr,FirstAndFollow *first,VarList *Terms,FirstAndFollow **node,char LHS,int i){
    FirstAndFollow *gotFirst;
    FirstAndFollow *gotFollow;
    int len = strlen(curr->rhs);
    if(i+1<len){
        // printf("next: %c,",curr->rhs[i+1]);
        checkAddFollow(rule,first,Terms,node,LHS,curr->rhs[i+1]);
    }
    else{
        //printf("GETTINGCOPYFOLLOW");
        // printf("\nGetting FOllow: %c,",curr->lhs);
        gotFollow = getFirstAndFollow(*node,curr->lhs);
        for(int k=0;k<gotFollow->len;k++){
            //  printf("#%c%c,",LHS,gotFollow->set[k]);
            pushFirstAndFollow(node,LHS,gotFollow->set[k]);
        }
                   /* printf("\nPrintingfollow: ");
                    printFirstAndFo*/
    }


}
void checkAddFollow(Rule *rule,FirstAndFollow *first,VarList *Terms,FirstAndFollow **node,char LHS,char element){

    Rule *curr = rule;
    FirstAndFollow *gotFirst;
    FirstAndFollow *gotFollow;
    int len,i=0;
    len = strlen(curr->rhs);
    if(checkInList(Terms,element)){
      //  printf("element: %c,\n",element);
        pushFirstAndFollow(node,LHS,element);
    }
    else{
        for(i=0;i<len;i++){
            if(curr->rhs[i]==element){
                break;
            }
        }
       // printf("E: %c%c#",LHS,element);
        gotFirst = getFirstAndFollow(first,element);
        for(int j=0;j<gotFirst->len;j++){
            if(gotFirst->set[j]!='e'){
                //printf("gF: %c-\n",gotFirst->set[j]);
                pushFirstAndFollow(node,LHS,gotFirst->set[j]);
               // printFirstAndFollowList(*node);
            }
            else{
                copyFollow(rule,curr,first,Terms,node,LHS,i);
            }
        }
    }
}

void addFollow(Rule *rule,FirstAndFollow *first,VarList *Terms,FirstAndFollow **node,char LHS){

    Rule *curr = rule;
    FirstAndFollow *gotFirst;
    FirstAndFollow *gotFollow;
    if(curr->lhs == LHS && curr->num==1){
            pushFirstAndFollow(node,LHS,'$');
    }
    int len;
    while(curr!=NULL){
        len = strlen(curr->rhs);
        for(int i=0;i<len;i++){
            if(curr->rhs[i] == LHS){
                //printf("LHS: %c\n",LHS);
                if(i+1<len){
                    //printf("%d-%c%c ",curr->num,LHS,curr->rhs[i+1]);
                    checkAddFollow(curr,first,Terms,node,LHS,curr->rhs[i+1]);
                }
                else{
                    copyFollow(rule,curr,first,Terms,node,LHS,i);
                }
            }
        }
        curr=curr->nextRule;
    }



}

void createFollowSet(Rule *rule,FirstAndFollow *first,VarList *Terms,FirstAndFollow **node){

    Rule *curr = rule;

    while(curr!=NULL){
        addFollow(rule,first,Terms,node,curr->lhs);
        curr = curr->nextRule;
    }

}



int main(){

    Rule *head = NULL;
    pushRule(&head,'E',"TD",1);
    pushRule(&head,'D',"+TD",2);
    pushRule(&head,'D',"e",3);
    pushRule(&head,'T',"FU",4);
    pushRule(&head,'U',"*FU",5);
    pushRule(&head,'U',"e",6);
    pushRule(&head,'F',"(E)",7);
    pushRule(&head,'F',"i",8);

    printf("The Given Grammar is: \n");
    printList(head);

    VarList *non_terminals = NULL;

    createNonTermList(head,&non_terminals);
    printf("\nNon Terminals are: ");
    printVarList(non_terminals);

    VarList *terminals = NULL;

    createTermList(head,non_terminals,&terminals);
    printf("\nTerminals are: ");
    printVarList(terminals);

    printf("\nThe First is: \n");
    FirstAndFollow *first_list = NULL;
    createFirstSet(head,&first_list,non_terminals);
    printFirstAndFollowList(first_list);

    printf("\nThe Follow is: ");
    FirstAndFollow *follow_list = NULL;
    createFollowSet(head,first_list,terminals,&follow_list);
    printFirstAndFollowList(follow_list);











return 0;
}
