#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static int swap(int a, int b){
    int c = b;
    b = a;
    a = c;
}
static int reverse(*m, int m_size){
    for(int i=0; i < m_size / 2; i++){
        swap( m[i] , m[m_size - i - 1] );
    }
}

static int max (int a, int b){
    if( a >= b ){ return a; }
    return b;
}
static int int_to_char(const int number){
    const char m[10] = "0123456789";
    return m[number];
}

static int char_to_int(const char number){
    int a[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    return a[ number - '0' ];
}

static int sum(bn *a, bn *b){
    int size = max( a->bodysize, b->bodysize ) + 1;
    reverse(a->body, a->bodysize);
    reverse(b->body, b->bodysize);
    a = realloc(a, sizeof(int)*size);
    for(int i = a->bodysize; i < size; i++){ a->body[i] = 0; }
    for(int i = 0; i < size; i++){
        a->body[i] += b->body[i];
        a->body[i+1] += a->body[i] / 10;
        a->body[i] %= 10;
    }
    if (a->body[size - 1] == 0){ a = realloc(a, sizeof(int) * (size - 1) ); }
}

static int diff(bn *a, bn* b ){
    reverse(a->body, a->bodysize);
    reverse(b->body, b->bodysize);
    
}

typedef  struct bn_s {
    int *body; // Тело большого числа
    int  bodysize; // Размер массива body
    int sign; // Знак числа:
}bn;

bn *bn_new(){
    bn *r = malloc(sizeof(bn));
    if (r == NULL){ return NULL; }
    r->bodysize = 1;
    r->sign = 0;
    r->body = malloc(sizeof(int) * r->bodysize);
    if (r->body == NULL) {
        free(r);
        return NULL;
    }
    r->body[0] = 0;
    return r;
}

bn *bn_init(bn const *orig){
    bn *copy = malloc(sizeof(bn));
    if (copy == NULL){ return NULL; }
    copy->bodysize = orig->bodysize;
    copy->sign = orig->sign;
    copy->body = malloc(sizeof(int) * orig->bodysize);
    for(int i = 0; i < orig->bodysize; i++){ copy->body[i] = orig->body[i];  }
    return copy;
}

int bn_delete(bn *t){
    free(t);
}

int bn_cmp(bn const *left, bn const *right){
    if (left->sign == right->sign && left->sign == 0){ return 0; }
    if(left->sign < right->sign){ return -1; }
    if (left->sign == right->sign && left->sign == -1){
        if(left->bodysize > right->bodysize ){ return -1; }
        if(left->bodysize < right->bodysize){ return 1; }
        for( int i=0; i < left->bodysize; i++){
            if(left->body[i] > right->body[i]){return -1; }
        }
        return 0;
    }
    if(left->bodysize > right->bodysize ){ return 1; }
    if(left->bodysize < right->bodysize){ return -1; }
    for( int i=0; i < left->bodysize; i++){
        if(left->body[i] > right->body[i]){return 1; }
    }
    return 0;
}

int bn_init_int(bn *t, int init_int){
    if ( init_int < 0){t->sign = -1; init_int = -init_int; }
    if ( init_int > 0){t->sign = 1; }
    int copy = init_int;
    int size = 0;
    while (copy != 0){
        size++;
        copy /= 10;
    }
    t->body = realloc(t->body, sizeof(int) * size);
    int i = 0;
    while (init_int != 0){
        t->body[i] = init_int % 10;
        init_int /= 10;
    }
}

int bn_init_string(bn *t, const char init_string){
    if(init_string[0] == '-'){
        t->sign = -1;
        t->bodysize = strlen( &init_string ) - 1;
        t->body = realloc(t->body, sizeof(int) * t->bodysize);
        for(int i = 0; i < t->bodysize; i++ ){ t->body[i] = char_to_int( init_string[i+1] ); }
    }
    else {
        t->sign = 1;
        t->bodysize = strlen(init_string);
        t->body = realloc(t->body, sizeof(int) * t->bodysize);
        for(int i = 0; i < t->bodysize; i++ ){ t->body[i] = char_to_int( init_string[i]; ) }
    }
}

int bn_neg(bn *t){
    t->sign = -t->sign;
}

int bn_abs(bn *t){
    t->sign*=t->sign;
}

int bn_sign(bn const *t){
    return t->sign;
}

int bn_add_to(bn *t, bn const *right){
    if( t->sign == right->sign ){ sum(t, right); }
    
}



