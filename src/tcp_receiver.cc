#include "tcp_receiver.hh"

using namespace std;

void TCPReceiver::receive( TCPSenderMessage message )
{
  if (message.RST) {
    // use set_error() from bytestream
  }
  // Unwrap gets you absolute sequence number
  // Find stream index.
  // Should be absolute seqno - 1
  uint64_t abs_seqno = message.seqno.unwrap(message.seqno, writer().bytes_pushed());
  uint64_t stream_index = abs_seqno - 1;

  if (message.SYN) {
    // this segment is beginning of the byte stream
    // don't start conversation until SYN flag is receieved 
    ISN = message.seqno;
  }
  // Conversation can be started
  if (ISN.has_value()) {
      uint64_t abs_seqno = message.seqno.unwrap(ISN.value(), writer().bytes_pushed());

      if (message.SYN && !message.FIN) {
        reassembler_.insert(0, message.payload, message.FIN);
      }
      else if (!message.SYN && !message.FIN) 
      {
        
      }
      



  }

  if (!message.SYN && !message.FIN) {
    reassembler_.insert(stream_index, message.payload, false);
  }

  if (message.FIN) {
    //this segment is the end of the byte stream
    // close conversation once FIN flag is received
    uint64_t abs_seqno = message.seqno.unwrap(message.seqno, writer().bytes_pushed());
    uint64_t stream_index = abs_seqno - 1;
    reassembler_.insert(stream_index, message.payload, message.FIN);
  }

  if (reassembler_.reader().is_closed()) {

  }
}

TCPReceiverMessage TCPReceiver::send() const
{
  // Your code here.
  return {};
}
