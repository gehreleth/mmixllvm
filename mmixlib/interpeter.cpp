#include "stdafx.h"
#include "interpreter.h"
#include "econtext.h"
#include "vamemstretegy.h"
#include "errorhandling.h"
#include "interpreter_ldst.h"
#include "interpreter_arith.h"
#include "interpreter_cond.h"
#include "interpreter_logic.h"
#include "interpreter_goto_branch.h"
#include "interpreter_fp.h"

using namespace MmixLlvm;

namespace {

class OpcodeSelector {
  typedef void(*OpcF)(ExecutionContext& ctx, MXUTetra opc);
  OpcF impls_[0x100];

  static void OpcodeSelector::notImplementedStub(ExecutionContext& ctx, MXUTetra opc);

  static void OpcodeSelector::noop(ExecutionContext& ctx, MXUTetra opc);

  size_t ix(MmixOpcode opc)
  {
    return static_cast<size_t>(opc);
  }

public:
  OpcodeSelector()
  {
    for (int i = 0; i < 0x100; i++) {
      impls_[i] = &notImplementedStub;
    }
    impls_[ix(MmixOpcode::SWYM)] = &noop;
    impls_[ix(MmixOpcode::LDOU)] = &InterpreterPrivate::ldou;
    impls_[ix(MmixOpcode::LDTU)] = &InterpreterPrivate::ldtu;
    impls_[ix(MmixOpcode::LDWU)] = &InterpreterPrivate::ldwu;
    impls_[ix(MmixOpcode::LDBU)] = &InterpreterPrivate::ldbu;
    impls_[ix(MmixOpcode::LDOUI)] = &InterpreterPrivate::ldoui;
    impls_[ix(MmixOpcode::LDTUI)] = &InterpreterPrivate::ldtui;
    impls_[ix(MmixOpcode::LDWUI)] = &InterpreterPrivate::ldwui;
    impls_[ix(MmixOpcode::LDBUI)] = &InterpreterPrivate::ldbui;
    impls_[ix(MmixOpcode::LDO)] = &InterpreterPrivate::ldo;
    impls_[ix(MmixOpcode::LDT)] = &InterpreterPrivate::ldt;
    impls_[ix(MmixOpcode::LDW)] = &InterpreterPrivate::ldw;
    impls_[ix(MmixOpcode::LDB)] = &InterpreterPrivate::ldb;
    impls_[ix(MmixOpcode::LDOI)] = &InterpreterPrivate::ldoi;
    impls_[ix(MmixOpcode::LDTI)] = &InterpreterPrivate::ldti;
    impls_[ix(MmixOpcode::LDWI)] = &InterpreterPrivate::ldwi;
    impls_[ix(MmixOpcode::LDBI)] = &InterpreterPrivate::ldbi;
    impls_[ix(MmixOpcode::LDHT)] = &InterpreterPrivate::ldht;
    impls_[ix(MmixOpcode::LDHTI)] = &InterpreterPrivate::ldhti;
    impls_[ix(MmixOpcode::STOU)] = &InterpreterPrivate::stou;
    impls_[ix(MmixOpcode::STTU)] = &InterpreterPrivate::sttu;
    impls_[ix(MmixOpcode::STWU)] = &InterpreterPrivate::stwu;
    impls_[ix(MmixOpcode::STBU)] = &InterpreterPrivate::stbu;
    impls_[ix(MmixOpcode::STOUI)] = &InterpreterPrivate::stoui;
    impls_[ix(MmixOpcode::STTUI)] = &InterpreterPrivate::sttui;
    impls_[ix(MmixOpcode::STWUI)] = &InterpreterPrivate::stwui;
    impls_[ix(MmixOpcode::STBUI)] = &InterpreterPrivate::stbui;
    impls_[ix(MmixOpcode::STO)] = &InterpreterPrivate::sto;
    impls_[ix(MmixOpcode::STT)] = &InterpreterPrivate::stt;
    impls_[ix(MmixOpcode::STW)] = &InterpreterPrivate::stw;
    impls_[ix(MmixOpcode::STB)] = &InterpreterPrivate::stb;
    impls_[ix(MmixOpcode::STOI)] = &InterpreterPrivate::stoi;
    impls_[ix(MmixOpcode::STTI)] = &InterpreterPrivate::stti;
    impls_[ix(MmixOpcode::STWI)] = &InterpreterPrivate::stwi;
    impls_[ix(MmixOpcode::STBI)] = &InterpreterPrivate::stbi;
    impls_[ix(MmixOpcode::STHT)] = &InterpreterPrivate::stht;
    impls_[ix(MmixOpcode::STHTI)] = &InterpreterPrivate::sthti;
    impls_[ix(MmixOpcode::STCO)] = &InterpreterPrivate::stco;
    impls_[ix(MmixOpcode::STCOI)] = &InterpreterPrivate::stcoi;
    impls_[ix(MmixOpcode::STSF)] = &InterpreterPrivate::stsf;
    impls_[ix(MmixOpcode::STSFI)] = &InterpreterPrivate::stsfi;
    impls_[ix(MmixOpcode::LDSF)] = &InterpreterPrivate::ldsf;
    impls_[ix(MmixOpcode::LDSFI)] = &InterpreterPrivate::ldsfi;
    impls_[ix(MmixOpcode::ADD)] = &InterpreterPrivate::add;
    impls_[ix(MmixOpcode::ADDI)] = &InterpreterPrivate::addi;
    impls_[ix(MmixOpcode::SUB)] = &InterpreterPrivate::sub;
    impls_[ix(MmixOpcode::SUBI)] = &InterpreterPrivate::subi;
    impls_[ix(MmixOpcode::ADDU)] = &InterpreterPrivate::addu;
    impls_[ix(MmixOpcode::ADDUI)] = &InterpreterPrivate::addui;
    impls_[ix(MmixOpcode::SUBU)] = &InterpreterPrivate::subu;
    impls_[ix(MmixOpcode::SUBUI)] = &InterpreterPrivate::subui;
    impls_[ix(MmixOpcode::NEG)] = &InterpreterPrivate::neg;
    impls_[ix(MmixOpcode::NEGI)] = &InterpreterPrivate::negi;
    impls_[ix(MmixOpcode::NEGU)] = &InterpreterPrivate::negu;
    impls_[ix(MmixOpcode::NEGUI)] = &InterpreterPrivate::negui;
    impls_[ix(MmixOpcode::_2ADDU)] = &InterpreterPrivate::_2addu;
    impls_[ix(MmixOpcode::_2ADDUI)] = &InterpreterPrivate::_2addui;
    impls_[ix(MmixOpcode::_4ADDU)] = &InterpreterPrivate::_4addu;
    impls_[ix(MmixOpcode::_4ADDUI)] = &InterpreterPrivate::_4addui;
    impls_[ix(MmixOpcode::_8ADDU)] = &InterpreterPrivate::_8addu;
    impls_[ix(MmixOpcode::_8ADDUI)] = &InterpreterPrivate::_8addui;
    impls_[ix(MmixOpcode::_16ADDU)] = &InterpreterPrivate::_16addu;
    impls_[ix(MmixOpcode::_16ADDUI)] = &InterpreterPrivate::_16addui;
    impls_[ix(MmixOpcode::DIV)] = &InterpreterPrivate::div;
    impls_[ix(MmixOpcode::DIVI)] = &InterpreterPrivate::divi;
    impls_[ix(MmixOpcode::DIVU)] = &InterpreterPrivate::divu;
    impls_[ix(MmixOpcode::DIVUI)] = &InterpreterPrivate::divui;
    impls_[ix(MmixOpcode::MUL)] = &InterpreterPrivate::mul;
    impls_[ix(MmixOpcode::MULI)] = &InterpreterPrivate::muli;
    impls_[ix(MmixOpcode::MULU)] = &InterpreterPrivate::mulu;
    impls_[ix(MmixOpcode::MULUI)] = &InterpreterPrivate::mului;
    impls_[ix(MmixOpcode::SL)] = &InterpreterPrivate::sl;
    impls_[ix(MmixOpcode::SLI)] = &InterpreterPrivate::sli;
    impls_[ix(MmixOpcode::SLU)] = &InterpreterPrivate::slu;
    impls_[ix(MmixOpcode::SLUI)] = &InterpreterPrivate::slui;
    impls_[ix(MmixOpcode::SR)] = &InterpreterPrivate::sr;
    impls_[ix(MmixOpcode::SRI)] = &InterpreterPrivate::sri;
    impls_[ix(MmixOpcode::SRU)] = &InterpreterPrivate::sru;
    impls_[ix(MmixOpcode::SRUI)] = &InterpreterPrivate::srui;
    impls_[ix(MmixOpcode::CMP)] = &InterpreterPrivate::cmp;
    impls_[ix(MmixOpcode::CMPI)] = &InterpreterPrivate::cmpi;
    impls_[ix(MmixOpcode::CMPU)] = &InterpreterPrivate::cmpu;
    impls_[ix(MmixOpcode::CMPUI)] = &InterpreterPrivate::cmpui;
    impls_[ix(MmixOpcode::CSN)] = &InterpreterPrivate::scn;
    impls_[ix(MmixOpcode::CSNI)] = &InterpreterPrivate::scni;
    impls_[ix(MmixOpcode::CSZ)] = &InterpreterPrivate::csz;
    impls_[ix(MmixOpcode::CSZI)] = &InterpreterPrivate::cszi;
    impls_[ix(MmixOpcode::CSP)] = &InterpreterPrivate::csp;
    impls_[ix(MmixOpcode::CSPI)] = &InterpreterPrivate::cspi;
    impls_[ix(MmixOpcode::CSOD)] = &InterpreterPrivate::csod;
    impls_[ix(MmixOpcode::CSODI)] = &InterpreterPrivate::csodi;
    impls_[ix(MmixOpcode::CSNN)] = &InterpreterPrivate::csnn;
    impls_[ix(MmixOpcode::CSNNI)] = &InterpreterPrivate::csnni;
    impls_[ix(MmixOpcode::CSNZ)] = &InterpreterPrivate::csnz;
    impls_[ix(MmixOpcode::CSNZI)] = &InterpreterPrivate::csnzi;
    impls_[ix(MmixOpcode::CSNP)] = &InterpreterPrivate::csnp;
    impls_[ix(MmixOpcode::CSNPI)] = &InterpreterPrivate::csnpi;
    impls_[ix(MmixOpcode::CSEV)] = &InterpreterPrivate::csev;
    impls_[ix(MmixOpcode::CSEVI)] = &InterpreterPrivate::csevi;
    impls_[ix(MmixOpcode::ZSN)] = &InterpreterPrivate::zcn;
    impls_[ix(MmixOpcode::ZSNI)] = &InterpreterPrivate::zcni;
    impls_[ix(MmixOpcode::ZSZ)] = &InterpreterPrivate::zsz;
    impls_[ix(MmixOpcode::ZSZI)] = &InterpreterPrivate::zszi;
    impls_[ix(MmixOpcode::ZSP)] = &InterpreterPrivate::zsp;
    impls_[ix(MmixOpcode::ZSPI)] = &InterpreterPrivate::zspi;
    impls_[ix(MmixOpcode::ZSOD)] = &InterpreterPrivate::zsod;
    impls_[ix(MmixOpcode::ZSODI)] = &InterpreterPrivate::zsodi;
    impls_[ix(MmixOpcode::ZSNN)] = &InterpreterPrivate::zsnn;
    impls_[ix(MmixOpcode::ZSNNI)] = &InterpreterPrivate::zsnni;
    impls_[ix(MmixOpcode::ZSNZ)] = &InterpreterPrivate::zsnz;
    impls_[ix(MmixOpcode::ZSNZI)] = &InterpreterPrivate::zsnzi;
    impls_[ix(MmixOpcode::ZSNP)] = &InterpreterPrivate::zsnp;
    impls_[ix(MmixOpcode::ZSNPI)] = &InterpreterPrivate::zsnpi;
    impls_[ix(MmixOpcode::ZSEV)] = &InterpreterPrivate::zsev;
    impls_[ix(MmixOpcode::ZSEVI)] = &InterpreterPrivate::zsevi;
    impls_[ix(MmixOpcode::AND)] = &InterpreterPrivate::and_;
    impls_[ix(MmixOpcode::ANDI)] = &InterpreterPrivate::andi;
    impls_[ix(MmixOpcode::OR)] = &InterpreterPrivate::or_;
    impls_[ix(MmixOpcode::ORI)] = &InterpreterPrivate::ori;
    impls_[ix(MmixOpcode::XOR)] = &InterpreterPrivate::xor_;
    impls_[ix(MmixOpcode::XORI)] = &InterpreterPrivate::xori;
    impls_[ix(MmixOpcode::ANDN)] = &InterpreterPrivate::andn;
    impls_[ix(MmixOpcode::ANDNI)] = &InterpreterPrivate::andni;
    impls_[ix(MmixOpcode::ORN)] = &InterpreterPrivate::orn;
    impls_[ix(MmixOpcode::ORNI)] = &InterpreterPrivate::orni;
    impls_[ix(MmixOpcode::NAND)] = &InterpreterPrivate::nand;
    impls_[ix(MmixOpcode::NANDI)] = &InterpreterPrivate::nandi;
    impls_[ix(MmixOpcode::NOR)] = &InterpreterPrivate::nor;
    impls_[ix(MmixOpcode::NORI)] = &InterpreterPrivate::nori;
    impls_[ix(MmixOpcode::NXOR)] = &InterpreterPrivate::nxor;
    impls_[ix(MmixOpcode::NXORI)] = &InterpreterPrivate::nxori;
    impls_[ix(MmixOpcode::MUX)] = &InterpreterPrivate::mux;
    impls_[ix(MmixOpcode::MUXI)] = &InterpreterPrivate::muxi;
    impls_[ix(MmixOpcode::SADD)] = &InterpreterPrivate::sadd;
    impls_[ix(MmixOpcode::SADDI)] = &InterpreterPrivate::saddi;
    impls_[ix(MmixOpcode::SETH)] = &InterpreterPrivate::seth;
    impls_[ix(MmixOpcode::SETMH)] = &InterpreterPrivate::setmh;
    impls_[ix(MmixOpcode::SETML)] = &InterpreterPrivate::setml;
    impls_[ix(MmixOpcode::SETL)] = &InterpreterPrivate::setl;
    impls_[ix(MmixOpcode::INCH)] = &InterpreterPrivate::inch;
    impls_[ix(MmixOpcode::INCMH)] = &InterpreterPrivate::incmh;
    impls_[ix(MmixOpcode::INCML)] = &InterpreterPrivate::incml;
    impls_[ix(MmixOpcode::INCL)] = &InterpreterPrivate::incl;
    impls_[ix(MmixOpcode::ORH)] = &InterpreterPrivate::orh;
    impls_[ix(MmixOpcode::ORMH)] = &InterpreterPrivate::ormh;
    impls_[ix(MmixOpcode::ORML)] = &InterpreterPrivate::orml;
    impls_[ix(MmixOpcode::ORL)] = &InterpreterPrivate::orl;
    impls_[ix(MmixOpcode::ANDNH)] = &InterpreterPrivate::andnh;
    impls_[ix(MmixOpcode::ANDNMH)] = &InterpreterPrivate::andnmh;
    impls_[ix(MmixOpcode::ANDNML)] = &InterpreterPrivate::andnml;
    impls_[ix(MmixOpcode::ANDNL)] = &InterpreterPrivate::andnl;
    impls_[ix(MmixOpcode::BDIF)] = &InterpreterPrivate::bdif;
    impls_[ix(MmixOpcode::BDIFI)] = &InterpreterPrivate::bdifi;
    impls_[ix(MmixOpcode::WDIF)] = &InterpreterPrivate::wdif;
    impls_[ix(MmixOpcode::WDIFI)] = &InterpreterPrivate::wdifi;
    impls_[ix(MmixOpcode::TDIF)] = &InterpreterPrivate::tdif;
    impls_[ix(MmixOpcode::TDIFI)] = &InterpreterPrivate::tdifi;
    impls_[ix(MmixOpcode::ODIF)] = &InterpreterPrivate::odif;
    impls_[ix(MmixOpcode::ODIFI)] = &InterpreterPrivate::odifi;
    impls_[ix(MmixOpcode::MOR)] = &InterpreterPrivate::mor;
    impls_[ix(MmixOpcode::MORI)] = &InterpreterPrivate::mori;
    impls_[ix(MmixOpcode::MXOR)] = &InterpreterPrivate::mxor;
    impls_[ix(MmixOpcode::MXORI)] = &InterpreterPrivate::mxori;
    impls_[ix(MmixOpcode::GET)] = &InterpreterPrivate::get;
    impls_[ix(MmixOpcode::GETA)] = &InterpreterPrivate::geta;
    impls_[ix(MmixOpcode::GETAB)] = &InterpreterPrivate::getab;
    impls_[ix(MmixOpcode::PUT)] = &InterpreterPrivate::put;
    impls_[ix(MmixOpcode::PUTI)] = &InterpreterPrivate::puti;
    impls_[ix(MmixOpcode::JMP)] = &InterpreterPrivate::jmp;
    impls_[ix(MmixOpcode::JMPB)] = &InterpreterPrivate::jmpb;
    impls_[ix(MmixOpcode::GO)] = &InterpreterPrivate::go;
    impls_[ix(MmixOpcode::GOI)] = &InterpreterPrivate::goi;
    impls_[ix(MmixOpcode::BN)] = &InterpreterPrivate::bn;
    impls_[ix(MmixOpcode::BZ)] = &InterpreterPrivate::bz;
    impls_[ix(MmixOpcode::BP)] = &InterpreterPrivate::bp;
    impls_[ix(MmixOpcode::BOD)] = &InterpreterPrivate::bod;
    impls_[ix(MmixOpcode::BNN)] = &InterpreterPrivate::bnn;
    impls_[ix(MmixOpcode::BNZ)] = &InterpreterPrivate::bnz;
    impls_[ix(MmixOpcode::BNP)] = &InterpreterPrivate::bnp;
    impls_[ix(MmixOpcode::BEV)] = &InterpreterPrivate::bev;
    impls_[ix(MmixOpcode::BNB)] = &InterpreterPrivate::bnb;
    impls_[ix(MmixOpcode::BZB)] = &InterpreterPrivate::bzb;
    impls_[ix(MmixOpcode::BPB)] = &InterpreterPrivate::bpb;
    impls_[ix(MmixOpcode::BODB)] = &InterpreterPrivate::bodb;
    impls_[ix(MmixOpcode::BNNB)] = &InterpreterPrivate::bnnb;
    impls_[ix(MmixOpcode::BNZB)] = &InterpreterPrivate::bnzb;
    impls_[ix(MmixOpcode::BNPB)] = &InterpreterPrivate::bnpb;
    impls_[ix(MmixOpcode::BEVB)] = &InterpreterPrivate::bevb;
    impls_[ix(MmixOpcode::PBN)] = &InterpreterPrivate::bn;
    impls_[ix(MmixOpcode::PBZ)] = &InterpreterPrivate::bz;
    impls_[ix(MmixOpcode::PBP)] = &InterpreterPrivate::bp;
    impls_[ix(MmixOpcode::PBOD)] = &InterpreterPrivate::bod;
    impls_[ix(MmixOpcode::PBNN)] = &InterpreterPrivate::bnn;
    impls_[ix(MmixOpcode::PBNZ)] = &InterpreterPrivate::bnz;
    impls_[ix(MmixOpcode::PBNP)] = &InterpreterPrivate::bnp;
    impls_[ix(MmixOpcode::PBEV)] = &InterpreterPrivate::bev;
    impls_[ix(MmixOpcode::PBNB)] = &InterpreterPrivate::bnb;
    impls_[ix(MmixOpcode::PBZB)] = &InterpreterPrivate::bzb;
    impls_[ix(MmixOpcode::PBPB)] = &InterpreterPrivate::bpb;
    impls_[ix(MmixOpcode::PBODB)] = &InterpreterPrivate::bodb;
    impls_[ix(MmixOpcode::PBNNB)] = &InterpreterPrivate::bnnb;
    impls_[ix(MmixOpcode::PBNZB)] = &InterpreterPrivate::bnzb;
    impls_[ix(MmixOpcode::PBNPB)] = &InterpreterPrivate::bnpb;
    impls_[ix(MmixOpcode::PBEVB)] = &InterpreterPrivate::bevb;
    impls_[ix(MmixOpcode::PUSHGO)] = &InterpreterPrivate::pushgo;
    impls_[ix(MmixOpcode::PUSHGOI)] = &InterpreterPrivate::pushgoi;
    impls_[ix(MmixOpcode::PUSHJ)] = &InterpreterPrivate::pushj;
    impls_[ix(MmixOpcode::PUSHJB)] = &InterpreterPrivate::pushjb;
    impls_[ix(MmixOpcode::POP)] = &InterpreterPrivate::pop;
    impls_[ix(MmixOpcode::TRIP)] = &InterpreterPrivate::trip;
    impls_[ix(MmixOpcode::TRAP)] = &InterpreterPrivate::trap;
    impls_[ix(MmixOpcode::RESUME)] = &InterpreterPrivate::resume;
    // TODO:
    //impls_[ix(MmixOpcode::SAVE)] = ... 
    //impls_[ix(MmixOpcode::UNSAVE)] = ...
    impls_[ix(MmixOpcode::FIX)] = &InterpreterPrivate::fix;
    impls_[ix(MmixOpcode::FIXU)] = &InterpreterPrivate::fixu;
    impls_[ix(MmixOpcode::FLOT)] = &InterpreterPrivate::flot;
    impls_[ix(MmixOpcode::FLOTI)] = &InterpreterPrivate::floti;
    impls_[ix(MmixOpcode::FLOTU)] = &InterpreterPrivate::flotu;
    impls_[ix(MmixOpcode::FLOTUI)] = &InterpreterPrivate::flotui;
    impls_[ix(MmixOpcode::FADD)] = &InterpreterPrivate::fadd;
    impls_[ix(MmixOpcode::FSUB)] = &InterpreterPrivate::fsub;
    impls_[ix(MmixOpcode::FMUL)] = &InterpreterPrivate::fmul;
    impls_[ix(MmixOpcode::FDIV)] = &InterpreterPrivate::fdiv;
    impls_[ix(MmixOpcode::FREM)] = &InterpreterPrivate::frem;
    impls_[ix(MmixOpcode::FSQRT)] = &InterpreterPrivate::fsqrt;
    impls_[ix(MmixOpcode::FINT)] = &InterpreterPrivate::fint;
    impls_[ix(MmixOpcode::FCMP)] = &InterpreterPrivate::fcmp;
    impls_[ix(MmixOpcode::FEQL)] = &InterpreterPrivate::feql;
    impls_[ix(MmixOpcode::FUN)] = &InterpreterPrivate::fun;
    impls_[ix(MmixOpcode::FCMPE)] = &InterpreterPrivate::fcmpe;
    impls_[ix(MmixOpcode::FEQLE)] = &InterpreterPrivate::feqle;
    impls_[ix(MmixOpcode::FUNE)] = &InterpreterPrivate::fune;
    impls_[ix(MmixOpcode::SFLOT)] = &InterpreterPrivate::sflot;
    impls_[ix(MmixOpcode::SFLOTI)] = &InterpreterPrivate::sfloti;
    impls_[ix(MmixOpcode::SFLOTU)] = &InterpreterPrivate::sflotu;
    impls_[ix(MmixOpcode::SFLOTUI)] = &InterpreterPrivate::sflotui;
    impls_[ix(MmixOpcode::SFLOTUI)] = &InterpreterPrivate::sflotui;
  }

  void exec(ExecutionContext& ctx, MXUTetra opc)
  {
    MXUOcta xptr = ctx.getXptr();
    impls_[opc >> 24](ctx, opc);
  }
};

void OpcodeSelector::notImplementedStub(ExecutionContext& ctx, MXUTetra opc)
{
  MMIXERROR("Unimplemented MMIX opcode", opc, "at", ctx.getXptr());
}

void OpcodeSelector::noop(ExecutionContext& ctx, MXUTetra opc) {}
}

void MmixLlvm::stepInto(ExecutionContext& ctx, MXUOcta loc)
{
  static OpcodeSelector ops;
  ctx.setEntryPoint(loc);
  while (!ctx.isHalted()) {
    ops.exec(ctx, ctx.fetchInstruction());
  }
}
