#!/bin/bash

echo original parameters=[$*]
echo original OPTIND=[$OPTIND]

optstring=":a:bc"

while getopts ${optstring} opt; do
    case ${opt} in
        a)
            echo "This is -a option. OPTARG=[${OPTARG}] OPTIND=[${OPTIND}]"
            ;;
        b)
            echo "This is -b option. OPTARG=[${OPTARG}] OPTIND=[${OPTIND}]"
            ;;
        c)
            echo "This is -c option. OPTARG=[${OPTARG}] OPTIND=[${OPTIND}]"
            ;;
        ?)
            echo "There is unrecognized parameter."
            exit 1
            ;;

    esac
done

shift $((${OPTIND} - 1))
echo remaining parameters[$*]
echo \$1=[$1]
echo \$2=[$2]

# bash getopts.sh -a 12 -b -c file1 file2
