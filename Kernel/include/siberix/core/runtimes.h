#pragma once

#include <siberix/core/time.h>
#include <siberix/device/connectivity.h>
#include <siberix/init/boot.h>
#include <siberix/mm/memory.h>
#include <siberix/mm/page.h>
#include <siberix/proc/process.h>
#include <siberix/proc/sched.h>

enum class Architecture {
    Undefined,
    X86_64,
    ARMv7,
    RISC_V
};

class SiberixKernel {
public:
    SiberixKernel(Architecture arch, BootConfig& bootConfig)
        : m_isInitialized(false),
          m_arch(arch),
          m_bootConfig(bootConfig) {}
    ~SiberixKernel();

    virtual bool setupArch() = 0;

    bool        isInitialized() { return m_isInitialized; }
    BootConfig& getBootConfig();

    MemoryService&      getMemory() { return m_memory; }
    DeviceConnectivity* getConnectivity() { return m_devices; }
    Scheduler*          getScheduler() { return m_scheduler; }
    ProcessFactory*     getProcessFactory() { return m_processFactory; }

    u64  getTimestamp();
    void sleep(u64 ms);

    Clock& getClock() { return m_clock; }

    void addTimer(TimerDevice& timer, bool setAsDefault = false) {
        if (!m_timers.contains(timer)) {
            m_timers.add(timer);
            if (setAsDefault) {
                m_defaultTimer = &timer;
            }
        }
    }

    TimerDevice& getDefaultTimer() {
        return m_defaultTimer == nullptr ? m_timers[0] : *m_defaultTimer;
    }

protected:
    bool         m_isInitialized;
    Architecture m_arch;
    BootConfig&  m_bootConfig;

    MemoryService            m_memory;
    DeviceConnectivity*      m_devices;
    Scheduler*               m_scheduler;
    ProcessFactory*          m_processFactory;
    Clock                    m_clock;
    LinkedList<TimerDevice&> m_timers;
    TimerDevice*             m_defaultTimer;
};

SiberixKernel* siberix();