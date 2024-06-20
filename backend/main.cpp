#include "../thirdparty/crow_all.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include "../thirdparty/json.hpp"

using json = nlohmann::json;

struct Process {
    int id;
    int priority;
    int arrivalTime;
    std::vector<int> burstTimes;
};

struct SchedulingResult {
    std::vector<int> completionTimes;
    std::vector<int> turnAroundTimes;
    std::vector<int> waitingTimes;
    std::vector<int> responseTimes;
    std::vector<std::pair<int, int>> schedule;
    int contextSwitches = 0;
    std::vector<json> timeLog;
};

// First Come First Serve (FCFS)
SchedulingResult scheduleFCFS(const std::vector<Process>& processes) {
    int n = processes.size();
    SchedulingResult result;
    result.completionTimes.resize(n);
    result.turnAroundTimes.resize(n);
    result.waitingTimes.resize(n);
    result.responseTimes.resize(n);

    int currentTime = 0;
    for (int i = 0; i < n; ++i) {
        const auto& p = processes[i];
        if (currentTime < p.arrivalTime) {
            currentTime = p.arrivalTime;
        }
        result.responseTimes[p.id] = currentTime - p.arrivalTime;
        result.completionTimes[p.id] = currentTime + p.burstTimes[0];
        result.turnAroundTimes[p.id] = result.completionTimes[p.id] - p.arrivalTime;
        result.waitingTimes[p.id] = result.turnAroundTimes[p.id] - p.burstTimes[0];
        result.schedule.push_back({p.id, p.burstTimes[0]});
        currentTime = result.completionTimes[p.id];
    }

    return result;
}

// Shortest Job First (SJF)
SchedulingResult scheduleSJF(std::vector<Process> processes) {
    int n = processes.size();
    SchedulingResult result;
    result.completionTimes.resize(n);
    result.turnAroundTimes.resize(n);
    result.waitingTimes.resize(n);
    result.responseTimes.resize(n);

    std::sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
        return a.arrivalTime < b.arrivalTime;
    });

    int currentTime = 0;
    std::vector<bool> done(n, false);
    for (int i = 0; i < n; ++i) {
        int minBurstIndex = -1;
        for (int j = 0; j < n; ++j) {
            if (!done[j] && processes[j].arrivalTime <= currentTime) {
                if (minBurstIndex == -1 || processes[j].burstTimes[0] < processes[minBurstIndex].burstTimes[0]) {
                    minBurstIndex = j;
                }
            }
        }

        if (minBurstIndex == -1) {
            currentTime++;
            i--;
            continue;
        }

        const auto& p = processes[minBurstIndex];
        result.responseTimes[p.id] = currentTime - p.arrivalTime;
        result.completionTimes[p.id] = currentTime + p.burstTimes[0];
        result.turnAroundTimes[p.id] = result.completionTimes[p.id] - p.arrivalTime;
        result.waitingTimes[p.id] = result.turnAroundTimes[p.id] - p.burstTimes[0];
        result.schedule.push_back({p.id, p.burstTimes[0]});
        currentTime = result.completionTimes[p.id];
        done[minBurstIndex] = true;
    }

    return result;
}

// Longest Job First (LJF)
SchedulingResult scheduleLJF(std::vector<Process> processes) {
    int n = processes.size();
    SchedulingResult result;
    result.completionTimes.resize(n);
    result.turnAroundTimes.resize(n);
    result.waitingTimes.resize(n);
    result.responseTimes.resize(n);

    std::sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
        return a.arrivalTime < b.arrivalTime;
    });

    int currentTime = 0;
    std::vector<bool> done(n, false);
    for (int i = 0; i < n; ++i) {
        int maxBurstIndex = -1;
        for (int j = 0; j < n; ++j) {
            if (!done[j] && processes[j].arrivalTime <= currentTime) {
                if (maxBurstIndex == -1 || processes[j].burstTimes[0] > processes[maxBurstIndex].burstTimes[0]) {
                    maxBurstIndex = j;
                }
            }
        }

        if (maxBurstIndex == -1) {
            currentTime++;
            i--;
            continue;
        }

        const auto& p = processes[maxBurstIndex];
        result.responseTimes[p.id] = currentTime - p.arrivalTime;
        result.completionTimes[p.id] = currentTime + p.burstTimes[0];
        result.turnAroundTimes[p.id] = result.completionTimes[p.id] - p.arrivalTime;
        result.waitingTimes[p.id] = result.turnAroundTimes[p.id] - p.burstTimes[0];
        result.schedule.push_back({p.id, p.burstTimes[0]});
        currentTime = result.completionTimes[p.id];
        done[maxBurstIndex] = true;
    }

    return result;
}

// Shortest Remaining Time First (SRTF)
SchedulingResult scheduleSRTF(std::vector<Process> processes) {
    int n = processes.size();
    SchedulingResult result;
    result.completionTimes.resize(n);
    result.turnAroundTimes.resize(n);
    result.waitingTimes.resize(n);
    result.responseTimes.resize(n);
    std::vector<int> remainingBurstTimes(n);

    for (int i = 0; i < n; ++i) {
        remainingBurstTimes[i] = processes[i].burstTimes[0];
    }

    int currentTime = 0;
    int completed = 0;
    std::vector<bool> started(n, false);
    while (completed != n) {
        int minBurstIndex = -1;
        for (int i = 0; i < n; ++i) {
            if (processes[i].arrivalTime <= currentTime && remainingBurstTimes[i] > 0) {
                if (minBurstIndex == -1 || remainingBurstTimes[i] < remainingBurstTimes[minBurstIndex]) {
                    minBurstIndex = i;
                }
            }
        }

        if (minBurstIndex == -1) {
            currentTime++;
            continue;
        }

        if (!started[minBurstIndex]) {
            result.responseTimes[processes[minBurstIndex].id] = currentTime - processes[minBurstIndex].arrivalTime;
            started[minBurstIndex] = true;
        }

        remainingBurstTimes[minBurstIndex]--;
        result.schedule.push_back({processes[minBurstIndex].id, 1});
        currentTime++;

        if (remainingBurstTimes[minBurstIndex] == 0) {
            completed++;
            result.completionTimes[processes[minBurstIndex].id] = currentTime;
            result.turnAroundTimes[processes[minBurstIndex].id] = result.completionTimes[processes[minBurstIndex].id] - processes[minBurstIndex].arrivalTime;
            result.waitingTimes[processes[minBurstIndex].id] = result.turnAroundTimes[processes[minBurstIndex].id] - processes[minBurstIndex].burstTimes[0];
        }
    }

    return result;
}

// Longest Remaining Time First (LRTF)
SchedulingResult scheduleLRTF(std::vector<Process> processes) {
    int n = processes.size();
    SchedulingResult result;
    result.completionTimes.resize(n);
    result.turnAroundTimes.resize(n);
    result.waitingTimes.resize(n);
    result.responseTimes.resize(n);
    std::vector<int> remainingBurstTimes(n);

    for (int i = 0; i < n; ++i) {
        remainingBurstTimes[i] = processes[i].burstTimes[0];
    }

    int currentTime = 0;
    int completed = 0;
    std::vector<bool> started(n, false);
    while (completed != n) {
        int maxBurstIndex = -1;
        for (int i = 0; i < n; ++i) {
            if (processes[i].arrivalTime <= currentTime && remainingBurstTimes[i] > 0) {
                if (maxBurstIndex == -1 || remainingBurstTimes[i] > remainingBurstTimes[maxBurstIndex]) {
                    maxBurstIndex = i;
                }
            }
        }

        if (maxBurstIndex == -1) {
            currentTime++;
            continue;
        }

        if (!started[maxBurstIndex]) {
            result.responseTimes[processes[maxBurstIndex].id] = currentTime - processes[maxBurstIndex].arrivalTime;
            started[maxBurstIndex] = true;
        }

        remainingBurstTimes[maxBurstIndex]--;
        result.schedule.push_back({processes[maxBurstIndex].id, 1});
        currentTime++;

        if (remainingBurstTimes[maxBurstIndex] == 0) {
            completed++;
            result.completionTimes[processes[maxBurstIndex].id] = currentTime;
            result.turnAroundTimes[processes[maxBurstIndex].id] = result.completionTimes[processes[maxBurstIndex].id] - processes[maxBurstIndex].arrivalTime;
            result.waitingTimes[processes[maxBurstIndex].id] = result.turnAroundTimes[processes[maxBurstIndex].id] - processes[maxBurstIndex].burstTimes[0];
        }
    }

    return result;
}

// Round Robin (RR)
SchedulingResult scheduleRR(std::vector<Process> processes, int timeQuantum, int contextSwitch) {
    int n = processes.size();
    SchedulingResult result;
    result.completionTimes.resize(n);
    result.turnAroundTimes.resize(n);
    result.waitingTimes.resize(n);
    result.responseTimes.resize(n);
    std::vector<int> remainingBurstTimes(n);

    for (int i = 0; i < n; ++i) {
        remainingBurstTimes[i] = processes[i].burstTimes[0];
    }

    int currentTime = 0;
    int completed = 0;
    std::queue<int> readyQueue;
    std::vector<bool> started(n, false);

    auto addToReadyQueue = [&](int currentTime) {
        for (int i = 0; i < n; ++i) {
            if (processes[i].arrivalTime == currentTime) {
                readyQueue.push(i);
            }
        }
    };

    addToReadyQueue(currentTime);

    while (completed != n) {
        if (readyQueue.empty()) {
            currentTime++;
            addToReadyQueue(currentTime);
            continue;
        }

        int index = readyQueue.front();
        readyQueue.pop();

        if (!started[index]) {
            result.responseTimes[processes[index].id] = currentTime - processes[index].arrivalTime;
            started[index] = true;
        }

        int executionTime = std::min(timeQuantum, remainingBurstTimes[index]);
        remainingBurstTimes[index] -= executionTime;
        result.schedule.push_back({processes[index].id, executionTime});
        currentTime += executionTime;

        if (remainingBurstTimes[index] == 0) {
            completed++;
            result.completionTimes[processes[index].id] = currentTime;
            result.turnAroundTimes[processes[index].id] = result.completionTimes[processes[index].id] - processes[index].arrivalTime;
            result.waitingTimes[processes[index].id] = result.turnAroundTimes[processes[index].id] - processes[index].burstTimes[0];
        } else {
            readyQueue.push(index);
        }

        addToReadyQueue(currentTime);

        if (!readyQueue.empty()) {
            result.schedule.push_back({-2, contextSwitch});
            currentTime += contextSwitch;
            result.contextSwitches++;
        }
    }

    return result;
}

// Priority (Non-Preemptive)
SchedulingResult schedulePriorityNP(std::vector<Process> processes) {
    int n = processes.size();
    SchedulingResult result;
    result.completionTimes.resize(n);
    result.turnAroundTimes.resize(n);
    result.waitingTimes.resize(n);
    result.responseTimes.resize(n);

    std::sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
        return a.arrivalTime < b.arrivalTime;
    });

    int currentTime = 0;
    std::vector<bool> done(n, false);
    for (int i = 0; i < n; ++i) {
        int highestPriorityIndex = -1;
        for (int j = 0; j < n; ++j) {
            if (!done[j] && processes[j].arrivalTime <= currentTime) {
                if (highestPriorityIndex == -1 || processes[j].priority < processes[highestPriorityIndex].priority) {
                    highestPriorityIndex = j;
                }
            }
        }

        if (highestPriorityIndex == -1) {
            currentTime++;
            i--;
            continue;
        }

        const auto& p = processes[highestPriorityIndex];
        result.responseTimes[p.id] = currentTime - p.arrivalTime;
        result.completionTimes[p.id] = currentTime + p.burstTimes[0];
        result.turnAroundTimes[p.id] = result.completionTimes[p.id] - p.arrivalTime;
        result.waitingTimes[p.id] = result.turnAroundTimes[p.id] - p.burstTimes[0];
        result.schedule.push_back({p.id, p.burstTimes[0]});
        currentTime = result.completionTimes[p.id];
        done[highestPriorityIndex] = true;
    }

    return result;
}

// Priority (Preemptive)
SchedulingResult schedulePriorityP(std::vector<Process> processes) {
    int n = processes.size();
    SchedulingResult result;
    result.completionTimes.resize(n);
    result.turnAroundTimes.resize(n);
    result.waitingTimes.resize(n);
    result.responseTimes.resize(n);
    std::vector<int> remainingBurstTimes(n);

    for (int i = 0; i < n; ++i) {
        remainingBurstTimes[i] = processes[i].burstTimes[0];
    }

    int currentTime = 0;
    int completed = 0;
    std::vector<bool> started(n, false);
    while (completed != n) {
        int highestPriorityIndex = -1;
        for (int i = 0; i < n; ++i) {
            if (processes[i].arrivalTime <= currentTime && remainingBurstTimes[i] > 0) {
                if (highestPriorityIndex == -1 || processes[i].priority < processes[highestPriorityIndex].priority) {
                    highestPriorityIndex = i;
                }
            }
        }

        if (highestPriorityIndex == -1) {
            currentTime++;
            continue;
        }

        if (!started[highestPriorityIndex]) {
            result.responseTimes[processes[highestPriorityIndex].id] = currentTime - processes[highestPriorityIndex].arrivalTime;
            started[highestPriorityIndex] = true;
        }

        remainingBurstTimes[highestPriorityIndex]--;
        result.schedule.push_back({processes[highestPriorityIndex].id, 1});
        currentTime++;

        if (remainingBurstTimes[highestPriorityIndex] == 0) {
            completed++;
            result.completionTimes[processes[highestPriorityIndex].id] = currentTime;
            result.turnAroundTimes[processes[highestPriorityIndex].id] = result.completionTimes[processes[highestPriorityIndex].id] - processes[highestPriorityIndex].arrivalTime;
            result.waitingTimes[processes[highestPriorityIndex].id] = result.turnAroundTimes[processes[highestPriorityIndex].id] - processes[highestPriorityIndex].burstTimes[0];
        }
    }

    return result;
}

// Highest Response Ratio Next (HRRN)
SchedulingResult scheduleHRRN(std::vector<Process> processes) {
    int n = processes.size();
    SchedulingResult result;
    result.completionTimes.resize(n);
    result.turnAroundTimes.resize(n);
    result.waitingTimes.resize(n);
    result.responseTimes.resize(n);

    std::sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
        return a.arrivalTime < b.arrivalTime;
    });

    int currentTime = 0;
    std::vector<bool> done(n, false);
    for (int i = 0; i < n; ++i) {
        int highestRRIndex = -1;
        double highestRR = -1;
        for (int j = 0; j < n; ++j) {
            if (!done[j] && processes[j].arrivalTime <= currentTime) {
                double responseRatio = (double)(currentTime - processes[j].arrivalTime + processes[j].burstTimes[0]) / processes[j].burstTimes[0];
                if (responseRatio > highestRR) {
                    highestRR = responseRatio;
                    highestRRIndex = j;
                }
            }
        }

        if (highestRRIndex == -1) {
            currentTime++;
            i--;
            continue;
        }

        const auto& p = processes[highestRRIndex];
        result.responseTimes[p.id] = currentTime - p.arrivalTime;
        result.completionTimes[p.id] = currentTime + p.burstTimes[0];
        result.turnAroundTimes[p.id] = result.completionTimes[p.id] - p.arrivalTime;
        result.waitingTimes[p.id] = result.turnAroundTimes[p.id] - p.burstTimes[0];
        result.schedule.push_back({p.id, p.burstTimes[0]});
        currentTime = result.completionTimes[p.id];
        done[highestRRIndex] = true;
    }

    return result;
}


std::pair<std::string, SchedulingResult> findBestAlgorithm(const std::vector<Process>& processes, int timeQuantum = 1, int contextSwitch = 1) {
    std::vector<std::pair<std::string, SchedulingResult>> results = {
        {"FCFS", scheduleFCFS(processes)},
        {"SJF", scheduleSJF(processes)},
        {"SJF(Preemptive)", scheduleSRTF(processes)},
        {"LJF", scheduleLJF(processes)},
        {"LJF(Preemptive)", scheduleLRTF(processes)},
        {"Priority", schedulePriorityNP(processes)},
        {"Priority(Preemptive)", schedulePriorityP(processes)},
        {"Round Robin", scheduleRR(processes, timeQuantum, contextSwitch)},
        {"Proposed Algorithm", scheduleHRRN(processes)}
    };

    auto rankAlgorithms = [](const std::vector<double>& values, bool ascending = true) {
        std::vector<int> rank(values.size());
        std::iota(rank.begin(), rank.end(), 0);
        std::sort(rank.begin(), rank.end(), [&](int a, int b) {
            return ascending ? values[a] < values[b] : values[a] > values[b];
        });
        return rank;
    };

    std::vector<double> avgWaitingTimes, avgTurnAroundTimes, avgResponseTimes, cpuUtils, throughputs;
    int minArrivalTime = INT_MAX;

    for (const auto& p : processes) {
        minArrivalTime = std::min(minArrivalTime, p.arrivalTime);
    }

    for (const auto& result : results) {
        const auto& res = result.second;
        double avgWaitingTime = std::accumulate(res.waitingTimes.begin(), res.waitingTimes.end(), 0.0) / res.waitingTimes.size();
        double avgTurnAroundTime = std::accumulate(res.turnAroundTimes.begin(), res.turnAroundTimes.end(), 0.0) / res.turnAroundTimes.size();
        double avgResponseTime = std::accumulate(res.responseTimes.begin(), res.responseTimes.end(), 0.0) / res.responseTimes.size();
        int totalBurstTime = std::accumulate(res.turnAroundTimes.begin(), res.turnAroundTimes.end(), 0);
        int completionTime = *std::max_element(res.completionTimes.begin(), res.completionTimes.end());
        double cpuUtil = static_cast<double>(totalBurstTime) / (completionTime + res.contextSwitches);
        double throughput = static_cast<double>(processes.size()) / (completionTime + res.contextSwitches - minArrivalTime);

        avgWaitingTimes.push_back(avgWaitingTime);
        avgTurnAroundTimes.push_back(avgTurnAroundTime);
        avgResponseTimes.push_back(avgResponseTime);
        cpuUtils.push_back(cpuUtil);
        throughputs.push_back(throughput);
    }

    auto wtRank = rankAlgorithms(avgWaitingTimes);
    auto tatRank = rankAlgorithms(avgTurnAroundTimes);
    auto rtRank = rankAlgorithms(avgResponseTimes);
    auto cpuUtilRank = rankAlgorithms(cpuUtils, false);
    auto throughputRank = rankAlgorithms(throughputs, false);

    std::vector<double> overallRanks(results.size(), 0);
    for (size_t i = 0; i < results.size(); ++i) {
        overallRanks[i] = (wtRank[i] + tatRank[i] + rtRank[i] + cpuUtilRank[i] + throughputRank[i]) / 5.0;
    }

    auto bestIdx = std::distance(overallRanks.begin(), std::min_element(overallRanks.begin(), overallRanks.end()));

    return results[bestIdx];
}


int main() {
    crow::SimpleApp app;

    CROW_ROUTE(app, "/schedule").methods(crow::HTTPMethod::POST)
    ([](const crow::request& req) {
        auto body = json::parse(req.body);
        std::string algorithm = body["algorithm"];
        auto processesJson = body["processId"];
        std::vector<Process> processes;

        for (size_t i = 0; i < processesJson.size(); ++i) {
            Process p;
            p.id = i;
            p.priority = body["priority"][i];
            p.arrivalTime = body["arrivalTime"][i];
            for (const auto& burstTime : body["processTime"][i]) {
                p.burstTimes.push_back(burstTime);
            }
            processes.push_back(p);
        }

        SchedulingResult result;
        if (algorithm == "fcfs") {
            result = scheduleFCFS(processes);
        } else if (algorithm == "sjf") {
            result = scheduleSJF(processes);
        } else if (algorithm == "ljf") {
            result = scheduleLJF(processes);
        } else if (algorithm == "srtf") {
            result = scheduleSRTF(processes);
        } else if (algorithm == "lrtf") {
            result = scheduleLRTF(processes);
        } else if (algorithm == "rr") {
            result = scheduleRR(processes, body["timeQuantum"], body["contextSwitch"]);
        } else if (algorithm == "pnp") {
            result = schedulePriorityNP(processes);
        } else if (algorithm == "pp") {
            result = schedulePriorityP(processes);
        } else if (algorithm == "hrrn") {
            result = scheduleHRRN(processes);
        }

        json response = {
            {"completionTimes", result.completionTimes},
            {"turnAroundTimes", result.turnAroundTimes},
            {"waitingTimes", result.waitingTimes},
            {"responseTimes", result.responseTimes},
            {"schedule", result.schedule},
            {"contextSwitches", result.contextSwitches},
            {"timeLog", result.timeLog}
        };

        return crow::response(response.dump());
    });
    
    CROW_ROUTE(app, "/best-algorithm").methods(crow::HTTPMethod::POST)
    ([](const crow::request& req) {
        auto body = json::parse(req.body);
        auto processesJson = body["processId"];
        std::vector<Process> processes;

        for (size_t i = 0; i < processesJson.size(); ++i) {
            Process p;
            p.id = i;
            p.priority = body["priority"][i];
            p.arrivalTime = body["arrivalTime"][i];
            for (const auto& burstTime : body["processTime"][i]) {
                p.burstTimes.push_back(burstTime);
            }
            processes.push_back(p);
        }

        int timeQuantum = body.contains("timeQuantum") ? body["timeQuantum"] : 1;
        int contextSwitch = body.contains("contextSwitch") ? body["contextSwitch"] : 1;
        
        auto bestAlgResult = findBestAlgorithm(processes, timeQuantum, contextSwitch);

        json response = {
            {"bestAlgorithm", bestAlgResult.first},
            {"cpuUtilization", bestAlgResult.second.cpuUtilization},
            {"throughput", bestAlgResult.second.throughput},
            {"turnAroundTime", bestAlgResult.second.turnAroundTimes},
            {"waitingTime", bestAlgResult.second.waitingTimes},
            {"responseTime", bestAlgResult.second.responseTimes}
        };

        return crow::response(response.dump());
    });


    app.port(18080).multithreaded().run();
}
