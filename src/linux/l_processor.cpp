/* System Monitor
 * Copyright (C) 2020  Sandro Wißmann
 *
 * System Monitor is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * System Monitor is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with System Monitor If not, see <http://www.gnu.org/licenses/>.
 *
 * Web-Site: https://github.com/SandroWissmann/System-Monitor
 */
#include "../../include/linux/l_processor.h"

#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <thread>

#include "../../include/linux/cpu_states.h"

namespace Sysmon::Linux {

const std::string kProcDirectory{"/proc/"};
const std::string kStatFilename{"/stat"};

Processor::Processor(const CpuStates& firstCpuState,
                     const CpuStates& secondCpuState,
                     const std::vector<CpuStates>& firstCoreStates,
                     const std::vector<CpuStates>& secondCoreStates)
    : mFirstCpuState{firstCpuState},
      mSecondCpuState{secondCpuState},
      mFirstCoreStates{firstCoreStates},
      mSecondCoreStates{secondCoreStates} {
    assert(firstCoreStates.size() == secondCoreStates.size());
}

double Processor::Utilization() const {
    return calcUtilization(mFirstCpuState, mSecondCpuState);
}

std::vector<double> Processor::CoreUtilizations() const {
    std::vector<double> coreUtilizations;
    coreUtilizations.reserve(mFirstCoreStates.size());

    for (std::size_t i = 0; i < mFirstCoreStates.size(); ++i) {
        coreUtilizations.emplace_back(
            calcUtilization(mFirstCoreStates[i], mSecondCoreStates[i]));
    }
    return coreUtilizations;
}

int Processor::CountOfCores() const { return mFirstCoreStates.size(); }

double Processor::calcUtilization(const CpuStates& firstState,
                                  const CpuStates& secondState) const {
    double deltaTotalTime = secondState.Jiffies() - firstState.Jiffies();
    double deltaActiveTime =
        secondState.ActiveJiffies() - firstState.ActiveJiffies();
    return deltaActiveTime / deltaTotalTime;
}

Processor Processor::createFromFile() {
    return createFromFile(kProcDirectory + kStatFilename);
}

Processor Processor::createFromFile(const std::string& filename) {
    auto firstStates = readFromFile(filename);
    if (!firstStates) {
        return Processor{};
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    auto secondStates = readFromFile(filename);
    if (!secondStates) {
        return Processor{};
    }

    auto firstCpuState = firstStates->front();
    auto firstCoreStates = std::vector<CpuStates>{
        firstStates.value().begin() + 1, firstStates.value().end()};

    auto secondCpuState = secondStates->front();
    auto secondCoreStates = std::vector<CpuStates>{
        secondStates.value().begin() + 1, secondStates.value().end()};

    return Processor{firstCpuState, secondCpuState, firstCoreStates,
                     secondCoreStates};
}

std::optional<std::vector<CpuStates>> Processor::readFromFile(
    const std::string& filename) {
    std::ifstream ifs{filename};
    if (!ifs.is_open()) {
        return {};
    }

    std::string line;
    // Assume there must be also the CPUState
    std::getline(ifs, line);
    auto optCpuState = parseCpuState(line);
    if (!optCpuState) {
        return {};
    }
    // Assume there must be atleast one CoreState
    std::getline(ifs, line);
    auto optCoreState = parseCpuState(line);
    if (!optCoreState) {
        return {};
    }
    // optional cores
    std::vector<CpuStates> states;
    states.emplace_back(*optCpuState);
    states.emplace_back(*optCoreState);

    while (std::getline(ifs, line)) {
        auto optNextCoreState = parseCpuState(line);
        if (!optNextCoreState) {
            break;
        }
        states.emplace_back(*optNextCoreState);
    }
    return {states};
}

std::optional<CpuStates> Processor::parseCpuState(const std::string line) {
    CpuStates cpuStates;
    std::istringstream ist{line};
    ist >> cpuStates;
    if (ist.fail()) {
        return {};
    }
    return {cpuStates};
}

}  // namespace Sysmon::Linux
