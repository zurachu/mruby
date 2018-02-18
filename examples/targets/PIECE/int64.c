/*
 *	int64.c
 *
 *	64�r�b�g�������Z���C�u�����i�ƃe�X�g�v���O�����j
 *	Copyright (C) 2002 Naoyuki Sawa
 *
 *	* Sun Oct 13 06:16:00 JST 2002 Naoyuki Sawa
 *	- �쐬�J�n�B
 */

/****************************************************************************
 *	64�r�b�g�������Z���C�u����
 ****************************************************************************/

/* * C�R���p�C��(gcc33)���̂�long long��F�����A�����ƃR�[�h����������̂ł����A
 *   64�r�b�g�������Z���C�u�������܂܂�Ă��Ȃ��̂ŁA�g���܂���ł����B
 *   �����ŁA64�r�b�g�������Z���C�u���������O�Ŏ������Ă݂܂����B
 *
 * * �{���́Aidiv.lib(������]���Z���C�u����)��fp.lib(���������_���Z���C�u����)�A
 *   fpp.lib(���������_���C�u����)�Ɠ��l�ɁA�R���p�C���ɕt���ė���ׂ����̂��Ǝv���̂ł����A
 *   �Ȃ����܂܂�Ă��܂���B
 *   P/ECE�̔z�z�Z�b�g�ɓ���Y�ꂽ�Ƃ����̂ł͂Ȃ��AEPSON�̔z�z�Z�b�g�ɓ����Ă��Ȃ��悤�ł��B
 *   �iC�R���p�C���}�j���A���̃��C�u�����ꗗ�ɂ��ڂ��Ă��Ȃ����Ƃ��琄���j
 *   ������������Agcc33�� long long ���Z�̏o�̓R�[�h�ɖ�肪�������肵�āA
 *   �Ή�����O���Ă��܂����̂����m��܂���B
 *   �������A������������ł͐������R�[�h����������Ă���悤�Ɍ����܂����̂ŁA
 *   64�r�b�g�������Z���C�u���������p�ӂ���� long long �����ʂɎg����Ɣ��f���܂����B
 *   ���̂Ƃ���A�����Ɠ����Ă���悤�Ɏv���܂��B
 *
 * * �߂�l�E������錾���Ă͂����܂���I
 *   �錾����ƁA�]�v�ȃv�����[�O�E�G�s���[�O�R�[�h����������Ă��܂��܂��B
 *
 * * ���������_���l��long long�̕ϊ��T�|�[�g�֐��͖������ł��B
 *   __fixsfdi,__fixunssfdi,__floatdisf�Ȃǂ��Ă΂��悤�ł��B
 *   �܂��Afloat����long long�֕ϊ�����Ƃ��ɁA__cmpdi2�Ƃ����֐����Ă΂�܂��B
 *   ���O���琄�������long long�̔�r�֐��̂悤�ł����A���ł͎g����l�q������܂���B
 *   �ʏ��long long�̔�r�����ł́AC�R���p�C�����C�����C���R�[�h�𐶐����Ă���̂ł��B
 *   �Ȃ�float����long long�ւ̕ϊ��������A__cmpdi2���Ă΂��̂��s���ł��B
 *
 * * 64�r�b�g�萔�̃T�t�B�b�N�X�́ull�v(�����t��)�uull�v�i���������j�ł��B
 *   �����̃T�t�B�b�N�X��t���Ȃ��Ă��A�傫�Ȓ萔�͂�����64�r�b�g�Ƃ��ĔF������܂����A
 *   �R���p�C�����Ɍx�����\������܂��B
 *   <��>
 *	long long a = 1000000000000000000;   // �x���i����͂n�j�j
 *	long long b = 1000000000000000000ll; // �n�j
 */

void/*long long*/
__negdi2(/*long long a*/) /* �������] */
{
	asm("
	ld.w %r10, 0		; %r11:%r10 = 0:0 - %r13:%r12
	ld.w %r11, 0
	sub %r10, %r12
	sbc %r11, %r13
	");
}

void/*long long*/
__adddi3(/*long long a, long long b*/) /* ���Z */
{
	asm("
	ld.w %r10, %r12		; %r11:%r10 = %r13:%r12 + %r15:%r14
	ld.w %r11, %r13
	add %r10, %r14
	adc %r11, %r15
	");
}

void/*long long*/
__subdi3(/*long long a, long long b*/) /* ���Z */
{
	asm("
	ld.w %r10, %r12		; %r11:%r10 = %r13:%r12 - %r15:%r14
	ld.w %r11, %r13
	sub %r10, %r14
	sbc %r11, %r15
	");
}

/* * �ÓT�I�ȕM�Z�^�̏�Z���[�`���ł��B
 *   �u64�r�b�g�~64�r�b�g��64�r�b�g�v�Ȃ̂ŁA�������l������K�v�͂���܂���B
 *   ���ۂ�gcc33�́A64�r�b�g��Z�̏ꍇ�͕����t���E���������Ɋւ�炸�A
 *   __muldi3���Ăяo���R�[�h�𐶐�����悤�ł��B�i__umuldi3�͂���܂���j
 * * ������Ƃ����e�N�j�b�N�̃����F
 *   S1C33�ɂ̓L�����[�t���O���݂̃V�t�g�E���[�e�[�g������܂���B
 *   �搔��1�r�b�g�ÂE�V�t�g���čŉ��ʃr�b�g�𒲂ׂ�K�v������܂����A
 *   �������W�X�^��A�����ĉE�V�t�g���鏈���́A�܂Ƃ��ɏ����Ɣ��ɒ����Ȃ�܂��B
 *   �Ⴆ�΁A%r15��%r14��A�����ĉE1�r�b�g�V�t�g���鏈���́A����Ȋ����ɂȂ�܂��B
 *	ld.w %r4, %r15	; %r4'31 = %r15'0
 *	xsll %r4, 31	; ��3���ߕK�v�ł�
 *	srl %r15, 1
 *	srl %r14, 1
 *	or %r14, %r4
 *   C�R���p�C���́A��1�r�b�g�V�t�g�ɂ��Ă����l�̃R�[�h�𐶐����܂����A
 *   �����ƊȒP�ȕ��@������܂��B
 *	add %r14, %r14
 *	adc %r15, %r15	; C <- %r15'31
 *   ���āA�M�Z�^��Z���[�`���ł́A�搔���ŉ��ʃr�b�g���璲�ׂčs���̂ŁA�E�V�t�g���K�v�ł��B
 *   �������Ȃ���O�q�̒ʂ�AS1C33 CPU�ł́A���V�t�g�����Ɋr�ׂĉE�V�t�g�����͓���̂ł��B
 *   �����ŁA�搔�̑S�r�b�g�����E���]�����Ă��܂��܂��傤�B
 *   �������Ă����΁A�ȒP�ȍ��V�t�g���g���Ȃ���A�ŏ�ʃr�b�g�𒲂ׂ�΂������ƂɂȂ�܂��B
 *	swap %r14, %r14		; %r14:%r15 = ���E���](%r15:%r14)
 *	mirror %r14, %r14	; ���W�X�^�͓���ւ��Ȃ��̂ŁA���]���%r14����ʂɂȂ邱�Ƃɒ���
 *	swap %r15, %r15
 *	mirror %r15, %r15
 *  S1C33�̃V�t�g�E���[�e�[�g���߂ł́A�ŏ�ʃr�b�g�͒P�Ɏ̂Ă��Ă��܂��܂����A
 *  add/adc�ɂ�鍶�V�t�g�����Ȃ�A�ŏ�ʃr�b�g���L����[�t���O�ɓ���̂ŁA
 *  ���V�t�g��ɃL�����[�t���O�𒲂ׂ邾���ōς݂܂��B
 */
void/*long long*/
__muldi3(/*long long a, long long b*/) /* ��Z */
{
	asm("
	ld.w %r10, 0		; %r11:%r10 = 0:0
	ld.w %r11, 0
	swap %r14, %r14		; %r14:%r15 = ���E���](%r15:%r14)
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

/* __divdi3/__udivdi3�̃T�|�[�g�֐��B
 * S1C33000�R�ACPU�}�j���A����div0s/12s/3s�̏ڍא����ǂ���Ɏ������܂����B
 * * �[�����Z��O�͐������܂���B
 * * S1C33�̊֐��Ăяo���K��ǂ���A%r11:%r10�ɏ���Ԃ��܂��B
 *   ����ɉ����āA%r13:%r12�ɗ]���Ԃ��܂��B�i__moddi3/__umoddi3�Ŏg���Ă��܂��j
 *   �{���́A%r13:%r12�ɖ߂�l���Z�b�g���Ă��A�����ƕԂ���邩�ǂ����ۏ؂���Ȃ��̂ł����A
 *   �Ƃ肠���������̊֐��̓v�����[�O�E�G�s���[�O�R�[�h���܂܂Ȃ��̂ŁA���v�݂����ł��B
 * * __divdi3/__udivdi3���炵���g��Ȃ��̂ŁAstatic�֐��ɂ������������̂ł����A��������ƁA
 *   C�R���p�C�����u�g���Ă��Ȃ�staic�֐�������܂��v�Ƃ����Ӗ��̌x�����o���Ă��܂��܂��B
 *   C�R���p�C���́A__divdi3/__udivdi3�̒���asm()�u���b�N����̌Ăяo����F�����Ȃ�����ł��B
 *   �d�����Ȃ��̂ŁA�O���[�o���֐��ɂ��Ă���܂��B
 * * div3s�̕������]������������Ƌꂵ���ł����A�uadc %rd, imm6�v���Ȃ��̂Ŏ~�ނ𓾂܂���...
 *   �uadc %rd, imm6�v������΁unot %r10,%r10; not %r11,%r11; add %r10,1; adc %r11,0�v�Ƃł���̂ɁB
 *   �ł��܂����ߐ���1�������Ȃ����A�T�C�N�������ő�2�������Ȃ��̂ŁA�������ĕς��Ȃ����ȁH
 * * �܂��e�X�g���s�\���Ȃ̂ŁA������������o�O���c���Ă邩���m��܂���B
 */
void/*long long*/
__div00(/*long long a, long long b*/) /* ������ */
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
__div01(/*long long a, long long b*/) /* ������ */
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
__div10(/*long long a, long long b*/) /* ������ */
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
__div11(/*long long a, long long b*/) /* ������ */
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

/* * ������Ƃ����e�N�j�b�N�̃����F
 *   S1C33�ɂ́AN(����)�t���O�ɂ��������򂪂���܂���B
 *   ������AZ80��x86�Ŏg���Ă����悤�ȁA����Ȋ����̃R�[�h�͎g���܂���B
 *	or a
 *	jp m, (���l�̏���)
 *	(���l�̏���)
 *   and�Œ��ׂĂ������ł����A0x80000000�Ƃ�and�͍������܂��B
 *	xand %r13, 0x80000000	; ��3���ߕK�v�ł�
 *	jrne (���l�̏���)
 *	(���l�̏���)
 *   �����ŁAscan1���g���܂��傤�B
 *   scan1�́A�ŏ�ʃr�b�g��1�𔭌������ꍇ�̂݁AZ�t���O���Z�b�g����܂��B
 *   �i�Ȃ��Ȃ�A�f�X�e�B�l�[�V�������W�X�^��0�ɂȂ邩��ł��B�j
 *	scan1 %r4, %r13		; %r4�͔j�󂳂�邱�Ƃɒ��ӁI
 *	jreq.d (���l�̏���)
 *	(���l�̏���)
 *   scan0/1�͈ꌩ���G�����ŕ����T�C�N�����߂��Ǝv���Ă��܂��܂����A1�T�C�N�����߂ł��B
 *   �f�B���C�h���߂ɂ��ł��܂���B
 */
void/*long long*/
__divdi3(/*long long a, long long b*/) /* �����t�����Z */
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
__udivdi3(/*long long a, long long b*/) /* �����������Z */
{
	asm("
	xjrne __div00
	");
}

void/*long long*/
__moddi3(/*long long a, long long b*/) /* �����t����] */
{
	asm("
	xcall __divdi3
	ld.w %r10, %r12
	ld.w %r11, %r13
	");
}

void/*long long*/
__umoddi3(/*long long a, long long b*/) /* ����������] */
{
	asm("
	xcall __udivdi3
	ld.w %r10, %r12
	ld.w %r11, %r13
	");
}

