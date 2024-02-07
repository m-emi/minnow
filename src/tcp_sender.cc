#include "tcp_sender.hh"
#include "tcp_config.hh"

using namespace std;

uint64_t TCPSender::sequence_numbers_in_flight() const
{
  return seqnos_.size();
}

uint64_t TCPSender::consecutive_retransmissions() const
{
  // Your code here.
  return {};
}

void TCPSender::push( const TransmitFunction& transmit )
{
  // TCPSender asked to fill the window
  // reads from the stream and sends as many TCPSenderMessages as possible as long as there are new bytes to be read and space in the window

  // make sure each TCPSenderMessage fits fully inside receiver's window
  // make each individual message as big as possible, but no bigger than MAX_PAYLOAD_SIZE

  // use TCPSenderMEssage::sequence_length() to count total number of sequence numbers occupied by segment
    // SYN and FIN also occupy space in window


  // Use read helper function from bytestream, it peeks and pops
  // checkpoint should be bytes_popped


  TCPSenderMessage sender_msg;
  sender_msg.SYN = 
  sender_msg.FIN = 
  sender_msg.payload =
  sender_msg.RST =  
  bool RST {};

  while (window_left > 0)
  {
    transmit(sender_message);
  }
}

TCPSenderMessage TCPSender::make_empty_message() const
{
  // keep track of acknos
  // set all the flags to false

  return {};
}

void TCPSender::receive( const TCPReceiverMessage& msg )
{
  uint16_t window_start = msg.window_size - sequence_numbers_in_flight();

  // window_left = ackno;
  // window_right = ackno + msg.window_size

  (void)msg;
}

void TCPSender::tick( uint64_t ms_since_last_tick, const TransmitFunction& transmit )
{
  // Your code here.
  (void)ms_since_last_tick;
  (void)transmit;
  (void)initial_RTO_ms_;
}
