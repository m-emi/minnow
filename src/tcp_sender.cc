#include "tcp_sender.hh"
#include "tcp_config.hh"
#include <iostream> 

using namespace std;

uint64_t TCPSender::sequence_numbers_in_flight() const
{
  return {};
  //return seqnos_.size();
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



  // construct a TCPSenderMessage
  TCPSenderMessage sender_msg;
  sender_msg.SYN = true;
  sender_msg.seqno = isn_;
  transmit(sender_msg);

  

  // sender_msg_queue_.push(sender_msg);

  
  // while (window_start > 0)
  // {
  //   transmit(sender_msg_queue_.front());
  //   sender_msg_queue.pop();
  // }
  // (void)transmit;
}

TCPSenderMessage TCPSender::make_empty_message() const
{
  // keep track of acknos
  // set all the flags to false

  return {};
}

void TCPSender::receive( const TCPReceiverMessage& msg )
{
  // total window 
  window_start = msg.ackno.value() + msg.window_size;

  
  // how do you know there is a SYN or FIN flag
  ack_seqno_ = msg.ackno.value();


}

void TCPSender::tick( uint64_t ms_since_last_tick, const TransmitFunction& transmit )
{
  // Your code here.
  (void)ms_since_last_tick;
  (void)transmit;
  (void)initial_RTO_ms_;
}
