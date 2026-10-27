#ifndef SOOTUP_NET_NET_STRUCTS_H_
#define SOOTUP_NET_NET_STRUCTS_H_

#include <optional>
#include <string>
#include <vector>

#include "glaze/glaze.hpp"  // NOTE: Ignore clangd here if it warns about an unused include.


namespace sootup::net {

// 1. Simple Github User (Author / Uploader)
struct SimpleGithubUser {   
  std::optional<std::string> name{};
  std::optional<std::string> email{};
  std::string login{};
  int64_t id{};
  std::string node_id{};
  std::string avatar_url{};
  std::optional<std::string> gravatar_id{};
  std::string url{};
  std::string html_url{};
  std::string followers_url{};
  std::string following_url{};
  std::string gists_url{};
  std::string starred_url{};
  std::string subscriptions_url{};
  std::string organizations_url{};
  std::string repos_url{};
  std::string events_url{};
  std::string received_events_url{};
  std::string type{};
  bool site_admin{};
  std::optional<std::string> starred_at{};
  std::optional<std::string> user_view_type{};
};

// 2. Release Asset
struct ReleaseAsset {
  std::string url{};
  std::string browser_download_url{};
  int64_t id{};
  std::string node_id{};
  std::string name{};
  std::optional<std::string> label{};
  std::string state{}; // Enum: "uploaded", "open"
  std::string content_type{};
  int64_t size{};
  std::optional<std::string> digest{};
  int64_t download_count{};
  std::string created_at{};
  std::string updated_at{};
  std::optional<SimpleGithubUser> uploader{}; // Handles anyOf null / SimpleGithubUser
};

// 3. Reaction Rollup
struct ReactionRollup {
  std::string url{};
  int64_t total_count{};
  int64_t plus_one{};   // Mapped from "+1"
  int64_t minus_one{};  // Mapped from "-1"
  int64_t laugh{};
  int64_t confused{};
  int64_t heart{};
  int64_t hooray{};
  int64_t eyes{};
  int64_t rocket{};
};

// 4. Main Release struct
struct Release {
  std::string url{};
  std::string html_url{};
  std::string assets_url{};
  std::string upload_url{};
  std::optional<std::string> tarball_url{};
  std::optional<std::string> zipball_url{};
  int64_t id{};
  std::string node_id{};
  std::string tag_name{};
  std::string target_commitish{};
  std::optional<std::string> name{};
  std::optional<std::string> body{};
  bool draft{};
  bool prerelease{};
  bool immutable{};
  std::string created_at{};
  std::optional<std::string> published_at{};
  std::optional<std::string> updated_at{};
  SimpleGithubUser author{};
  std::vector<ReleaseAsset> assets{};
  std::string body_html{};
  std::string body_text{};
  int64_t mentions_count{};
  std::string discussion_url{};
  ReactionRollup reactions{};
};

}  // namespace sootup::net

template <>
struct glz::meta<sootup::net::ReactionRollup> {
  using T = sootup::net::ReactionRollup;
  static constexpr auto value = object(
    "url", &T::url,
    "total_count", &T::total_count,
    "+1", &T::plus_one,
    "-1", &T::minus_one,
    "laugh", &T::laugh,
    "confused", &T::confused,
    "heart", &T::heart,
    "hooray", &T::hooray,
    "eyes", &T::eyes,
    "rocket", &T::rocket
  );
};

#endif
