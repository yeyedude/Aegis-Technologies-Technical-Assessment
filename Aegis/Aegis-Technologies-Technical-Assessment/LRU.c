/*
Create a time based LRU,
cache entries will be added to the cache,
when the cache is full, the least recently used entry will be removed,
ensure cache will contain only maxiumum number of items,
ensure cache entry will expire when last accessed time is older than expiry time,
thread safe, can use hash library // not done yet
*/

#include <stdio.h>
#include <stdlib.h>
//#include <string.h>
#include <time.h>

#define MAX_ENTRIES 3
#define EXPIRY_TIME 15
//#define MAX_TIME 1000000

int cache_size = 0;
int first_flag = 0;
//unsigned int countdown = 0;

/******************************************************************************
Struct: cache
Description: cache struct
Variables:
    int           value;
    clock_t       last_accessed;
    clock_t       expiration_time;
    struct cache* next;
    struct cache* prev;
******************************************************************************/
struct cache{
    int           value;
    clock_t       last_accessed;
    clock_t       expiration_time;
    struct cache* next;
    struct cache* prev;
};

/******************************************************************************
Function: cache constructor
Inputs: struct cache* cache_entry, int value, clock_t last_accessed,
        struct cache* next, struct cache* prev
Outputs: void
Description: cache constructor
******************************************************************************/
cache(struct cache* cache_entry, int value, clock_t last_accessed, struct cache* next, struct cache* prev){
        cache_entry->value = value;
        cache_entry->last_accessed = last_accessed;
        cache_entry->expiration_time = last_accessed+EXPIRY_TIME;
        cache_entry->next = next;
        cache_entry->prev = prev;
}

/******************************************************************************
Function: add_to_cache
Inputs: struct cache* first
Outputs: struct cache*
Description: add new item to cache
******************************************************************************/
struct cache* add_to_cache(struct cache* first){
    struct cache* new_cache_entry = (struct cache*)malloc(sizeof(struct cache));
    struct cache* temp = first;
    if(cache_size >= MAX_ENTRIES){
        while(temp->next != NULL){
            temp = temp->next;
        }
        remove_oldest(temp);
    }
    cache_size++;
    new_cache_entry->value = cache_size;
    new_cache_entry->last_accessed = clock();
    new_cache_entry->expiration_time = new_cache_entry->last_accessed+EXPIRY_TIME;
    new_cache_entry->next = first;
    new_cache_entry->prev = NULL;
    first->prev = new_cache_entry;
    first = new_cache_entry;
    return first;
}

/******************************************************************************
Function: remove_oldest
Inputs: struct cache* last
Outputs: void
Description: remove oldest item in cache for space
******************************************************************************/
void remove_oldest(struct cache* last){
    last->prev->next = NULL;
    free(last);
    cache_size--;
}

/******************************************************************************
Function: free_cache
Inputs: struct cache* first
Outputs: void
Description: empty the cache
******************************************************************************/
void free_cache(struct cache* first){
    struct cache* last = find_last(first);
    struct cache* temp = last->prev;
    while(cache_size != 0){
        remove_oldest(last);
        last = temp;
        temp = last->prev;
    }
}

/******************************************************************************
Function: print_cache
Inputs: struct cache* first
Outputs: void
Description: prints the cache
******************************************************************************/
void print_cache(struct cache* first){
    struct cache* temp = first;
    while(temp != NULL){
        printf("%d\n", temp->value);
        temp = temp->next;
    }
}

/******************************************************************************
Function: find_last
Inputs: struct cache* first
Outputs: struct cache*
Description: find last pointer to the cache
******************************************************************************/
struct cache* find_last(struct cache* first){
    struct cache* temp = first;
    while(temp->next != NULL){
        temp = temp->next;
    }
    return temp;
}

/******************************************************************************
Function: check_expiry
Inputs: struct cache* first
Outputs: void
Description: check cache item expiration, if expired, delete it
******************************************************************************/
void check_expiry(struct cache* first){
    struct cache* temp = find_last(first);
    while(temp != NULL){
        if(temp->last_accessed > temp->expiration_time){
                remove_oldest(temp);
        }
        temp = temp->prev;
    }
}

/******************************************************************************
Function: get_cache
Inputs: struct cache* first, int value
Outputs: void
Description: get specific cache item according to value
******************************************************************************/
void get_cache(struct cache* first, int value){
    struct cache* temp = first;
    while(temp != NULL){
        if(temp->value == value){
            temp->last_accessed = clock();
            printf("%d\n", temp->value);
            return;
        }
        temp = temp->next;
    }
    if(temp == NULL){
        printf("404: Cache item not found.\n");
    }
}

/******************************************************************************
Function: main
Inputs: void
Outputs: void
Description: driver function
******************************************************************************/
int main(void){
    char* input = "";
    struct cache* first = (struct cache*)malloc(sizeof(struct cache));
    first->value = 1;
    first->last_accessed = clock();
    first->next = NULL;
    first->prev = NULL;
    cache_size = 1;
    first_flag = 1;
    while(1){
        // check if cache item is expired
        check_expiry(first);

        // ask user if add item to cache
        printf("Add to cache? (y/n)\n");
        scanf("%s", &input);
        if(input == "y"){
            //check if cache is empty
            if(first == NULL){
                first = (struct cache*)malloc(sizeof(struct cache));
                first->value = 1;
                first->last_accessed = clock();
                first->next = NULL;
                first->prev = NULL;
                cache_size = 1;
                first_flag = 1;
            }
            // check if first item to add to cache
            else if((cache_size == 1)&&(first_flag == 1)){
                first->last_accessed = clock();
                first_flag = 0;
            }
            // add item to cache
            else{
                first = add_to_cache(first);
            }
            input = "";
        }
        // see cache items
        printf("See cache? (y/n)\n");
        scanf("%s", &input);
        if(input == "y"){
            print_cache(first);
            input = "";
        }
        // get cache item
        printf("Get cache? (y/n)\n");
        scanf("%s", &input);
        if(input == "y"){
            printf("Enter value to get:  (1 to 3)\n");
            scanf("%d", &input);
            get_cache(first, input);
            input = "";
        }
    }
}