#include <stdlib.h>
#include <stddef.h>
#include <string.h>
typedef  struct bn_s {
    int *body; // Тело большого числа
    int  bodysize; // Размер массива body
    int sign; // Знак числа
}bn;

bn *bn_new(){ //новое число
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

bn *bn_init(bn const *orig){ //копия числа
    bn *copy = malloc(sizeof(bn));
    if (copy == NULL){ return NULL; }
    copy->bodysize = orig->bodysize;
    copy->sign = orig->sign;
    copy->body = malloc(sizeof(int) * orig->bodysize);
    if(copy->body == NULL){
        free(copy);
        return NULL;
    }
    for(int i = 0; i < orig->bodysize; i++){ copy->body[i] = orig->body[i];  }
    return copy;
}

int bn_delete(bn *t){ //освободить память
    if(t != NULL) free(t);
    return 0;
}

int swap(int *a, int *b){ //поменять местами переменные
    int c = *b;
    *b = *a;
    *a = c;
    return 0;
}

 int reverse(int *m, int m_size){  //зеркально отразить массив
    for(int i = 0; i < m_size / 2; i++){
        swap( &m[i] , &m[m_size - i - 1] );
    }
    return 0;
}

 int max (int a, int b){ //максимум из 2 чисел
    if( a >= b ){ return a; }
    return b;
}

 int char_to_int(const char number){ //символ в цифру
    int *a = malloc(10 * sizeof(int) );
    for(int i = 0; i < 10; i++){
        a[i] = i;
    }
    int res = a[ number - '0' ];
    free(a);
    return res;
}

 int sum(bn *a, bn *b){ //вспомогательная сумма; результат в а
    int size = max( a->bodysize, b->bodysize ) + 1;
    bn *b_copy = bn_init(b);
    reverse(a->body, a->bodysize);
    reverse(b_copy->body, b_copy->bodysize);
    a->body = realloc(a->body, sizeof(int) * size);
    b_copy->body = realloc(b_copy->body, sizeof(int) * size);
    for(int i = a->bodysize; i < size; i++){ a->body[i] = 0; }
    for(int i = b_copy->bodysize; i < size; i++){ b_copy->body[i] = 0; }
    for(int i = 0; i < size - 1; i++){
        a->body[i] += b_copy->body[i];
        a->body[i + 1] += a->body[i] / 10;
        a->body[i] %= 10;
    }
    if (a->body[size - 1] == 0){
        a->body = realloc(a->body, sizeof(int) * (size - 1) );
        size--;
    }
    a->bodysize = size;
    reverse(a->body, a->bodysize);
    bn_delete(b_copy);
    return 0;
}

 int diff(bn *a, bn *b ){ //вспомогательная разность; результат в а
    bn *b_copy = bn_init(b);
    reverse(a->body, a->bodysize); //здесь a "длиннее" b
    reverse(b_copy->body, b_copy->bodysize);
    for(int i = 0; i < a->bodysize; i++){
        if(i < b_copy->bodysize){
            a->body[i] -= b_copy->body[i];
        }
        if(a->body[i] <= -1){ a->body[i + 1]--; a->body[i] += 10; }
    }
    int i = a->bodysize - 1;
    while (a->body[i] == 0 && i > 0){
        i--;
    }
    i++;
    a->body = realloc(a->body, i * sizeof(int));
    a->bodysize = i;
    reverse (a->body, a->bodysize);
    bn_delete(b_copy);
    if(a->body[0] == 0){
        a->sign = 0;
    }
    return 0;
}

int bn_cmp(bn const *left, bn const *right){ // сравнение чисел
    if (left->sign == right->sign && left->sign == 0){ return 0; }
    if(left->sign < right->sign){ return -1; }
    if(left->sign > right->sign) {return 1; }
    if (left->sign == -1){
        if(left->bodysize > right->bodysize ){ return -1; }
        if(left->bodysize < right->bodysize){ return 1; }
        for( int i = 0; i < left->bodysize; i++){
            if(left->body[i] > right->body[i]){return -1; }
            if(left->body[i] < right->body[i]){ return 1; }
        }
        return 0;
    }
    if(left->bodysize > right->bodysize ){ return 1; }
    if(left->bodysize < right->bodysize){ return -1; }
    for( int i = 0; i < left->bodysize; i++){
        if(left->body[i] > right->body[i]){return 1; }
        if(left->body[i] < right->body[i]){ return -1; }
    }
    return 0;
}

int bn_init_int(bn *t, int init_int){ //инициализация числа int числом bn
    if ( init_int > 0){t->sign = 1;}
    if ( init_int < 0){t->sign = -1; init_int = -init_int; }
    if(t->sign == 0){ return 0;}
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
        i++;
    }
    t->bodysize = size;
    reverse(t->body, size);
    return 0;
}

int bn_init_string(bn *t, const char *init_string){ //инициализация строки числом bn
    if(init_string[0] == '0'){ return 0; }
    if(init_string[0] == '-'){
        t->sign = -1;
        t->bodysize = strlen( init_string ) - 1;
        t->body = realloc(t->body, sizeof(int) * t->bodysize);
        for(int i = 0; i < t->bodysize; i++ ){ t->body[i] = char_to_int( init_string[i+1] ); }
    }
    else {
        t->sign = 1;
        t->bodysize = strlen(init_string);
        t->body = realloc(t->body, sizeof(int) * t->bodysize);
        for(int i = 0; i < t->bodysize; i++ ){ t->body[i] = char_to_int( init_string[i] ); }
    }
    return 0;
}

int bn_neg(bn *t){ //поменять знак
    t->sign = -t->sign;
    return 0;
}

int bn_abs(bn *t){ //модуль числа
    t->sign *= t->sign;
    return 0;
}

int bn_sign(bn const *t){ //знак числа
    return t->sign;
    return 0;
}


int bn_add_to(bn *t, bn const *right){ //сумма чисел; результат в т
    bn *copy = bn_init(right);
    int sign_copy = copy->sign;
    int sign_t = t->sign;
    bn_abs(t);
    bn_abs(copy);
    int compare = bn_cmp( t, copy );
    t->sign = sign_t;
    copy->sign = sign_copy;
    if(copy->sign == 0){ bn_delete(copy); return 0; }
    if( t->sign == copy->sign || t->sign == 0){
        sum(t, copy);
        t->sign = copy->sign;
        bn_delete(copy);
        return 0;
    }
    if(compare == 1){
        diff(t, copy);
        bn_delete(copy);
        return 0;
    }
    if(compare == -1){
        diff(copy, t);
        t->sign = copy->sign;
        free(t->body);
        t->body = malloc(copy->bodysize * sizeof(int));
        t->bodysize = copy->bodysize;
        for(int i = 0; i < t->bodysize; i++){
            t->body[i] = copy->body[i];
        }
        bn_delete(copy);
        return 0;
    }
    if(compare == 0 && t->sign * copy->sign == -1){
        diff(t, copy);
        t->sign = 0;
        bn_delete(copy);
        return 0;
    }
    if(compare == 0 && t->sign * copy->sign == 1){
        sum(t, copy);
        bn_delete(copy);
    }
    return 0;
}

int bn_sub_to(bn *t, bn const *right){ //вычитание; результат в т
    bn *copy = bn_init(right);
    bn_neg(copy);
    bn_add_to(t, copy);
    bn_delete(copy);
    return 0;
}

int bn_mul_to(bn *t, bn const *right){ //умножение; результат в т
    bn *ans = bn_new();
    bn *copy = bn_init(right);
    ans->bodysize = t->bodysize + copy->bodysize + 1;
    ans->body = realloc(ans->body, ans->bodysize * sizeof(int));
    for(int i = 0; i < ans->bodysize; i++){
        ans->body[i] = 0;
    }
    ans->sign = t->sign * copy->sign;
    reverse(t->body, t->bodysize);
    reverse(copy->body, copy->bodysize);
    t->body = realloc(t->body, max(t->bodysize, copy->bodysize) * sizeof(int) );
    copy->body = realloc(copy->body, max(t->bodysize, copy->bodysize) * sizeof(int) );
    for(int i = t->bodysize; i < max(t->bodysize, copy->bodysize); i++){
        t->body[i] = 0;
    }
    for(int i = copy->bodysize; i < max(t->bodysize, copy->bodysize); i++){
        copy->body[i] = 0;
    }
    for(int i = 0; i < t->bodysize; i++){
        for(int j = 0; j < copy->bodysize; j++){
            ans->body[i + j] += t->body[i] * copy->body[j];
            ans->body[i + j + 1] += ans->body[i + j] / 10;
            ans->body[i + j] %= 10;
        }
    }
    int index = ans->bodysize - 1;
    while(ans->body[index] == 0 && index > 0){
        index--;
    }
    ans->bodysize = index + 1;
    ans->body = realloc(ans->body, ans->bodysize * sizeof(int) );
    reverse(ans->body, ans->bodysize);
    t->bodysize = ans->bodysize;
    t->sign = ans->sign;
    for(int i = 0; i < ans->bodysize; i++){
        t->body[i] = ans->body[i];
    }
    bn_delete(ans);
    bn_delete(copy);
    return 0;
}




 int division(bn *t, bn const *right){ //вспомогательное деление; результат t
    bn *ans = bn_new();
    ans->body = realloc(ans->body, t->bodysize * sizeof(int));
    ans->sign = t->sign * right->sign;
    bn *current = bn_new();
    int progress = 0; //количество элементов t->body, которые участвовали в делении
    int ind = 0; //свободный элемент в массиве ans->body
    
    while (progress < t->bodysize ){
        current->sign = 1;
        if(current->body[0] == 0){ current->bodysize = 0; current->sign = 0;}//чтобы current не начинался с 0
        current->body = realloc(current->body, right->bodysize * sizeof(int) );
        int point; //свободное место в current->body
        int copy_progress = progress;
        point = current->bodysize;
        
        //дописываем цифры из t в current
        for(int i = copy_progress; i < t->bodysize; i++){
            if(point >= right->bodysize ){ copy_progress = -1; break; }
            if(current->body[0] != 0){
                current->body[point] = t->body[i];
                progress++;
                point++;
            }
            if(current->body[0] == 0 && t->body[i] != 0){ //чтобы current не начинался с 0
                current->body[point] = t->body[i];
                progress++;
                point++;
                current->sign = 1; //чтобы cmp работало правильно
            }
            if(current->body[0] == 0 && t->body[i] == 0){ progress++; }
            
            if(point <= right->bodysize - 1 && ans->body[0] != 0){
                ans->body[ind] = 0;
                ind++;
            }
        }
        
        if(point < right->bodysize){ break; } //закончились цифры из t
        //
        current->bodysize = right->bodysize;
        //создаем вспомогательный right1, чтобы  cmp работало верно
        bn *right1 = bn_new();
        right1 = bn_init(right);
        bn_abs(right1);
        //
        if(bn_cmp(current, right1) == -1 && progress < t->bodysize){
            current->body = realloc(current->body, (right->bodysize + 1) * sizeof(int));
            current->bodysize++;
            current->body[current->bodysize - 1] = t->body[progress];
            progress++;
            
            //если мы дописвли хотя бы 1 цифру в цикле for
            if(ans->body[0] != 0 && copy_progress != -1){
                ans->body[ind] = 0;
                ind++;
            }
        }
        
        if(bn_cmp(current, right1) == -1 && progress == t->bodysize && ans->body[0] != 0){
            ans->body[ind] = 0;
            ind++;
            /////   ВНИМАНИЕ!!!!!!!!!!!!!!!!!
            bn_delete(right1);
            break;
        }
        bn_delete(right1); // удаляем right1
        
        //начинаем деление
        int l = 0, r = 10;
        while(l < r - 1){
            int m = (l + r) / 2;
            //создадим делитель
            bn *div = bn_new();
            bn_init_int(div, m);
            //
            bn *copy_right = bn_new();
            copy_right = bn_init(right);
            bn_abs(copy_right);
            bn_mul_to(copy_right, div);
            int compare = bn_cmp( copy_right , current );
            if( compare <= 0){
                l = m;
            }
            if(compare == 1){
                r = m;
            }
            bn_delete(div);
            bn_delete(copy_right);
        }
        ///найдем остаток
        ans->body[ind] = l;
        ind++;
        bn *copy_right = bn_new();
        copy_right = bn_init(right);
        bn_abs(copy_right);
        
        bn *div = bn_new();
        bn_init_int(div, l);
        bn_mul_to(copy_right, div);
        bn_sub_to(current, copy_right);
        //удалим лишнее
        bn_delete(div);
        bn_delete(copy_right);
        
    }
    //присвоим значение t
    t->sign = ans->sign;
    for(int i = 0; i < ind; i++){
        t->body[i] = ans->body[i];
    }
    t->body = realloc(t->body, ind * sizeof(int) );
    t->bodysize = ind;
    
    if(ans->body[0] == 0){ // если в итоге ответ 0
        t->body = realloc(t->body, sizeof(int));
        t->body[0] = 0;
        t->bodysize = 1;
        t->sign = 0;
    }
    //удалим лишнее
    bn_delete(ans);
    return 0;
}


int bn_div_to(bn *t, bn const *right){ //деленеие; результат в а
    if(t->sign * right->sign >= 0){
        division(t, right);
        return 0;
    }
    if(t->sign * right->sign == -1){
        bn *t_copy = bn_new();
        t_copy = bn_init(t);
        //поделим
        division(t_copy, right);
        bn_mul_to(t_copy, right);
        int compare = bn_cmp(t_copy, t);
        bn_delete(t_copy);
        //
        if(compare != 0){
            bn *t_copy = bn_new();
            t_copy = bn_init(t);
            division(t_copy, right);
            //вычтем 1
            bn *d = bn_new();
            bn_init_int(d, -1);
            bn_add_to(t_copy, d);
            bn_delete(d);
            
            //запишем ответ
            t->sign = t_copy->sign;
            t->bodysize = t_copy->bodysize;
            t->body = realloc(t->body, t_copy->bodysize * sizeof(int) );
            for(int j = 0; j < t->bodysize; j++){
                t->body[j] = t_copy->body[j];
            }
            bn_delete(t_copy);
            return 0;
        }
        //если compare == 0
        division(t, right);
        
        //удалим лишнее
        return 0;
    }
    return 0;
}


int bn_mod_to(bn *t, bn const *right){ //остаток от деления; результат в t
    bn *t_copy = bn_new();
    t_copy = bn_init(t);
    bn_div_to(t_copy, right);
    bn_mul_to(t_copy, right);
    //найдем остаток по формуле А - (А/Б)*Б
    bn_sub_to(t, t_copy);
    bn_delete(t_copy);
    return 0;
}

//
bn* bn_add(bn const *left, bn const *right){ //сложение; возвращает указатель
    bn *left_copy = bn_new();
    left_copy = bn_init(left);
    bn_add_to(left_copy, right);
    return left_copy;
}

bn* bn_sub(bn const *left, bn const *right){ //разность; возвращает указатель
    bn *left_copy = bn_new();
    left_copy = bn_init(left);
    bn_sub_to(left_copy, right);
    return left_copy;
}

bn* bn_mul(bn const *left, bn const *right){ //произведение; возвращает указатель
    bn *left_copy = bn_new();
    left_copy = bn_init(left);
    bn_mul_to(left_copy, right);
    return left_copy;
}

bn* bn_div(bn const *left, bn const *right){ //частное; возвращает указатель
    bn *left_copy = bn_new();
    left_copy = bn_init(left);
    bn_div_to(left_copy, right);
    return left_copy;
}

bn* bn_mod(bn const *left, bn const *right){ //остаток; возвращает указатель
    bn *left_copy = bn_new();
    left_copy = bn_init(left);
    bn_mod_to(left_copy, right);
    return left_copy;
}


int bn_pow_to(bn *t, int degree){ //возведение в степень
    bn *ans = bn_new();
    bn_init_int(ans, 1);
    for(int i = 0; i < degree; i++){
        bn_mul_to(ans, t);
    }
    // запишем ответ
    t->sign = ans->sign;
    t->bodysize = ans->bodysize;
    t->body = realloc(t->body, t->bodysize * sizeof(int) );
    for(int i = 0; i < t->bodysize; i++){
        t->body[i] = ans->body[i];
    }
    bn_delete(ans);
    return 0;
}

int bn_init_string_radix(bn *t, const char *init_string, int radix){
    return 0;
}

const char *bn_to_string(bn const *t, int radix){
    if(radix == 10){
        char *s = malloc(t->bodysize * sizeof(char));
        for(int i = 0; i < t->bodysize; i++){
            s[i] = '0' + t->body[i];
        }
        return s;
    }
}

int bn_root_to(bn *t, int reciprocal){
    return 0;
}
