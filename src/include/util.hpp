#pragma once

#include <boost/asio.hpp>
#include <boost/process.hpp>
#include <future>
#include <string>

namespace util {

[[nodiscard("process output must be used")]]
inline std::future<std::string> getProcessResult(
    boost::asio::readable_pipe& process_pipe,
    boost::system::error_code error_code)

{
  return std::async(std::launch::async, [&] {
    std::string result;

    boost::asio::read(process_pipe, boost::asio::dynamic_buffer(result),
                      error_code);

    return result;
  });
}

}  // namespace util
