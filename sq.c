#include <stdio.h>
#include <stdlib.h>
#include "sq.h"
#define FORMAT " %i "

typedef struct node{
    int val;
    struct node *next;
    struct node *prev;
    int inQueue;
} NODE;

typedef struct list_struct{
    NODE *front;
    NODE *back;
    int size;
}LIST;

typedef struct list_vector{
    NODE ** the_array;
    int size;
    int cap;
}VEC;

typedef struct service_queue{
    LIST * the_queue;
    LIST * buzzer_bucket;
    VEC * the_vector;
}SQ;

LIST *lst_create(){
    LIST *l = malloc(sizeof(LIST));
    l->front = NULL;
    l->back = NULL;
    return l;
}

void lst_pop_front(LIST *l){
    if (l->size == 1){
        l->front = NULL;
        l->back = NULL;
        l->size--;
    }else if (l->size > 1){
        NODE *popped = l->front;
        NODE *successor = l->front->next;
        popped->next = NULL;
        popped->prev = NULL;
        successor->prev = NULL;
        l->front = successor;
        l->size--;
    }
}

void lst_push_front(LIST *l, NODE *node){
    if (l->size == 0){
        node->next = NULL;
        node->prev = NULL;
        l->front = node;
        l->back = node;
        //l->size++;
    }else{
        l->front->prev = node;
        node->next = l->front;
        node->prev = NULL;
        l->front = node;
        //l->size++;
    }
}

void lst_push_back(LIST *l, NODE *buzzer){
    if (l->size == 0){
        l->front = buzzer;
        l->back = buzzer;
        l->front->next = NULL;
        l->front->prev = NULL;
        l->size++;
    }else{
        buzzer->next = NULL;
        buzzer->prev = l->back;
        l->back->next = buzzer;
        l->back = buzzer;
        l->size++;
    }
}

void grow_vector(SQ *q){
    int oldCap = q->the_vector->cap;
    int newCap = oldCap * 2;
    NODE **grownArray = (NODE**)malloc(newCap * sizeof(NODE)); 
    for (int i = 0; i < oldCap; i++){
        grownArray[i] = q->the_vector->the_array[i];
    }
    free(q->the_vector->the_array);
    q->the_vector->the_array = grownArray;
    q->the_vector->cap = newCap;
}

NODE *create_buzzer(SQ *q){
    if(q->the_vector->size == q->the_vector->cap){
        grow_vector(q);
    }
    NODE *buzzer = malloc(sizeof(NODE));
    buzzer->next = NULL;
    buzzer->prev = NULL;
    buzzer->val = q->the_vector->size;
    buzzer->inQueue = 0;
    q->the_vector->size++;
    q->the_vector->the_array[buzzer->val] = buzzer;
    return buzzer;
}

SQ * sq_create(){
    SQ *q = malloc(sizeof(SQ));
    q->the_queue = lst_create();
    q->buzzer_bucket = lst_create();
    q->the_queue->size = 0;
    q->buzzer_bucket->size = 0;
    q->the_vector = malloc(sizeof(VEC));
    q->the_vector->the_array = (NODE**)malloc(4 * sizeof(NODE));
    q->the_vector->size = 0;
    q->the_vector->cap = 4;
    return q;
}

void lst_free(LIST *l) {
    NODE *p = l->front;
    NODE *cur;
    while(p != NULL) {
        cur = p->next;
        free(p);
        p = cur;
    }
    free(l);
}

void lst_print(LIST *l) {
    NODE *p = l->front;
    printf("[");
    while(p != NULL) {
        printf(FORMAT, p->val);
        p = p->next;
    }
    printf("]\n");
}

void sq_free(SQ *q){
    lst_free(q->the_queue);
    lst_free(q->buzzer_bucket);
    free(q->the_vector->the_array);
    q->the_vector->size = 0;
    free(q->the_vector);
    free(q);
}

void sq_display(SQ *q){
    printf("current-queue contents:\n    ");
    lst_print(q->the_queue);
    printf("\n");
}

int sq_length(SQ *q){
    return q->the_queue->size;
}

int sq_give_buzzer(SQ *q){
    NODE *buzzer;
    if(q->buzzer_bucket->size != 0){
        buzzer = q->buzzer_bucket->front;
        lst_pop_front(q->buzzer_bucket);
        lst_push_back(q->the_queue, buzzer);
        //q->buzzer_bucket->size--;
        buzzer->inQueue = 1;
        return buzzer->val;
    }else{
        buzzer = create_buzzer(q);
        lst_push_back(q->the_queue, buzzer);
        buzzer->inQueue = 1;
        return buzzer->val;
    }
}

int sq_seat(SQ *q){
    NODE *buzzer;
    if(q->the_queue->size == 0){
        return -1;
    }else{
        buzzer = q->the_queue->front;
        lst_pop_front(q->the_queue);
        lst_push_front(q->buzzer_bucket, buzzer);
        buzzer->inQueue = 0;
        q->buzzer_bucket->size++;
        //q->the_queue->size--;
        return buzzer->val;
    }
}

int sq_kick_out(SQ *q, int buzzer){
    NODE *buzzerNode = q->the_vector->the_array[buzzer];
    if (buzzerNode->inQueue == 1){
        if (buzzerNode->prev == NULL){
            lst_pop_front(q->the_queue);
        }else if (buzzerNode->next == NULL){
            buzzerNode->prev->next = NULL;
            q->the_queue->back = buzzerNode->prev;
            buzzerNode->prev = NULL;
            q->the_queue->size--;
        }else{
            buzzerNode->prev->next = buzzerNode->next;
            buzzerNode->next->prev = buzzerNode->prev;
            buzzerNode->next = NULL;
            buzzerNode->prev = NULL;
            q->the_queue->size--;
        }
        
        lst_push_front(q->buzzer_bucket, buzzerNode);
        q->buzzer_bucket->size++;
        buzzerNode->inQueue = 0;
        return 1;
    }else{
        //buzzer is not in the service queue
        return 0;
    }
}

int sq_take_bribe(SQ *q, int buzzer){
    if (buzzer > q->the_vector->size){
        return 0;
    }
    NODE *buzzerNode = q->the_vector->the_array[buzzer];
    if (buzzerNode->inQueue == 1){
        if (buzzerNode->prev == NULL){
            //the buzzer is already at the front
            return 1;
        }else if (buzzerNode->next == NULL){
            buzzerNode->prev->next = NULL;
            q->the_queue->back = buzzerNode->prev;
            buzzerNode->prev = NULL;
        }else{
            buzzerNode->prev->next = buzzerNode->next;
            buzzerNode->next->prev = buzzerNode->prev;
            buzzerNode->next = NULL;
            buzzerNode->prev = NULL;
        }
        lst_push_front(q->the_queue, buzzerNode);
        return 1;
    }else{
        //buzzer is not in the service queue
        return 0;
    }
}