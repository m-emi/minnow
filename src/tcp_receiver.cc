#include "tcp_receiver.hh"

using namespace std;

void TCPReceiver::receive( TCPSenderMessage message )
{
  if (message.SYN) {
    // this segment is beginning of the byte stream
    // conversation is starting
    // guaranteed to be sent first
    // don't start conversation until SYN flag is receieved 
  }
  if (message.FIN) {
    //this segment is the end of the byte stream
    // close conversation once FIN flag is received
  }

insert(message.seqno, message.payload, message.FIN)


}

TCPReceiverMessage TCPReceiver::send() const
{
  // Your code here.
  return {};
}
