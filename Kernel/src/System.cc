#include <proc/sched.h>
#include <proc/proc.h>
#include <libkern/printf.h>
#include <driver/video.h>
#include <macros>

namespace System
{
    void Out(const char* fmt, ...) {
        va_list args;
        va_start(args, fmt);
        vprintf(fmt, args);
        va_end(args);
        Video::Newline();
    }

    [[noreturn]] void Panic(const char *fmt, ...) {
        Video::ClearScreen();
        
        va_list args;
        va_start(args, fmt);
        vprintf(fmt, args);
        va_end(args);

        asm("cli; hlt;");
    }
} // namespace System

using namespace Task;

[[noreturn]] void KernelInit()
{
    (g_Scheduler = Scheduler())->Register(&g_KernelProcess);

    for (;;) asm("hlt");
}
