/*-----------------------------------------------------------------------------
 * Class: Process.h
 * Author: Ricardo N. Guntur <ricardo.guntur@nyu.edu>
 * Course: Operating Systems -- Professor Allan Gottlieb
 -----------------------------------------------------------------------------*/
#ifndef LAB_2_TESTS_PROCESS_H
#define LAB_2_TESTS_PROCESS_H
#include <sstream>
#include <vector>
#include <iostream>
#include <string>

using namespace std;

//Process Object
class Process {

public:


    void set_arrival_time (int A);
    void set_max_burst (int B);
    void set_CPU_time (int C);
    void set_multi(int M);
    void set_cycle(int cyc);
    void set_cpu_burst(int burst);
    void set_total_wait_time(int wait);
    void set_turn_around_time(int tat);
    void set_block_time(int b);
	void set_cpu_burst_left(int cpub);
    void set_status(string s);
    void set_finish_time(int f);
    void set_total_block_time(int bl);
    void set_initial_cpu_time(int cp);
	void set_quantum(int q);
	void set_ready_time(int t);


    int get_arrival_time();
    int get_max_burst();
    int get_CPU_time();
    int get_multi();
    int get_cycle();
    int get_cpu_burst();
    int get_total_wait_time();
    int get_turn_around_time();
    int get_block_time();
	int get_cpu_burst_left();
    int get_finish_time();
    string get_status();
    int get_total_block_time();
    int get_initial_cpu_time();
	int get_quantum();
	int get_ready_time();


    Process(int a, int b, int c, int m);
private:

    int arrival_time;        //Arrival Time
    int max_burst;           //CPU Burst Time (randomOS(b))
    int CPU_time;            //Total CPU Time
    int multi;               //I/O Burst Time
    int cycle;               //Current cycle
    int block_time;          //Block time
    int cpu_burst;           //Calculated with randomOS()   //maybe remove
    int total_wait_time;     //Wait Time
    int total_block_time;    //Total Block Time for a particular algorithms
    int turn_around_time;    //Turn around Time: Finishing time - arrival time
    int initial_cpu_time;
	int cpu_burst_left;		 //Number of cycles left for current cpu burst
    int finish_time;         //cycle when process finishes
	int quantum;             //Utilized for round robin
	int ready_time;          //Current length of time a process has been in ready state
	string status;           //Status of Process: unstarted, ready, blocked, terminated



};

#endif //LAB_2_TESTS_PROCESS_H