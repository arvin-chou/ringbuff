# ringbuff

ringbuff implement, plz refer:
http://stackoverflow.com/questions/827691/how-do-you-implement-a-circular-buffer-in-c
http://c.learncodethehardway.org/book/ex44.html

please see main function to learn how to use, such as below:

```c
    circular_buffer cb; 
    size_t sz = 10; 
    size_t capacity = 5;
    char* item = malloc(sz);
    cb_init(&cb, capacity, sz);
    snprintf(item, sz, "1 The simplest solution would be to keep track\n");
    cb_push_back(&cb, item);

    snprintf(item, sz, "2 Circular buffer in JavaScript");
    cb_push_back(&cb, item);
    
    //print
    //data(0) =1 The sim
    //data(1) =2 Circula
    cb_print_all(&cb);
    
```

# compile
`gcc main.c ringbuff.c -o main && ./main`
