# RAMBench

## Table of Contents:

1) [ An Overview ](#an-overview)
2) [ Usage ](#usage)
3) [ Building RAMBench ](#getting-started)
3) [ Running RAMBench ](#running-command)
4) [ Automated Simulation ](#running-sim)

<a name="an-overview"></a>
## An Overview

RAMBench is a framework for Benchmark generation suite to evaluate different
memory architectures, that are built on SW simulators:

- DRAMsim2 \[1\] 
- Ramulator \[2\]
- MCsim \[3\]

[\[1\] Rosenfeld et al. *Dramsim2: A cycle accurate memory system simulator.* IEEE CAL 2011.](https://user.eng.umd.edu/~blj/papers/cal10-1.pdf)

[\[2\] Kim et al. *Ramulator: A Fast and Extensible DRAM Simulator.* IEEE CAL
2015.](https://people.inf.ethz.ch/omutlu/pub/ramulator_dram_simulator-ieee-cal15.pdf)

[\[3\] Mirosanlou et al. *Mcsim: An extensible dram memory controller simulator.* IEEE CAL 2020.](https://www.ece.mcmaster.ca/faculty/hassan/assets/publications/reza_MCsim_CAL.pdf)


<a name="usage"></a>
## Usage

RAMBench creates both a C++ program that is runnable in any machine or full-system simulator as well as memory traces that can be used with standalone memory simulators.



<a name="getting-started"></a>
## Building RAMBench

RAMBench requires a C++11 compiler (e.g., `clang++`).



```
      $ make
```
<a name="running-command"></a>
## Running RAMBench
To run RAMBench, you need to provide three configuration files: 1) application, 2) device, and 3) system that models your use-case. 
Examples of these configuration files are under config/app, config/device, and config/system, respectively.
An example of a run command is as follows:

```
      
    $ /RAMBench --app config/app/app.ini --device config/device/device.ini --system config/system/system.ini > exampleOutput.txt
    
```
<a name="running-sim"></a>
## Automated Simulation

Once you have built and run the RAMBench, it should be easy to use the automation bash script "do_sim.sh" to run both directed and regression test using the following command

```
./do_sim.sh <TestType> <TestCategory> <TestCaseNum> 
```
Where, 

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
