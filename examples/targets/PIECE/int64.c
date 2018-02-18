/*
 *	int64.c
 *
 *	64ビット整数演算ライブラリ（とテストプログラム）
 *	Copyright (C) 2002 Naoyuki Sawa
 *
 *	* Sun Oct 13 06:16:00 JST 2002 Naoyuki Sawa
 *	- 作成開始。
 */

/****************************************************************************
 *	64ビット整数演算ライブラリ
 ****************************************************************************/

/* * Cコンパイラ(gcc33)自体はlong longを認識し、ちゃんとコードも生成するのですが、
 *   64ビット整数演算ライブラリが含まれていないので、使えませんでした。
 *   そこで、64ビット整数演算ライブラリを自前で実装してみました。
 *
 * * 本当は、idiv.lib(整数剰余演算ライブラリ)やfp.lib(浮動小数点演算ライブラリ)、
 *   fpp.lib(浮動小数点ライブラリ)と同様に、コンパイラに付いて来るべきものだと思うのですが、
 *   なぜか含まれていません。
 *   P/ECEの配布セットに入れ忘れたというのではなく、EPSONの配布セットに入っていないようです。
 *   （Cコンパイラマニュアルのライブラリ一覧にも載っていないことから推測）
 *   もしかしたら、gcc33の long long 演算の出力コードに問題があったりして、
 *   対応から外してしまったのかも知れません。
 *   しかし、実験した限りでは正しいコードが生成されているように見えましたので、
 *   64ビット整数演算ライブラリさえ用意すれば long long が普通に使えると判断しました。
 *   今のところ、ちゃんと動いているように思います。
 *
 * * 戻り値・引数を宣言してはいけません！
 *   宣言すると、余計なプロローグ・エピローグコードが生成されてしまいます。
 *
 * * 浮動小数点数値⇔long longの変換サポート関数は未実装です。
 *   __fixsfdi,__fixunssfdi,__floatdisfなどが呼ばれるようです。
 *   また、floatからlong longへ変換するときに、__cmpdi2という関数も呼ばれます。
 *   名前から推測するとlong longの比較関数のようですが、他では使われる様子がありません。
 *   通常のlong longの比較処理では、Cコンパイラがインラインコードを生成しているのです。
 *   なぜfloatからlong longへの変換時だけ、__cmpdi2が呼ばれるのか不明です。
 *
 * * 64ビット定数のサフィックスは「ll」(符号付き)「ull」（符号無し）です。
 *   これらのサフィックスを付けなくても、大きな定数はちゃんと64ビットとして認識されますが、
 *   コンパイル時に警告が表示されます。
 *   <例>
 *	long long a = 1000000000000000000;   // 警告（動作はＯＫ）
 *	long long b = 1000000000000000000ll; // ＯＫ
 */

void/*long long*/
__negdi2(/*long long a*/) /* 符号反転 */
{
	asm("
	ld.w %r10, 0		; %r11:%r10 = 0:0 - %r13:%r12
	ld.w %r11, 0
	sub %r10, %r12
	sbc %r11, %r13
	");
}

void/*long long*/
__adddi3(/*long long a, long long b*/) /* 加算 */
{
	asm("
	ld.w %r10, %r12		; %r11:%r10 = %r13:%r12 + %r15:%r14
	ld.w %r11, %r13
	add %r10, %r14
	adc %r11, %r15
	");
}

void/*long long*/
__subdi3(/*long long a, long long b*/) /* 減算 */
{
	asm("
	ld.w %r10, %r12		; %r11:%r10 = %r13:%r12 - %r15:%r14
	ld.w %r11, %r13
	sub %r10, %r14
	sbc %r11, %r15
	");
}

/* * 古典的な筆算型の乗算ルーチンです。
 *   「64ビット×64ビット→64ビット」なので、符号を考慮する必要はありません。
 *   実際にgcc33は、64ビット乗算の場合は符号付き・符号無しに関わらず、
 *   __muldi3を呼び出すコードを生成するようです。（__umuldi3はありません）
 * * ちょっとしたテクニックのメモ：
 *   S1C33にはキャリーフラグ込みのシフト・ローテートがありません。
 *   乗数を1ビットづつ右シフトして最下位ビットを調べる必要がありますが、
 *   複数レジスタを連結して右シフトする処理は、まともに書くと非常に長くなります。
 *   例えば、%r15と%r14を連結して右1ビットシフトする処理は、こんな感じになります。
 *	ld.w %r4, %r15	; %r4'31 = %r15'0
 *	xsll %r4, 31	; ←3命令必要です
 *	srl %r15, 1
 *	srl %r14, 1
 *	or %r14, %r4
 *   Cコンパイラは、左1ビットシフトについても同様のコードを生成しますが、
 *   もっと簡単な方法があります。
 *	add %r14, %r14
 *	adc %r15, %r15	; C <- %r15'31
 *   さて、筆算型乗算ルーチンでは、乗数を最下位ビットから調べて行くので、右シフトが必要です。
 *   しかしながら前述の通り、S1C33 CPUでは、左シフト処理に較べて右シフト処理は難しいのです。
 *   そこで、乗数の全ビットを左右反転させてしまいましょう。
 *   そうしておけば、簡単な左シフトを使いながら、最上位ビットを調べればいいことになります。
 *	swap %r14, %r14		; %r14:%r15 = 左右反転(%r15:%r14)
 *	mirror %r14, %r14	; レジスタは入れ替えないので、反転後は%r14が上位になることに注意
 *	swap %r15, %r15
 *	mirror %r15, %r15
 *  S1C33のシフト・ローテート命令では、最上位ビットは単に捨てられてしまいますが、
 *  add/adcによる左シフト処理なら、最上位ビットがキャりーフラグに入るので、
 *  左シフト後にキャリーフラグを調べるだけで済みます。
 */
void/*long long*/
__muldi3(/*long long a, long long b*/) /* 乗算 */
{
	asm("
	ld.w %r10, 0		; %r11:%r10 = 0:0
	ld.w %r11, 0
	swap %r14, %r14		; %r14:%r15 = 左右反転(%r15:%r14)
	mirror %r14, %r14
	swap %r15, %r15
	mirror %r15, %r15
__muldi3_L10:
	add %r15, %r15		; C:%r14:%r15 <<= 1
	adc %r14, %r14
	jruge __muldi3_L30	; if(C) {
	add %r10, %r12		;   %r11:%r10 += %r13:%r12
	adc %r11, %r13
__muldi3_L20:
	add %r12, %r12		;   %r13:%r12 <<= 1
	jp.d __muldi3_L10	;   goto L10
	adc %r13, %r13
__muldi3_L30:			; } else {
	jrne __muldi3_L20	;   if(%r14 != 0) goto L20
	or %r15, %r15		;   if(%r15 != 0) goto L20
	jrne __muldi3_L20
				; }
	");
}

/* __divdi3/__udivdi3のサポート関数。
 * S1C33000コアCPUマニュアルのdiv0s/12s/3sの詳細説明どおりに実装しました。
 * * ゼロ除算例外は生成しません。
 * * S1C33の関数呼び出し規約どおり、%r11:%r10に商を返します。
 *   さらに加えて、%r13:%r12に余りを返します。（__moddi3/__umoddi3で使っています）
 *   本当は、%r13:%r12に戻り値をセットしても、ちゃんと返されるかどうか保証されないのですが、
 *   とりあえずこれらの関数はプロローグ・エピローグコードを含まないので、大丈夫みたいです。
 * * __divdi3/__udivdi3からしか使わないので、static関数にした方がいいのですが、そうすると、
 *   Cコンパイラが「使われていないstaic関数があります」という意味の警告を出してしまいます。
 *   Cコンパイラは、__divdi3/__udivdi3の中のasm()ブロックからの呼び出しを認識しないからです。
 *   仕方がないので、グローバル関数にしてあります。
 * * div3sの符号反転処理がちょっと苦しいですが、「adc %rd, imm6」がないので止むを得ません...
 *   「adc %rd, imm6」があれば「not %r10,%r10; not %r11,%r11; add %r10,1; adc %r11,0」とできるのに。
 *   でもまあ命令数は1つしか違わないし、サイクル数も最大2しか違わないので、たいして変わらないかな？
 * * まだテストが不十分なので、もしかしたらバグが残ってるかも知れません。
 */
void/*long long*/
__div00(/*long long a, long long b*/) /* 正÷正 */
{
	asm("
	;***** div0s *****
	ld.w %r10, %r12		; %r13:%r12:%r11:%r10 = 0:0:%r13:%r12
	ld.w %r11, %r13
	ld.w %r12, 0
	ld.w %r13, 0
	;***** div1 *****
	xld.w %r4, 64		; %r4 = 64
__div00_L10:
	add %r10, %r10		; %r13:%r12:%r11:%r10 <<= 1
	adc %r11, %r11
	adc %r12, %r12
	adc %r13, %r13
	sub %r12, %r14		; %r13:%r12 -= %r15:%r14
	sbc %r13, %r15
	jrult __div00_L20	; if(!C) {
	jp.d __div00_L30	;   %r10'0 = 1
	or %r10, 1
__div00_L20:			; } else {
	add %r12, %r14		;   %r13:%r12 += %r15:%r14
	adc %r13, %r15
__div00_L30:			; }
	sub %r4, 1		; if(--%r4) goto L10
	jrne __div00_L10
	;***** div2s *****
	;***** div3s *****
	");
}
void/*long long*/
__div01(/*long long a, long long b*/) /* 正÷負 */
{
	asm("
	;***** div0s *****
	ld.w %r10, %r12		; %r13:%r12:%r11:%r10 = 0:0:%r13:%r12
	ld.w %r11, %r13
	ld.w %r12, 0
	ld.w %r13, 0
	;***** div1 *****
	xld.w %r4, 64		; %r4 = 64
__div01_L10:
	add %r10, %r10		; %r13:%r12:%r11:%r10 <<= 1
	adc %r11, %r11
	adc %r12, %r12
	adc %r13, %r13
	add %r12, %r14		; %r13:%r12 += %r15:%r14
	adc %r13, %r15
	jruge __div01_L20	; if(C) {
	jp.d __div01_L30	;   %r10'0 = 1
	or %r10, 1
__div01_L20:			; } else {
	sub %r12, %r14		;   %r13:%r12 -= %r15:%r14
	sbc %r13, %r15
__div01_L30:			; }
	sub %r4, 1		; if(--%r4) goto L10
	jrne __div01_L10
	;***** div2s *****
	;***** div3s *****
	not %r10, %r10		; %r11:%r10 = -%r11:%r10
	add %r10, 1
	jruge.d __div01_L60
	not %r11, %r11
	add %r11, 1
__div01_L60:
	");
}
void/*long long*/
__div10(/*long long a, long long b*/) /* 負÷正 */
{
	asm("
	;***** div0s *****
	ld.w %r10, %r12		; %r13:%r12:%r11:%r10 = -1:-1:%r13:%r12
	ld.w %r11, %r13
	ld.w %r12, -1
	ld.w %r13, -1
	;***** div1 *****
	xld.w %r4, 64		; %r4 = 64
__div10_L10:
	add %r10, %r10		; %r13:%r12:%r11:%r10 <<= 1
	adc %r11, %r11
	adc %r12, %r12
	adc %r13, %r13
	add %r12, %r14		; %r13:%r12 += %r15:%r14
	adc %r13, %r15
	jrult __div10_L20	; if(!C) {
	jp.d __div10_L30	;   %r10'0 = 1
	or %r10, 1
__div10_L20:			; } else {
	sub %r12, %r14		;   %r13:%r12 -= %r15:%r14
	sbc %r13, %r15
__div10_L30:			; }
	sub %r4, 1		; if(--%r4) goto L10
	jrne __div10_L10
	;***** div2s *****
	add %r12, %r14		; %r13:%r12 += %r15:%r14
	adc %r13, %r15
	or %r12, %r13
	jrne __div10_L40	; if(%r13:%r12 == 0) {
	add %r10, 1		;   %r11:%r10 += 1
	jruge __div10_L50
	jp.d __div10_L50
	add %r11, 1
__div10_L40:			; } else {
	sub %r12, %r14		;   %r13:%r12 -= %r15:%r14
	sbc %r13, %r15
__div10_L50:			; }
	;***** div3s *****
	not %r10, %r10		; %r11:%r10 = -%r11:%r10
	add %r10, 1
	jruge.d __div10_L60
	not %r11, %r11
	add %r11, 1
__div10_L60:
	");
}
void/*long long*/
__div11(/*long long a, long long b*/) /* 負÷負 */
{
	asm("
	;***** div0s *****
	ld.w %r10, %r12		; %r13:%r12:%r11:%r10 = -1:-1:%r13:%r12
	ld.w %r11, %r13
	ld.w %r12, -1
	ld.w %r13, -1
	;***** div1 *****
	xld.w %r4, 64		; %r4 = 64
__div11_L10:
	add %r10, %r10		; %r13:%r12:%r11:%r10 <<= 1
	adc %r11, %r11
	adc %r12, %r12
	adc %r13, %r13
	sub %r12, %r14		; %r13:%r12 -= %r15:%r14
	sbc %r13, %r15
	jruge __div11_L20	; if(C) {
	jp.d __div11_L30	;   %r10'0 = 1
	or %r10, 1
__div11_L20:			; } else {
	add %r12, %r14		;   %r13:%r12 += %r15:%r14
	adc %r13, %r15
__div11_L30:			; }
	sub %r4, 1		; if(--%r4) goto L10
	jrne __div11_L10
	;***** div2s *****
	sub %r12, %r14		; %r13:%r12 -= %r15:%r14
	sbc %r13, %r15
	or %r12, %r13
	jrne __div11_L40	; if(%r13:%r12 == 0) {
	add %r10, 1		;   %r11:%r10 += 1
	jruge __div11_L50
	jp.d __div11_L50
	add %r11, 1
__div11_L40:			; } else {
	add %r12, %r14		;   %r13:%r12 += %r15:%r14
	adc %r13, %r15
__div11_L50:			; }
	;***** div3s *****
	");
}

/* * ちょっとしたテクニックのメモ：
 *   S1C33には、N(符号)フラグによる条件分岐がありません。
 *   だから、Z80やx86で使っていたような、こんな感じのコードは使えません。
 *	or a
 *	jp m, (負値の処理)
 *	(正値の処理)
 *   andで調べてもいいですが、0x80000000とのandは高くつきます。
 *	xand %r13, 0x80000000	; ←3命令必要です
 *	jrne (負値の処理)
 *	(正値の処理)
 *   そこで、scan1を使いましょう。
 *   scan1は、最上位ビットに1を発見した場合のみ、Zフラグがセットされます。
 *   （なぜなら、デスティネーションレジスタが0になるからです。）
 *	scan1 %r4, %r13		; %r4は破壊されることに注意！
 *	jreq.d (負値の処理)
 *	(正値の処理)
 *   scan0/1は一見複雑そうで複数サイクル命令かと思ってしまいますが、1サイクル命令です。
 *   ディレイド命令にもできますよ。
 */
void/*long long*/
__divdi3(/*long long a, long long b*/) /* 符号付き除算 */
{
	asm("
	scan1 %r4, %r13
	jreq.d __divdi3_L10
	scan1 %r4, %r15
	xjrne __div00
	xjp   __div01
__divdi3_L10:
	xjrne __div10
	xjp   __div11
	");
}

void/*long long*/
__udivdi3(/*long long a, long long b*/) /* 符号無し除算 */
{
	asm("
	xjrne __div00
	");
}

void/*long long*/
__moddi3(/*long long a, long long b*/) /* 符号付き剰余 */
{
	asm("
	xcall __divdi3
	ld.w %r10, %r12
	ld.w %r11, %r13
	");
}

void/*long long*/
__umoddi3(/*long long a, long long b*/) /* 符号無し剰余 */
{
	asm("
	xcall __udivdi3
	ld.w %r10, %r12
	ld.w %r11, %r13
	");
}

