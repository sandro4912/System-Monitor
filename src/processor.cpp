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
#include "../include/processor.h"

#include "include/linux/l_processor.h"

namespace Sysmon {

std::shared_ptr<Processor> Processor::makeProcessor() {
#ifdef __linux__
    Linux::Processor processor = Linux::Processor::createFromFile();
    return std::move(std::make_unique<Linux::Processor>(processor));
#elif _WIN32
#endif
}

}  // namespace Sysmon
