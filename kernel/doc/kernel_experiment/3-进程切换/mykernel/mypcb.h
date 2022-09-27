#define MAX_TASK_NUM 4
#define KERNEL_STACK_SIZE 1024*8

struct Thread {
    unsigned long ip;
    unsigned long sp;
};

typedef struct PCB {
    int pid;
    volatile long state;	/* -1  unrunnable, 0 runnable, >0 stoped*/
    char stack[KERNEL_STACK_SIZE];
    struct Thread thread;
    unsigned long task_entry;
    struct PCB *next;
}tPCB;

void my_schedule(void);
