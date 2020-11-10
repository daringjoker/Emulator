//
// Created by DaringJoker on 11/7/2020.
//

#ifndef EMULATOR_EMULATOR_H
#define EMULATOR_EMULATOR_H
#include <vector>
#include "../Components/AllComponents.h"
#include "DataTypes.h"
using namespace std;

class emulator {
public:
    Register A=Register("A"),B=Register("B"),C=Register("C"),\
    D=Register("D"),E=Register("E"),H=Register("H"),L=Register("L")\
    ,PCl=Register("PC_low"),PCh=Register("PC_high"),SPl=Register("SP_low"),SPh=Register("SP_high");
    FlagRegister flags;
    RegisterPair PSW=RegisterPair(A,flags);
    RegisterPair BC=RegisterPair(B,C);
    RegisterPair DE=RegisterPair(D,E);
    RegisterPair HL=RegisterPair(H,L);
    RegisterPair SP=RegisterPair(SPh,SPl);
    RegisterPair PC=RegisterPair(PCh,PCl);
    DataBus Dbus;
    AddressBus Abus;
    Memory Ram=Memory(Abus,Dbus);
    Ports ports=Ports(Abus,Dbus);
    Word start_of_code=0x4000;
    vector<emu_error> Error_list;
public://methods
    //public methods for client use
    emulator()=default;
    void load_binary(vector<Byte> binary);
    void single_step();
    void run();
    void reset();
private:
    bool fatal_error=false;
    bool halted=false;
private:
    //Errors during execution
    void record_error(string errorstring,Word address,Errorclass errclass);
    //fetching api
    Byte fetchByte();
    Word fetchWord();
    //for decoding the instructions
    Decoded_Instr Decode(Byte Opcode);
    //Executing instructions
    void Execute(Decoded_Instr instr);

    //Different mnemonics and their handling functions
    void IN(Decoded_Instr instr);
    void JPO(Decoded_Instr instr);
    void INX(Decoded_Instr instr);
    void SUB(Decoded_Instr instr);
    void CC(Decoded_Instr instr);
    void PUSH(Decoded_Instr instr);
    void ACI(Decoded_Instr instr);
    void LDAX(Decoded_Instr instr);
    void XRA(Decoded_Instr instr);
    void DCX(Decoded_Instr instr);
    void RAL(Decoded_Instr instr);
    void ANI(Decoded_Instr instr);
    void RP(Decoded_Instr instr);
    void MVI(Decoded_Instr instr);
    void CNZ(Decoded_Instr instr);
    void DI(Decoded_Instr instr);
    void XRI(Decoded_Instr instr);
    void RRC(Decoded_Instr instr);
    void JM(Decoded_Instr instr);
    void INR(Decoded_Instr instr);
    void DAD(Decoded_Instr instr);
    void SUI(Decoded_Instr instr);
    void JMP(Decoded_Instr instr);
    void CPO(Decoded_Instr instr);
    void CZ(Decoded_Instr instr);
    void ORI(Decoded_Instr instr);
    void STA(Decoded_Instr instr);
    void CNC(Decoded_Instr instr);
    void LDA(Decoded_Instr instr);
    void SBI(Decoded_Instr instr);
    void EI(Decoded_Instr instr);
    void RIM(Decoded_Instr instr);
    void JP(Decoded_Instr instr);
    void STAX(Decoded_Instr instr);
    void RZ(Decoded_Instr instr);
    void CPI(Decoded_Instr instr);
    void XCHG(Decoded_Instr instr);
    void CMP(Decoded_Instr instr);
    void SIM(Decoded_Instr instr);
    void DCR(Decoded_Instr instr);
    void CMC(Decoded_Instr instr);
    void PCHL(Decoded_Instr instr);
    void JC(Decoded_Instr instr);
    void XTHL(Decoded_Instr instr);
    void NOP(Decoded_Instr instr);
    void CPE(Decoded_Instr instr);
    void SPHL(Decoded_Instr instr);
    void CMA(Decoded_Instr instr);
    void LHLD(Decoded_Instr instr);
    void JNZ(Decoded_Instr instr);
    void RNZ(Decoded_Instr instr);
    void ADC(Decoded_Instr instr);
    void RM(Decoded_Instr instr);
    void CM(Decoded_Instr instr);
    void ANA(Decoded_Instr instr);
    void RAR(Decoded_Instr instr);
    void LXI(Decoded_Instr instr);
    void SHLD(Decoded_Instr instr);
    void POP(Decoded_Instr instr);
    void ORA(Decoded_Instr instr);
    void RNC(Decoded_Instr instr);
    void RPE(Decoded_Instr instr);
    void DAA(Decoded_Instr instr);
    void STC(Decoded_Instr instr);
    void JPE(Decoded_Instr instr);
    void CALL(Decoded_Instr instr);
    void RC(Decoded_Instr instr);
    void RST(Decoded_Instr instr);
    void RPO(Decoded_Instr instr);
    void SBB(Decoded_Instr instr);
    void JNC(Decoded_Instr instr);
    void HLT(Decoded_Instr instr);
    void ADD(Decoded_Instr instr);
    void OUT(Decoded_Instr instr);
    void RLC(Decoded_Instr instr);
    void CP(Decoded_Instr instr);
    void RET(Decoded_Instr instr);
    void JZ(Decoded_Instr instr);
    void MOV(Decoded_Instr instr);
    void ADI(Decoded_Instr instr);

    Register & getRegByName(string regname);
};
#endif //EMULATOR_EMULATOR_H
