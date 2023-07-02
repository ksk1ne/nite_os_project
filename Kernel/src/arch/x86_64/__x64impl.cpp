#include <common/arch.h>
#include <arch/x86_64/arch.h>
#include <arch/x86_64/types.h>
#include <arch/x86_64/paging.hpp>
#include <siberix/mm/page.hpp>

static X64RuntimeSupport x64rt;
extern "C" void _lgdt(u64);
extern "C" void _lidt(u64);

RuntimeSupport& getRuntimeArch() {
    if (!x64rt.isInitialized()) {
        x64rt = X64RuntimeSupport();
    }
    return x64rt;
}

TaskStateSegment tss = {
    .rsp = {},
    .ist = {},
    .iopbOffset = 0};
GdtPackage gdtPackage;
GdtPtr gdtPtr;
IdtPtr idtPtr;

static Memory::SegAlloc _segAlloc;
static Memory::BuddyAlloc _buddyAlloc;

void X64RuntimeSupport::setup()
{
    /* load global descriptor table */
    gdtPackage = GdtPackage(tss);
    gdtPtr = {.limit = sizeof(GdtPackage) - 1, .base = (u64)&gdtPackage};
    _lgdt((u64)&gdtPtr);
    /* load interrupt descriptor table */
    for (int i = 0; i < IDT_ENTRY_COUNT; i++)
        idtEntries[i] = IdtEntry(i, isrTables[i], 0x08, IDT_FLAGS_INTGATE, 0);
    idtPtr = {.limit = sizeof(IdtEntry) * IDT_ENTRY_COUNT, .base = (u64)&idtEntryList};
    _lidt((u64)&idtPtr);

    Paging::init();
    this->loadMemory();
}

void X64RuntimeSupport::loadMemory() {
    this->pageAlloc = &(_segAlloc = Memory::SegAlloc());
}