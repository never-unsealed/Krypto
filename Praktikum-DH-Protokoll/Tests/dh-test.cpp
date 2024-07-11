#include <gtest/gtest.h>
#include "gmock/gmock.h"
#include "MessageEncoder.h"
#include "DHProtocol.h"
#include "netintro.h"
#include <thread>

using asio::ip::tcp;
using namespace std;

// Make sure to end each line sent with a endl or the
// program will stall.

void netintro_mim_alice() {
    // Setup man in the middle
    asio::io_context io_context_mim;
    tcp::acceptor acceptor_mim(
        io_context_mim, 
        tcp::endpoint(tcp::v4(), 9999)
    );
    tcp::iostream stream_mim;
    if (!stream_mim) FAIL();
    acceptor_mim.accept(*stream_mim.rdbuf());

    // Check message from Alice
    string mim_msg;
    getline(stream_mim, mim_msg);
    EXPECT_THAT(mim_msg, "Hello Server, I am Alice!");

    // Send message to alice
    mim_msg = "Hello from Bob.";
    stream_mim << mim_msg << endl;

    // Check message from Alice
    getline(stream_mim, mim_msg);
    EXPECT_THAT(mim_msg, "27F8#22E6");

    // Send message to alice
    mim_msg = "4ADE";
    stream_mim << mim_msg << endl;
}

void netintro_mim_bob() {
    // Connect to Bob
    tcp::iostream stream(tcp::resolver::query{"localhost", "9999"});

    // Send message to Bob
    string msg = "Hello Server, I am Alice!";
    stream << msg << endl;

    // Check message from Bob
    getline(stream, msg);
    EXPECT_THAT(msg, "Hello from Bob.");

    // Send message to Bob
    msg = "27F8#22E6";
    stream << msg << endl;

    // Check message from Bob
    getline(stream, msg);
    EXPECT_THAT(msg, "4ADE");
}

void netintro_alice() {
    // Make alice connect to mim
    alice("localhost", "9999");
}

void netintro_bob() {
    // Setup server
    asio::io_context io_context;
    tcp::acceptor acceptor(
        io_context, 
        tcp::endpoint(tcp::v4(), 9999)
    );
    tcp::iostream stream;
    if (!stream) FAIL();
    acceptor.accept(*stream.rdbuf());
    bob(stream);
}

TEST(DhTest, NetintroAlice) {
    // Just test Alice
    thread ni1 (netintro_mim_alice);
    thread ni2 (netintro_alice);

    ni1.join();
    ni2.join();
}

TEST(DhTest, NetintroBob) {
    // Just test Bob
    thread ni1 (netintro_bob);
    thread ni2 (netintro_mim_bob);

    ni1.join();
    ni2.join();
}

// ###################### DH ##############################

// TODO: no idea how to test this or what to test...

void netintro_mim() {
    // Setup man in the middle

    // Connect to Bob
    tcp::iostream stream_bob(tcp::resolver::query{"localhost", "8888"});
    
    // Await connection from alice
    asio::io_context io_context_alice;
    tcp::acceptor acceptor_alice(
        io_context_alice, 
        tcp::endpoint(tcp::v4(), 9999)
    );
    tcp::iostream stream_alice;
    if (!stream_alice) FAIL();
    acceptor_alice.accept(*stream_alice.rdbuf());

    // Check message from Alice
    //string mim_msg;
    //getline(stream_mim, mim_msg);
    //EXPECT_THAT(mim_msg, "Hello Server, I am Alice!");

    //// Send message to alice
    //mim_msg = "Hello from Bob.";
    //stream_mim << mim_msg << endl;

    //// Check message from Alice
    //getline(stream_mim, mim_msg);
    //EXPECT_THAT(mim_msg, "27F8#22E6");

    //// Send message to alice
    //mim_msg = "4ADE";
    //stream_mim << mim_msg << endl;
}
