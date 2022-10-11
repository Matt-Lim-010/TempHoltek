#ifndef HOLTEK_BUILDIN
#define HOLTEK_BUILDIN
// _rl(int8 varname)
#define GCC_RL(varname) 	asm volatile ("rl %0" : "=m"(varname): "m"(varname))
// _rlc(int8 varname)
#define GCC_RLC(varname)	asm volatile ("rlc %0" : "=m"(varname): "m"(varname))
// _rr(int8 varname)
#define GCC_RR(varname)		asm volatile ("rr %0" : "=m"(varname): "m"(varname))
// _rrc(int8 varname)
#define GCC_RRC(varname) 	asm volatile ("rrc %0" : "=m"(varname): "m"(varname))
// _nop()
#define GCC_NOP()			asm("nop")
// _swap(int8 varname)
#define GCC_SWAP(varname)	asm volatile ("swap %0" : "=m"(varname): "m"(varname))
// _halt()
#define GCC_HALT()			asm("halt")
// _clrwdt1
#define GCC_CLRWDT1()		asm("clr wdt1")
// _clrwdt2
#define GCC_CLRWDT2()		asm("clr wdt2")
// _clrwdt
#define GCC_CLRWDT()		asm("clr wdt")

//V2 built-in
#define _nop()			asm("nop")
#define _halt()			asm("halt")
#define _clrwdt1()		asm("clr wdt1")
#define _clrwdt2()		asm("clr wdt2")
#define _clrwdt()		asm("clr wdt")


#define ___mkstr1(x)    #x
#ifndef __PBP__
#define GCC_DELAY(varname) \
                         do{asm("IF " ___mkstr1(varname) ">263690");\
                         	asm("ERRMESSAGE 'the value " ___mkstr1(varname) "of the argument exceeds the maxinum 263690'\n\tENDIF");\
						 asm("IF " ___mkstr1(varname) "<0");\
                         	asm("ERRMESSAGE 'the value " ___mkstr1(varname) " of the argument exceeds the mininum 0'\n\tENDIF");\
                         asm("IF " ___mkstr1(varname) "==0");\
                         	asm("JMP $\n\tENDIF");\
                         asm("IF "___mkstr1(varname) "==1");\
                         	asm("NOP\n\tENDIF");\
                         asm("IF "___mkstr1(varname) "==2");\
                         	asm("NOP");\
                         	asm("NOP\n\tENDIF");\
                         asm("IF "___mkstr1(varname) "==3");\
                         	asm("NOP");\
                         	asm("NOP");\
                         	asm("NOP\n\tENDIF");\
                         asm("IF ("___mkstr1(varname) "<8) AND (" ___mkstr1(varname)">3)" );\
                            asm("CALL __DELAY3 + 7 -" ___mkstr1(varname)"\n\tENDIF");\
                         asm("IF ("___mkstr1(varname) "< 0404h) AND (" ___mkstr1(varname)">7)");\
                         	asm("MOV A," ___mkstr1(varname)"/4-1");\
                         	asm("CALL __DELAYX3 + 3 -(" ___mkstr1(varname) " MOD 4)\n\tENDIF");\
                         asm("IF ("___mkstr1(varname) "< 0x40B) AND (" ___mkstr1(varname)"> 0403h)");\
                         	asm("MOV A,0 ");\
                         	asm("CALL __DELAYX6 + 0x40a -" ___mkstr1(varname)"\n\tENDIF");\
                         asm("IF ("___mkstr1(varname) " >= 0x40b) AND ((((" ___mkstr1(varname)"-9)%0402h)/4+1)>256)" );\
							asm("MOV A,("___mkstr1(varname)"-9)/0402h");\
							asm("MOV _builtin_holtek_delay_2,A");\
							asm("MOV A,0");\
                         	asm("CALL __DELAYY5 +5- (((" ___mkstr1(varname)"-9)%0402h)%6)\n\tENDIF");\
                         asm("IF ("___mkstr1(varname) " >= 0x40b) AND ((((" ___mkstr1(varname)"-9)%0402h)/4+1)<=256)" );\
						    asm("MOV A,("___mkstr1(varname)"-9)/0402h");\
							asm("MOV _builtin_holtek_delay_2,A");\
							asm("MOV A,(("___mkstr1(varname)"-9)%0402h)/4+1");\
                         	asm("CALL __DELAYY3 +3-(((" ___mkstr1(varname)"-9)%0402h)%4)\n\tENDIF");}while(0)  					 
#else
#define GCC_DELAY(varname) \
                         do{asm("IF " ___mkstr1(varname) ">263690");\
                         	asm("ERRMESSAGE 'the value " ___mkstr1(varname) "of the argument exceeds the maxinum 263690'\n\tENDIF");\
						 asm("IF " ___mkstr1(varname) "<0");\
                         	asm("ERRMESSAGE 'the value " ___mkstr1(varname) " of the argument exceeds the mininum 0'\n\tENDIF");\
                         asm("IF " ___mkstr1(varname) "==0");\
                         	asm("JMP $\n\tENDIF");\
                         asm("IF ("___mkstr1(varname) ">=1) AND (" ___mkstr1(varname) "<( 4 + __PBP__ * 2))" );\
                         	asm("NOP\n\tENDIF");\
                         asm("IF ("___mkstr1(varname) ">=2) AND ("___mkstr1(varname) "<( 4 + __PBP__ * 2))" );\
                         	asm("NOP\n\tENDIF");\
                         asm("IF ("___mkstr1(varname) ">=3) AND ("___mkstr1(varname) "<( 4 + __PBP__ * 2))" );\
                         	asm("NOP\n\tENDIF");\
                         asm("IF ("___mkstr1(varname) ">=4) AND ("___mkstr1(varname) "<( 4 + __PBP__ * 2)) AND ( __PBP__ > 0 )" );\
                         	asm("NOP\n\tENDIF");\
                         asm("IF ("___mkstr1(varname) ">=5) AND ("___mkstr1(varname) "<( 4 + __PBP__ * 2)) AND ( __PBP__ > 0 )" );\
                         	asm("NOP\n\tENDIF");\
                         asm("IF ("___mkstr1(varname) ">=6) AND ("___mkstr1(varname) "<( 4 + __PBP__ * 2))  AND ( __PBP__ > 1 )" );\
                         	asm("NOP\n\tENDIF");\
                         asm("IF ("___mkstr1(varname) ">=7) AND ("___mkstr1(varname) "<( 4 + __PBP__ * 2))  AND ( __PBP__ > 1 )" );\
                         	asm("NOP\n\tENDIF");\
                         asm("IF ("___mkstr1(varname) ">=8) AND ("___mkstr1(varname) "<( 4 + __PBP__ * 2))  AND ( __PBP__ > 2 )" );\
                         	asm("NOP\n\tENDIF");\
                         asm("IF ("___mkstr1(varname) "==9)  AND ( __PBP__ > 2 )");\
                         	asm("NOP\n\tENDIF");\
                         asm("IF ("___mkstr1(varname) "< ( 8 + __PBP__ * 2)) AND (" ___mkstr1(varname)">( 3 + __PBP__ * 2))" );\
                            asm("fcall __DELAY3 + ( 7 + __PBP__ * 2) -" ___mkstr1(varname)"\n\tENDIF");\
                         asm("IF ("___mkstr1(varname) "< ( 0404h + __PBP__ * 2)) AND (" ___mkstr1(varname)">( 7 + __PBP__ * 2))");\
                         	asm("MOV A,(" ___mkstr1(varname)"-( 4 + __PBP__ * 2))/4");\
                         	asm("fcall __DELAYX3 + 3 -((" ___mkstr1(varname) "-( 4 + __PBP__ * 2)) MOD 4) \n\tENDIF");\
                         asm("IF ("___mkstr1(varname) "< ( 040bh + __PBP__ * 2)) AND (" ___mkstr1(varname)"> ( 0403h + __PBP__ * 2))");\
                         	asm("MOV A,0 ");\
                         	asm("fcall __DELAYX6 + ( 040ah + __PBP__ * 2) -" ___mkstr1(varname)"\n\tENDIF");\
                         asm("IF ("___mkstr1(varname) " >= ( 040bh + __PBP__ * 2)) AND ((((" ___mkstr1(varname)"-( 9 + __PBP__ * 2))%0402h)/4+1)>256)" );\
							asm("MOV A,("___mkstr1(varname)"-( 9 + __PBP__ * 2))/0402h");\
							asm("MOV _builtin_holtek_delay_2,A");\
							asm("MOV A,0");\
                         	asm("fcall __DELAYY5 +5- (((" ___mkstr1(varname)"-( 9 + __PBP__ * 2))%0402h)%6) \n\tENDIF");\
                         asm("IF ("___mkstr1(varname) " >= ( 040bh + __PBP__ * 2)) AND ((((" ___mkstr1(varname)"-( 9 + __PBP__ * 2))%0402h)/4+1)<=256)" );\
						    asm("MOV A,("___mkstr1(varname)"-( 9 + __PBP__ * 2))/0402h");\
							asm("MOV _builtin_holtek_delay_2,A");\
							asm("MOV A,(("___mkstr1(varname)"-( 9 + __PBP__ * 2))%0402h)/4+1");\
                         	asm("fcall __DELAYY3 +3-(((" ___mkstr1(varname)"-( 9 + __PBP__ * 2))%0402h)%4) \n\tENDIF");}while(0)  

#endif
asm("extern __DELAY3:near");                                         
asm("extern __DELAYX3:near");
asm("extern __DELAYX6:near");
asm("extern __DELAYY5:near");
asm("extern __DELAYY3:near");
asm("extern _builtin_holtek_delay_2:byte");
#endif
