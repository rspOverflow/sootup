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

#ifndef SOOTUP_NET_NET_H_
#define SOOTUP_NET_NET_H_

#include <expected>
#include <filesystem>
#include <mutex>
#include <string>

extern "C" {
#include <curl/curl.h>
}


namespace sootup {

enum class NetError {
  kOK,
  kCURLFailedGlobalInit,
  kCURLNotGlobalInitYet
};

class Net {
 public:
  ~Net();

  static std::expected<Net, NetError> AttemptToAcquireNet();
  
  NetError DownloadFileToDirectory(const std::string& url_of_file, std::filesystem::path directory);

 private:
  inline static bool curl_init_ok = false; // NOTE: Initialized only once
  inline static int curl_usage_count = 0; // NOTE: Initialized only once
  inline static std::mutex curl_init_mutex;

  CURL* curl = nullptr;

  Net();
};

}

#endif
