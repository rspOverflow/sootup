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

#include <expected>
#include <memory>
#include <print>

extern "C" {
#include <curl/curl.h>
#include <curl/easy.h>
}

#include "net.h"
#include "net_structs.h"


namespace sootup {
namespace net {

std::expected<Net, net::NetError> AttemptToAcquireNet() {
  std::lock_guard<std::recursive_mutex> lock{Net::curl_init_mutex};

  if (!Net::curl_init_ok) {
    CURLcode global_init_res = curl_global_init(CURL_GLOBAL_DEFAULT);

    if (global_init_res != CURLE_OK) {
      std::println("sootup: net: curl failed to globally initialize. REASON: {}", curl_easy_strerror(global_init_res));
      return std::unexpected(net::NetError::kCURLFailedGlobalInit);
    }
    
    Net::curl_init_ok = true;
  }

  Net net{};

  if (net.HasValidCURLHandle()) {
    // NOTE: No need to manually increment here anymore.
    //       the Net default constructor took care of it safely.
    return std::move(net);
  }

  return std::unexpected(net::NetError::kNetFailedToGetCURLHandle);
}

size_t WriteCURLResponseCallback(void* contents, size_t size, size_t nmemb, void* userp) {
  size_t total_response_size = size * nmemb;

  // NOTE: Cast the user pointer to a std::string and append the incoming data
  std::string* str = static_cast<std::string*>(userp);
  str->append(static_cast<char*>(contents), total_response_size);

  return total_response_size;
}

}


Net::Net() {
  std::lock_guard<std::recursive_mutex> lock(curl_init_mutex);
  curl = curl_easy_init();
  if (curl) {
    curl_usage_count++;
  }
}

Net::~Net() {
  if (curl) {
    std::lock_guard<std::recursive_mutex> lock(curl_init_mutex);
    curl_easy_cleanup(curl); 

    if (curl_usage_count > 0) {
      curl_usage_count--;
    }

    if (curl_usage_count == 0) {
      curl_global_cleanup();
      curl_init_ok = false;
    }
  }
}

Net::Net(Net&& other) noexcept : curl(other.curl) {
  other.curl = nullptr;
  // NOTE: Since 'other' transfers its existing handle ownership to 'this', 
  //       the net global usage count doesn't change. However, 'other' must be left 
  //       in a valid empty state so its destructor doesn't decrement the count.
}

Net& Net::operator=(Net&& other) noexcept {
  if (this != &other) {
    // NOTE: Clean up current resource safely under the lock
    if (curl) {
      curl_easy_cleanup(curl);
      
      std::lock_guard<std::recursive_mutex> lock(curl_init_mutex);
      if (curl_usage_count > 0) {
        curl_usage_count--;
      }

      if (curl_usage_count == 0) {
        curl_global_cleanup();
        curl_init_ok = false;
      }
    }

    // NOTE: Transfer ownership from 'other'
    curl = other.curl;
    other.curl = nullptr;
  }

  return *this;
}

std::expected<std::vector<net::Release>, net::NetError> Net::FetchListOfToolchainReleases(int num_of_releases_wanted) {
  constexpr std::string_view kCarbonToolchainReleasesEndpointURL = "https://api.github.com/repos/carbon-language/carbon-lang/releases?per_page={}";
  std::string releases_endpoint_url = std::format(
    kCarbonToolchainReleasesEndpointURL, 
    num_of_releases_wanted
  );

  std::string response_str;

  // Ensure CURL handle is fresh
  curl_easy_reset(curl);

  // Set URL
  curl_easy_setopt(curl, CURLOPT_URL, releases_endpoint_url.c_str());

  // Set User-Agent
  // NOTE: Each time you pass std::string_view into a C function it MUST be something like: Net::kSootupUserAgent.data()
  //       If you set it to something like Net::kSootupUserAgent (where Net::kSootupUserAgent is a std::string_view), it is UB.
  curl_easy_setopt(curl, CURLOPT_USERAGENT, Net::kSootupUserAgent.data());

  // Set HTTP Headers
  std::unique_ptr<curl_slist, net::CurlSlistDeleter> headers{nullptr};
  headers.reset(curl_slist_append(headers.release(), "Accept: application/vnd.github+json"));
  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers.get());

  // Set Response Callback and Response String
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, net::WriteCURLResponseCallback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_str);

  // Set Redirect Behavior
  curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

  // Set Timeout Behavior
  curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10L);
  curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L);

  // Perform Request to API
  CURLcode curl_result = curl_easy_perform(curl);

  if (curl_result != CURLE_OK) {
    std::println("sootup: net: curl request failed. REASON: {}", curl_easy_strerror(curl_result));
    return std::unexpected(net::NetError::kNetFailedCURLRequest);
  }

  // Check HTTP code
  long http_code = 0;
  curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
  
  if (http_code != 200) {
    std::println("sootup: net: GitHub API returned non-200 status (HTTP {}):\n{}", http_code, response_str);
    return std::unexpected(net::NetError::kNetHTTPNotOK);
  }

  // Extract Releases from API response
  std::vector<net::Release> releases;
  auto parse_res = glz::read_json<std::vector<net::Release>>(releases, response_str);

  if (parse_res) {
    std::println("sootup: net: failed to parse GitHub releases JSON.");
    return std::unexpected(net::NetError::kNetFailedToParseJSON);
  }

  return releases;
}

}  // namespace sootup
