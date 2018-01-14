OUTDIR=./out
TARGET=wcc

CCPARAMS="-Wno-parentheses -Wno-int-to-pointer-cast -S"
CC=cc
#CC=wcc

AS=as
LD=cc

mkdir -p $OUTDIR
cd $OUTDIR

cp ../src/*.s .

for i in ../src/*.c
do echo Compiling $i...
$CC $CCPARAMS $i
done

for i in ./*.s
do echo Compiling $i...
$AS -o $i.o $i
done

echo Generating the target file $TARGET...
$LD -o $TARGET *.o;

cd ..

