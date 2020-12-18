#!/bin/bash

#little script to compute the differences between the original nestest log and the log produced by my emu. Only the PC is taken into account.
awk '{print $1}' ~/Documents/Work/NESemu/log/nestest_log.txt > ~/Documents/Work/NESemu/log/PC_nestest_log.txt
awk '{print $1}' ~/Documents/Work/NESemu/log/my_log.txt > ~/Documents/Work/NESemu/log/PC_my_log.txt

diff ~/Documents/Work/NESemu/log/PC_nestest_log.txt ~/Documents/Work/NESemu/log/PC_my_log.txt > ~/Documents/Work/NESemu/log/diffs.txt
rm -f ~/Documents/Work/NESemu/log/PC_nestest_log.txt 
rm -f ~/Documents/Work/NESemu/log/PC_my_log.txt
echo "done"
