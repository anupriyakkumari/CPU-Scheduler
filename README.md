# CPU Scheduler

## Project Description

This CPU Scheduler project is a comprehensive web application designed to evaluate and compare various CPU scheduling algorithms. The primary goal is to determine the most optimal algorithm for a given set of processes based on key performance metrics. The project includes a backend written in C++ and a frontend utilizing HTML, JavaScript, and CSS, with visualizations powered by Google Charts and Chart.js. The project provides an interactive interface to visualize CPU scheduling through Gantt and Timeline charts, compare different algorithms, and simulate the scheduling process with animations. We display various metrics and visualizations and finally suggest the best Scheduling algorithm based on an efficient ranking algorithm.


Please refer to the comprehensive documentation **"CPU_Scheduler_Documentation.pdf"** for complete details on the ideation and implementation of the project. It also includes snippets of the front end in action.

## How to Run the Project

### Dependencies

- **Backend**:
   - C++ Compiler (Supporting C++14 standard)
   - CMake (Version 3.10 or higher)
   - Crow (A C++ micro web framework)
   - nlohmann/json (JSON for Modern C++)
   - Asio (Networking library)
  
- **Frontend**:
  - HTML: Structure of the web pages.
  - CSS: Styling of the web pages.
  - Vanilla JS: Client-side scripting.
  - Google Charts: Visualization of Gantt and Timeline charts.
  - Chart.js: Additional charting for comparative analysis.


### Installation and Setup

1. **Clone the Repository:**
   ```bash
   git clone https://github.com/anupriyakkumari/cpu_scheduler.git
   cd cpu_scheduler
   
2. **Install dependencies:**
   - Install Crow: Follow instructions at Crow Documentation
   - Install nlohmann/json: Follow instructions at nlohmann/json
   - Install Asio: brew install asio (if using Homebrew)
  
     
After installing Crow, follow the instruction in their official documentation to expand the 'crow_all.h' file.
Locate the json.hpp in the nlohmann/json repository. Include these two files in a subdirectory 'thirdparty' alongside the 'backend' and 'frontend' subdirectories.
3. **Build the project:**
   ```bash
   mkdir build
   cd build
   cmake ..
   make
4. **Run the project:**
   ``` bash
   ./cpu_scheduling

## Functionalities
- Implementation of 9 CPU scheduling algorithms.
- Support for multiple CPU and I/O burst times for each process.
- Visualization through Gantt and Timeline charts.
- Context switching time consideration.
- Time log animation for process scheduling.
- Round Robin algorithm comparison for all time quantums.
- Comparative analysis of all algorithms based on Average Completion Time, Turn Around Time, Waiting Time, and Response Time.

## Scheduling Algorithms
- **First Come First Serve (FCFS)**: Non-Preemptive
  - Processes are executed in the order they arrive in the ready queue.
  
- **Shortest Job First (SJF)**: Non-Preemptive
  - The shortest job in the ready queue is executed first.
  
- **Shortest Remaining Job First (SRJF)**: Preemptive
  - The job with the shortest remaining time is executed first.
  
- **Longest Job First (LJF)**: Non-Preemptive
  - The longest job in the ready queue is executed first.
  
- **Longest Remaining Job First (LRJF)**: Preemptive
  - The job with the longest remaining time is executed first.
  
- **Priority Non-Preemptive (PNP)**: Non-Preemptive
  - The highest priority job in the ready queue is executed first.
  
- **Priority Preemptive (PP)**: Preemptive
  - The highest priority job preempts the running process.
  
- **Round Robin (RR)**: Preemptive
  - Processes are given a fixed time quantum and cycled through.
  
- **Highest Response Ratio Next (HRRN)**: Non-Preemptive
  - The job with the highest response ratio is executed first.

## CPU Scheduling States
- **Remain**: Processes that are yet to arrive.
- **Ready**: Processes that are ready for execution.
- **Running**: The current process running on the CPU.
- **Block**: Processes blocked for I/O time.
- **Terminate**: Processes that have completed all CPU and I/O operations.

### The best scheduling algorithm:

This algorithm determines the best scheduling algorithm based on the following steps:

1. **Run All Scheduling Algorithms:** It executes all implemented scheduling algorithms and stores their results.
2. **Rank Algorithms:** It ranks the algorithms based on key performance metrics: average waiting time, turnaround time, response time, completion time, and cpu utilisation and throughput.
3. **Calculate Averages:** It calculates the average of each performance metric for each algorithm.
4. **Determine Overall Rank:** It averages the ranks across all metrics for each algorithm.
5. **Select the Best Algorithm:** The algorithm with the lowest overall rank is identified as the best algorithm.

The method ensures a balanced evaluation by giving equal weight to each metric.


## Usage
- Open the web application in a browser.
- Input processes with their respective CPU and I/O burst times.
- Select a scheduling algorithm to visualize its execution.
- Compare different algorithms based on various performance metrics.
- Observe animations to understand the scheduling process better.
- See the best algorithm for the given set of tasks and information.

### Resources and References

The following resources were instrumental in the development of this project:

#### GeeksforGeeks
- [CPU Scheduling in Operating Systems](https://www.geeksforgeeks.org/cpu-scheduling-in-operating-systems/)

#### Scaler Topics
- [Operating System - CPU Scheduling](https://www.scaler.com/topics/operating-system/cpu-scheduling/)

#### Documentation and Tutorials
- [Crow Documentation](https://crowcpp.org/master/getting_started/setup/linux/)
- [nlohmann/json Integration](https://json.nlohmann.me/integration/)

#### Books
- *Linux Kernel Development, 3rd Edition* by Robert Love

#### Research Papers
- [An Improved Round Robin Scheduling Algorithm for CPU Scheduling on ResearchGate](https://www.researchgate.net/publication/49619229_An_Improved_Round_Robin_Schedduling_Algorithm_for_CPU_Scheduling)

#### GitHub Repositories
- [Ravipatel1309/CPUScheduler](https://github.com/Ravipatel1309/CPUScheduler)
- [CrowCpp/Crow](https://github.com/CrowCpp/Crow)
- [nlohmann/json](https://github.com/nlohmann/json)
- [Various CPU Scheduling Algorithm Implementations](https://github.com/search?q=CPU+Scheduling+Algorithms)

