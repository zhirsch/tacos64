#ifndef KPRINTF_H
#define KPRINTF_H

extern int kprintf(const char* fmt, ...) __attribute__ ((format (printf, 1, 2)));
extern void kputchar(char ch);

#endif /* KPRINTF_H */
