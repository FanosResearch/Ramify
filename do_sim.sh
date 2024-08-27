#!/bin/bash
#
# File  : do_sim.sh
# Author: Mohamed Abuelala
# Email : abuelalm@mcmaster.ca
#
# Created On Dec 20, 2020
#
#!/bin/bash
#
# how to run the script
# 1) Script arguments:
#    ./do_sim.sh <TestType> <TestCategory> <TestCaseNum>
#
#    <TestType>        : It can be either "Directed" or "Regression".
#
#    <TestCategory>    : It can be either "hit_R_TC" or "hit_W_TC".
#                        (will be extended)
#
#    <TestCaseNum>     : This argument used when running the script in
#    "TestType = Directed" to  select a certain test case number from
#    "TestCategory", for example:
#    0  = "hit_0"
#    1  = "hit_10"
#    2  = "hit_20"
#    3  = "hit_30"
#    4  = "hit_40"
#    5  = "hit_50"
#    6  = "hit_60"
#    7  = "hit_70"
#    8  = "hit_80"
#    9  = "hit_90"
#    10 = "hit_100"
# ------------------------------------------------------
# 2) Example:
#    2.1) Directed Mode
#            ./do_sim.sh Directed hit_R_TC 5
#            ./do_sim.sh Directed hit_R_TC 3
# ------------------------------------------------------
#    2.2) Regression Mode
#            ./do_sim.sh Regression hit_R_TC 0
#            ./do_sim.sh Regression hit_R_TC 0

start="`date`"

# script arguments
TestType="$1"
TestCATG="$2"
#TCNum=$3

# Bin directory
ConfigBasePath="./config"
AppPath="$ConfigBasePath/app"
DevPath="$ConfigBasePath/device"
SysPath="$ConfigBasePath/system"

# Output directory
OutBasePath="./outputs"

if [ $TestCATG == 'RW_SWP' ]
then
  OutPath="$OutBasePath/RW_SWP"
else
  echo "wrong Testcase."
fi

# benchmark names
declare -a RW_SW_TC
RW_SW_TC=("hit0"
          "hit1"
          "hit2"
          "hit3"
          "hit4"
          "miss0"
          "miss1"
          "miss2"
          "miss3"
          "miss4"
          "random"
          "sequential")

declare -a RW_SW_SZ
RW_SW_SZ=(0
          10
          20
          30
          40
          50
          60
          70
          80
          90
          100)

# check input configuration and run simulations
case "$TestType" in

  Regression)

    echo "Run Regression Test ...."

    case "$TestCATG" in
      RW_SWP)
        
        echo "Running RW_SWP BM"

        for TC in "${RW_SW_TC[@]}"
        do

          TCAppPath="$AppPath/$TC.ini"
          TCDevPath="$DevPath/device.ini"
          TCSysPath="$SysPath/system.ini"

          TCOutPath="$OutPath/$TC"

          for RW_SW in "${!RW_SW_SZ[@]}"
          do

            sed -i "5s! \([0-9]\+\)! 20000000!" "$TCAppPath"
            sed -i "60s! \([0-9]\+\)! ${RW_SW_SZ[$RW_SW]}!" "$TCAppPath"

            echo "============================================================== "
            echo "Running ${TC}_${RW_SW_SZ[$RW_SW]} Trace:"
            echo " "

            ./RAMBench --app "$TCAppPath" --device "$TCDevPath" --system "$TCSysPath" > "$TCOutPath/rw_switch_${RW_SW_SZ[$RW_SW]}.trace"


          done
        done
        echo "Test done "
        echo "============================================================== "
      ;; #Test

    esac
    ;; #Regression

esac
