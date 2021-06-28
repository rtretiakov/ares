struct LockOn : Interface {
  using Interface::Interface;
  Memory::Readable<n16> rom;
  Memory::Readable<n16> patch;
  CartridgeSlot slot{"Cartridge Slot"};

  auto load() -> void override {
    Interface::load(rom, "program.rom");
    Interface::load(patch, "patch.rom");
    slot.load(cartridge->node);
  }

  auto save() -> void override {
  }

  auto read(n1 upper, n1 lower, n22 address, n16 data) -> n16 override {
    if(address < 0x200000) return data = rom[address >> 1];
    if(address >= 0x300000 && patchEnable) return data = patch[address >> 1];
    if(slot.connected()) return data = slot.cartridge.read(upper, lower, address, data);
    return data;
  }

  auto write(n1 upper, n1 lower, n22 address, n16 data) -> void override {
    if(slot.connected()) slot.cartridge.write(upper, lower, address, data);
  }

  auto readIO(n1 upper, n1 lower, n24 address, n16 data) -> n16 override {
    if(slot.connected()) slot.cartridge.readIO(upper, lower, address, data);
    return data;
  }

  auto writeIO(n1 upper, n1 lower, n24 address, n16 data) -> void override {
    if(slot.connected()) slot.cartridge.writeIO(upper, lower, address, data);
    if(!lower) return;  //todo: unconfirmed
    if(address == 0xa130f0) patchEnable = data.bit(0);
  }

  auto power(bool reset) -> void override {
    if(slot.connected()) slot.cartridge.power(reset);
    patchEnable = 1;
  }

  auto serialize(serializer& s) -> void override {
    if(slot.connected()) s(slot.cartridge);
    s(patchEnable);
  }

  n1 patchEnable;
};
