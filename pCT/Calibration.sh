#!/bin/bash

usage() { echo -e "Usage: $0\n\
    --dir=<output directory>\n\
    [--width=<width of detector in mm>]\n\
    [--height=<height of detector in mm>]\n\
    [--pdensity=<number of protons / mm^2]\n" 1>&2; exit 1; }

#set -x
if [[ $# == 0 ]]
then
    usage
fi

# read the options
TEMP=$(getopt -o '' -l "width:,height:,dir:,pdensity::" -n $0 -- "$@")
if [[ $? != 0 ]]
then
    usage
fi
eval set -- "$TEMP"

HEIGHT=86.2
WIDTH=349.0
DIR=
PDENSITY=100

# extract options and their arguments into variables.
while true ; do
#    echo '$1='$1 '$2='$2
    case "$1" in
        --dir)
            DIR=$2 ; shift 2 ;;
	--pdensity)
	    PDENSITY=$2 ; shift 2 ;;
        --width)
            WIDTH=$2 ; shift 2 ;;
        --height)
            HEIGHT=$2 ; shift 2 ;;
        --) shift ; break ;;
        *) usage ;;
    esac
done

if [[ -z "$HEIGHT" ]]
then
    echo Required parameter --height is missing
    usage
fi
if [[ -z "$WIDTH" ]]
then
    echo Required parameter --width is missing
    usage
fi
if [[ -z "$DIR" ]]
then
    echo Required parameter --dir is missing
    usage
fi

NPROTON=$(bc <<< "($PDENSITY*$WIDTH*$HEIGHT)/1")
HWID=$(bc <<< $WIDTH/2)
HHGT=$(bc <<< $HEIGHT/2)
#echo NPROTON=$NPROTON

topas=$HOME/topas/topas
echo $topas
mkdir -p jobs
i=0
while (( $i < 5 ))
do 
    SEED=$RANDOM
    BASE=jobs/Calibration_$i
    TOPAS=${BASE}.topas
#    echo '$TOPAS='$TOPAS
    QSUB=${BASE}.qsub
#    echo '$QSUB='$QSUB
    sed -e "s/NPROTON/${NPROTON}/" -e "s/BRICKS/$i/"\
        -e "s;DIR;${DIR};" -e "s/HHGT/$HHGT/" -e "s/HWID/$HWID/" -e "s/SEED/$SEED/" \
        < Calibration.topas > $TOPAS
    if [[ $i == 0 ]]
    then
        sed -i '/Bricks.topas/d' $TOPAS
    fi
    if [[ $NPROTON < 50000 ]]
    then
        mkdir -p $DIR/Calibration_$i
        $topas $TOPAS
    else
        cat >${QSUB} <<END
#!/bin/sh
#PBS -o ${DIR}/${PHANTOM}_${ANGLE}.out
#PBS -e ${DIR}/${PHANTOM}_${ANGLE}.err

echo "Job working directory: ${PWD}"
cd ${PWD}
pwd

alias topas=$HOME/topas/topas

mkdir -p $DIR/Calibration_$i
topas $TOPAS
END
        qsub $QSUB
    fi
    i=$(bc <<< $i+1)
done
