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

#ifndef SOOTUP_COMMANDS_COMMAND_H_
#define SOOTUP_COMMANDS_COMMAND_H_

#include <functional>

#include "sootup/args/args.h"


namespace sootup {

class Command {
 public:
  Command(std::function<void(const Arguments&)> callback,
      std::string params = "",
      std::string description = "No description provided.") 
      : callback(callback),
        params(std::move(params)),
        description(std::move(description)) {}
  
  void Invoke(const Arguments& args) const { return callback(args); };
  const std::string& GetParams() const { return params; }
  const std::string& GetDescription() const { return description; }
 private:
  std::function<void(const Arguments&)> callback;
  std::string params;
  std::string description;
};

}  // namespace sootup

#endif  // SOOTUP_APP_COMMANDS_COMMAND_H_
