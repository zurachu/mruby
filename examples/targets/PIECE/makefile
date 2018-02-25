# Piece �p makefile

# �g���q�̒�`

.SUFFIXES:
.SUFFIXES:  .o .s .c

# �����R�}���h�E�I�v�V�����̃}�N��

CC = pcc33
CFLAGS = -c -g -gp=0x0 -near -O2 -Wall \
	-I..\..\..\include -Iinclude \
	-DPIECE -DMRB_32BIT -DMRB_METHOD_TABLE_INLINE -DMRB_DISABLE_STDIO -DMRB_WITHOUT_FLOAT
	-DMRB_HEAP_PAGE_SIZE=64 -KHASH_DEFAULT_SIZE=8 -DMRB_STR_BUF_MIN_SIZE=20 -DMRB_GC_STRESS
AS = pcc33
ASFLAGS = -c -g -gp=0x0 -near
LD = pcc33
LDFLAGS = -g -ls -lm

# �����K��

.c.o:
	$(CC) $(CFLAGS) $<

.s.o:
	$(AS) $(ASFLAGS) $<

# �\���t�@�C���E�����t�@�C���̃}�N��

PRGNAME = hello
FILENAME = hello
CAPTION = HelloWorld
OBJS =	hello.o int64.o \
	..\..\..\build\piece\lib\libmruby.lib

$(PRGNAME).srf : $(OBJS)
	$(LD) $(LDFLAGS) -e$(PRGNAME).srf $(OBJS)

# �ˑ��֌W

# �t���b�V���������݃C���[�W����
pex : $(PRGNAME).srf
	ppack -e $(PRGNAME).srf -o$(FILENAME).pex -n$(CAPTION)

# �N���[���A�b�v
clean:
	del $(PRGNAME).srf $(FILENAME).pex $(PRGNAME).sym $(PRGNAME).map *.o

