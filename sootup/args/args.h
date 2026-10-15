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

#ifndef SOOTUP_ARGS_ARGS_H_
#define SOOTUP_ARGS_ARGS_H_

#include <string>
#include <span>
#include <vector>


namespace sootup {

class Arguments {
 public:
  Arguments(int argc, char* argv[]);

  std::string GetCommand() const { return command; }
  std::span<const std::string> GetMiscellaneousArguments() const { return misc; }

  bool HasNoArguments() const { return command.empty() && misc.empty(); }

 private:
  std::string command;
  std::vector<std::string> misc;
};

}  // namespace sootup

#endif  // SOOTUP_ARGS_ARGS_H_
