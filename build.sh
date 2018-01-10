OUTDIR=./out
TARGET=wallac
CC=cc
AS=as
LD=cc
CFLAGS="-Wno-parentheses -Wno-int-to-pointer-cast"

mkdir -p $OUTDIR
cd $OUTDIR

for i in ../src/*.c
do echo Compiling $i...
$CC $CFLAGS -S $i
done

for i in ./*.s
do echo Compiling $i...
$AS -o $i.o $i
done

echo Generating the target file $TARGET...
$LD -o $TARGET *.o;

cd ..

