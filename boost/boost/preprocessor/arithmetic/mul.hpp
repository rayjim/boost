#ifndef BOOST_PREPROCESSOR_ARITHMETIC_MUL_HPP
#define BOOST_PREPROCESSOR_ARITHMETIC_MUL_HPP

//  Copyright (C) 2001
//  Housemarque, Inc.
//  http://www.housemarque.com
//  
//  Permission to copy, use, modify, sell and distribute this software is
//  granted provided this copyright notice appears in all copies. This
//  software is provided "as is" without express or implied warranty, and
//  with no claim as to its suitability for any purpose.

//  See http://www.boost.org for most recent version.

/*! \file

<a href="../../../../boost/preprocessor/arithmetic/mul.hpp">Click here to see the header.</a>
*/

#ifndef BOOST_PREPROCESSOR_ARITHMETIC_ADD_HPP
#  include <boost/preprocessor/arithmetic/add.hpp>
#endif

//! Expands to the product of X and Y.
#define BOOST_PREPROCESSOR_MUL(X,Y) BOOST_PREPROCESSOR_MUL_DELAY(X,Y)

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#define BOOST_PREPROCESSOR_MUL_DELAY(X,Y) BOOST_PREPROCESSOR_MUL##X(Y,0)
#define BOOST_PREPROCESSOR_MUL0(Y,A) A
#define BOOST_PREPROCESSOR_MUL1(Y,A) BOOST_PREPROCESSOR_ADD(Y,A)
#define BOOST_PREPROCESSOR_MUL2(Y,A) BOOST_PREPROCESSOR_MUL1(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL3(Y,A) BOOST_PREPROCESSOR_MUL2(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL4(Y,A) BOOST_PREPROCESSOR_MUL3(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL5(Y,A) BOOST_PREPROCESSOR_MUL4(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL6(Y,A) BOOST_PREPROCESSOR_MUL5(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL7(Y,A) BOOST_PREPROCESSOR_MUL6(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL8(Y,A) BOOST_PREPROCESSOR_MUL7(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL9(Y,A) BOOST_PREPROCESSOR_MUL8(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL10(Y,A) BOOST_PREPROCESSOR_MUL9(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL11(Y,A) BOOST_PREPROCESSOR_MUL10(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL12(Y,A) BOOST_PREPROCESSOR_MUL11(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL13(Y,A) BOOST_PREPROCESSOR_MUL12(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL14(Y,A) BOOST_PREPROCESSOR_MUL13(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL15(Y,A) BOOST_PREPROCESSOR_MUL14(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL16(Y,A) BOOST_PREPROCESSOR_MUL15(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL17(Y,A) BOOST_PREPROCESSOR_MUL16(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL18(Y,A) BOOST_PREPROCESSOR_MUL17(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL19(Y,A) BOOST_PREPROCESSOR_MUL18(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL20(Y,A) BOOST_PREPROCESSOR_MUL19(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL21(Y,A) BOOST_PREPROCESSOR_MUL20(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL22(Y,A) BOOST_PREPROCESSOR_MUL21(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL23(Y,A) BOOST_PREPROCESSOR_MUL22(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL24(Y,A) BOOST_PREPROCESSOR_MUL23(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL25(Y,A) BOOST_PREPROCESSOR_MUL24(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL26(Y,A) BOOST_PREPROCESSOR_MUL25(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL27(Y,A) BOOST_PREPROCESSOR_MUL26(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL28(Y,A) BOOST_PREPROCESSOR_MUL27(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL29(Y,A) BOOST_PREPROCESSOR_MUL28(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL30(Y,A) BOOST_PREPROCESSOR_MUL29(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL31(Y,A) BOOST_PREPROCESSOR_MUL30(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL32(Y,A) BOOST_PREPROCESSOR_MUL31(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL33(Y,A) BOOST_PREPROCESSOR_MUL32(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL34(Y,A) BOOST_PREPROCESSOR_MUL33(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL35(Y,A) BOOST_PREPROCESSOR_MUL34(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL36(Y,A) BOOST_PREPROCESSOR_MUL35(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL37(Y,A) BOOST_PREPROCESSOR_MUL36(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL38(Y,A) BOOST_PREPROCESSOR_MUL37(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL39(Y,A) BOOST_PREPROCESSOR_MUL38(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL40(Y,A) BOOST_PREPROCESSOR_MUL39(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL41(Y,A) BOOST_PREPROCESSOR_MUL40(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL42(Y,A) BOOST_PREPROCESSOR_MUL41(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL43(Y,A) BOOST_PREPROCESSOR_MUL42(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL44(Y,A) BOOST_PREPROCESSOR_MUL43(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL45(Y,A) BOOST_PREPROCESSOR_MUL44(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL46(Y,A) BOOST_PREPROCESSOR_MUL45(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL47(Y,A) BOOST_PREPROCESSOR_MUL46(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL48(Y,A) BOOST_PREPROCESSOR_MUL47(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL49(Y,A) BOOST_PREPROCESSOR_MUL48(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL50(Y,A) BOOST_PREPROCESSOR_MUL49(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL51(Y,A) BOOST_PREPROCESSOR_MUL50(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL52(Y,A) BOOST_PREPROCESSOR_MUL51(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL53(Y,A) BOOST_PREPROCESSOR_MUL52(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL54(Y,A) BOOST_PREPROCESSOR_MUL53(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL55(Y,A) BOOST_PREPROCESSOR_MUL54(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL56(Y,A) BOOST_PREPROCESSOR_MUL55(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL57(Y,A) BOOST_PREPROCESSOR_MUL56(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL58(Y,A) BOOST_PREPROCESSOR_MUL57(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL59(Y,A) BOOST_PREPROCESSOR_MUL58(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL60(Y,A) BOOST_PREPROCESSOR_MUL59(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL61(Y,A) BOOST_PREPROCESSOR_MUL60(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL62(Y,A) BOOST_PREPROCESSOR_MUL61(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL63(Y,A) BOOST_PREPROCESSOR_MUL62(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL64(Y,A) BOOST_PREPROCESSOR_MUL63(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL65(Y,A) BOOST_PREPROCESSOR_MUL64(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL66(Y,A) BOOST_PREPROCESSOR_MUL65(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL67(Y,A) BOOST_PREPROCESSOR_MUL66(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL68(Y,A) BOOST_PREPROCESSOR_MUL67(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL69(Y,A) BOOST_PREPROCESSOR_MUL68(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL70(Y,A) BOOST_PREPROCESSOR_MUL69(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL71(Y,A) BOOST_PREPROCESSOR_MUL70(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL72(Y,A) BOOST_PREPROCESSOR_MUL71(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL73(Y,A) BOOST_PREPROCESSOR_MUL72(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL74(Y,A) BOOST_PREPROCESSOR_MUL73(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL75(Y,A) BOOST_PREPROCESSOR_MUL74(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL76(Y,A) BOOST_PREPROCESSOR_MUL75(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL77(Y,A) BOOST_PREPROCESSOR_MUL76(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL78(Y,A) BOOST_PREPROCESSOR_MUL77(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL79(Y,A) BOOST_PREPROCESSOR_MUL78(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL80(Y,A) BOOST_PREPROCESSOR_MUL79(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL81(Y,A) BOOST_PREPROCESSOR_MUL80(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL82(Y,A) BOOST_PREPROCESSOR_MUL81(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL83(Y,A) BOOST_PREPROCESSOR_MUL82(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL84(Y,A) BOOST_PREPROCESSOR_MUL83(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL85(Y,A) BOOST_PREPROCESSOR_MUL84(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL86(Y,A) BOOST_PREPROCESSOR_MUL85(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL87(Y,A) BOOST_PREPROCESSOR_MUL86(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL88(Y,A) BOOST_PREPROCESSOR_MUL87(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL89(Y,A) BOOST_PREPROCESSOR_MUL88(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL90(Y,A) BOOST_PREPROCESSOR_MUL89(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL91(Y,A) BOOST_PREPROCESSOR_MUL90(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL92(Y,A) BOOST_PREPROCESSOR_MUL91(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL93(Y,A) BOOST_PREPROCESSOR_MUL92(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL94(Y,A) BOOST_PREPROCESSOR_MUL93(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL95(Y,A) BOOST_PREPROCESSOR_MUL94(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL96(Y,A) BOOST_PREPROCESSOR_MUL95(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL97(Y,A) BOOST_PREPROCESSOR_MUL96(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL98(Y,A) BOOST_PREPROCESSOR_MUL97(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL99(Y,A) BOOST_PREPROCESSOR_MUL98(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL100(Y,A) BOOST_PREPROCESSOR_MUL99(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL101(Y,A) BOOST_PREPROCESSOR_MUL100(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL102(Y,A) BOOST_PREPROCESSOR_MUL101(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL103(Y,A) BOOST_PREPROCESSOR_MUL102(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL104(Y,A) BOOST_PREPROCESSOR_MUL103(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL105(Y,A) BOOST_PREPROCESSOR_MUL104(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL106(Y,A) BOOST_PREPROCESSOR_MUL105(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL107(Y,A) BOOST_PREPROCESSOR_MUL106(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL108(Y,A) BOOST_PREPROCESSOR_MUL107(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL109(Y,A) BOOST_PREPROCESSOR_MUL108(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL110(Y,A) BOOST_PREPROCESSOR_MUL109(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL111(Y,A) BOOST_PREPROCESSOR_MUL110(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL112(Y,A) BOOST_PREPROCESSOR_MUL111(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL113(Y,A) BOOST_PREPROCESSOR_MUL112(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL114(Y,A) BOOST_PREPROCESSOR_MUL113(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL115(Y,A) BOOST_PREPROCESSOR_MUL114(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL116(Y,A) BOOST_PREPROCESSOR_MUL115(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL117(Y,A) BOOST_PREPROCESSOR_MUL116(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL118(Y,A) BOOST_PREPROCESSOR_MUL117(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL119(Y,A) BOOST_PREPROCESSOR_MUL118(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL120(Y,A) BOOST_PREPROCESSOR_MUL119(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL121(Y,A) BOOST_PREPROCESSOR_MUL120(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL122(Y,A) BOOST_PREPROCESSOR_MUL121(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL123(Y,A) BOOST_PREPROCESSOR_MUL122(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL124(Y,A) BOOST_PREPROCESSOR_MUL123(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL125(Y,A) BOOST_PREPROCESSOR_MUL124(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL126(Y,A) BOOST_PREPROCESSOR_MUL125(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL127(Y,A) BOOST_PREPROCESSOR_MUL126(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#define BOOST_PREPROCESSOR_MUL128(Y,A) BOOST_PREPROCESSOR_MUL127(Y,BOOST_PREPROCESSOR_ADD(Y,A))
#endif /* DOXYGEN_SHOULD_SKIP_THIS */
#endif
