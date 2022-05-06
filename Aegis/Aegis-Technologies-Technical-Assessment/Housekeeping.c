
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

#define MAX_CACHE_SIZE 5
#define MAX_CACHE_ENTRY_SIZE 100
#define MAX_KEY_SIZE 10
#define MAX_VALUE_SIZE 100
#define MAX_NUM_ENTRIES 10
#define EXPIRY_TIME 10

struct cache_entry{
	char key[MAX_KEY_SIZE];
	char value[MAX_VALUE_SIZE];
	time_t last_accessed_time;
};

struct cache_entry cache[MAX_NUM_ENTRIES];

pthread_mutex_t mutex;

void *add_to_cache(void *);
void *remove_from_cache(void *);
void *check_cache(void *);
void *print_cache(void *);
void *remove_expired_entries(void *);

int main(){
	pthread_t add_thread, remove_thread, check_thread, print_thread, remove_expired_thread;
	int add_thread_id, remove_thread_id, check_thread_id, print_thread_id, remove_expired_thread_id;
	
	int i;
	for(i=0;i<MAX_NUM_ENTRIES;i++){
		cache[i].key[0] = '\0';
		cache[i].value[0] = '\0';
		cache[i].last_accessed_time = 0;
	}
	
	pthread_mutex_init(&mutex, NULL);
	
	add_thread_id = pthread_create(&add_thread, NULL, add_to_cache, NULL);
	remove_thread_id = pthread_create(&remove_thread, NULL, remove_from_cache, NULL);
	check_thread_id = pthread_create(&check_thread, NULL, check_cache, NULL);
	print_thread