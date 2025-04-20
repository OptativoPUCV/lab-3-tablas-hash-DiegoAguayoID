#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value) {
    long index = hash(key, map -> capacity) ;

    for (long i = 0 ; i < map -> capacity ; i++) {
        long pos = (index + i) % map -> capacity ;
        Pair * inst = map -> buckets[pos] ;

        if (inst == NULL || inst -> key == NULL) {
            map -> buckets[pos] = createPair(key, value) ;
            map -> size ++ ;
            map -> current = pos ;
        }

        if (inst -> key != NULL && strcmp(inst -> key, key) == 0) {
            return ;
        }
    }
}

void enlarge(HashMap * map) {
    enlarge_called = 1; //no borrar (testing purposes)
    
    Pair ** prevArr = map -> buckets ;
    long prevCap = map -> capacity ;

    map -> capacity *= 2 ;
    map -> buckets = (Pair **) malloc (sizeof(Pair *) * map -> capacity) ;
    for (long i = 0 ; i < map -> capacity ; i++) {
        map -> buckets[i] = NULL ;
    }

    map -> size = 0 ;
    for (long i = 0 ; i < prevCap ; i++) {
        Pair *pair = prevArr[i] ;
        if (pair != NULL && pair -> key != NULL) {
            insertMap(map, pair -> key, pair -> value) ;
        }
    }

    free(prevArr) ;
}


HashMap * createMap(long capacity) {
    HashMap *mapa = (HashMap *) malloc (sizeof(HashMap)) ;
    if (mapa == NULL) return NULL ;

    mapa -> buckets = (Pair **) malloc (sizeof(Pair *) * capacity) ;
    if (mapa -> buckets == NULL) {
        free(mapa) ;
        return NULL ;
    }

    for (long i = 0 ; i < capacity ; i++) {
        mapa -> buckets[i] = NULL ;
    }

    mapa -> size = 0 ;
    mapa -> capacity = capacity ;
    mapa -> current = -1 ;

    return mapa ;
}

void eraseMap(HashMap * map,  char * key) {    
    Pair *pairBusc = searchMap(map, key) ;

    if (pairBusc != NULL) {
        pairBusc -> key = NULL ;
        map -> size -- ;
    }

}

Pair * searchMap(HashMap * map,  char * key) {   
    long index = hash(key, map -> capacity) ;

    for (long i = 0 ; i < map -> capacity ; i++) {
        long pos = (index + i) % map -> capacity ;
        Pair * pair = map -> buckets [pos] ;

        if (pair == NULL) {
            return NULL ;
        }

        if (pair -> key != NULL && strcmp(pair -> key, key) == 0) {
            map -> current = pos ;
            return pair ;
        }
    }

    return NULL ;
}

Pair * firstMap(HashMap * map) {
    for (long i = 0 ; i < map -> capacity ; i++) {
        Pair *pair = map -> buckets[i] ;
        if (pair != NULL && pair -> key != NULL) {
            map -> current = i ;
            return pair ;
        }
    }

    return NULL ;
}

Pair * nextMap(HashMap * map) {
    for (long i = map -> current + 1 ; i < map -> capacity ; i++) {
        Pair *pair = map -> buckets[i] ;
        if (pair != NULL && pair -> key != NULL) {
            map -> current = i ;
            return pair ;
        }
    }

    return NULL;
}
