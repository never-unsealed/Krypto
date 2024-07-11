#include <asio.hpp>

void alice(std::string host, std::string port);
bool bob(asio::ip::tcp::iostream &stream);
void serverBob(std::string port);
