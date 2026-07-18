#include <string_view>

constexpr size_t maxNameLength = 128; // The longest a column or table can be called
inline constexpr std::string_view UNKNOWN_TOKEN_TYPE = "Unknown";
inline constexpr std::string_view EMPTY_VALUE = "Null";
inline constexpr std::string_view UNKNOWN_TYPE = "Unknown";
