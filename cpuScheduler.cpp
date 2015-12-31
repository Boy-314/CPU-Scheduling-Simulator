/*-----------------------------------------------------------------------------
 * Class: cpuScheduler.cpp
 * Author: Ricardo N. Guntur <ricardo.guntur@nyu.edu>
 * Course: Operating Systems -- Professor Allan Gottlieb
 -----------------------------------------------------------------------------*/

#include <iostream>
#include "verboseMethods.h"
#include "schedulerMethods.h"

using namespace std;

verboseMethods verbose;

//*****************************Scheduler Methods*****************************

/*-----------------------------------------------------------------------------
 * Method:print_processes()
 * Description: Prints the input processes as per lab instructions.
 * IE. 1 (0 1 5 1) where the first 1 is the number of processes
 -----------------------------------------------------------------------------*/
void schedulerMethods::print_processes() {
    cout << number_processes << " ";
    for (int i = 0; i < number_processes; i++) {
        cout << "(";
        cout << process_vector[i].get_arrival_time() << " ";
        cout << process_vector[i].get_max_burst() << " ";
        cout << process_vector[i].get_initial_cpu_time() << " ";
        cout << process_vector[i].get_multi();
        cout << ") ";
        cout.flush();
    }
}

/*-----------------------------------------------------------------------------
 * Method:randomOS(int U)
 * Description: Returns a Uniformly Distributed Random Integer to
 * process max burst time
 -----------------------------------------------------------------------------*/
int schedulerMethods::randomOS(int U) {
    int currentNumber;
    int value;

    file >> currentNumber;
    value = ((currentNumber % U) + 1);
    return value;
}


schedulerMethods::schedulerMethods() {

    //remove path when program is finished
    file.open("random-numbers.txt");

    if (!file) {
        cout << endl << "Failed to open file: random-numbers.txt" << endl;
    }
}

/*-----------------------------------------------------------------------------
 * Method:reset_processes()
 * Description: Resets the process objects data such that the next
 * algorithm can run with unaltered data.
 -----------------------------------------------------------------------------*/
void schedulerMethods::reset_processes() {
    file.close();
    file.open("random-numbers.txt");

    //Set initial values for processes
    for (int j = 0; j < process_vector.size(); j++) {
        process_vector[j].set_status("unstarted");          //Set to unstarted
        process_vector[j].set_cycle(0);                     //Set to cycle 0
        process_vector[j].set_total_wait_time(0);           //Set initial total wait time
        process_vector[j].set_total_block_time(0);          //Set initial total block time
        process_vector[j].set_CPU_time(process_vector[j].get_initial_cpu_time());
        process_vector[j].set_block_time(0);
        process_vector[j].set_cpu_burst_left(0);
        process_vector[j].set_cpu_burst(0);
        process_vector[j].set_cycle(0);
        process_vector[j].set_turn_around_time(0);
        process_vector[j].set_ready_time(0);

        //Reset initial values for algorithm
        cpu_burst = 0;
        any_running = false;
        has_waited = false;
        currentCycle = -1;
        total_processes_terminated = 0;
        total_cpu_time = 0;
        finish_time = 0.00;
        total_block_time = 0.00;
        total_wait_time = 0.00;
        total_cpu_time = 0;
        total_turnaround_time = 0.00;
        currentProcess = 0;

        //Empty queue
        swap(ready_queue, empty);
        temp_vector.clear();
    }
}

/*-----------------------------------------------------------------------------
 * Method:process_input(string file)
 * Description: Stores the input as Process objects and stores those objects
 * in a vector. Sorts those processes.
 -----------------------------------------------------------------------------*/
void schedulerMethods::process_input(string file) {
    verbose.log("Start Process Input");
    ifstream processes;
    processes.open(file.c_str());
    processes >> number_processes;
    verbose.log("Start looping to store processes into object");

    //Pull out all the data from each process
    for (int i = 0; i < number_processes; i++) {
        processes >> unused;            // Throw out left parentheses
        processes >> a;                 // Arrival Time
        processes >> b;                 // Max Burst
        processes >> c;                 // Total CPU Time needed
        processes >> m;                 //Max IO Block time (M)
        processes >> unused;            // Throw out left parentheses

        Process process(a, b, c, m);
        process_vector.push_back(process);
    }
    //Print unsorted input
    cout << "The original input was: ";
    cout.flush();
    print_processes();

    //Sorts processes based one ARRIVAL TIME
    //ie. input : 3 (3 1 5 1) (2 1 5 1) (1 1 10 1)
    //ie. output: 3 (1 1 10 1) (2 1 5 1) (3 1 5 1)
    while (sorted_vector.size() != number_processes) {
        min = 9999;
        for (int i = 0; i < process_vector.size(); i++) {
            if (process_vector[i].get_arrival_time() <= min) {
                min = process_vector[i].get_arrival_time();
            }
        }

        for (int j = 0; j < process_vector.size(); j++) {
            if (min == process_vector[j].get_arrival_time()) {
                sorted_vector.push_back(process_vector[j]);
                process_vector.erase(process_vector.begin() + j);
                break;
            }
        }
    }
    cout << endl;

    //Clear vector
    process_vector.clear();

    //duplicate SORTED vector to print
    process_vector = sorted_vector;

    //Print sorted input
    cout << "The sorted input was:   ";
    print_processes();
    cout << endl;
}

/*-----------------------------------------------------------------------------
 * Method:print_output()
 * Description: Prints the results of an algorithm being simulated.
 * Formatted as per lab instructions.
 -----------------------------------------------------------------------------*/
void schedulerMethods::print_output() {
    for (int i = 0; i < process_vector.size(); i++) {

        cout << "Process " << i << ":" << endl;
        cout << "(A,B,C,M) = " << "("
        << process_vector[i].get_arrival_time() << ","
        << process_vector[i].get_max_burst() << ","
        << process_vector[i].get_initial_cpu_time() << ","
        << process_vector[i].get_multi() << ")" << endl;
        cout << "Finishing Time: " << process_vector[i].get_finish_time() << endl;          //Finish Time

        total_turnaround_time += (process_vector[i].get_finish_time()
                                  - process_vector[i].get_arrival_time());

        cout << "Turnaround Time: " << (process_vector[i].get_finish_time()
                                        - process_vector[i].get_arrival_time()) << endl;

        cout << "I/O Time: " << process_vector[i].get_total_block_time() << endl;           //Total time blocked
        cout << "Waiting Time: " << process_vector[i].get_total_wait_time() << endl;        //Total time at ready state

        cout << endl << endl;
    }
}

/*-----------------------------------------------------------------------------
 * Method:print_summary()
 * Description: Prints the summary of the data from each of the algorithms.
 -----------------------------------------------------------------------------*/
void schedulerMethods::print_summary() {
    cout << "Summary Data: " << endl;
    cout << "Finishing Time: " << finish_time << endl;
    cout << "CPU Utilization: " << total_cpu_time / finish_time << endl;
    cout << "I/O Utilization: " << total_block_time / finish_time << endl;
    cout << "Throughput: " << ((process_vector.size() / finish_time) * 100) << " processes per hundred cycles" << endl;
    cout << "Average Turnaround: " << (total_turnaround_time / process_vector.size()) << endl;
    cout << "Average Waiting Time: " << (total_wait_time / process_vector.size()) << endl;
}

//*****************************Methods Used In Algorithms*****************************

void schedulerMethods::set_process(int currentCycle) {

    for (int i = 0; i < process_vector.size(); i++) {

        if (process_vector[i].get_status() == "unstarted") {
            if (process_vector[i].get_arrival_time() == currentCycle) {
                process_vector[i].set_status("ready");
                process_vector[i].set_ready_time(0);
                //append process into ready queue
                ready_queue.push(i);
            }
        }


        else if (process_vector[i].get_status() == "blocked") {
            //If block time is finished - set to ready
            if (process_vector[i].get_block_time() <= 0) {
                process_vector[i].set_status("ready");
                process_vector[i].set_ready_time(0);
                //append process into ready queue
                ready_queue.push(i);
            }
        }


        else if (process_vector[i].get_status() == "running") {
            any_running = true;
            //If it is finished, set to terminated
            if (process_vector[i].get_CPU_time() <= 0) {
                process_vector[i].set_status("terminated");
                process_vector[i].set_finish_time(currentCycle); //set finished time
                total_processes_terminated++;
                any_running = false;
            }


                //If there is no burst left and it is not finished, then set to block
            else if (process_vector[i].get_cpu_burst_left() <= 0) {
                process_vector[i].set_status("blocked");

                //Set block time: cpu_burst * multi
                process_vector[i].set_block_time(process_vector[i].get_cpu_burst() * process_vector[i].get_multi());
                any_running = false;
            }
        }
    }
}

//Set increments and decrements for time remaining (runs processes)
void schedulerMethods::run_process(int currentCycle) {
    has_blocked = false;
    for (int i = 0; i < process_vector.size(); i++) {

        //If there is currently a running process. Then decrement that processes cpu_burst_left
        if (process_vector[i].get_status() == "running") {
            process_vector[i].set_cpu_burst_left(process_vector[i].get_cpu_burst_left() - 1);
//            cout << "cpu_burst_left: " << process_vector[i].get_cpu_burst_left() << endl;
            total_cpu_time++;
            process_vector[i].set_CPU_time(process_vector[i].get_CPU_time() - 1);
        }
        if (process_vector[i].get_status() == "blocked") {
            process_vector[i].set_block_time(process_vector[i].get_block_time() - 1);
            process_vector[i].set_total_block_time(process_vector[i].get_total_block_time() + 1);
            has_blocked = true;
        }

        if (process_vector[i].get_status() == "ready") {
            process_vector[i].set_ready_time(process_vector[i].get_ready_time() + 1);
        }
    }

    if (has_blocked) {
        total_block_time++;
    }
}

//*****************************Scheduling Algorithms*****************************



/////////////////////////////First Come First Serve/////////////////////////////
void schedulerMethods::Fcfs() {
    cout << endl << "=========================First Come First Serve Start=============================" << endl <<
    endl;
    //Loop until all processes are terminated
    while (total_processes_terminated != process_vector.size()) {
        currentCycle++;
        verbose.print(currentCycle, process_vector);

        //Update statuses
        set_process(currentCycle);

        //If no processes are running. Set next ready process to running
        if (any_running == false) {
            if (!ready_queue.empty()) {
                front_queue = ready_queue.front();
                if (process_vector[front_queue].get_status() == "ready") {
                    ready_queue.pop();

                    process_vector[front_queue].set_status("running");
                    any_running = true;
                    cpu_burst = randomOS(process_vector[front_queue].get_max_burst());
                    process_vector[front_queue].set_cpu_burst_left(cpu_burst);
                    process_vector[front_queue].set_cpu_burst(cpu_burst);
                }
            }
        }

        //decrement values for running and blocked processes
        run_process(currentCycle);
        for (int i = 0; i < process_vector.size(); i++) {
            if (process_vector[i].get_status() == "ready") {
                process_vector[i].set_total_wait_time(process_vector[i].get_total_wait_time() + 1);
                has_waited = true;
            }
        }

        if (has_waited) {
            total_wait_time = total_wait_time + 1;
            has_waited = false;
        }
    }
    cout << "The Scheduling Algorithm used was First Come First Serve" << endl << endl;
    finish_time = currentCycle;
    print_output();
    print_summary();
    //Reset for next algorithm
    reset_processes();
    cout << endl << "=========================First Come First Serve End======================" << endl << endl;
}
/////////////////////////////First Come First Serve FINISH/////////////////////////////


//////////////////////////////////////Uniprogrammed////////////////////////////////////
void schedulerMethods::Uniprogrammed() {
    cout << endl << "=========================Uniprogrammed Start=============================" << endl << endl;

    //Loop until all processes are terminated
    while (total_processes_terminated != process_vector.size()) {
        currentCycle++;
        verbose.print(currentCycle, process_vector);

        //Update statuses
        set_process(currentCycle);

        //set next ready process to running
        //GO TO THE NEXT PROCESS ONLY WHEN CURRENT ONE IS TERMINATED
        if (process_vector[currentProcess].get_status() == "terminated") {
            currentProcess++;
        }


        if (currentProcess != process_vector.size()) {
            if (process_vector[currentProcess].get_status() == "ready") {
                process_vector[currentProcess].set_status("running");
                cpu_burst = randomOS(process_vector[currentProcess].get_max_burst());
                process_vector[currentProcess].set_cpu_burst_left(cpu_burst);
                process_vector[currentProcess].set_cpu_burst(cpu_burst);
            }
        }

        //Run Processes
        run_process(currentCycle);
        for (int i = 0; i < process_vector.size(); i++) {
            if (process_vector[i].get_status() == "ready") {
                process_vector[i].set_total_wait_time(process_vector[i].get_total_wait_time() + 1);
                has_waited = true;
            }
        }

        if (has_waited) {
            total_wait_time = total_wait_time + 1;
            has_waited = false;
        }
    }
    cout << "The Scheduling Algorithm used was Uniprogrammed" << endl << endl;
    finish_time = currentCycle;
    print_output();
    print_summary();
    reset_processes();
    cout << endl << "=========================Uniprogrammed End=============================" << endl << endl;
}

////////////////////////////////Uniprogrammed Finished///////////////////////////////

////////////////////////////////Shortest Job First Start///////////////////////////////
void schedulerMethods::sjf() {
    cout << endl << "=========================Shortest Job First Start======================" << endl << endl;

    //Loop until all processes are terminated
    while (total_processes_terminated != process_vector.size()) {
        currentCycle++;
        verbose.print(currentCycle, process_vector);

        //Update statuses
        set_process(currentCycle);

        temp_vector.clear();
        min = 9999;
        num_ready = 0;
        is_found = false;

        if (any_running == false) {
            for (int i = 0; i < process_vector.size(); i++) {

                if (process_vector[i].get_status() == "ready") {
                    temp_vector.push_back(i);
                }
            }
            while ((is_found == false) && (temp_vector.size() != 0)) {
                for (int i = 0; i < temp_vector.size(); i++) {
                    if (process_vector[temp_vector[i]].get_CPU_time() <= min) {
                        min = process_vector[temp_vector[i]].get_CPU_time();
                    }
                }
                for (int i = 0; i < temp_vector.size(); i++) {
                    if (min == process_vector[temp_vector[i]].get_CPU_time()) {
                        sjf_queue.push_back(temp_vector[i]);
                        is_found = true;
                        break;
                    }
                }
            }
        }

        //If no processes are running. Set next ready process to running
        if (any_running == false) {
            if (!sjf_queue.empty()) {
                front_queue = sjf_queue.front();
                if (process_vector[front_queue].get_status() == "ready") {
                    sjf_queue.pop_front();

                    process_vector[front_queue].set_status("running");
                    any_running = true;
                    cpu_burst = randomOS(process_vector[front_queue].get_max_burst());
                    process_vector[front_queue].set_cpu_burst_left(cpu_burst);
                    process_vector[front_queue].set_cpu_burst(cpu_burst);
                }
            }
        }

        //Run Processes
        run_process(currentCycle);
        for (int i = 0; i < process_vector.size(); i++) {
            if (process_vector[i].get_status() == "ready") {
                process_vector[i].set_total_wait_time(process_vector[i].get_total_wait_time() + 1);
                has_waited = true;
            }
        }

        if (has_waited) {
            total_wait_time = total_wait_time + 1;
            has_waited = false;
        }
    }
    cout << "The Scheduling Algorithm used was Shortest Job First" << endl << endl;
    finish_time = currentCycle;
    print_output();
    print_summary();
    reset_processes();
    cout << endl << "=========================Shortest Job First End========================" << endl << endl;
}
////////////////////////////////Shortest Job First Finished///////////////////////////////


////////////////////////////////Round Robin Start//////////////////////////////////

void schedulerMethods::rr() {
    cout << endl << "=========================Round Robin Start=============================" << endl << endl;
    is_found = false;
    min_vec_index = 0;

    //Loop until all processes are terminated
    while (total_processes_terminated != process_vector.size()) {
        currentCycle++;
        verbose.print(currentCycle, process_vector);

        //Update statuses
        for (int i = 0; i < process_vector.size(); i++) {
            if (process_vector[i].get_status() == "unstarted") {
                if (process_vector[i].get_arrival_time() == currentCycle) {
                    process_vector[i].set_status("ready");
                    process_vector[i].set_ready_time(0);
                }
            }


            else if (process_vector[i].get_status() == "blocked") {
                //If block time is finished - set to ready
                if (process_vector[i].get_block_time() <= 0) {
                    process_vector[i].set_status("ready");
                    process_vector[i].set_ready_time(0);
                }
            }


            else if (process_vector[i].get_status() == "running") {
                //If it is finished, set to terminated
                if (process_vector[i].get_CPU_time() <= 0) {
                    process_vector[i].set_status("terminated");
                    process_vector[i].set_finish_time(currentCycle); //set finished time
                    total_processes_terminated++;
                    any_running = false;
                }

                    //If there is no burst left and it is not finished, then set to block
                else if (process_vector[i].get_cpu_burst_left() <= 0) {
                    process_vector[i].set_status("blocked");
                    //Set block time: cpu_burst * multi
                    process_vector[i].set_block_time(process_vector[i].get_cpu_burst() * process_vector[i].get_multi());
                    process_vector[i].set_ready_time(0);
                    any_running = false;
                }
            }
        }
        //END UPDATE STATUS


        //If process is running and it's quantum is 0; set to ready and add to end of ReadyQ
        for (int i = 0; i < process_vector.size(); i++) {
            if (process_vector[i].get_quantum() == 0) {
                if (process_vector[i].get_status() == "running") {
                    process_vector[i].set_status("ready");
                    process_vector[i].set_ready_time(0);
                    any_running = false;
                }
            }
        }

        //HANDLE TIE BREAK AS PER LAB INSTRUCTIONS

        longest_ready_time = -1;
        ready_process = -1;
        for (int i = 0; i < process_vector.size(); i++) {
            if (process_vector[i].get_status() == "ready" && process_vector[i].get_ready_time() > longest_ready_time) {
                ready_process = i;
                longest_ready_time = process_vector[i].get_ready_time();
            }
        }

        //If no processes are running. Set next ready process to running
        if (any_running == false & ready_process != -1) {
            process_vector[ready_process].set_status("running");
            any_running = true;
            process_vector[ready_process].set_quantum(2);

            if (process_vector[ready_process].get_cpu_burst_left() == 0) {
                cpu_burst = randomOS(process_vector[ready_process].get_max_burst());
                process_vector[ready_process].set_cpu_burst_left(cpu_burst);
                process_vector[ready_process].set_cpu_burst(cpu_burst);
            }
        }

        //need to decrement the running processes quantum
        for (int i = 0; i < process_vector.size(); i++) {

            //If there is currently a running process. Then decrement QUANTUM
            if (process_vector[i].get_status() == "running") {
                process_vector[i].set_quantum(process_vector[i].get_quantum() - 1);
            }
        }

        //Run Processes
        run_process(currentCycle);

        for (int i = 0; i < process_vector.size(); i++) {
            if (process_vector[i].get_status() == "ready") {
                process_vector[i].set_total_wait_time(process_vector[i].get_total_wait_time() + 1);
                has_waited = true;
            }
        }

        if (has_waited) {
            total_wait_time = total_wait_time + 1;
            has_waited = false;
        }
    }
    cout << "The Scheduling Algorithm used was Round Robin" << endl << endl;
    finish_time = currentCycle;
    print_output();
    print_summary();
    reset_processes();
    cout << endl << "=========================Round Robin End=============================" << endl << endl;
}

////////////////////////////////Round Robin Finished///////////////////////////////
Process::Process(int a, int b, int c, int m) {
    arrival_time = a;       //Arrival Time
    max_burst = b;          //randomOS(max_burst) = cpu_burst
    initial_cpu_time = c;           //Total time needed
    multi = m;              //multi * cpu_burst = IO_burst
}

//*****************************Verbose Methods*****************************

//Naive and simple implementations of my first verbose functions utilized for debugging purposes.
void verboseMethods::setVerbosity(bool userInput) {
    is_verbose = userInput;
}


bool verboseMethods::getVerbosity() {
    return is_verbose;
}

void verboseMethods::print(int currentCycle, vector<Process> process_vector) {

    if (verbose.getVerbosity()) {
        cout << "Before cycle " << setw(5) << currentCycle << ":";
        for (int i = 0; i < process_vector.size(); i++) {
            //should give state and remaining burst
            cout << "\t" << process_vector[i].get_status(); //need to replace 0 with remaining burst

            if (process_vector[i].get_status() == "unstarted") {
                cout << setw(5) << "0" << setw(5);
            }

            if (process_vector[i].get_status() == "ready") {
                cout << setw(9) << "0" << setw(5);
            }

            if (process_vector[i].get_status() == "running") {
                cout << setw(7) << process_vector[i].get_cpu_burst_left() + 1 << setw(5);
            }

            if (process_vector[i].get_status() == "blocked") {
                cout << setw(7) << process_vector[i].get_block_time() + 1 << setw(5);
            }

            if (process_vector[i].get_status() == "terminated") {
                cout << setw(4) << "0" << setw(5);
            }

        }
        cout << endl;
    }
}

void verboseMethods::log(string message) {
    if (is_verbose) {
        cout << "VERBOSE ON: " << message << endl;
    }
}

void verboseMethods::log(const char *message, int var) {
    if (is_verbose) {
        cout << "VERBOSE ON: " << message << var << endl;
    }
}

void verboseMethods::log(const char *message, string str) {
    if (is_verbose) {
        cout << "VERBOSE ON: " << message << endl;
        cout << "Current String: " << str << endl;
    }
}


//*****************************Setters*****************************
void Process::set_arrival_time(int A) {
    arrival_time = A;
}

void Process::set_max_burst(int B) {
    max_burst = B;
}

void Process::set_CPU_time(int C) {
    CPU_time = C;
}

void Process::set_multi(int M) {
    multi = M;
}

void Process::set_cycle(int cyc) {
    cycle = cyc;
}

void Process::set_cpu_burst(int burst) {
    cpu_burst = burst;
}

void Process::set_total_wait_time(int wait) {
    total_wait_time = wait;
}

void Process::set_turn_around_time(int tat) {
    turn_around_time = tat;
}

void Process::set_status(string s) {
    status = s;
}

void Process::set_block_time(int b) {
    block_time = b;
}

void Process::set_cpu_burst_left(int cpub) {
    cpu_burst_left = cpub;
}

void Process::set_finish_time(int f) {
    finish_time = f;
}

void Process::set_total_block_time(int bl) {
    total_block_time = bl;
}

void Process::set_initial_cpu_time(int cp) {
    initial_cpu_time = cp;
}

void Process::set_quantum(int q) {
    quantum = q;
}

void Process::set_ready_time(int t) {
    ready_time = t;
}

//*****************************Getters*****************************
int Process::get_arrival_time() {
    return arrival_time;
}

int Process::get_max_burst() {
    return max_burst;
}

int Process::get_CPU_time() {
    return CPU_time;
}

int Process::get_multi() {
    return multi;
}

int Process::get_cycle() {
    return cycle;
}

int Process::get_cpu_burst() {
    return cpu_burst;
}

int Process::get_total_wait_time() {
    return total_wait_time;
}

int Process::get_turn_around_time() {
    return turn_around_time;
}

string Process::get_status() {
    return status;
}

int Process::get_block_time() {
    return block_time;
}

int Process::get_cpu_burst_left() {
    return cpu_burst_left;
}

int Process::get_finish_time() {
    return finish_time;
}

int Process::get_total_block_time() {
    return total_block_time;
}

int Process::get_initial_cpu_time() {
    return initial_cpu_time;
}

int Process::get_quantum() {
    return quantum;
}

int Process::get_ready_time() {
    return ready_time;
}

//*****************************End of Setters/Getters**************