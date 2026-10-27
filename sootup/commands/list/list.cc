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

#include "sootup/net/net.h"
#include "sootup/net/net_structs.h"

#include "list.h"


namespace sootup::commands {

void List(const Arguments& args) {
  auto net_res = net::AttemptToAcquireNet();
  if (!net_res.has_value()) {
    return std::println("sootup: list: failed to acquire network.");
  }

  Net net{std::move(net_res.value())};
  
  auto releases_res = net.FetchListOfToolchainReleases();
  if (!releases_res.has_value()) {
    return std::println("sootup: list: failed to acquire list of toolchain releases.");
  }

  std::println("\nLatest Carbon Language toolchain versions available:");
  std::vector<net::Release> releases = releases_res.value();
  for (auto rel : releases) {
    std::println("{}", rel.tag_name);
  }
  std::println("");
}

}
