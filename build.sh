OUTDIR=./out
TARGET=wallac
CC=gcc
CFLAGS="-Wno-logical-op-parentheses -Wno-int-to-void-pointer-cast"


mkdir -p $OUTDIR
cd $OUTDIR

for i in ../src/*.c
do echo Compiling $i...
$CC $CFLAGS -c $i
done

echo Generating the target file $TARGET...
$CC -o $TARGET *.o;

cd ..

