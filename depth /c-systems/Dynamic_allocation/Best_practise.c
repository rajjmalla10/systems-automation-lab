// // 1. ALways check return value 

// int *safe_malloc(size_t size){
//     int *ptr = malloc(size);
//     if(ptr == NULL){
//         fprintf(stderr, "memory allocation failed for size: %zu\n", size);
//         exit(EXIT_FAILURE);

//     }
//     return ptr;
// }

// //2. initialize meory 

// int *create_zerored_array(int size){
//     int *arr = calloc(size, sizeof(int));
//     if (!arr) return NULL;
//     return arr;

// }

// //3. clear poninter after free 

// void safe_free(int **ptr){
//     if(ptr && *ptr){
//         free(*ptr);
//         *ptr = NULL; //p[revet] dangling pointers
//     }
// }