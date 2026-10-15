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

#include "net.h"

extern "C" {

#include <curl/curl.h>
#include <curl/easy.h>

}


namespace sootup {

Net::Net() {
  curl = curl_easy_init();
}

Net::~Net() {
  curl_easy_cleanup(curl);

  if (curl_usage_count != 0) {
    curl_usage_count--;
  }

  if (curl_usage_count <= 0) {
    curl_global_cleanup();
  }
}


std::expected<Net, NetError> Net::AttemptToAcquireNet() {
  if (!curl_init_ok) {
    CURLcode global_init_res = curl_global_init(CURL_GLOBAL_DEFAULT);

    if (global_init_res != CURLE_OK) {
      return std::unexpected(NetError::kCURLFailedGlobalInit);
    }
    
    curl_init_ok = true;
  }

  curl_usage_count++;
  return Net{};
}




}
