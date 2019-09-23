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
WIDTH=349
HEIGHT=86.2
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

NPROTON=$(bc <<< $PDENSITY*$WIDTH*$HEIGHT/1)
HWID=$(bc <<< $WIDTH/2)
HHGT=$(bc <<< $HEIGHT/2)
#echo NPROTON=$NPROTON
LAST=$(bc <<< $START+$NPROJ*$INCR-$INCR)
BINDIR=${DIR}/${PHANTOM}/bin
mkdir -p ${BINDIR}

MATLAB=$(type -P matlab) && MATLAB="$MATLAB -nodisplay -nosplash -r"
test -z "$MATLAB" && MATLAB=$(type -P octave) && MATLAB="$MATLAB --no-gui --eval"
test -z "$MATLAB" && ( echo "either octave or matlab is required" ; exit 1 )
echo $MATLAB

mkdir -p jobs
LOGDIR=${DIR}/${PHANTOM}/Preprocessing_log
mkdir -p ${LOGDIR}
SEED=$RANDOM
QSUB=jobs/Pre_${PHANTOM}.qsub
cat >${QSUB} <<END
#!/bin/sh
#PBS -J $START-$LAST:$INCR
#PBS -o ${LOGDIR}/topas_^array_index^.out
#PBS -e ${LOGDIR}/topas_^array_index^.err

echo hostname=$(hostname) 1>&2
module load matlab
module list
echo "Job working directory: ${PWD}"
cd ${PWD}/matlab
pwd
echo PBS_ARRAY_INDEX=\$PBS_ARRAY_INDEX
ANGLE=\$PBS_ARRAY_INDEX
echo ANGLE=\${ANGLE}
$MATLAB "Preprocessing(\"$DIR\", \"$PHANTOM\", \${ANGLE}, \"$PREPARER\"); exit"
END
qsub $QSUB