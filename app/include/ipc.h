#ifndef _IPC_H
#define _IPC_H

void    TELL_WAIT(void);                /* parent/child from {Sec race_conditions} */
void    TELL_PARENT(pid_t);
void    TELL_CHILD(pid_t);
void    WAIT_PARENT(void);
void    WAIT_CHILD(void);

#endif	/* _IPC_H */
