#include <Arch/x86_64/pci.h>
#include <Arch/x86_64/ports.h>

namespace PCI
{
    void MsiCapSetData(MSICapability *msiCap, uint32_t dat)
    {
        if (msiCap->msiControl & PCI_CAP_MSI_CONTROL_64)
        {
            msiCap->data64 = dat;
        }
        else
        {
            msiCap->data = dat;
        }
    }

    uint32_t MsiCapGetData(MSICapability *msiCap)
    {
        if (msiCap->msiControl & PCI_CAP_MSI_CONTROL_64)
        {
            return msiCap->data64;
        }
        else
        {
            return msiCap->data;
        }
    }

    void MsiCapSetAddress(MSICapability *msiCap, int cpu)
    {
        msiCap->addressLow = PCI_CAP_MSI_ADDRESS_BASE | (((uint32_t) cpu) << 12);
        if (msiCap->msiControl & PCI_CAP_MSI_CONTROL_64)
        {
            msiCap->addressHigh = 0;
        }
    }

    using namespace Ports;

    void SetDevice(PCIDevice *info, uint8_t bus, uint8_t slot, uint8_t func);
    void SetDeviceFromInfo(const PCIInfo *info);

    uint32_t ConfigReadDword(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset)
    {
        WriteDword32(0xCF8, PCI_PACKAGE_ADDRESS(bus, slot, func, offset));

        return ReadDword32(0xCFC);
    }

    uint16_t ConfigReadWord(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset)
    {
        WriteDword32(0xCF8, PCI_PACKAGE_ADDRESS(bus, slot, func, offset));

        return (uint16_t)((ReadDword32(0xCFC) >> (offset & 2) * 8) & 0xFFFF);
    }

    uint8_t ConfigReadByte(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset) 
    {
        WriteDword32(0xCF8, PCI_PACKAGE_ADDRESS(bus, slot, func, offset));

        return (uint8_t)((ReadDword32(0xCFC) >> (offset & 2) * 8) & 0xFF);
    }

    void ConfigWriteDword(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset, uint32_t data)
    {
        WriteDword32(0xCF8, PCI_PACKAGE_ADDRESS(bus, slot, func, offset));
        WriteDword32(0xCFC, data);
    }

    void ConfigWriteWord(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset, uint16_t data)
    {
        WriteDword32(0xCF8, PCI_PACKAGE_ADDRESS(bus, slot, func, offset));
        WriteDword32(0xCFC, (ReadDword32(0xCFC) & (~(0xFFFF << ((offset & 2) * 8)))) |
                (static_cast<uint32_t>(data) << ((offset & 2) * 8)));
    }

    void ConfigWriteByte(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset, uint8_t data)
    {
        WriteDword32(0xCF8, PCI_PACKAGE_ADDRESS(bus, slot, func, offset));
        WriteDword32(0xCFC, (ReadDword32(0xCFC) & (~(0xFF << ((offset & 3) * 8)))) |
                (static_cast<uint32_t>(data) << ((offset & 3) * 8)));
    }

    uint16_t GetVendor(uint8_t bus, uint8_t slot, uint8_t func) {
        uint16_t vendor;

        vendor = ConfigReadWord(bus, slot, func, PCIVendorID);
        return vendor;
    }

    uint16_t GetDeviceID(uint8_t bus, uint8_t slot, uint8_t func) {
        uint16_t id;

        id = ConfigReadWord(bus, slot, func, PCIDeviceID);
        return id;
    }

    uint8_t GetClassCode(uint8_t bus, uint8_t slot, uint8_t func) { return ConfigReadByte(bus, slot, func, PCI_CLASS_CODE); }

    uint8_t GetSubclass(uint8_t bus, uint8_t slot, uint8_t func) { return ConfigReadByte(bus, slot, func, PCI_SUB_CLASS); }

    uint8_t GetProgIf(uint8_t bus, uint8_t slot, uint8_t func) { return ConfigReadByte(bus, slot, func, PCI_PROG_IF); }

    uint8_t GetHeaderType(uint8_t bus, uint8_t slot, uint8_t func) {
        return PCI::ConfigReadByte(bus, slot, func, PCI_HEADER_TYPE);
    }

    bool CheckDevice(uint8_t bus, uint8_t device, uint8_t func)
    {
        return !(GetVendor(bus, device, func) == 0xFFFF);
    }

    bool FindDevice(uint16_t deviceID, uint16_t vendorID)
    {

    }
    
    bool FindGenericDevice(uint16_t classCode, uint16_t subclass);

    void EnumerateDevice(uint16_t deviceID, uint16_t vendorID, void(*func)(const PCIInfo *info));
    void EnumerateGenericDevice(uint8_t classCode, uint8_t subclass, void(*func)(const PCIInfo *info));

    void Initialize()
    {

    }
} // namespace PCI
