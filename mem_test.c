#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include "my_malloc.h"

//Checks if free blocks have been joined
void check_block_joined(){

    struct block *current = block_list;
    while(current && current->next){

        assert(!(current->free && current->next->free));
        current = current->next;
    }
}

int main(){

    mem_init();

    //Allocate and free
    int *arr1 = (int *)my_malloc(5 * sizeof(int));
    arr1[2] = 21;
    printf("arr1[2] = %d\n", arr1[2]);
    my_free(arr1);
    check_block_joined();


    //Allocate and free multiple blocks
    int *arr2 = (int *)my_malloc(3 * sizeof(int));
    int *arr3 = (int *)my_malloc(2 * sizeof(int));
    arr2[2] = 6;
    arr3[0] = 10;
    printf("arr2[2] = %d\n", arr2[2]);
    printf("arr3[0] = %d\n", arr3[0]);
    int x = arr2[2] + arr3[0];
    printf("arr2[2] + arr3[0] = %d\n", x);
    my_free(arr2);
    my_free(arr3);
    check_block_joined();


    //Allocate, free, and allocate again
    int *num1 = (int *)my_malloc(sizeof(int));
    num1[0] = 90;
    printf("num1 = %d\n", num1[0]);
    my_free(num1);
    int *num2 = (int *)my_malloc(sizeof(int));
    num2[0] = 80;
    printf("num2 = %d\n", num2[0]);
    check_block_joined();


    //Allocate and free a struct
    struct Test_struct{

        int a;
        char b;
    };
    struct Test_struct *struct1 = (struct Test_struct *)my_malloc(sizeof(struct Test_struct));
    struct1->a = 5;
    struct1->b = 4;
    printf("a = %d\n", struct1->a);
    printf("b = %d\n", struct1->b);
    my_free(struct1);
    check_block_joined();


    //Allocate and free float
    float *flt1 = (float *)my_malloc(sizeof(float));
    flt1[0] = 12.3455;
    printf("flt1 = %f\n", flt1[0]);
    my_free(flt1);
    check_block_joined();


    //Allocate and free multiple blocks for doubles
    double *dbl1 = (double *)my_malloc(2 * sizeof(double));
    double *dbl2 = (double *)my_malloc(3 * sizeof(double));
    dbl1[1] = 3.1111111111;
    dbl2[2] = 4.2222222222;
    printf("dbl1 = %f\n", dbl1[1]);
    printf("dbl2 = %f\n", dbl2[2]);
    double y = dbl1[1] + dbl2[2];
    printf("dbl1 + dbl2 = %f\n", y);
    my_free(dbl1);
    my_free(dbl2);
    check_block_joined();


    //Allocate and free struct of different data types
    struct AnotherStruct{

        double x;
        int y;
        char* z;
    };
    struct AnotherStruct *anotherStruct = (struct AnotherStruct *)my_malloc(sizeof(struct AnotherStruct));
    anotherStruct->x = 3.121216;
    anotherStruct->y = 20;
    anotherStruct->z = "Testing hehe";
    printf("x = %f\n", anotherStruct->x );
    printf("y = %d\n", anotherStruct->y);
    printf("z = %s\n", anotherStruct->z);
    my_free(anotherStruct);
    check_block_joined();


    //Allocate and free string
    char *str1 = (char *)my_malloc(10 * sizeof(char));
    strcpy(str1, "Hello");
    printf("str1 = %s\n", str1);
    my_free(str1);
    check_block_joined();


    //Allocate and free multiple large blocks to ensure free works (1000 byte total)
    char *str2 = (char *)my_malloc(200 * sizeof(char));
    char *str3 = (char *)my_malloc(700 * sizeof(char));
    my_free(str2);
    my_free(str3);
    check_block_joined();
    char *str4 = (char *)my_malloc(800 * sizeof(char));
    my_free(str4);
    check_block_joined();
    

    //Allocate and free an array and fill with loops
    double *doubleArray = (double *)my_malloc(5 * sizeof(double));
    for(int i = 0; i < 5; ++i){

        doubleArray[i] = i * 1.5;
    }
    printf("Allocated double array: ");
    for(int i = 0; i < 5; ++i){

        printf("%.2f ", doubleArray[i]);
    }
    printf("\n");
    my_free(doubleArray);
    check_block_joined();
    

    //Allocate and free each cell individually in an array
    char **stringArray = (char **)my_malloc(3 * sizeof(char *));
    for(int i = 0; i < 3; ++i){

        stringArray[i] = (char *)my_malloc(10 * sizeof(char));
        sprintf(stringArray[i], "String %d", i + 1);
    }
    printf("Allocated string array: %s, %s, %s\n", stringArray[0], stringArray[1], stringArray[2]);
    for(int i = 0; i < 3; ++i){

        my_free(stringArray[i]);
    }
    my_free(stringArray);
    check_block_joined();
    

    //Use my_malloc with size 0
    int *empty_array = (int *)my_malloc(0);
    printf("Empty array pointer: %p\n", (void *)empty_array);
    my_free(empty_array);


    //Try to use my_free with NULL pointer
    int *null_pointer = NULL;
    my_free(null_pointer);


    //Use my_malloc with a very large size
    char *large_string = (char *)my_malloc(850 * sizeof(char));
    strcpy(large_string, "Test");
    printf("large_string: %s\n", large_string);
    my_free(large_string);
    check_block_joined();


    free_block();
    printf("\nAll tests passed successfully!\n");

    return 0;
}
