#ifndef BOOST_PREPROCESSOR_REPEAT_HPP
#define BOOST_PREPROCESSOR_REPEAT_HPP

/* Copyright (C) 2001
 * Housemarque Oy
 * http://www.housemarque.com
 *
 * Permission to copy, use, modify, sell and distribute this software is
 * granted provided this copyright notice appears in all copies. This
 * software is provided "as is" without express or implied warranty, and
 * with no claim as to its suitability for any purpose.
 *
 * See http://www.boost.org for most recent version.
 */

/** <p>Repeats the macro <code>MACRO(INDEX,DATA)</code> for <code>INDEX = [0,COUNT)</code>.</p>

<p>In other words, expands to the sequence:</p>

<pre>
  MACRO(0,DATA) MACRO(1,DATA) ... MACRO(BOOST_PP_DEC(COUNT),DATA)
</pre>

<p>For example,</p>

<pre>
  #define TEST(INDEX,DATA) DATA(INDEX);
  BOOST_PP_REPEAT(3,TEST,X)
</pre>

<p>expands to:</p>

<pre>
  X(0); X(1); X(2);
</pre>

<h3>2D and 3D repetition</h3>

<p>2D and 3D repetition are supported with the BOOST_PP_REPEAT_2ND() and
BOOST_PP_REPEAT_3RD() macros.</p>

<h3>Example</h3>
<ul>
  <li><a href="../../example/duffs_device.c">duffs_device.c</a></li>
  <li><a href="../../example/repeat_2d.c">repeat_2d.c</a></li>
</ul>

<h3>See</h3>
<ul>
  <li>BOOST_PP_FOR()</li>
  <li>BOOST_PP_LIMIT_MAG</li>
</ul>
*/
#define BOOST_PP_REPEAT(COUNT,MACRO,DATA) BOOST_PP_REPEAT_DELAY(COUNT)(MACRO,DATA)

#define BOOST_PP_REPEAT_DELAY(N) BOOST_PP_REPEAT##N
#define BOOST_PP_REPEAT0(M,D)
#define BOOST_PP_REPEAT1(M,D) M(0,D)
#define BOOST_PP_REPEAT2(M,D) M(0,D) M(1,D)
#define BOOST_PP_REPEAT3(M,D) M(0,D) M(1,D) M(2,D)
#define BOOST_PP_REPEAT4(M,D) M(0,D) M(1,D) M(2,D) M(3,D)
#define BOOST_PP_REPEAT5(M,D) BOOST_PP_REPEAT4(M,D) M(4,D)
#define BOOST_PP_REPEAT6(M,D) BOOST_PP_REPEAT5(M,D) M(5,D)
#define BOOST_PP_REPEAT7(M,D) BOOST_PP_REPEAT6(M,D) M(6,D)
#define BOOST_PP_REPEAT8(M,D) BOOST_PP_REPEAT7(M,D) M(7,D)
#define BOOST_PP_REPEAT9(M,D) BOOST_PP_REPEAT8(M,D) M(8,D)
#define BOOST_PP_REPEAT10(M,D) BOOST_PP_REPEAT9(M,D) M(9,D)
#define BOOST_PP_REPEAT11(M,D) BOOST_PP_REPEAT10(M,D) M(10,D)
#define BOOST_PP_REPEAT12(M,D) BOOST_PP_REPEAT11(M,D) M(11,D)
#define BOOST_PP_REPEAT13(M,D) BOOST_PP_REPEAT12(M,D) M(12,D)
#define BOOST_PP_REPEAT14(M,D) BOOST_PP_REPEAT13(M,D) M(13,D)
#define BOOST_PP_REPEAT15(M,D) BOOST_PP_REPEAT14(M,D) M(14,D)
#define BOOST_PP_REPEAT16(M,D) BOOST_PP_REPEAT15(M,D) M(15,D)
#define BOOST_PP_REPEAT17(M,D) BOOST_PP_REPEAT16(M,D) M(16,D)
#define BOOST_PP_REPEAT18(M,D) BOOST_PP_REPEAT17(M,D) M(17,D)
#define BOOST_PP_REPEAT19(M,D) BOOST_PP_REPEAT18(M,D) M(18,D)
#define BOOST_PP_REPEAT20(M,D) BOOST_PP_REPEAT19(M,D) M(19,D)
#define BOOST_PP_REPEAT21(M,D) BOOST_PP_REPEAT20(M,D) M(20,D)
#define BOOST_PP_REPEAT22(M,D) BOOST_PP_REPEAT21(M,D) M(21,D)
#define BOOST_PP_REPEAT23(M,D) BOOST_PP_REPEAT22(M,D) M(22,D)
#define BOOST_PP_REPEAT24(M,D) BOOST_PP_REPEAT23(M,D) M(23,D)
#define BOOST_PP_REPEAT25(M,D) BOOST_PP_REPEAT24(M,D) M(24,D)
#define BOOST_PP_REPEAT26(M,D) BOOST_PP_REPEAT25(M,D) M(25,D)
#define BOOST_PP_REPEAT27(M,D) BOOST_PP_REPEAT26(M,D) M(26,D)
#define BOOST_PP_REPEAT28(M,D) BOOST_PP_REPEAT27(M,D) M(27,D)
#define BOOST_PP_REPEAT29(M,D) BOOST_PP_REPEAT28(M,D) M(28,D)
#define BOOST_PP_REPEAT30(M,D) BOOST_PP_REPEAT29(M,D) M(29,D)
#define BOOST_PP_REPEAT31(M,D) BOOST_PP_REPEAT30(M,D) M(30,D)
#define BOOST_PP_REPEAT32(M,D) BOOST_PP_REPEAT31(M,D) M(31,D)
#define BOOST_PP_REPEAT33(M,D) BOOST_PP_REPEAT32(M,D) M(32,D)
#define BOOST_PP_REPEAT34(M,D) BOOST_PP_REPEAT33(M,D) M(33,D)
#define BOOST_PP_REPEAT35(M,D) BOOST_PP_REPEAT34(M,D) M(34,D)
#define BOOST_PP_REPEAT36(M,D) BOOST_PP_REPEAT35(M,D) M(35,D)
#define BOOST_PP_REPEAT37(M,D) BOOST_PP_REPEAT36(M,D) M(36,D)
#define BOOST_PP_REPEAT38(M,D) BOOST_PP_REPEAT37(M,D) M(37,D)
#define BOOST_PP_REPEAT39(M,D) BOOST_PP_REPEAT38(M,D) M(38,D)
#define BOOST_PP_REPEAT40(M,D) BOOST_PP_REPEAT39(M,D) M(39,D)
#define BOOST_PP_REPEAT41(M,D) BOOST_PP_REPEAT40(M,D) M(40,D)
#define BOOST_PP_REPEAT42(M,D) BOOST_PP_REPEAT41(M,D) M(41,D)
#define BOOST_PP_REPEAT43(M,D) BOOST_PP_REPEAT42(M,D) M(42,D)
#define BOOST_PP_REPEAT44(M,D) BOOST_PP_REPEAT43(M,D) M(43,D)
#define BOOST_PP_REPEAT45(M,D) BOOST_PP_REPEAT44(M,D) M(44,D)
#define BOOST_PP_REPEAT46(M,D) BOOST_PP_REPEAT45(M,D) M(45,D)
#define BOOST_PP_REPEAT47(M,D) BOOST_PP_REPEAT46(M,D) M(46,D)
#define BOOST_PP_REPEAT48(M,D) BOOST_PP_REPEAT47(M,D) M(47,D)
#define BOOST_PP_REPEAT49(M,D) BOOST_PP_REPEAT48(M,D) M(48,D)
#define BOOST_PP_REPEAT50(M,D) BOOST_PP_REPEAT49(M,D) M(49,D)
#define BOOST_PP_REPEAT51(M,D) BOOST_PP_REPEAT50(M,D) M(50,D)
#define BOOST_PP_REPEAT52(M,D) BOOST_PP_REPEAT51(M,D) M(51,D)
#define BOOST_PP_REPEAT53(M,D) BOOST_PP_REPEAT52(M,D) M(52,D)
#define BOOST_PP_REPEAT54(M,D) BOOST_PP_REPEAT53(M,D) M(53,D)
#define BOOST_PP_REPEAT55(M,D) BOOST_PP_REPEAT54(M,D) M(54,D)
#define BOOST_PP_REPEAT56(M,D) BOOST_PP_REPEAT55(M,D) M(55,D)
#define BOOST_PP_REPEAT57(M,D) BOOST_PP_REPEAT56(M,D) M(56,D)
#define BOOST_PP_REPEAT58(M,D) BOOST_PP_REPEAT57(M,D) M(57,D)
#define BOOST_PP_REPEAT59(M,D) BOOST_PP_REPEAT58(M,D) M(58,D)
#define BOOST_PP_REPEAT60(M,D) BOOST_PP_REPEAT59(M,D) M(59,D)
#define BOOST_PP_REPEAT61(M,D) BOOST_PP_REPEAT60(M,D) M(60,D)
#define BOOST_PP_REPEAT62(M,D) BOOST_PP_REPEAT61(M,D) M(61,D)
#define BOOST_PP_REPEAT63(M,D) BOOST_PP_REPEAT62(M,D) M(62,D)
#define BOOST_PP_REPEAT64(M,D) BOOST_PP_REPEAT63(M,D) M(63,D)
#define BOOST_PP_REPEAT65(M,D) BOOST_PP_REPEAT64(M,D) M(64,D)
#define BOOST_PP_REPEAT66(M,D) BOOST_PP_REPEAT65(M,D) M(65,D)
#define BOOST_PP_REPEAT67(M,D) BOOST_PP_REPEAT66(M,D) M(66,D)
#define BOOST_PP_REPEAT68(M,D) BOOST_PP_REPEAT67(M,D) M(67,D)
#define BOOST_PP_REPEAT69(M,D) BOOST_PP_REPEAT68(M,D) M(68,D)
#define BOOST_PP_REPEAT70(M,D) BOOST_PP_REPEAT69(M,D) M(69,D)
#define BOOST_PP_REPEAT71(M,D) BOOST_PP_REPEAT70(M,D) M(70,D)
#define BOOST_PP_REPEAT72(M,D) BOOST_PP_REPEAT71(M,D) M(71,D)
#define BOOST_PP_REPEAT73(M,D) BOOST_PP_REPEAT72(M,D) M(72,D)
#define BOOST_PP_REPEAT74(M,D) BOOST_PP_REPEAT73(M,D) M(73,D)
#define BOOST_PP_REPEAT75(M,D) BOOST_PP_REPEAT74(M,D) M(74,D)
#define BOOST_PP_REPEAT76(M,D) BOOST_PP_REPEAT75(M,D) M(75,D)
#define BOOST_PP_REPEAT77(M,D) BOOST_PP_REPEAT76(M,D) M(76,D)
#define BOOST_PP_REPEAT78(M,D) BOOST_PP_REPEAT77(M,D) M(77,D)
#define BOOST_PP_REPEAT79(M,D) BOOST_PP_REPEAT78(M,D) M(78,D)
#define BOOST_PP_REPEAT80(M,D) BOOST_PP_REPEAT79(M,D) M(79,D)
#define BOOST_PP_REPEAT81(M,D) BOOST_PP_REPEAT80(M,D) M(80,D)
#define BOOST_PP_REPEAT82(M,D) BOOST_PP_REPEAT81(M,D) M(81,D)
#define BOOST_PP_REPEAT83(M,D) BOOST_PP_REPEAT82(M,D) M(82,D)
#define BOOST_PP_REPEAT84(M,D) BOOST_PP_REPEAT83(M,D) M(83,D)
#define BOOST_PP_REPEAT85(M,D) BOOST_PP_REPEAT84(M,D) M(84,D)
#define BOOST_PP_REPEAT86(M,D) BOOST_PP_REPEAT85(M,D) M(85,D)
#define BOOST_PP_REPEAT87(M,D) BOOST_PP_REPEAT86(M,D) M(86,D)
#define BOOST_PP_REPEAT88(M,D) BOOST_PP_REPEAT87(M,D) M(87,D)
#define BOOST_PP_REPEAT89(M,D) BOOST_PP_REPEAT88(M,D) M(88,D)
#define BOOST_PP_REPEAT90(M,D) BOOST_PP_REPEAT89(M,D) M(89,D)
#define BOOST_PP_REPEAT91(M,D) BOOST_PP_REPEAT90(M,D) M(90,D)
#define BOOST_PP_REPEAT92(M,D) BOOST_PP_REPEAT91(M,D) M(91,D)
#define BOOST_PP_REPEAT93(M,D) BOOST_PP_REPEAT92(M,D) M(92,D)
#define BOOST_PP_REPEAT94(M,D) BOOST_PP_REPEAT93(M,D) M(93,D)
#define BOOST_PP_REPEAT95(M,D) BOOST_PP_REPEAT94(M,D) M(94,D)
#define BOOST_PP_REPEAT96(M,D) BOOST_PP_REPEAT95(M,D) M(95,D)
#define BOOST_PP_REPEAT97(M,D) BOOST_PP_REPEAT96(M,D) M(96,D)
#define BOOST_PP_REPEAT98(M,D) BOOST_PP_REPEAT97(M,D) M(97,D)
#define BOOST_PP_REPEAT99(M,D) BOOST_PP_REPEAT98(M,D) M(98,D)
#define BOOST_PP_REPEAT100(M,D) BOOST_PP_REPEAT99(M,D) M(99,D)
#define BOOST_PP_REPEAT101(M,D) BOOST_PP_REPEAT100(M,D) M(100,D)
#define BOOST_PP_REPEAT102(M,D) BOOST_PP_REPEAT101(M,D) M(101,D)
#define BOOST_PP_REPEAT103(M,D) BOOST_PP_REPEAT102(M,D) M(102,D)
#define BOOST_PP_REPEAT104(M,D) BOOST_PP_REPEAT103(M,D) M(103,D)
#define BOOST_PP_REPEAT105(M,D) BOOST_PP_REPEAT104(M,D) M(104,D)
#define BOOST_PP_REPEAT106(M,D) BOOST_PP_REPEAT105(M,D) M(105,D)
#define BOOST_PP_REPEAT107(M,D) BOOST_PP_REPEAT106(M,D) M(106,D)
#define BOOST_PP_REPEAT108(M,D) BOOST_PP_REPEAT107(M,D) M(107,D)
#define BOOST_PP_REPEAT109(M,D) BOOST_PP_REPEAT108(M,D) M(108,D)
#define BOOST_PP_REPEAT110(M,D) BOOST_PP_REPEAT109(M,D) M(109,D)
#define BOOST_PP_REPEAT111(M,D) BOOST_PP_REPEAT110(M,D) M(110,D)
#define BOOST_PP_REPEAT112(M,D) BOOST_PP_REPEAT111(M,D) M(111,D)
#define BOOST_PP_REPEAT113(M,D) BOOST_PP_REPEAT112(M,D) M(112,D)
#define BOOST_PP_REPEAT114(M,D) BOOST_PP_REPEAT113(M,D) M(113,D)
#define BOOST_PP_REPEAT115(M,D) BOOST_PP_REPEAT114(M,D) M(114,D)
#define BOOST_PP_REPEAT116(M,D) BOOST_PP_REPEAT115(M,D) M(115,D)
#define BOOST_PP_REPEAT117(M,D) BOOST_PP_REPEAT116(M,D) M(116,D)
#define BOOST_PP_REPEAT118(M,D) BOOST_PP_REPEAT117(M,D) M(117,D)
#define BOOST_PP_REPEAT119(M,D) BOOST_PP_REPEAT118(M,D) M(118,D)
#define BOOST_PP_REPEAT120(M,D) BOOST_PP_REPEAT119(M,D) M(119,D)
#define BOOST_PP_REPEAT121(M,D) BOOST_PP_REPEAT120(M,D) M(120,D)
#define BOOST_PP_REPEAT122(M,D) BOOST_PP_REPEAT121(M,D) M(121,D)
#define BOOST_PP_REPEAT123(M,D) BOOST_PP_REPEAT122(M,D) M(122,D)
#define BOOST_PP_REPEAT124(M,D) BOOST_PP_REPEAT123(M,D) M(123,D)
#define BOOST_PP_REPEAT125(M,D) BOOST_PP_REPEAT124(M,D) M(124,D)
#define BOOST_PP_REPEAT126(M,D) BOOST_PP_REPEAT125(M,D) M(125,D)
#define BOOST_PP_REPEAT127(M,D) BOOST_PP_REPEAT126(M,D) M(126,D)
#define BOOST_PP_REPEAT128(M,D) BOOST_PP_REPEAT127(M,D) M(127,D)

/* <p>Obsolete. Use BOOST_PP_REPEAT().</p> */
#define BOOST_PREPROCESSOR_REPEAT(C,M,D) BOOST_PP_REPEAT(C,M,D)
#endif
