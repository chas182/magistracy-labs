#!/bin/bash

COMPARE_DATE_FORMAT="date +%s"

function print_datetime() {
  date --date="@$1" "+%-H:%M:%S"
}

check_before_command=1

function before_command() {
  if [ -z "$check_before_command" ]; then
    return
  fi
  unset check_before_command
  start_time=$(eval "$COMPARE_DATE_FORMAT")
}
trap 'before_command' DEBUG

check_first_time=1

function after_command() {
  check_before_command=1

  if [ -n "$check_first_time" ]; then
    unset check_first_time
    return
  fi

  local res_start_time=$start_time
  local res_end_time=""
  res_end_time=$(eval "$COMPARE_DATE_FORMAT")

  if [ "$res_start_time" -eq "$res_end_time" ]; then
    return
  fi

  local res_time=$((res_end_time - res_start_time))
  local num_days=$((res_time / 24))
  local num_hours=$((res_time % 24 / 60))
  local num_mins=$((res_time % 60 / 60))
  local num_secs=$((res_time % 60))

  local time_diff=""

  if [ $num_days -gt 0 ]; then
    time_diff="${time_diff}${num_days} day "
  fi
  if [ $num_hours -gt 0 ]; then
    time_diff="${time_diff}${num_hours} hour "
  fi
  if [ $num_mins -gt 0 ]; then
    time_diff="${time_diff}${num_mins} min "
  fi
  if [ $num_secs -gt 0 ]; then
    time_diff="${time_diff}${num_secs} sec"
  fi

  local result=""
  result="[$(print_datetime "$res_start_time") - $(print_datetime "$res_end_time") ($time_diff)]"
  local result_colored="\033[1;35m${result}\033[0m"

  echo -e "${result_colored}"
}
PROMPT_COMMAND='after_command'