#include <stdio.h>
#include <stdlib.h>

typedef struct {int a;} str_t;


int fun_str_uproszczona(int* str_wsk){

    int str_lok = *str_wsk;
    int* str_lok_wsk = str_wsk;

    str_lok += 729;

    *str_lok_wsk += 357;


    return(*str_lok_wsk);
}

int fun_str_uproszczona_skrocona(int* str_wsk) {
    *str_wsk += 357;
    return(*str_wsk);
}

str_t fun_str(str_t* str_wsk){
    str_t str_lok = *str_wsk;
    str_t* str_lok_wsk = str_wsk;

    str_lok.a += 729;
    str_lok_wsk -> a += 357;

    return(*str_lok_wsk);
}

typedef int typ;


typ f(typ* a_p){
    typ a = *a_p;
    a += 1186;
    return(a);
}

/*
typedef struct {int* a;} str_tt;

void fun_str2(str_tt *str_wsk[]){
    *(*(*str_wsk)).a++;
}
*/

str_t* fun_str222(str_t* str_wsk_arg){
    (*str_wsk_arg).a +=773;

    str_t* str_h_wsk = malloc(sizeof(str_t));
    *str_h_wsk = *str_wsk_arg;

    str_h_wsk->a +=532;

    return(str_h_wsk);
}

double fnnn(void pa[]);

void fun_strsss(str_t* str_wsk[]){
    (*str_wsk)[0].a++;
}

void fnnnn (char* c[]);

int main() {
    // Zadanie 5

    int str_cellar_int = 342;
    int str_result_int = fun_str_uproszczona(&str_cellar_int);
    printf("str_cellar: %d\nstr_result: %d\n\n", str_cellar_int, str_result_int);


    str_t str_cellar = {.a = 342};
    str_t str_result = fun_str(&str_cellar); // I do funkcji podaję wskaźnik na ten obiekt
    printf("str_cellar: %d\nstr_result: %d\n\n", str_cellar.a, str_result.a);


    // Zadanie 6
    typ m = 185;
    typ n = f(&m);
    printf("m: %d\nn: %d\n", m, n);


    str_t str_caller = {.a = 803};
    str_t* result_wynik = fun_str222(&str_caller);

    printf("%d", 2*str_caller.a + result_wynik->a);


    return 0;
}
