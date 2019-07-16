#!/bin/bash

usage() { echo -e "Usage: $0\n\
    --phantom=[WaterPhantom|CTP404]\n\
    --dir=<output directory>\n\
    [--preparer=<name of preparer>]\n\
    [--width=<width of detector in mm>]\n\
    [--height=<height of detector in mm>]\n\
    [--pdensity=<number of protons / mm^2]\n\
    [--nproj=<number of projections>]\n\
    [--start=<start angle>]\n\
    [--incr=<angle increment>]" 1>&2; exit 1; }

#set -x
if [[ $# == 0 ]]
then
    usage
fi

# read the options
TEMP=$(getopt -o '' -l "phantom:,width:,height:,dir:,preparer:,pdensity::,nproj::,incr::,start::" -n $0 -- "$@")
if [[ $? != 0 ]]
then
    usage
fi
eval set -- "$TEMP"

PHANTOM=
HEIGHT=200
WIDTH=400
DIR=
NPROJ=1
INCR=4
START=0
PDENSITY=100
PREPARER=$(grep "^$USER:" /etc/passwd | cut -f5 -d:)

# extract options and their arguments into variables.
while true ; do
#    echo '$1='$1 '$2='$2
    case "$1" in
        --phantom)
            PHANTOM=$2 ; shift 2 ;;
        --dir)
            DIR=$2 ; shift 2 ;;
	--pdensity)
	    PDENSITY=$2 ; shift 2 ;;
        --nproj)
            NPROJ=$2 ; shift 2 ;;
        --width)
            WIDTH=$2 ; shift 2 ;;
        --height)
            HEIGHT=$2 ; shift 2 ;;
        --incr)
            INCR=$2 ; shift 2 ;;
        --start)
            START=$2 ; shift 2 ;;
        --preparer)
            PREPARER=$2 ; shift 2 ;;
        --) shift ; break ;;
        *) usage ;;
    esac
done

if [[ -z "$PHANTOM" ]]
then
    echo Required parameter --phantom is missing
    usage
fi
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

NPROTON=$(bc <<< $PDENSITY*$WIDTH*$HEIGHT)
HWID=$(bc <<< $WIDTH/2)
HHGT=$(bc <<< $HEIGHT/2)
#echo NPROTON=$NPROTON


mkdir -p jobs
i=0
while (( $i < $NPROJ ))
do 
    SEED=$RANDOM
#    echo ${START}+${i}*${INCR}
    ANGLE=$(bc <<< ${START}+${i}*${INCR})
#    echo '$ANGLE='$ANGLE
    BASE=jobs/${PHANTOM}_${ANGLE}
    TOPAS=${BASE}.topas
#    echo '$TOPAS='$TOPAS
    QSUB=${BASE}.qsub
#    echo '$QSUB='$QSUB
    sed -e "s/PHANTOM/${PHANTOM}/" -e "s/NPROTON/${NPROTON}/" -e "s/ANGLE/${ANGLE}/" \
        -e "s;DIR;${DIR};" -e "s/HHGT/$HHGT/" -e "s/HWID/$HWID/" -e "s/SEED/$SEED/" \
        < Template.topas > $TOPAS
    if [[ $NPROTON < 10000 ]]
    then
        mkdir -p $DIR/$ANGLE
        topas $TOPAS
        python InputEdiffBin.py --dir=$DIR --angle=$ANGLE --phantom=$PHANTOM --preparer="$PREPARER" --histories=$NPROTON
    else
        cat >${QSUB} <<END
#!/bin/sh
#PBS -o ${DIR}/${PHANTOM}_${ANGLE}.out
#PBS -e ${DIR}/${PHANTOM}_${ANGLE}.err

echo "Job working directory: ${PWD}"
cd ${PWD}
pwd

mkdir -p $DIR/$ANGLE
topas $TOPAS
python InputEdiffBin.py --dir=$DIR --angle=$ANGLE --phantom=$PHANTOM --preparer="$PREPARER" --histories=$NPROTON
END
        qsub $QSUB
    fi
    i=$(bc <<< $i+1)
done
