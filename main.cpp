/*-----------------------------------------------------------------------------
 * Class: main.cpp
 * Author: Ricardo N. Guntur <ricardo.guntur@nyu.edu>
 * Course: Operating Systems -- Professor Allan Gottlieb
 *
 * Program: Short-Term CPU Scheduling
 * Description:
 * Simulation of CPU scheduling in order to observed how the time required
 * depends on the scheduling algorithm and the request patterns. A process is
 * characterized by four numbers A, B, C, and M.
 * A = arrival time
 * B = Used to calculate Max burst
 * C = Total CPU time needed
 * M = Max I/O Block time
 *
 * Example Input: 1 (A B C M)
 * With this data we simulate FCFS, Round Robin with Quantum 2,
 * Uniprogrammed and Shortest Job First.
 *
 * While simulating we collect data on Finishing Time, Turnaround Time, I/O Time
 * and Waiting time.
 *
 * ..More details in the Lab instructions provided in the repository
 -----------------------------------------------------------------------------*/
#include "verboseMethods.h"
#include "schedulerMethods.h"
#include "Process.h"

//Constant Variables
const string VERBOSE = "--verbose";
string input_processes;
bool verboseMethods::is_verbose = false;

int main(int argc, char *argv[]) {

/*-----------------------------------------------------------------------------
 * There are 2 possible invocations
 * <program-name> <input-filename>
 * <program-name> --verbose <input-filename>
 -----------------------------------------------------------------------------*/

    schedulerMethods scheduler;
    verboseMethods verbose;


    cout << "///////////////////SCHEDULER START///////////////////" << endl;

    //If there are a total of 2 command line arguments then one of them MUST be the verbose flag
    if (argc == 3) {

        //Check if the first argument was the verbose invocation
        if (argv[1] == VERBOSE) {
            verbose.setVerbosity(true);
            cout << "VERBOSE MODE ON" << endl;
            input_processes = argv[2];

            //Creates Process objects with input. Stores those objects in vectors and sorts them.
            scheduler.process_input(input_processes);
            scheduler.reset_processes();
            scheduler.Fcfs();
            scheduler.Uniprogrammed();
            scheduler.sjf();
            scheduler.rr();
        }

        else {
            cout << "There are 2 possible invocations: \n"
                            "<program-name> <input-filename> or\n"
                            "<program-name> --verbose <input-filename>";

        }
    }

    //ELSE if there was only 1 argument then there was no verbose flag
    else if (argc == 2){
        cout << "Input File: " << argv[1] << endl;

        verbose.setVerbosity(false);
        input_processes = argv[1];

        //Creates Process objects with input. Stores those objects in vectors and sorts them.
        scheduler.process_input(input_processes);
        scheduler.reset_processes();
        scheduler.Fcfs();
        scheduler.Uniprogrammed();
        scheduler.sjf();
        scheduler.rr();
    }

    return 0;
}