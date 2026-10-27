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
#include <mutex>
#include <string_view>
#include <vector>

extern "C" {
#include <curl/curl.h>
}

#include "net_fwd_decl.h"
#include "net_structs.h"


namespace sootup {
namespace net {

enum class NetError {
  kOK,
  kCURLFailedGlobalInit,
  kCURLNotGlobalInitYet,
  kNetFailedToGetCURLHandle,
  kNetFailedCURLRequest,
  kNetHTTPNotOK,
  kNetFailedToParseJSON
};


struct CurlSlistDeleter {
  void operator()(curl_slist* slist) const {
    if (slist) {
      curl_slist_free_all(slist);
    }
  }
};


std::expected<Net, net::NetError> AttemptToAcquireNet();
size_t WriteCURLResponseCallback(void* contents, size_t size, size_t nmemb, void* userp);

}

class Net {
 public:
  Net(const Net&) = delete; // NOTE: Copying CURL handles are a bad idea.
  Net& operator=(const Net&) = delete; // NOTE: Copying CURL handles are a bad idea.

  Net(Net&& other) noexcept;
  Net& operator=(Net&& other) noexcept;

  ~Net();


  friend std::expected<Net, net::NetError> net::AttemptToAcquireNet();

  std::expected<std::vector<net::Release>, net::NetError> FetchListOfToolchainReleases(int num_of_releases_wanted=5);

 private:
  static constexpr std::string_view kSootupUserAgent = "sootup-tcm";
  
  static inline bool curl_init_ok = false; // NOTE: Initialized only once
  static inline int curl_usage_count = 0; // NOTE: Initialized only once
  static inline std::recursive_mutex curl_init_mutex; // NOTE: We do recursively lock

  CURL* curl = nullptr;


  Net();

  bool HasValidCURLHandle() { return curl != nullptr; }
};

}

#endif
