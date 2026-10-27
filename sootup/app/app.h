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

#ifndef SOOTUP_CORE_APP_H_
#define SOOTUP_CORE_APP_H_

#include <string>
#include <unordered_map>

#include "sootup/commands/command.h"
#include "sootup/commands/install/install.h"
#include "sootup/commands/uninstall/uninstall.h"
#include "sootup/commands/list/list.h"

#include "sootup/args/args.h"


namespace sootup {

class App {
 public:
  using RoutingMap = std::unordered_map<std::string, Command>;

  static const auto& GetRoutingMap() {
    // NOTE: We do leak memory here but this is purely to avoid the Static Destruction Order Fiasco.
    static const auto* routing_map = new RoutingMap{
      {"help", Command{EmitHelp, "", "Displays helpful information"}},
      {"install", Command{commands::Install, "[version]", "Installs Carbon toolchain w/ optional version, uninstalls all older versions. By default, installs 'latest'."}},
      {"uninstall", Command{commands::Uninstall, "", "Uninstalls all Carbon toolchains."}},
      {"list", Command(commands::List, "Lists the Top 5 latest versions of the Carbon toolchain available.")}
    };
    
    return *routing_map;
  }

  void Execute(const Arguments& args);

 private:
  static void EmitHelp(const Arguments& args);
};

}  // namespace sootup

#endif  // SOOTUP_CORE_APP_H_
