/** Z80: portable Z80 emulator *******************************/
/**                                                         **/
/**                         CodesXX.h                       **/
/**                                                         **/
/** This file contains implementation for FD/DD tables of   **/
/** Z80 commands. It is included from Z80.c.                **/
/**                                                         **/
/** Copyright (C) Marat Fayzullin 1994-2002                 **/
/**     You are not allowed to distribute this software     **/
/**     commercially. Please, notify me, if you make any    **/
/**     changes to this file.                               **/
/*************************************************************/

case JR_NZ:   if(AF_L&_ZF) PC_W++; else { CCOUNTER-=5;M_JR; } break;
case JR_NC:   if(AF_L&_CF) PC_W++; else { CCOUNTER-=5;M_JR; } break;
case JR_Z:    if(AF_L&_ZF) { CCOUNTER-=5;M_JR; } else PC_W++; break;
case JR_C:    if(AF_L&_CF) { CCOUNTER-=5;M_JR; } else PC_W++; break;

case JP_NZ:   if(AF_L&_ZF) PC_W+=2; else { M_JP; } break;
case JP_NC:   if(AF_L&_CF) PC_W+=2; else { M_JP; } break;
case JP_PO:   if(AF_L&_PF) PC_W+=2; else { M_JP; } break;
case JP_P:    if(AF_L&_SF) PC_W+=2; else { M_JP; } break;
case JP_Z:    if(AF_L&_ZF) { M_JP; } else PC_W+=2; break;
case JP_C:    if(AF_L&_CF) { M_JP; } else PC_W+=2; break;
case JP_PE:   if(AF_L&_PF) { M_JP; } else PC_W+=2; break;
case JP_M:    if(AF_L&_SF) { M_JP; } else PC_W+=2; break;

case RET_NZ:  if(!(AF_L&_ZF)) { CCOUNTER-=6;M_RET; } break;
case RET_NC:  if(!(AF_L&_CF)) { CCOUNTER-=6;M_RET; } break;
case RET_PO:  if(!(AF_L&_PF)) { CCOUNTER-=6;M_RET; } break;
case RET_P:   if(!(AF_L&_SF)) { CCOUNTER-=6;M_RET; } break;
case RET_Z:   if(AF_L&_ZF)    { CCOUNTER-=6;M_RET; } break;
case RET_C:   if(AF_L&_CF)    { CCOUNTER-=6;M_RET; } break;
case RET_PE:  if(AF_L&_PF)    { CCOUNTER-=6;M_RET; } break;
case RET_M:   if(AF_L&_SF)    { CCOUNTER-=6;M_RET; } break;

case CALL_NZ: if(AF_L&_ZF) PC_W+=2; else { CCOUNTER-=7;M_CALL; } break;
case CALL_NC: if(AF_L&_CF) PC_W+=2; else { CCOUNTER-=7;M_CALL; } break;
case CALL_PO: if(AF_L&_PF) PC_W+=2; else { CCOUNTER-=7;M_CALL; } break;
case CALL_P:  if(AF_L&_SF) PC_W+=2; else { CCOUNTER-=7;M_CALL; } break;
case CALL_Z:  if(AF_L&_ZF) { CCOUNTER-=7;M_CALL; } else PC_W+=2; break;
case CALL_C:  if(AF_L&_CF) { CCOUNTER-=7;M_CALL; } else PC_W+=2; break;
case CALL_PE: if(AF_L&_PF) { CCOUNTER-=7;M_CALL; } else PC_W+=2; break;
case CALL_M:  if(AF_L&_SF) { CCOUNTER-=7;M_CALL; } else PC_W+=2; break;

case LD_BC_WORD: M_LDWORD(BC);break;
case LD_DE_WORD: M_LDWORD(DE);break;
case LD_HL_WORD: M_LDWORD(XX);break;
case LD_SP_WORD: M_LDWORD(SP);break;

case LD_PC_HL: PC_W=XX_W;break;
case LD_SP_HL: SP_W=XX_W;break;
case LD_A_xBC: AF_H=RdZ80(BC_W);break;
case LD_A_xDE: AF_H=RdZ80(DE_W);break;

case ADD_HL_BC:  M_ADDW(XX,BC);break;
case ADD_HL_DE:  M_ADDW(XX,DE);break;
case ADD_HL_HL:  M_ADDW(XX,XX);break;
case ADD_HL_SP:  M_ADDW(XX,SP);break;

case DEC_B:    M_DEC(BC_H);break;
case DEC_C:    M_DEC(BC_L);break;
case DEC_D:    M_DEC(DE_H);break;
case DEC_E:    M_DEC(DE_L);break;
case DEC_H:    M_DEC(XX_H);break;
case DEC_L:    M_DEC(XX_L);break;
case DEC_A:    M_DEC(AF_H);break;
case DEC_xHL:  TMP1=RdZ80(XX_W+(gint8)RdZ80(PC_W));M_DEC(TMP1);
               WrZ80(XX_W+(gint8)RdZ80(PC_W++),TMP1);
               break;

case INC_B:    M_INC(BC_H);break;
case INC_C:    M_INC(BC_L);break;
case INC_D:    M_INC(DE_H);break;
case INC_E:    M_INC(DE_L);break;
case INC_H:    M_INC(XX_H);break;
case INC_L:    M_INC(XX_L);break;
case INC_A:    M_INC(AF_H);break;
case INC_xHL:  TMP1=RdZ80(XX_W+(gint8)RdZ80(PC_W));M_INC(TMP1);
               WrZ80(XX_W+(gint8)RdZ80(PC_W++),TMP1);
               break;

case RST00:    M_RST(0x0000);break;
case RST08:    M_RST(0x0008);break;
case RST10:    M_RST(0x0010);break;
case RST18:    M_RST(0x0018);break;
case RST20:    M_RST(0x0020);break;
case RST28:    M_RST(0x0028);break;
case RST30:    M_RST(0x0030);break;
case RST38:    M_RST(0x0038);break;

case PUSH_BC:  M_PUSH(BC);break;
case PUSH_DE:  M_PUSH(DE);break;
case PUSH_HL:  M_PUSH(XX);break;
case PUSH_AF:  M_PUSH(AF);break;

case POP_BC:   M_POP(BC);break;
case POP_DE:   M_POP(DE);break;
case POP_HL:   M_POP(XX);break;
case POP_AF:   M_POP(AF);break;

case DJNZ: if(--BC_H) { CCOUNTER-=5;M_JR; } else PC_W++;break;
case JP:   M_JP;break;
case JR:   M_JR;break;
case CALL: M_CALL;break;
case RET:  M_RET;break;
case SCF:  S(_CF);R(_NF|_HF);break;
case CPL:  AF_H=~AF_H;S(_NF|_HF);break;
case OUTA: TMP1=RdZ80(PC_W++);OutZ80((AF_W & 0xff00) | (TMP1 & 0x00ff), AF_H);break;
case INA:  TMP1=RdZ80(PC_W++);AF_H=InZ80((AF_W & 0xff00) | (TMP1 & 0x00ff));break;

case DI:
  IF_W &= ~(_IFF1 | _IFF2);
  break;

case EI:
  IF_W |=  (_IFF1 | _IFF2);
  break;

case CCF:
  AF_L^=_CF;R(_NF|_HF);
  AF_L|=AF_L&_CF? 0:_HF;
  break;

case EXX:
  WZ_W=BC_W;BC_W=BC_P;BC_P=WZ_W;
  WZ_W=DE_W;DE_W=DE_P;DE_P=WZ_W;
  WZ_W=HL_W;HL_W=HL_P;HL_P=WZ_W;
  break;

case EX_DE_HL: WZ_W=DE_W;DE_W=HL_W;HL_W=WZ_W;break;
case EX_AF_AF: WZ_W=AF_W;AF_W=AF_P;AF_P=WZ_W;break;  
  
case LD_xBC_A: WrZ80(BC_W,AF_H);break;
case LD_xDE_A: WrZ80(DE_W,AF_H);break;

case LD_xHL_BYTE: WZ_W=XX_W+(gint8)RdZ80(PC_W++);
                  WrZ80(WZ_W,RdZ80(PC_W++));break;

case LD_xWORD_HL:
  WZ_L=RdZ80(PC_W++);
  WZ_H=RdZ80(PC_W++);
  WrZ80(WZ_W++,XX_L);
  WrZ80(WZ_W,XX_H);
  break;

case LD_HL_xWORD:
  WZ_L=RdZ80(PC_W++);
  WZ_H=RdZ80(PC_W++);
  XX_L=RdZ80(WZ_W++);
  XX_H=RdZ80(WZ_W);
  break;

case LD_A_xWORD:
  WZ_L=RdZ80(PC_W++);
  WZ_H=RdZ80(PC_W++);
  AF_H=RdZ80(WZ_W);
  break;

case LD_xWORD_A:
  WZ_L=RdZ80(PC_W++);
  WZ_H=RdZ80(PC_W++);
  WrZ80(WZ_W,AF_H);
  break;

case EX_HL_xSP:
  WZ_L=RdZ80(SP_W);WrZ80(SP_W++,XX_L);
  WZ_H=RdZ80(SP_W);WrZ80(SP_W--,XX_H);
  XX_W=WZ_W;
  break;
