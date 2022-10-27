#include <ev.h>
#include <amqpcpp.h>
#include <amqpcpp/libev.h>
#include <fmt/core.h>

/**
 *  Main procedure
 *  @return int
 */
int main(void)
{
    // we use the libev event loop handler
    AMQP::LibEvHandler handler(EV_DEFAULT);

    // set up a tcp connection
    AMQP::TcpConnection connection(&handler, AMQP::Address("amqp://guest:guest@localhost/"));
    AMQP::TcpChannel channel(&connection);

    // we can consume up to 5 msgs at the same time
    channel.setQos(5);

    // make sure queue exists
    channel.declareQueue("Q_RPC");
    fmt::print("Created Queue");

    // start consuming like crazy
    static long count = 0;
    channel.consume("Q_RPC",AMQP::noack).onReceived([&channel](const AMQP::Message &message, uint64_t deliveryTag, bool redelivered) {
        auto  end = ev_time();
	fmt::print("get message {}\n", end);
    });

    // run event loop
    ev_run(EV_DEFAULT);

    // unreachable...
    return 0;
}
