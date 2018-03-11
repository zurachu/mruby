# Piece 用 makefile

# 拡張子の定義

.SUFFIXES:
.SUFFIXES:  .o .s .c

# 生成コマンド・オプションのマクロ

CC = pcc33
CFLAGS = -c -g -gp=0x0 -near -O2 -Wall \
	-I..\..\..\include -Iinclude \
	-DPIECE -DMRB_32BIT -DMRB_METHOD_TABLE_INLINE -DMRB_DISABLE_STDIO -DMRB_WITHOUT_FLOAT
	-DMRB_HEAP_PAGE_SIZE=8 -DKHASH_DEFAULT_SIZE=8 -DMRB_STR_BUF_MIN_SIZE=20 -DMRB_GC_STRESS
AS = pcc33
ASFLAGS = -c -g -gp=0x0 -near
LD = pcc33
LDFLAGS = -g -ls -lm

MRBC = ..\..\..\build\host\bin\mrbc

# 生成規則

%.c : %.rb
	$(MRBC) -B$(notdir $(basename $<)) $<

.c.o:
	$(CC) $(CFLAGS) $<

.s.o:
	$(AS) $(ASFLAGS) $<

# 構成ファイル・生成ファイルのマクロ

PRGNAME = hello
FILENAME = hello
CAPTION = HelloWorld
OBJS =	hello.o int64.o mrb_hello.o \
	..\..\..\build\piece\lib\libmruby.lib

$(PRGNAME).srf : $(OBJS)
	$(LD) $(LDFLAGS) -e$(PRGNAME).srf $(OBJS)

# 依存関係

# フラッシュ書き込みイメージ生成
pex : $(PRGNAME).srf
	ppack -e $(PRGNAME).srf -o$(FILENAME).pex -n$(CAPTION)

# クリーンアップ
clean:
	del $(PRGNAME).srf $(FILENAME).pex $(PRGNAME).sym $(PRGNAME).map *.o

