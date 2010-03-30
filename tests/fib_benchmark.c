#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX 1000

struct number_t {
    char *data;
    int data_size;
    int sign;
};

struct number_t number_from_int(int value);
struct number_t add_numbers(struct number_t one, struct number_t two);
void normalize(struct number_t *number);
char *print_number(struct number_t number);

struct number_t number_from_int(int value) {
    struct number_t result;
    result.data = NULL;
    result.sign = (value>0?1:-1);
    value *= result.sign;
    int digit = 0;
    while(value > 0) {
        result.data = realloc(result.data, sizeof(char) * (digit+1));
        result.data[digit] = (value / (int)pow(10, digit)) % 10;
        value -= powl(10, digit)*result.data[digit];
        digit ++;
    }
    result.data_size = digit;
    return result;
}

struct number_t add_numbers(struct number_t one, struct number_t two) {
    struct number_t added;
    added.data = NULL;
    added.sign = 1;
    added.data_size = 0;
    int x, max;
    if(!one.data_size) return two;
    else if(!two.data_size) return one;

    if(one.data_size > two.data_size) max = one.data_size;
    else max = two.data_size;

    for(x = 0; x < max; x ++) {
        added.data = realloc(added.data, sizeof(char) + (x+1));
        added.data[x] = 0;
        if(x < one.data_size) added.data[x] += one.data[x];
        if(x < two.data_size) added.data[x] += two.data[x];
    }
    added.data_size = x;
    normalize(&added);
    return added;
}

void normalize(struct number_t *number) {
    int x = 0;
    for(; x < number->data_size-1; x ++) {
        while(number->data[x] > 9) {
            number->data[x+1] ++;
            number->data[x] -= 10;
        }
    }
    while(number->data[number->data_size-1] > 9) {
        number->data = realloc(number->data, sizeof(char) * (++number->data_size));
        number->data[number->data_size-1] = 1;
        number->data[number->data_size-2] -= 10;
    }
    
}

char *print_number(struct number_t number) {
    static char buffer[65536];
    int x;
    for(x = 0; x < number.data_size; x ++) {
        buffer[x] = number.data[(number.data_size-1)-x] + '0';
    }
    buffer[number.data_size] = 0;
    return buffer;
}

int main(int argc, char *argv[]) {
    struct number_t *fib_numbers, temp;
    int x = 2;
    fib_numbers = malloc(sizeof(struct number_t) * 2);
    fib_numbers[0] = number_from_int(1);
    fib_numbers[1] = number_from_int(1);

    for(; x < MAX; x ++) {
        fib_numbers = realloc(fib_numbers, sizeof(struct number_t) * (x+1));
        fib_numbers[x] = add_numbers(fib_numbers[x-2], fib_numbers[x-1]);
        if(!(x%100)) printf("Calculated %dth fibonacci . . .\n", x+1);
    }

    printf("%ith fibonacci number is %s.\n", x, print_number(fib_numbers[x-1]));

    for(x = 0; x < MAX; x ++) {
        free(fib_numbers[x].data);
    }
    
    free(fib_numbers);
    return 0;
}
