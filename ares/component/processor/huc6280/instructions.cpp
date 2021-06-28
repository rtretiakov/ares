auto HuC6280::instructionAbsoluteModify(fp alu, n8 index) -> void {
  n16 absolute = operand();
  absolute |= operand() << 8;
  idle();
  idle();
  auto data = ALU(load16(absolute + index));
L store16(absolute + index, data);
}

auto HuC6280::instructionAbsoluteRead(fp alu, n8& data, n8 index) -> void {
  n16 absolute = operand();
  absolute |= operand() << 8;
  idle();
L data = ALU(load16(absolute + index));
}

auto HuC6280::instructionAbsoluteReadMemory(fp alu, n8 index) -> void {
  auto a = A;
  A = load8(X);

  n16 absolute = operand();
  absolute |= operand() << 8;
  idle();
  A = ALU(load16(absolute + index));

L store8(X, A);
  A = a;
}

auto HuC6280::instructionAbsoluteWrite(n8 data, n8 index) -> void {
  n16 absolute = operand();
  absolute |= operand() << 8;
  idle();
L store16(absolute + index, data);
}

auto HuC6280::instructionBlockMove(bp alu) -> void {
  n16 source = operand();
  source |= operand() << 8;
  n16 target = operand();
  target |= operand() << 8;
  n16 length = operand();
  length |= operand() << 8;
  push(Y);
  push(A);
  push(X);
  idle();
  idle();
  idle();
  idle();
  blockMove = 1;
  bool alternate = 0;
  do {
    auto data = load16(source);
    store16(target, data);
    ALU(source, target, alternate);
    alternate ^= 1;
    idle();
    idle();
    idle();
    idle();
  } while(--length);
  blockMove = 0;
  X = pull();
  A = pull();
L Y = pull();
}

auto HuC6280::instructionBranch(bool take) -> void {
  if(!take) {
  L operand();
  } else {
    auto displacement = operand();
    idle();
  L idle();
    PC += (i8)displacement;
  }
}

auto HuC6280::instructionBranchIfBitReset(n3 index) -> void {
  auto zeropage = operand();
  auto displacement = operand();
  idle();
  idle();
L auto data = load8(zeropage);
  if(data.bit(index) == 0) {
    idle();
    idle();
    PC += (i8)displacement;
  }
}

auto HuC6280::instructionBranchIfBitSet(n3 index) -> void {
  auto zeropage = operand();
  auto displacement = operand();
  idle();
  idle();
L auto data = load8(zeropage);
  if(data.bit(index) == 1) {
    idle();
    idle();
    PC += (i8)displacement;
  }
}

auto HuC6280::instructionBranchSubroutine() -> void {
  auto displacement = operand();
  idle();
  idle();
  idle();
  idle();
  push(PC - 1 >> 8);
L push(PC - 1 >> 0);
  PC += (i8)displacement;
}

auto HuC6280::instructionBreak() -> void {
  operand();
  idle();
  push(PC >> 8);
  push(PC >> 0);
  push(P() | 0x10);  //B flag is set
  I = 1;
  D = 0;
  T = 0;
  PC.byte(0) = load16(0xfff6);
L PC.byte(1) = load16(0xfff7);
}

auto HuC6280::instructionCallAbsolute() -> void {
  n16 address = operand();
  address |= operand() << 8;
  idle();
  idle();
  push(PC - 1 >> 8);
L push(PC - 1 >> 0);
  PC = address;
}

auto HuC6280::instructionChangeSpeedLow() -> void {
  idle();
  CS = 12;
L idle();
}

auto HuC6280::instructionChangeSpeedHigh() -> void {
  idle();
  CS = 3;
L idle();
}

auto HuC6280::instructionClear(n8& data) -> void {
L idle();
  data = 0;
}

auto HuC6280::instructionClear(bool& flag) -> void {
L idle();
  flag = 0;
}

auto HuC6280::instructionImmediate(fp alu, n8& data) -> void {
L data = ALU(operand());
}

auto HuC6280::instructionImmediateMemory(fp alu) -> void {
  auto a = A;
  A = load8(X);

  A = ALU(operand());

L store8(X, A);
  A = a;
}

auto HuC6280::instructionImplied(fp alu, n8& data) -> void {
L idle();
  data = ALU(data);
}

auto HuC6280::instructionIndirectRead(fp alu, n8& data, n8 index) -> void {
  auto zeropage = operand();
  idle();
  n16 absolute = load8(zeropage + index + 0);
  absolute |= load8(zeropage + index + 1) << 8;
  idle();
L data = ALU(load16(absolute));
}

auto HuC6280::instructionIndirectReadMemory(fp alu, n8 index) -> void {
  auto a = A;
  A = load8(X);

  auto zeropage = operand();
  idle();
  n16 absolute = load8(zeropage + index + 0);
  absolute |= load8(zeropage + index + 1) << 8;
  idle();
  A = ALU(load16(absolute));

L store8(X, A);
  A = a;
}

auto HuC6280::instructionIndirectWrite(n8 data, n8 index) -> void {
  auto zeropage = operand();
  idle();
  n16 absolute = load8(zeropage + index + 0);
  absolute |= load8(zeropage + index + 1) << 8;
  idle();
L store16(absolute, data);
}

auto HuC6280::instructionIndirectYRead(fp alu, n8& data) -> void {
  auto zeropage = operand();
  idle();
  n16 absolute = load8(zeropage + 0);
  absolute |= load8(zeropage + 1) << 8;
  idle();
L data = ALU(load16(absolute + Y));
}

auto HuC6280::instructionIndirectYReadMemory(fp alu) -> void {
  auto a = A;
  A = load8(X);

  auto zeropage = operand();
  idle();
  n16 absolute = load8(zeropage + 0);
  absolute |= load8(zeropage + 1) << 8;
  idle();
  A = ALU(load16(absolute + Y));

L store8(X, A);
  A = a;
}

auto HuC6280::instructionIndirectYWrite(n8 data) -> void {
  auto zeropage = operand();
  idle();
  n16 absolute = load8(zeropage + 0);
  absolute |= load8(zeropage + 1) << 8;
  idle();
L store16(absolute + Y, data);
}

auto HuC6280::instructionJumpAbsolute() -> void {
  n16 address = operand();
  address |= operand() << 8;
L idle();
  PC = address;
}

auto HuC6280::instructionJumpIndirect(n8 index) -> void {
  n16 address = operand();
  address |= operand() << 8;
  idle();
  idle();
  PC.byte(0) = load16(address + index + 0);
L PC.byte(1) = load16(address + index + 1);
}

auto HuC6280::instructionNoOperation() -> void {
L idle();
}

auto HuC6280::instructionPull(n8& data) -> void {
  idle();
  idle();
L data = pull();
  Z = data == 0;
  N = data.bit(7);
}

auto HuC6280::instructionPullP() -> void {
  idle();
  idle();
L P = pull() | 0x10;  //B flag is set
}

auto HuC6280::instructionPush(n8 data) -> void {
  idle();
L push(data);
}

auto HuC6280::instructionResetMemoryBit(n3 index) -> void {
  auto zeropage = operand();
  idle();
  idle();
  idle();
  auto data = load8(zeropage);
  data.bit(index) = 0;
L store8(zeropage, data);
}

auto HuC6280::instructionReturnInterrupt() -> void {
  idle();
  idle();
  idle();
  P = pull();
  PC.byte(0) = pull();
L PC.byte(1) = pull();
}

auto HuC6280::instructionReturnSubroutine() -> void {
  idle();
  idle();
  idle();
  PC.byte(0) = pull();
  PC.byte(1) = pull();
L idle();
  PC++;
}

auto HuC6280::instructionSet(bool& flag) -> void {
L idle();
  flag = 1;
}

auto HuC6280::instructionSetMemoryBit(n3 index) -> void {
  auto zeropage = operand();
  idle();
  idle();
  idle();
  auto data = load8(zeropage);
  data.bit(index) = 1;
L store8(zeropage, data);
}

auto HuC6280::instructionStoreImplied(n2 index) -> void {
  auto data = operand();
  idle();
  idle();
L store(index, data);
}

auto HuC6280::instructionSwap(n8& lhs, n8& rhs) -> void {
  idle();
L idle();
  swap(lhs, rhs);
}

auto HuC6280::instructionTestAbsolute(n8 index) -> void {
  auto mask = operand();
  n16 absolute = operand();
  absolute |= operand() << 8;
  idle();
  idle();
  idle();
L n8 data = load16(absolute + index);
  Z = (data & mask) == 0;
  V = data.bit(6);
  N = data.bit(7);
}

auto HuC6280::instructionTestZeroPage(n8 index) -> void {
  auto mask = operand();
  auto zeropage = operand();
  idle();
  idle();
  idle();
L n8 data = load8(zeropage + index);
  Z = (data & mask) == 0;
  V = data.bit(6);
  N = data.bit(7);
}

auto HuC6280::instructionTransfer(n8& source, n8& target) -> void {
L idle();
  target = source;
  Z = target == 0;
  N = target.bit(7);
}

auto HuC6280::instructionTransferAccumulatorToMPR() -> void {
  auto mask = operand();
  idle();
  idle();
L idle();
  for(u32 index : range(8)) {
    if(mask.bit(index)) MPR[index] = MPL = A;
  }
}

auto HuC6280::instructionTransferMPRToAccumulator() -> void {
  auto mask = operand();
  idle();
L idle();
  if(mask) MPL = ~0;
  for(u32 index : range(8)) {
    //exact logic if multiple bits are set is unknown; but it is combinatorial
    if(mask.bit(index)) MPL &= MPR[index];
  }
  //in the event no bits are set, A is loaded with the previously latched value
  A = MPL;
}

auto HuC6280::instructionTransferXS() -> void {
L idle();
  S = X;
}

auto HuC6280::instructionZeroPageModify(fp alu, n8 index) -> void {
  auto zeropage = operand();
  idle();
  idle();
  auto data = ALU(load8(zeropage + index));
L store8(zeropage + index, data);
}

auto HuC6280::instructionZeroPageRead(fp alu, n8& data, n8 index) -> void {
  auto zeropage = operand();
  idle();
L data = ALU(load8(zeropage + index));
}

auto HuC6280::instructionZeroPageReadMemory(fp alu, n8 index) -> void {
  auto a = A;
  A = load8(X);

  auto zeropage = operand();
  idle();
  A = ALU(load8(zeropage + index));

L store8(X, A);
  A = a;
}

auto HuC6280::instructionZeroPageWrite(n8 data, n8 index) -> void {
  auto zeropage = operand();
  idle();
L store8(zeropage + index, data);
}
