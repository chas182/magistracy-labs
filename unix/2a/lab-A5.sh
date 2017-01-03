#!/bin/bash

print_usage () {                        
  echo "usage: $(basename "$0") [-h|--help] <MESSAGE> [<DEFAULT_ANSWER>]"
}

print_help () {
  print_usage
  echo "
Asks question untill positive or negative answer. Answers are only the following: [y, n, Y, N].
In case of answer is not specified and <DEFAULT_ANSWER> is provided - it will be used as answer.

Positional args:
  MESSAGE             question that will be asked to user untill his answer
  DEFAULT_ANSWER      [Y|N] - answer, that will be used in case of empty string

Options:
  -h, --help          help

Exit status:
  0: for positive answer
  1: for negative answer
"
}

if [ "$#" == "0" ]; then
  print_usage
  exit -1
fi

if [ "$1" == "-h" ] || [ "$1" == "--help" ]; then 
  print_help
  exit -1
fi

if [ -n "$2" ] && [ "$2" != "Y" ] && [ "$2" != "--help" ]; then 
  print_help
  exit -1
fi

[[ $2 = "Y" ]] && answ="[Y/n]" || [[ $2 = "N" ]] && answ="[Y/n]" || answ="[y/n]"
a="-"
while ([ "$a" != "Y" ] && [ "$a" != "N" ]) && ([ -n "$a" ] || [ ! "$2" ])
do
  echo "$1" "$answ"
  IFS="\n"
  read a
  a=${a:0:1}
  a=${a^}
done

[[ "$a" == "" ]] && a="$2"

if [ "$a" == "Y" ]; then
  exit 0
else
  exit 1
fi