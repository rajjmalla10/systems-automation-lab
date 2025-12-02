// 1. memory leak 

void leak(){
    int *ptr = 100;
}

//2. use after free 

void use_after_free(){
    int *ptr = malloc(sizeof(int));
    free(ptr);
    *ptr = 100;
}

// double free 

void double_free(){
    void* ptr = malloc(sizeof(int));
    free(ptr);
    free(ptr); //crash
}

// 4 memory corruption 

void coruption(){
    int *arr = malloc(5 * sizeof(int));
    arr[5] = 10; //buffer overflow
    free(arr);
}

