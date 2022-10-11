//	[VERSION] 1.0, 3
// Do not modify manually.

#ifndef _HT45F5541_H_
#define _HT45F5541_H_

#ifndef __DISABLE_HGCC_BUILD_IN__
#include "build-in.h"
#endif

#define DEFINE_ISR(isr_name, vector) void __attribute((interrupt(vector))) isr_name(void)

#define DEFINE_SFR(sfr_type, sfr, addr) static volatile sfr_type sfr __attribute__ ((at(addr)))

typedef unsigned char __sfr_byte;

typedef struct {
	unsigned char __c : 1;
	unsigned char __ac : 1;
	unsigned char __z : 1;
	unsigned char __ov : 1;
	unsigned char __pdf : 1;
	unsigned char __to : 1;
	unsigned char __cz : 1;
	unsigned char __sc : 1;
} __status_bits;

typedef union {
	__status_bits bits;
	__sfr_byte byte;
} __status_type;

typedef struct {
	unsigned char __wrf : 1;
	unsigned char : 1;
	unsigned char __lvrf : 1;
	unsigned char : 5;
} __rstfc_bits;

typedef union {
	__rstfc_bits bits;
	__sfr_byte byte;
} __rstfc_type;

typedef struct {
	unsigned char __tb00 : 1;
	unsigned char __tb01 : 1;
	unsigned char __tb02 : 1;
	unsigned char : 4;
	unsigned char __tb0on : 1;
} __tb0c_bits;

typedef union {
	__tb0c_bits bits;
	__sfr_byte byte;
} __tb0c_type;

typedef struct {
	unsigned char __tb10 : 1;
	unsigned char __tb11 : 1;
	unsigned char __tb12 : 1;
	unsigned char : 4;
	unsigned char __tb1on : 1;
} __tb1c_bits;

typedef union {
	__tb1c_bits bits;
	__sfr_byte byte;
} __tb1c_type;

typedef struct {
	unsigned char __fsiden : 1;
	unsigned char __fhiden : 1;
	unsigned char : 3;
	unsigned char __cks0 : 1;
	unsigned char __cks1 : 1;
	unsigned char __cks2 : 1;
} __scc_bits;

typedef union {
	__scc_bits bits;
	__sfr_byte byte;
} __scc_type;

typedef struct {
	unsigned char __hircen : 1;
	unsigned char __hircf : 1;
	unsigned char __hirc0 : 1;
	unsigned char __hirc1 : 1;
	unsigned char : 4;
} __hircc_bits;

typedef union {
	__hircc_bits bits;
	__sfr_byte byte;
} __hircc_type;

typedef struct {
	unsigned char __pa0 : 1;
	unsigned char __pa1 : 1;
	unsigned char __pa2 : 1;
	unsigned char __pa3 : 1;
	unsigned char __pa4 : 1;
	unsigned char __pa5 : 1;
	unsigned char __pa6 : 1;
	unsigned char __pa7 : 1;
} __pa_bits;

typedef union {
	__pa_bits bits;
	__sfr_byte byte;
} __pa_type;

typedef struct {
	unsigned char __pac0 : 1;
	unsigned char __pac1 : 1;
	unsigned char __pac2 : 1;
	unsigned char __pac3 : 1;
	unsigned char __pac4 : 1;
	unsigned char __pac5 : 1;
	unsigned char __pac6 : 1;
	unsigned char __pac7 : 1;
} __pac_bits;

typedef union {
	__pac_bits bits;
	__sfr_byte byte;
} __pac_type;

typedef struct {
	unsigned char __papu0 : 1;
	unsigned char __papu1 : 1;
	unsigned char __papu2 : 1;
	unsigned char __papu3 : 1;
	unsigned char __papu4 : 1;
	unsigned char __papu5 : 1;
	unsigned char __papu6 : 1;
	unsigned char __papu7 : 1;
} __papu_bits;

typedef union {
	__papu_bits bits;
	__sfr_byte byte;
} __papu_type;

typedef struct {
	unsigned char __pawu0 : 1;
	unsigned char __pawu1 : 1;
	unsigned char __pawu2 : 1;
	unsigned char __pawu3 : 1;
	unsigned char __pawu4 : 1;
	unsigned char __pawu5 : 1;
	unsigned char __pawu6 : 1;
	unsigned char __pawu7 : 1;
} __pawu_bits;

typedef union {
	__pawu_bits bits;
	__sfr_byte byte;
} __pawu_type;

typedef struct {
	unsigned char __pb0 : 1;
	unsigned char __pb1 : 1;
	unsigned char __pb2 : 1;
	unsigned char __pb3 : 1;
	unsigned char __pb4 : 1;
	unsigned char : 3;
} __pb_bits;

typedef union {
	__pb_bits bits;
	__sfr_byte byte;
} __pb_type;

typedef struct {
	unsigned char __pbc0 : 1;
	unsigned char __pbc1 : 1;
	unsigned char __pbc2 : 1;
	unsigned char __pbc3 : 1;
	unsigned char __pbc4 : 1;
	unsigned char : 3;
} __pbc_bits;

typedef union {
	__pbc_bits bits;
	__sfr_byte byte;
} __pbc_type;

typedef struct {
	unsigned char __pbpu0 : 1;
	unsigned char __pbpu1 : 1;
	unsigned char __pbpu2 : 1;
	unsigned char __pbpu3 : 1;
	unsigned char __pbpu4 : 1;
	unsigned char : 3;
} __pbpu_bits;

typedef union {
	__pbpu_bits bits;
	__sfr_byte byte;
} __pbpu_type;

typedef struct {
	unsigned char __pc0 : 1;
	unsigned char __pc1 : 1;
	unsigned char __pc2 : 1;
	unsigned char __pc3 : 1;
	unsigned char __pc4 : 1;
	unsigned char : 3;
} __pc_bits;

typedef union {
	__pc_bits bits;
	__sfr_byte byte;
} __pc_type;

typedef struct {
	unsigned char __pcc0 : 1;
	unsigned char __pcc1 : 1;
	unsigned char __pcc2 : 1;
	unsigned char __pcc3 : 1;
	unsigned char __pcc4 : 1;
	unsigned char : 3;
} __pcc_bits;

typedef union {
	__pcc_bits bits;
	__sfr_byte byte;
} __pcc_type;

typedef struct {
	unsigned char __pcpu0 : 1;
	unsigned char __pcpu1 : 1;
	unsigned char __pcpu2 : 1;
	unsigned char __pcpu3 : 1;
	unsigned char __pcpu4 : 1;
	unsigned char : 3;
} __pcpu_bits;

typedef union {
	__pcpu_bits bits;
	__sfr_byte byte;
} __pcpu_type;

typedef struct {
	unsigned char __sledc00 : 1;
	unsigned char __sledc01 : 1;
	unsigned char __sledc02 : 1;
	unsigned char __sledc03 : 1;
	unsigned char __sledc04 : 1;
	unsigned char __sledc05 : 1;
	unsigned char __sledc06 : 1;
	unsigned char __sledc07 : 1;
} __sledc0_bits;

typedef union {
	__sledc0_bits bits;
	__sfr_byte byte;
} __sledc0_type;

typedef struct {
	unsigned char __sledc10 : 1;
	unsigned char __sledc11 : 1;
	unsigned char __sledc12 : 1;
	unsigned char __sledc13 : 1;
	unsigned char : 4;
} __sledc1_bits;

typedef union {
	__sledc1_bits bits;
	__sfr_byte byte;
} __sledc1_type;

typedef struct {
	unsigned char __clksel0 : 1;
	unsigned char __clksel1 : 1;
	unsigned char : 6;
} __pscr_bits;

typedef union {
	__pscr_bits bits;
	__sfr_byte byte;
} __pscr_type;

typedef struct {
	unsigned char __vlvd0 : 1;
	unsigned char __vlvd1 : 1;
	unsigned char __vlvd2 : 1;
	unsigned char __vbgen : 1;
	unsigned char __lvden : 1;
	unsigned char __lvdo : 1;
	unsigned char : 2;
} __lvdc_bits;

typedef union {
	__lvdc_bits bits;
	__sfr_byte byte;
} __lvdc_type;

typedef struct {
	unsigned char __ormc0 : 1;
	unsigned char __ormc1 : 1;
	unsigned char __ormc2 : 1;
	unsigned char __ormc3 : 1;
	unsigned char __ormc4 : 1;
	unsigned char __ormc5 : 1;
	unsigned char __ormc6 : 1;
	unsigned char __ormc7 : 1;
} __ormc_bits;

typedef union {
	__ormc_bits bits;
	__sfr_byte byte;
} __ormc_type;

typedef struct {
	unsigned char __ct0rp0 : 1;
	unsigned char __ct0rp1 : 1;
	unsigned char __ct0rp2 : 1;
	unsigned char __ct0on : 1;
	unsigned char __ct0ck0 : 1;
	unsigned char __ct0ck1 : 1;
	unsigned char __ct0ck2 : 1;
	unsigned char __ct0pau : 1;
} __ctm0c0_bits;

typedef union {
	__ctm0c0_bits bits;
	__sfr_byte byte;
} __ctm0c0_type;

typedef struct {
	unsigned char __ct0cclr : 1;
	unsigned char __ct0dpx : 1;
	unsigned char __ct0pol : 1;
	unsigned char __ct0oc : 1;
	unsigned char __ct0io0 : 1;
	unsigned char __ct0io1 : 1;
	unsigned char __ct0m0 : 1;
	unsigned char __ct0m1 : 1;
} __ctm0c1_bits;

typedef union {
	__ctm0c1_bits bits;
	__sfr_byte byte;
} __ctm0c1_type;

typedef struct {
	unsigned char __sacs0 : 1;
	unsigned char __sacs1 : 1;
	unsigned char __sacs2 : 1;
	unsigned char __sacs3 : 1;
	unsigned char __adrfs : 1;
	unsigned char __adcen : 1;
	unsigned char __adbz : 1;
	unsigned char __start : 1;
} __sadc0_bits;

typedef union {
	__sadc0_bits bits;
	__sfr_byte byte;
} __sadc0_type;

typedef struct {
	unsigned char __sacks0 : 1;
	unsigned char __sacks1 : 1;
	unsigned char __sacks2 : 1;
	unsigned char __savrs0 : 1;
	unsigned char __savrs1 : 1;
	unsigned char __sains0 : 1;
	unsigned char __sains1 : 1;
	unsigned char __sains2 : 1;
} __sadc1_bits;

typedef union {
	__sadc1_bits bits;
	__sfr_byte byte;
} __sadc1_type;

typedef struct {
	unsigned char __plts0 : 1;
	unsigned char __plts1 : 1;
	unsigned char __plts2 : 1;
	unsigned char : 5;
} __pltsw_bits;

typedef union {
	__pltsw_bits bits;
	__sfr_byte byte;
} __pltsw_type;

typedef struct {
	unsigned char __pltdac0en : 1;
	unsigned char __pltdac1en : 1;
	unsigned char __pltdac2en : 1;
	unsigned char : 5;
} __pltdacc_bits;

typedef union {
	__pltdacc_bits bits;
	__sfr_byte byte;
} __pltdacc_type;

typedef struct {
	unsigned char __pltc0is0 : 1;
	unsigned char __pltc0is1 : 1;
	unsigned char __pltc0deb0 : 1;
	unsigned char __pltc0deb1 : 1;
	unsigned char : 1;
	unsigned char __pltc0o : 1;
	unsigned char __pltc0en : 1;
	unsigned char __pltc0out : 1;
} __pltc0c_bits;

typedef union {
	__pltc0c_bits bits;
	__sfr_byte byte;
} __pltc0c_type;

typedef struct {
	unsigned char __pltc0of0 : 1;
	unsigned char __pltc0of1 : 1;
	unsigned char __pltc0of2 : 1;
	unsigned char __pltc0of3 : 1;
	unsigned char __pltc0of4 : 1;
	unsigned char __pltc0rsp : 1;
	unsigned char __pltc0ofm : 1;
	unsigned char : 1;
} __pltc0vos_bits;

typedef union {
	__pltc0vos_bits bits;
	__sfr_byte byte;
} __pltc0vos_type;

typedef struct {
	unsigned char __pltc1is0 : 1;
	unsigned char __pltc1is1 : 1;
	unsigned char __pltc1deb0 : 1;
	unsigned char __pltc1deb1 : 1;
	unsigned char : 1;
	unsigned char __pltc1o : 1;
	unsigned char __pltc1en : 1;
	unsigned char __pltc1out : 1;
} __pltc1c_bits;

typedef union {
	__pltc1c_bits bits;
	__sfr_byte byte;
} __pltc1c_type;

typedef struct {
	unsigned char __pltc1of0 : 1;
	unsigned char __pltc1of1 : 1;
	unsigned char __pltc1of2 : 1;
	unsigned char __pltc1of3 : 1;
	unsigned char __pltc1of4 : 1;
	unsigned char __pltc1rsp : 1;
	unsigned char __pltc1ofm : 1;
	unsigned char : 1;
} __pltc1vos_bits;

typedef union {
	__pltc1vos_bits bits;
	__sfr_byte byte;
} __pltc1vos_type;

typedef struct {
	unsigned char __pltc0hys0 : 1;
	unsigned char __pltc0hys1 : 1;
	unsigned char __pltc1hys0 : 1;
	unsigned char __pltc1hys1 : 1;
	unsigned char __pltc0pol : 1;
	unsigned char __pltc1pol : 1;
	unsigned char __pltcxosw : 1;
	unsigned char : 1;
} __pltchyc_bits;

typedef union {
	__pltchyc_bits bits;
	__sfr_byte byte;
} __pltchyc_type;

typedef struct {
	unsigned char __pltabw : 1;
	unsigned char : 4;
	unsigned char __pltao : 1;
	unsigned char __pltaen : 1;
	unsigned char : 1;
} __pltac_bits;

typedef union {
	__pltac_bits bits;
	__sfr_byte byte;
} __pltac_type;

typedef struct {
	unsigned char __pltaof0 : 1;
	unsigned char __pltaof1 : 1;
	unsigned char __pltaof2 : 1;
	unsigned char __pltaof3 : 1;
	unsigned char __pltaof4 : 1;
	unsigned char __pltaof5 : 1;
	unsigned char __pltarsp : 1;
	unsigned char __pltaofm : 1;
} __pltavos_bits;

typedef union {
	__pltavos_bits bits;
	__sfr_byte byte;
} __pltavos_type;

typedef struct {
	unsigned char __ws0 : 1;
	unsigned char __ws1 : 1;
	unsigned char __ws2 : 1;
	unsigned char __we0 : 1;
	unsigned char __we1 : 1;
	unsigned char __we2 : 1;
	unsigned char __we3 : 1;
	unsigned char __we4 : 1;
} __wdtc_bits;

typedef union {
	__wdtc_bits bits;
	__sfr_byte byte;
} __wdtc_type;

typedef struct {
	unsigned char __eea0 : 1;
	unsigned char __eea1 : 1;
	unsigned char __eea2 : 1;
	unsigned char __eea3 : 1;
	unsigned char __eea4 : 1;
	unsigned char __eea5 : 1;
	unsigned char : 2;
} __eea_bits;

typedef union {
	__eea_bits bits;
	__sfr_byte byte;
} __eea_type;

typedef struct {
	unsigned char __eed0 : 1;
	unsigned char __eed1 : 1;
	unsigned char __eed2 : 1;
	unsigned char __eed3 : 1;
	unsigned char __eed4 : 1;
	unsigned char __eed5 : 1;
	unsigned char __eed6 : 1;
	unsigned char __eed7 : 1;
} __eed_bits;

typedef union {
	__eed_bits bits;
	__sfr_byte byte;
} __eed_type;

typedef struct {
	unsigned char __txif : 1;
	unsigned char __tidle : 1;
	unsigned char __rxif : 1;
	unsigned char __ridle : 1;
	unsigned char __oerr : 1;
	unsigned char __ferr : 1;
	unsigned char __nf : 1;
	unsigned char __perr : 1;
} __usr_bits;

typedef union {
	__usr_bits bits;
	__sfr_byte byte;
} __usr_type;

typedef struct {
	unsigned char __tx8 : 1;
	unsigned char __rx8 : 1;
	unsigned char __txbrk : 1;
	unsigned char __stops : 1;
	unsigned char __prt : 1;
	unsigned char __pren : 1;
	unsigned char __bno : 1;
	unsigned char __uarten : 1;
} __ucr1_bits;

typedef union {
	__ucr1_bits bits;
	__sfr_byte byte;
} __ucr1_type;

typedef struct {
	unsigned char __teie : 1;
	unsigned char __tiie : 1;
	unsigned char __rie : 1;
	unsigned char __wake : 1;
	unsigned char __adden : 1;
	unsigned char __brgh : 1;
	unsigned char __rxen : 1;
	unsigned char __txen : 1;
} __ucr2_bits;

typedef union {
	__ucr2_bits bits;
	__sfr_byte byte;
} __ucr2_type;

typedef struct {
	unsigned char __swm : 1;
	unsigned char : 7;
} __ucr3_bits;

typedef union {
	__ucr3_bits bits;
	__sfr_byte byte;
} __ucr3_type;

typedef struct {
	unsigned char __txrx0 : 1;
	unsigned char __txrx1 : 1;
	unsigned char __txrx2 : 1;
	unsigned char __txrx3 : 1;
	unsigned char __txrx4 : 1;
	unsigned char __txrx5 : 1;
	unsigned char __txrx6 : 1;
	unsigned char __txrx7 : 1;
} __txr_rxr_bits;

typedef union {
	__txr_rxr_bits bits;
	__sfr_byte byte;
} __txr_rxr_type;

typedef struct {
	unsigned char __brg0 : 1;
	unsigned char __brg1 : 1;
	unsigned char __brg2 : 1;
	unsigned char __brg3 : 1;
	unsigned char __brg4 : 1;
	unsigned char __brg5 : 1;
	unsigned char __brg6 : 1;
	unsigned char __brg7 : 1;
} __brg_bits;

typedef union {
	__brg_bits bits;
	__sfr_byte byte;
} __brg_type;

typedef struct {
	unsigned char __int0s0 : 1;
	unsigned char __int0s1 : 1;
	unsigned char __int1s0 : 1;
	unsigned char __int1s1 : 1;
	unsigned char : 4;
} __integ_bits;

typedef union {
	__integ_bits bits;
	__sfr_byte byte;
} __integ_type;

typedef struct {
	unsigned char __emi : 1;
	unsigned char __pltc0e : 1;
	unsigned char __int0e : 1;
	unsigned char __int1e : 1;
	unsigned char __pltc0f : 1;
	unsigned char __int0f : 1;
	unsigned char __int1f : 1;
	unsigned char : 1;
} __intc0_bits;

typedef union {
	__intc0_bits bits;
	__sfr_byte byte;
} __intc0_type;

typedef struct {
	unsigned char __ure : 1;
	unsigned char __lve : 1;
	unsigned char __mf0e : 1;
	unsigned char __mf1e : 1;
	unsigned char __urf : 1;
	unsigned char __lvf : 1;
	unsigned char __mf0f : 1;
	unsigned char __mf1f : 1;
} __intc1_bits;

typedef union {
	__intc1_bits bits;
	__sfr_byte byte;
} __intc1_type;

typedef struct {
	unsigned char __mf2e : 1;
	unsigned char __mf3e : 1;
	unsigned char __mf4e : 1;
	unsigned char __tb0e : 1;
	unsigned char __mf2f : 1;
	unsigned char __mf3f : 1;
	unsigned char __mf4f : 1;
	unsigned char __tb0f : 1;
} __intc2_bits;

typedef union {
	__intc2_bits bits;
	__sfr_byte byte;
} __intc2_type;

typedef struct {
	unsigned char __tb1e : 1;
	unsigned char __pltc1e : 1;
	unsigned char : 2;
	unsigned char __tb1f : 1;
	unsigned char __pltc1f : 1;
	unsigned char : 2;
} __intc3_bits;

typedef union {
	__intc3_bits bits;
	__sfr_byte byte;
} __intc3_type;

typedef struct {
	unsigned char __ade : 1;
	unsigned char __dee : 1;
	unsigned char : 2;
	unsigned char __adf : 1;
	unsigned char __def : 1;
	unsigned char : 2;
} __mfi0_bits;

typedef union {
	__mfi0_bits bits;
	__sfr_byte byte;
} __mfi0_type;

typedef struct {
	unsigned char __ptm0pe : 1;
	unsigned char __ptm0ae : 1;
	unsigned char : 2;
	unsigned char __ptm0pf : 1;
	unsigned char __ptm0af : 1;
	unsigned char : 2;
} __mfi1_bits;

typedef union {
	__mfi1_bits bits;
	__sfr_byte byte;
} __mfi1_type;

typedef struct {
	unsigned char __ptm1pe : 1;
	unsigned char __ptm1ae : 1;
	unsigned char : 2;
	unsigned char __ptm1pf : 1;
	unsigned char __ptm1af : 1;
	unsigned char : 2;
} __mfi2_bits;

typedef union {
	__mfi2_bits bits;
	__sfr_byte byte;
} __mfi2_type;

typedef struct {
	unsigned char __ctm0pe : 1;
	unsigned char __ctm0ae : 1;
	unsigned char : 2;
	unsigned char __ctm0pf : 1;
	unsigned char __ctm0af : 1;
	unsigned char : 2;
} __mfi3_bits;

typedef union {
	__mfi3_bits bits;
	__sfr_byte byte;
} __mfi3_type;

typedef struct {
	unsigned char __ctm1pe : 1;
	unsigned char __ctm1ae : 1;
	unsigned char : 2;
	unsigned char __ctm1pf : 1;
	unsigned char __ctm1af : 1;
	unsigned char : 2;
} __mfi4_bits;

typedef union {
	__mfi4_bits bits;
	__sfr_byte byte;
} __mfi4_type;

typedef struct {
	unsigned char : 3;
	unsigned char __pt0on : 1;
	unsigned char __pt0ck0 : 1;
	unsigned char __pt0ck1 : 1;
	unsigned char __pt0ck2 : 1;
	unsigned char __pt0pau : 1;
} __ptm0c0_bits;

typedef union {
	__ptm0c0_bits bits;
	__sfr_byte byte;
} __ptm0c0_type;

typedef struct {
	unsigned char __pt0cclr : 1;
	unsigned char __pt0capts : 1;
	unsigned char __pt0pol : 1;
	unsigned char __pt0oc : 1;
	unsigned char __pt0io0 : 1;
	unsigned char __pt0io1 : 1;
	unsigned char __pt0m0 : 1;
	unsigned char __pt0m1 : 1;
} __ptm0c1_bits;

typedef union {
	__ptm0c1_bits bits;
	__sfr_byte byte;
} __ptm0c1_type;

typedef struct {
	unsigned char __pt0vlf : 1;
	unsigned char __pt0tclr0 : 1;
	unsigned char __pt0tclr1 : 1;
	unsigned char : 5;
} __ptm0c2_bits;

typedef union {
	__ptm0c2_bits bits;
	__sfr_byte byte;
} __ptm0c2_type;

typedef struct {
	unsigned char : 3;
	unsigned char __pt1on : 1;
	unsigned char __pt1ck0 : 1;
	unsigned char __pt1ck1 : 1;
	unsigned char __pt1ck2 : 1;
	unsigned char __pt1pau : 1;
} __ptm1c0_bits;

typedef union {
	__ptm1c0_bits bits;
	__sfr_byte byte;
} __ptm1c0_type;

typedef struct {
	unsigned char __pt1cclr : 1;
	unsigned char __pt1capts : 1;
	unsigned char __pt1pol : 1;
	unsigned char __pt1oc : 1;
	unsigned char __pt1io0 : 1;
	unsigned char __pt1io1 : 1;
	unsigned char __pt1m0 : 1;
	unsigned char __pt1m1 : 1;
} __ptm1c1_bits;

typedef union {
	__ptm1c1_bits bits;
	__sfr_byte byte;
} __ptm1c1_type;

typedef struct {
	unsigned char __ct1rp0 : 1;
	unsigned char __ct1rp1 : 1;
	unsigned char __ct1rp2 : 1;
	unsigned char __ct1on : 1;
	unsigned char __ct1ck0 : 1;
	unsigned char __ct1ck1 : 1;
	unsigned char __ct1ck2 : 1;
	unsigned char __ct1pau : 1;
} __ctm1c0_bits;

typedef union {
	__ctm1c0_bits bits;
	__sfr_byte byte;
} __ctm1c0_type;

typedef struct {
	unsigned char __ct1cclr : 1;
	unsigned char __ct1dpx : 1;
	unsigned char __ct1pol : 1;
	unsigned char __ct1oc : 1;
	unsigned char __ct1io0 : 1;
	unsigned char __ct1io1 : 1;
	unsigned char __ct1m0 : 1;
	unsigned char __ct1m1 : 1;
} __ctm1c1_bits;

typedef union {
	__ctm1c1_bits bits;
	__sfr_byte byte;
} __ctm1c1_type;

typedef struct {
	unsigned char __pas00 : 1;
	unsigned char __pas01 : 1;
	unsigned char __pas02 : 1;
	unsigned char __pas03 : 1;
	unsigned char __pas04 : 1;
	unsigned char __pas05 : 1;
	unsigned char __pas06 : 1;
	unsigned char __pas07 : 1;
} __pas0_bits;

typedef union {
	__pas0_bits bits;
	__sfr_byte byte;
} __pas0_type;

typedef struct {
	unsigned char __pas10 : 1;
	unsigned char __pas11 : 1;
	unsigned char __pas12 : 1;
	unsigned char __pas13 : 1;
	unsigned char __pas14 : 1;
	unsigned char __pas15 : 1;
	unsigned char __pas16 : 1;
	unsigned char __pas17 : 1;
} __pas1_bits;

typedef union {
	__pas1_bits bits;
	__sfr_byte byte;
} __pas1_type;

typedef struct {
	unsigned char __pbs00 : 1;
	unsigned char __pbs01 : 1;
	unsigned char __pbs02 : 1;
	unsigned char __pbs03 : 1;
	unsigned char __pbs04 : 1;
	unsigned char __pbs05 : 1;
	unsigned char __pbs06 : 1;
	unsigned char __pbs07 : 1;
} __pbs0_bits;

typedef union {
	__pbs0_bits bits;
	__sfr_byte byte;
} __pbs0_type;

typedef struct {
	unsigned char __pcs00 : 1;
	unsigned char __pcs01 : 1;
	unsigned char __pcs02 : 1;
	unsigned char __pcs03 : 1;
	unsigned char __pcs04 : 1;
	unsigned char __pcs05 : 1;
	unsigned char __pcs06 : 1;
	unsigned char __pcs07 : 1;
} __pcs0_bits;

typedef union {
	__pcs0_bits bits;
	__sfr_byte byte;
} __pcs0_type;

typedef struct {
	unsigned char __ifs0 : 1;
	unsigned char __ifs1 : 1;
	unsigned char __ifs2 : 1;
	unsigned char __ifs3 : 1;
	unsigned char : 4;
} __ifs_bits;

typedef union {
	__ifs_bits bits;
	__sfr_byte byte;
} __ifs_type;

typedef struct {
	unsigned char __rd : 1;
	unsigned char __rden : 1;
	unsigned char __wr : 1;
	unsigned char __wren : 1;
	unsigned char : 4;
} __eec_bits;

typedef union {
	__eec_bits bits;
	__sfr_byte byte;
} __eec_type;


DEFINE_SFR(__sfr_byte, __mp0, 0x01);
DEFINE_SFR(__sfr_byte, __mp1l, 0x03);
DEFINE_SFR(__sfr_byte, __mp1h, 0x04);
DEFINE_SFR(__sfr_byte, __acc, 0x05);
DEFINE_SFR(__sfr_byte, __pcl, 0x06);
DEFINE_SFR(__sfr_byte, __tblp, 0x07);
DEFINE_SFR(__sfr_byte, __tblh, 0x08);
DEFINE_SFR(__sfr_byte, __tbhp, 0x09);
DEFINE_SFR(__status_type, __status, 0x0a);
DEFINE_SFR(__sfr_byte, __mp2l, 0x0d);
DEFINE_SFR(__sfr_byte, __mp2h, 0x0e);
DEFINE_SFR(__rstfc_type, __rstfc, 0x0f);
DEFINE_SFR(__tb0c_type, __tb0c, 0x10);
DEFINE_SFR(__tb1c_type, __tb1c, 0x11);
DEFINE_SFR(__scc_type, __scc, 0x12);
DEFINE_SFR(__hircc_type, __hircc, 0x13);
DEFINE_SFR(__pa_type, __pa, 0x14);
DEFINE_SFR(__pac_type, __pac, 0x15);
DEFINE_SFR(__papu_type, __papu, 0x16);
DEFINE_SFR(__pawu_type, __pawu, 0x17);
DEFINE_SFR(__pb_type, __pb, 0x18);
DEFINE_SFR(__pbc_type, __pbc, 0x19);
DEFINE_SFR(__pbpu_type, __pbpu, 0x1a);
DEFINE_SFR(__pc_type, __pc, 0x1b);
DEFINE_SFR(__pcc_type, __pcc, 0x1c);
DEFINE_SFR(__pcpu_type, __pcpu, 0x1d);
DEFINE_SFR(__sledc0_type, __sledc0, 0x1e);
DEFINE_SFR(__sledc1_type, __sledc1, 0x1f);
DEFINE_SFR(__pscr_type, __pscr, 0x20);
DEFINE_SFR(__lvdc_type, __lvdc, 0x21);
DEFINE_SFR(__ormc_type, __ormc, 0x23);
DEFINE_SFR(__ctm0c0_type, __ctm0c0, 0x27);
DEFINE_SFR(__ctm0c1_type, __ctm0c1, 0x28);
DEFINE_SFR(__sfr_byte, __ctm0dl, 0x29);
DEFINE_SFR(__sfr_byte, __ctm0dh, 0x2a);
DEFINE_SFR(__sfr_byte, __ctm0al, 0x2b);
DEFINE_SFR(__sfr_byte, __ctm0ah, 0x2c);
DEFINE_SFR(__sfr_byte, __sadoh, 0x2d);
DEFINE_SFR(__sfr_byte, __sadol, 0x2e);
DEFINE_SFR(__sadc0_type, __sadc0, 0x2f);
DEFINE_SFR(__sadc1_type, __sadc1, 0x30);
DEFINE_SFR(__pltsw_type, __pltsw, 0x31);
DEFINE_SFR(__pltdacc_type, __pltdacc, 0x32);
DEFINE_SFR(__sfr_byte, __pltda0l, 0x33);
DEFINE_SFR(__sfr_byte, __pltda1l, 0x34);
DEFINE_SFR(__sfr_byte, __pltda2l, 0x35);
DEFINE_SFR(__pltc0c_type, __pltc0c, 0x36);
DEFINE_SFR(__pltc0vos_type, __pltc0vos, 0x37);
DEFINE_SFR(__pltc1c_type, __pltc1c, 0x38);
DEFINE_SFR(__pltc1vos_type, __pltc1vos, 0x39);
DEFINE_SFR(__pltchyc_type, __pltchyc, 0x3a);
DEFINE_SFR(__pltac_type, __pltac, 0x3b);
DEFINE_SFR(__pltavos_type, __pltavos, 0x3c);
DEFINE_SFR(__wdtc_type, __wdtc, 0x3d);
DEFINE_SFR(__eea_type, __eea, 0x3e);
DEFINE_SFR(__eed_type, __eed, 0x3f);
DEFINE_SFR(__usr_type, __usr, 0x41);
DEFINE_SFR(__ucr1_type, __ucr1, 0x42);
DEFINE_SFR(__ucr2_type, __ucr2, 0x43);
DEFINE_SFR(__ucr3_type, __ucr3, 0x44);
DEFINE_SFR(__txr_rxr_type, __txr_rxr, 0x45);
DEFINE_SFR(__brg_type, __brg, 0x46);
DEFINE_SFR(__integ_type, __integ, 0x47);
DEFINE_SFR(__intc0_type, __intc0, 0x48);
DEFINE_SFR(__intc1_type, __intc1, 0x49);
DEFINE_SFR(__intc2_type, __intc2, 0x4a);
DEFINE_SFR(__intc3_type, __intc3, 0x4b);
DEFINE_SFR(__mfi0_type, __mfi0, 0x4c);
DEFINE_SFR(__mfi1_type, __mfi1, 0x4d);
DEFINE_SFR(__mfi2_type, __mfi2, 0x4e);
DEFINE_SFR(__mfi3_type, __mfi3, 0x4f);
DEFINE_SFR(__mfi4_type, __mfi4, 0x50);
DEFINE_SFR(__ptm0c0_type, __ptm0c0, 0x52);
DEFINE_SFR(__ptm0c1_type, __ptm0c1, 0x53);
DEFINE_SFR(__ptm0c2_type, __ptm0c2, 0x54);
DEFINE_SFR(__sfr_byte, __ptm0dl, 0x55);
DEFINE_SFR(__sfr_byte, __ptm0dh, 0x56);
DEFINE_SFR(__sfr_byte, __ptm0al, 0x57);
DEFINE_SFR(__sfr_byte, __ptm0ah, 0x58);
DEFINE_SFR(__sfr_byte, __ptm0bl, 0x59);
DEFINE_SFR(__sfr_byte, __ptm0bh, 0x5a);
DEFINE_SFR(__sfr_byte, __ptm0rpl, 0x5b);
DEFINE_SFR(__sfr_byte, __ptm0rph, 0x5c);
DEFINE_SFR(__ptm1c0_type, __ptm1c0, 0x5d);
DEFINE_SFR(__ptm1c1_type, __ptm1c1, 0x5e);
DEFINE_SFR(__sfr_byte, __ptm1dl, 0x5f);
DEFINE_SFR(__sfr_byte, __ptm1dh, 0x60);
DEFINE_SFR(__sfr_byte, __ptm1al, 0x61);
DEFINE_SFR(__sfr_byte, __ptm1ah, 0x62);
DEFINE_SFR(__sfr_byte, __ptm1rpl, 0x63);
DEFINE_SFR(__sfr_byte, __ptm1rph, 0x64);
DEFINE_SFR(__ctm1c0_type, __ctm1c0, 0x65);
DEFINE_SFR(__ctm1c1_type, __ctm1c1, 0x66);
DEFINE_SFR(__sfr_byte, __ctm1dl, 0x67);
DEFINE_SFR(__sfr_byte, __ctm1dh, 0x68);
DEFINE_SFR(__sfr_byte, __ctm1al, 0x69);
DEFINE_SFR(__sfr_byte, __ctm1ah, 0x6a);
DEFINE_SFR(__pas0_type, __pas0, 0x6b);
DEFINE_SFR(__pas1_type, __pas1, 0x6c);
DEFINE_SFR(__pbs0_type, __pbs0, 0x6d);
DEFINE_SFR(__pcs0_type, __pcs0, 0x6e);
DEFINE_SFR(__ifs_type, __ifs, 0x70);
DEFINE_SFR(__eec_type, __eec, 0x140);
DEFINE_SFR(__sfr_byte, __iar0, 0x00);
DEFINE_SFR(__sfr_byte, __iar1, 0x02);
DEFINE_SFR(__sfr_byte, __iar2, 0x0C);

#define _iar0     __iar0
#define _iar1     __iar1
#define _iar2     __iar2
#define _mp0      __mp0
#define _mp1l     __mp1l
#define _mp1h     __mp1h
#define _acc      __acc
#define _pcl      __pcl
#define _tblp     __tblp
#define _tblh     __tblh
#define _tbhp     __tbhp
#define _status   __status.byte
#define _mp2l     __mp2l
#define _mp2h     __mp2h
#define _rstfc    __rstfc.byte
#define _tb0c     __tb0c.byte
#define _tb1c     __tb1c.byte
#define _scc      __scc.byte
#define _hircc    __hircc.byte
#define _pa       __pa.byte
#define _pac      __pac.byte
#define _papu     __papu.byte
#define _pawu     __pawu.byte
#define _pb       __pb.byte
#define _pbc      __pbc.byte
#define _pbpu     __pbpu.byte
#define _pc       __pc.byte
#define _pcc      __pcc.byte
#define _pcpu     __pcpu.byte
#define _sledc0   __sledc0.byte
#define _sledc1   __sledc1.byte
#define _pscr     __pscr.byte
#define _lvdc     __lvdc.byte
#define _ormc     __ormc.byte
#define _ctm0c0   __ctm0c0.byte
#define _ctm0c1   __ctm0c1.byte
#define _ctm0dl   __ctm0dl
#define _ctm0dh   __ctm0dh
#define _ctm0al   __ctm0al
#define _ctm0ah   __ctm0ah
#define _sadoh    __sadoh
#define _sadol    __sadol
#define _sadc0    __sadc0.byte
#define _sadc1    __sadc1.byte
#define _pltsw    __pltsw.byte
#define _pltdacc  __pltdacc.byte
#define _pltda0l  __pltda0l
#define _pltda1l  __pltda1l
#define _pltda2l  __pltda2l
#define _pltc0c   __pltc0c.byte
#define _pltc0vos __pltc0vos.byte
#define _pltc1c   __pltc1c.byte
#define _pltc1vos __pltc1vos.byte
#define _pltchyc  __pltchyc.byte
#define _pltac    __pltac.byte
#define _pltavos  __pltavos.byte
#define _wdtc     __wdtc.byte
#define _eea      __eea.byte
#define _eed      __eed.byte
#define _usr      __usr.byte
#define _ucr1     __ucr1.byte
#define _ucr2     __ucr2.byte
#define _ucr3     __ucr3.byte
#define _txr_rxr  __txr_rxr.byte
#define _brg      __brg.byte
#define _integ    __integ.byte
#define _intc0    __intc0.byte
#define _intc1    __intc1.byte
#define _intc2    __intc2.byte
#define _intc3    __intc3.byte
#define _mfi0     __mfi0.byte
#define _mfi1     __mfi1.byte
#define _mfi2     __mfi2.byte
#define _mfi3     __mfi3.byte
#define _mfi4     __mfi4.byte
#define _ptm0c0   __ptm0c0.byte
#define _ptm0c1   __ptm0c1.byte
#define _ptm0c2   __ptm0c2.byte
#define _ptm0dl   __ptm0dl
#define _ptm0dh   __ptm0dh
#define _ptm0al   __ptm0al
#define _ptm0ah   __ptm0ah
#define _ptm0bl   __ptm0bl
#define _ptm0bh   __ptm0bh
#define _ptm0rpl  __ptm0rpl
#define _ptm0rph  __ptm0rph
#define _ptm1c0   __ptm1c0.byte
#define _ptm1c1   __ptm1c1.byte
#define _ptm1dl   __ptm1dl
#define _ptm1dh   __ptm1dh
#define _ptm1al   __ptm1al
#define _ptm1ah   __ptm1ah
#define _ptm1rpl  __ptm1rpl
#define _ptm1rph  __ptm1rph
#define _ctm1c0   __ctm1c0.byte
#define _ctm1c1   __ctm1c1.byte
#define _ctm1dl   __ctm1dl
#define _ctm1dh   __ctm1dh
#define _ctm1al   __ctm1al
#define _ctm1ah   __ctm1ah
#define _pas0     __pas0.byte
#define _pas1     __pas1.byte
#define _pbs0     __pbs0.byte
#define _pcs0     __pcs0.byte
#define _ifs      __ifs.byte
#define _eec      __eec.byte

#define _c        __status.bits.__c
#define _ac       __status.bits.__ac
#define _z        __status.bits.__z
#define _ov       __status.bits.__ov
#define _pdf      __status.bits.__pdf
#define _to       __status.bits.__to
#define _cz       __status.bits.__cz
#define _sc       __status.bits.__sc
#define _wrf      __rstfc.bits.__wrf
#define _lvrf     __rstfc.bits.__lvrf
#define _tb00     __tb0c.bits.__tb00
#define _tb01     __tb0c.bits.__tb01
#define _tb02     __tb0c.bits.__tb02
#define _tb0on    __tb0c.bits.__tb0on
#define _tb10     __tb1c.bits.__tb10
#define _tb11     __tb1c.bits.__tb11
#define _tb12     __tb1c.bits.__tb12
#define _tb1on    __tb1c.bits.__tb1on
#define _fsiden   __scc.bits.__fsiden
#define _fhiden   __scc.bits.__fhiden
#define _cks0     __scc.bits.__cks0
#define _cks1     __scc.bits.__cks1
#define _cks2     __scc.bits.__cks2
#define _hircen   __hircc.bits.__hircen
#define _hircf    __hircc.bits.__hircf
#define _hirc0    __hircc.bits.__hirc0
#define _hirc1    __hircc.bits.__hirc1
#define _pa0      __pa.bits.__pa0
#define _pa1      __pa.bits.__pa1
#define _pa2      __pa.bits.__pa2
#define _pa3      __pa.bits.__pa3
#define _pa4      __pa.bits.__pa4
#define _pa5      __pa.bits.__pa5
#define _pa6      __pa.bits.__pa6
#define _pa7      __pa.bits.__pa7
#define _pac0     __pac.bits.__pac0
#define _pac1     __pac.bits.__pac1
#define _pac2     __pac.bits.__pac2
#define _pac3     __pac.bits.__pac3
#define _pac4     __pac.bits.__pac4
#define _pac5     __pac.bits.__pac5
#define _pac6     __pac.bits.__pac6
#define _pac7     __pac.bits.__pac7
#define _papu0    __papu.bits.__papu0
#define _papu1    __papu.bits.__papu1
#define _papu2    __papu.bits.__papu2
#define _papu3    __papu.bits.__papu3
#define _papu4    __papu.bits.__papu4
#define _papu5    __papu.bits.__papu5
#define _papu6    __papu.bits.__papu6
#define _papu7    __papu.bits.__papu7
#define _pawu0    __pawu.bits.__pawu0
#define _pawu1    __pawu.bits.__pawu1
#define _pawu2    __pawu.bits.__pawu2
#define _pawu3    __pawu.bits.__pawu3
#define _pawu4    __pawu.bits.__pawu4
#define _pawu5    __pawu.bits.__pawu5
#define _pawu6    __pawu.bits.__pawu6
#define _pawu7    __pawu.bits.__pawu7
#define _pb0      __pb.bits.__pb0
#define _pb1      __pb.bits.__pb1
#define _pb2      __pb.bits.__pb2
#define _pb3      __pb.bits.__pb3
#define _pb4      __pb.bits.__pb4
#define _pbc0     __pbc.bits.__pbc0
#define _pbc1     __pbc.bits.__pbc1
#define _pbc2     __pbc.bits.__pbc2
#define _pbc3     __pbc.bits.__pbc3
#define _pbc4     __pbc.bits.__pbc4
#define _pbpu0    __pbpu.bits.__pbpu0
#define _pbpu1    __pbpu.bits.__pbpu1
#define _pbpu2    __pbpu.bits.__pbpu2
#define _pbpu3    __pbpu.bits.__pbpu3
#define _pbpu4    __pbpu.bits.__pbpu4
#define _pc0      __pc.bits.__pc0
#define _pc1      __pc.bits.__pc1
#define _pc2      __pc.bits.__pc2
#define _pc3      __pc.bits.__pc3
#define _pc4      __pc.bits.__pc4
#define _pcc0     __pcc.bits.__pcc0
#define _pcc1     __pcc.bits.__pcc1
#define _pcc2     __pcc.bits.__pcc2
#define _pcc3     __pcc.bits.__pcc3
#define _pcc4     __pcc.bits.__pcc4
#define _pcpu0    __pcpu.bits.__pcpu0
#define _pcpu1    __pcpu.bits.__pcpu1
#define _pcpu2    __pcpu.bits.__pcpu2
#define _pcpu3    __pcpu.bits.__pcpu3
#define _pcpu4    __pcpu.bits.__pcpu4
#define _sledc00  __sledc0.bits.__sledc00
#define _sledc01  __sledc0.bits.__sledc01
#define _sledc02  __sledc0.bits.__sledc02
#define _sledc03  __sledc0.bits.__sledc03
#define _sledc04  __sledc0.bits.__sledc04
#define _sledc05  __sledc0.bits.__sledc05
#define _sledc06  __sledc0.bits.__sledc06
#define _sledc07  __sledc0.bits.__sledc07
#define _sledc10  __sledc1.bits.__sledc10
#define _sledc11  __sledc1.bits.__sledc11
#define _sledc12  __sledc1.bits.__sledc12
#define _sledc13  __sledc1.bits.__sledc13
#define _clksel0  __pscr.bits.__clksel0
#define _clksel1  __pscr.bits.__clksel1
#define _vlvd0    __lvdc.bits.__vlvd0
#define _vlvd1    __lvdc.bits.__vlvd1
#define _vlvd2    __lvdc.bits.__vlvd2
#define _vbgen    __lvdc.bits.__vbgen
#define _lvden    __lvdc.bits.__lvden
#define _lvdo     __lvdc.bits.__lvdo
#define _ormc0    __ormc.bits.__ormc0
#define _ormc1    __ormc.bits.__ormc1
#define _ormc2    __ormc.bits.__ormc2
#define _ormc3    __ormc.bits.__ormc3
#define _ormc4    __ormc.bits.__ormc4
#define _ormc5    __ormc.bits.__ormc5
#define _ormc6    __ormc.bits.__ormc6
#define _ormc7    __ormc.bits.__ormc7
#define _ct0rp0   __ctm0c0.bits.__ct0rp0
#define _ct0rp1   __ctm0c0.bits.__ct0rp1
#define _ct0rp2   __ctm0c0.bits.__ct0rp2
#define _ct0on    __ctm0c0.bits.__ct0on
#define _ct0ck0   __ctm0c0.bits.__ct0ck0
#define _ct0ck1   __ctm0c0.bits.__ct0ck1
#define _ct0ck2   __ctm0c0.bits.__ct0ck2
#define _ct0pau   __ctm0c0.bits.__ct0pau
#define _ct0cclr  __ctm0c1.bits.__ct0cclr
#define _ct0dpx   __ctm0c1.bits.__ct0dpx
#define _ct0pol   __ctm0c1.bits.__ct0pol
#define _ct0oc    __ctm0c1.bits.__ct0oc
#define _ct0io0   __ctm0c1.bits.__ct0io0
#define _ct0io1   __ctm0c1.bits.__ct0io1
#define _ct0m0    __ctm0c1.bits.__ct0m0
#define _ct0m1    __ctm0c1.bits.__ct0m1
#define _sacs0    __sadc0.bits.__sacs0
#define _sacs1    __sadc0.bits.__sacs1
#define _sacs2    __sadc0.bits.__sacs2
#define _sacs3    __sadc0.bits.__sacs3
#define _adrfs    __sadc0.bits.__adrfs
#define _adcen    __sadc0.bits.__adcen
#define _adbz     __sadc0.bits.__adbz
#define _start    __sadc0.bits.__start
#define _sacks0   __sadc1.bits.__sacks0
#define _sacks1   __sadc1.bits.__sacks1
#define _sacks2   __sadc1.bits.__sacks2
#define _savrs0   __sadc1.bits.__savrs0
#define _savrs1   __sadc1.bits.__savrs1
#define _sains0   __sadc1.bits.__sains0
#define _sains1   __sadc1.bits.__sains1
#define _sains2   __sadc1.bits.__sains2
#define _plts0    __pltsw.bits.__plts0
#define _plts1    __pltsw.bits.__plts1
#define _plts2    __pltsw.bits.__plts2
#define _pltdac0en __pltdacc.bits.__pltdac0en
#define _pltdac1en __pltdacc.bits.__pltdac1en
#define _pltdac2en __pltdacc.bits.__pltdac2en
#define _pltc0is0 __pltc0c.bits.__pltc0is0
#define _pltc0is1 __pltc0c.bits.__pltc0is1
#define _pltc0deb0 __pltc0c.bits.__pltc0deb0
#define _pltc0deb1 __pltc0c.bits.__pltc0deb1
#define _pltc0o   __pltc0c.bits.__pltc0o
#define _pltc0en  __pltc0c.bits.__pltc0en
#define _pltc0out __pltc0c.bits.__pltc0out
#define _pltc0of0 __pltc0vos.bits.__pltc0of0
#define _pltc0of1 __pltc0vos.bits.__pltc0of1
#define _pltc0of2 __pltc0vos.bits.__pltc0of2
#define _pltc0of3 __pltc0vos.bits.__pltc0of3
#define _pltc0of4 __pltc0vos.bits.__pltc0of4
#define _pltc0rsp __pltc0vos.bits.__pltc0rsp
#define _pltc0ofm __pltc0vos.bits.__pltc0ofm
#define _pltc1is0 __pltc1c.bits.__pltc1is0
#define _pltc1is1 __pltc1c.bits.__pltc1is1
#define _pltc1deb0 __pltc1c.bits.__pltc1deb0
#define _pltc1deb1 __pltc1c.bits.__pltc1deb1
#define _pltc1o   __pltc1c.bits.__pltc1o
#define _pltc1en  __pltc1c.bits.__pltc1en
#define _pltc1out __pltc1c.bits.__pltc1out
#define _pltc1of0 __pltc1vos.bits.__pltc1of0
#define _pltc1of1 __pltc1vos.bits.__pltc1of1
#define _pltc1of2 __pltc1vos.bits.__pltc1of2
#define _pltc1of3 __pltc1vos.bits.__pltc1of3
#define _pltc1of4 __pltc1vos.bits.__pltc1of4
#define _pltc1rsp __pltc1vos.bits.__pltc1rsp
#define _pltc1ofm __pltc1vos.bits.__pltc1ofm
#define _pltc0hys0 __pltchyc.bits.__pltc0hys0
#define _pltc0hys1 __pltchyc.bits.__pltc0hys1
#define _pltc1hys0 __pltchyc.bits.__pltc1hys0
#define _pltc1hys1 __pltchyc.bits.__pltc1hys1
#define _pltc0pol __pltchyc.bits.__pltc0pol
#define _pltc1pol __pltchyc.bits.__pltc1pol
#define _pltcxosw __pltchyc.bits.__pltcxosw
#define _pltabw   __pltac.bits.__pltabw
#define _pltao    __pltac.bits.__pltao
#define _pltaen   __pltac.bits.__pltaen
#define _pltaof0  __pltavos.bits.__pltaof0
#define _pltaof1  __pltavos.bits.__pltaof1
#define _pltaof2  __pltavos.bits.__pltaof2
#define _pltaof3  __pltavos.bits.__pltaof3
#define _pltaof4  __pltavos.bits.__pltaof4
#define _pltaof5  __pltavos.bits.__pltaof5
#define _pltarsp  __pltavos.bits.__pltarsp
#define _pltaofm  __pltavos.bits.__pltaofm
#define _ws0      __wdtc.bits.__ws0
#define _ws1      __wdtc.bits.__ws1
#define _ws2      __wdtc.bits.__ws2
#define _we0      __wdtc.bits.__we0
#define _we1      __wdtc.bits.__we1
#define _we2      __wdtc.bits.__we2
#define _we3      __wdtc.bits.__we3
#define _we4      __wdtc.bits.__we4
#define _eea0     __eea.bits.__eea0
#define _eea1     __eea.bits.__eea1
#define _eea2     __eea.bits.__eea2
#define _eea3     __eea.bits.__eea3
#define _eea4     __eea.bits.__eea4
#define _eea5     __eea.bits.__eea5
#define _eed0     __eed.bits.__eed0
#define _eed1     __eed.bits.__eed1
#define _eed2     __eed.bits.__eed2
#define _eed3     __eed.bits.__eed3
#define _eed4     __eed.bits.__eed4
#define _eed5     __eed.bits.__eed5
#define _eed6     __eed.bits.__eed6
#define _eed7     __eed.bits.__eed7
#define _txif     __usr.bits.__txif
#define _tidle    __usr.bits.__tidle
#define _rxif     __usr.bits.__rxif
#define _ridle    __usr.bits.__ridle
#define _oerr     __usr.bits.__oerr
#define _ferr     __usr.bits.__ferr
#define _nf       __usr.bits.__nf
#define _perr     __usr.bits.__perr
#define _tx8      __ucr1.bits.__tx8
#define _rx8      __ucr1.bits.__rx8
#define _txbrk    __ucr1.bits.__txbrk
#define _stops    __ucr1.bits.__stops
#define _prt      __ucr1.bits.__prt
#define _pren     __ucr1.bits.__pren
#define _bno      __ucr1.bits.__bno
#define _uarten   __ucr1.bits.__uarten
#define _teie     __ucr2.bits.__teie
#define _tiie     __ucr2.bits.__tiie
#define _rie      __ucr2.bits.__rie
#define _wake     __ucr2.bits.__wake
#define _adden    __ucr2.bits.__adden
#define _brgh     __ucr2.bits.__brgh
#define _rxen     __ucr2.bits.__rxen
#define _txen     __ucr2.bits.__txen
#define _swm      __ucr3.bits.__swm
#define _txrx0    __txr_rxr.bits.__txrx0
#define _txrx1    __txr_rxr.bits.__txrx1
#define _txrx2    __txr_rxr.bits.__txrx2
#define _txrx3    __txr_rxr.bits.__txrx3
#define _txrx4    __txr_rxr.bits.__txrx4
#define _txrx5    __txr_rxr.bits.__txrx5
#define _txrx6    __txr_rxr.bits.__txrx6
#define _txrx7    __txr_rxr.bits.__txrx7
#define _brg0     __brg.bits.__brg0
#define _brg1     __brg.bits.__brg1
#define _brg2     __brg.bits.__brg2
#define _brg3     __brg.bits.__brg3
#define _brg4     __brg.bits.__brg4
#define _brg5     __brg.bits.__brg5
#define _brg6     __brg.bits.__brg6
#define _brg7     __brg.bits.__brg7
#define _int0s0   __integ.bits.__int0s0
#define _int0s1   __integ.bits.__int0s1
#define _int1s0   __integ.bits.__int1s0
#define _int1s1   __integ.bits.__int1s1
#define _emi      __intc0.bits.__emi
#define _pltc0e   __intc0.bits.__pltc0e
#define _int0e    __intc0.bits.__int0e
#define _int1e    __intc0.bits.__int1e
#define _pltc0f   __intc0.bits.__pltc0f
#define _int0f    __intc0.bits.__int0f
#define _int1f    __intc0.bits.__int1f
#define _ure      __intc1.bits.__ure
#define _lve      __intc1.bits.__lve
#define _mf0e     __intc1.bits.__mf0e
#define _mf1e     __intc1.bits.__mf1e
#define _urf      __intc1.bits.__urf
#define _lvf      __intc1.bits.__lvf
#define _mf0f     __intc1.bits.__mf0f
#define _mf1f     __intc1.bits.__mf1f
#define _mf2e     __intc2.bits.__mf2e
#define _mf3e     __intc2.bits.__mf3e
#define _mf4e     __intc2.bits.__mf4e
#define _tb0e     __intc2.bits.__tb0e
#define _mf2f     __intc2.bits.__mf2f
#define _mf3f     __intc2.bits.__mf3f
#define _mf4f     __intc2.bits.__mf4f
#define _tb0f     __intc2.bits.__tb0f
#define _tb1e     __intc3.bits.__tb1e
#define _pltc1e   __intc3.bits.__pltc1e
#define _tb1f     __intc3.bits.__tb1f
#define _pltc1f   __intc3.bits.__pltc1f
#define _ade      __mfi0.bits.__ade
#define _dee      __mfi0.bits.__dee
#define _adf      __mfi0.bits.__adf
#define _def      __mfi0.bits.__def
#define _ptm0pe   __mfi1.bits.__ptm0pe
#define _ptm0ae   __mfi1.bits.__ptm0ae
#define _ptm0pf   __mfi1.bits.__ptm0pf
#define _ptm0af   __mfi1.bits.__ptm0af
#define _ptm1pe   __mfi2.bits.__ptm1pe
#define _ptm1ae   __mfi2.bits.__ptm1ae
#define _ptm1pf   __mfi2.bits.__ptm1pf
#define _ptm1af   __mfi2.bits.__ptm1af
#define _ctm0pe   __mfi3.bits.__ctm0pe
#define _ctm0ae   __mfi3.bits.__ctm0ae
#define _ctm0pf   __mfi3.bits.__ctm0pf
#define _ctm0af   __mfi3.bits.__ctm0af
#define _ctm1pe   __mfi4.bits.__ctm1pe
#define _ctm1ae   __mfi4.bits.__ctm1ae
#define _ctm1pf   __mfi4.bits.__ctm1pf
#define _ctm1af   __mfi4.bits.__ctm1af
#define _pt0on    __ptm0c0.bits.__pt0on
#define _pt0ck0   __ptm0c0.bits.__pt0ck0
#define _pt0ck1   __ptm0c0.bits.__pt0ck1
#define _pt0ck2   __ptm0c0.bits.__pt0ck2
#define _pt0pau   __ptm0c0.bits.__pt0pau
#define _pt0cclr  __ptm0c1.bits.__pt0cclr
#define _pt0capts __ptm0c1.bits.__pt0capts
#define _pt0pol   __ptm0c1.bits.__pt0pol
#define _pt0oc    __ptm0c1.bits.__pt0oc
#define _pt0io0   __ptm0c1.bits.__pt0io0
#define _pt0io1   __ptm0c1.bits.__pt0io1
#define _pt0m0    __ptm0c1.bits.__pt0m0
#define _pt0m1    __ptm0c1.bits.__pt0m1
#define _pt0vlf   __ptm0c2.bits.__pt0vlf
#define _pt0tclr0 __ptm0c2.bits.__pt0tclr0
#define _pt0tclr1 __ptm0c2.bits.__pt0tclr1
#define _pt1on    __ptm1c0.bits.__pt1on
#define _pt1ck0   __ptm1c0.bits.__pt1ck0
#define _pt1ck1   __ptm1c0.bits.__pt1ck1
#define _pt1ck2   __ptm1c0.bits.__pt1ck2
#define _pt1pau   __ptm1c0.bits.__pt1pau
#define _pt1cclr  __ptm1c1.bits.__pt1cclr
#define _pt1capts __ptm1c1.bits.__pt1capts
#define _pt1pol   __ptm1c1.bits.__pt1pol
#define _pt1oc    __ptm1c1.bits.__pt1oc
#define _pt1io0   __ptm1c1.bits.__pt1io0
#define _pt1io1   __ptm1c1.bits.__pt1io1
#define _pt1m0    __ptm1c1.bits.__pt1m0
#define _pt1m1    __ptm1c1.bits.__pt1m1
#define _ct1rp0   __ctm1c0.bits.__ct1rp0
#define _ct1rp1   __ctm1c0.bits.__ct1rp1
#define _ct1rp2   __ctm1c0.bits.__ct1rp2
#define _ct1on    __ctm1c0.bits.__ct1on
#define _ct1ck0   __ctm1c0.bits.__ct1ck0
#define _ct1ck1   __ctm1c0.bits.__ct1ck1
#define _ct1ck2   __ctm1c0.bits.__ct1ck2
#define _ct1pau   __ctm1c0.bits.__ct1pau
#define _ct1cclr  __ctm1c1.bits.__ct1cclr
#define _ct1dpx   __ctm1c1.bits.__ct1dpx
#define _ct1pol   __ctm1c1.bits.__ct1pol
#define _ct1oc    __ctm1c1.bits.__ct1oc
#define _ct1io0   __ctm1c1.bits.__ct1io0
#define _ct1io1   __ctm1c1.bits.__ct1io1
#define _ct1m0    __ctm1c1.bits.__ct1m0
#define _ct1m1    __ctm1c1.bits.__ct1m1
#define _pas00    __pas0.bits.__pas00
#define _pas01    __pas0.bits.__pas01
#define _pas02    __pas0.bits.__pas02
#define _pas03    __pas0.bits.__pas03
#define _pas04    __pas0.bits.__pas04
#define _pas05    __pas0.bits.__pas05
#define _pas06    __pas0.bits.__pas06
#define _pas07    __pas0.bits.__pas07
#define _pas10    __pas1.bits.__pas10
#define _pas11    __pas1.bits.__pas11
#define _pas12    __pas1.bits.__pas12
#define _pas13    __pas1.bits.__pas13
#define _pas14    __pas1.bits.__pas14
#define _pas15    __pas1.bits.__pas15
#define _pas16    __pas1.bits.__pas16
#define _pas17    __pas1.bits.__pas17
#define _pbs00    __pbs0.bits.__pbs00
#define _pbs01    __pbs0.bits.__pbs01
#define _pbs02    __pbs0.bits.__pbs02
#define _pbs03    __pbs0.bits.__pbs03
#define _pbs04    __pbs0.bits.__pbs04
#define _pbs05    __pbs0.bits.__pbs05
#define _pbs06    __pbs0.bits.__pbs06
#define _pbs07    __pbs0.bits.__pbs07
#define _pcs00    __pcs0.bits.__pcs00
#define _pcs01    __pcs0.bits.__pcs01
#define _pcs02    __pcs0.bits.__pcs02
#define _pcs03    __pcs0.bits.__pcs03
#define _pcs04    __pcs0.bits.__pcs04
#define _pcs05    __pcs0.bits.__pcs05
#define _pcs06    __pcs0.bits.__pcs06
#define _pcs07    __pcs0.bits.__pcs07
#define _ifs0     __ifs.bits.__ifs0
#define _ifs1     __ifs.bits.__ifs1
#define _ifs2     __ifs.bits.__ifs2
#define _ifs3     __ifs.bits.__ifs3
#define _rd       __eec.bits.__rd
#define _rden     __eec.bits.__rden
#define _wr       __eec.bits.__wr
#define _wren     __eec.bits.__wren

#define ___mkstr(x)                #x
#define __EEPROM_DATA(a, b, c, d, e, f, g, h) \
                         asm("eeprom_data .section 'eeprom'"); \
                         asm("db\t" ___mkstr(a));        \
                         asm("db\t" ___mkstr(b));        \
                         asm("db\t" ___mkstr(c));        \
                         asm("db\t" ___mkstr(d));        \
                         asm("db\t" ___mkstr(e));        \
                         asm("db\t" ___mkstr(f));        \
                         asm("db\t" ___mkstr(g));        \
                         asm("db\t" ___mkstr(h));        

#endif // _HT45F5541_H_
