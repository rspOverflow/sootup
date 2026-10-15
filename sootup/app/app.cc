/*
 * Copyright 2026 rspOverflow
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <print>

#include "app.h"


namespace sootup {

void App::Execute(const Arguments& args) {
  if (args.HasNoArguments() || args.GetCommand() == "help") {
    return EmitHelp(args);
  }
   
  auto it = GetRoutingMap().find(args.GetCommand());
  if (it != GetRoutingMap().end()) {
    return it->second.Invoke(args);
  } else {
    return std::println("sootup: Error! I don't know what '{}' means! Do 'sootup help' to get help!", args.GetCommand());
  }
}

void App::EmitHelp(const Arguments &args) {
  std::println("A tool for managing the Carbon Programming Language toolchain\n");
  std::println("Usage: sootup [command] [arguments...]\n");
  std::println("Commands:");

  const auto& command_map = GetRoutingMap();

  size_t max_command_width = 0;
  for (const auto& [command, tooltip] : command_map) {
    std::string full_command = command;

    if (!tooltip.GetParams().empty()) {
      full_command += " " + tooltip.GetParams();
    }

    max_command_width = std::max(max_command_width, full_command.size());
  }

  for (const auto& [command, tooltip] : command_map) {
    std::string full_command = command;

    if (!tooltip.GetParams().empty()) {
      full_command += " " + tooltip.GetParams();
    }

    std::println("\tsootup {: <{}}\t{}", full_command, max_command_width, tooltip.GetDescription());
  }

  std::println("\nsootup v0.0.1");
  std::println("Copyright (C) 2026 rspOverflow");
}

}
