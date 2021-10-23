#ifndef _ERROR_H
#define _ERROR_H

void    err_msg(const char *, ...);                     /* {App misc_source} */
void    err_dump(const char *, ...) __attribute__((noreturn));
void    err_quit(const char *, ...) __attribute__((noreturn));
void    err_cont(int, const char *, ...);
void    err_exit(int, const char *, ...) __attribute__((noreturn));
void    err_ret(const char *, ...);
void    err_sys(const char *, ...) __attribute__((noreturn));

#endif	/* _ERROR_H */
