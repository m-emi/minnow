#include "tcp_sender.hh"
#include "tcp_config.hh"
#include "tcp_sender_message.hh"
#include "wrapping_integers.hh"
#include <cstdint>
#include <iostream> 

using namespace std;

uint64_t TCPSender::sequence_numbers_in_flight() const
{
  if (!outstanding_queue_.empty())
  {
    return next_seqno_ - outstanding_queue_.front().seqno.unwrap(isn_, next_seqno_);
  }
  return 0;
}

uint64_t TCPSender::consecutive_retransmissions() const
{
  return consecutive_retransmissions_;
}

void TCPSender::push( const TransmitFunction& transmit )
{
  uint64_t window_size = window_size_;
  if (window_size == 0)
  {
    window_size++;
  }
  // TCPSender asked to fill the window unti it can't.
  while (window_size > sequence_numbers_in_flight() ) 
  {
    TCPSenderMessage msg;
    // track if SYN has been sent
    if (next_seqno_ == 0) 
    {
      msg.SYN = true;
      msg.seqno = isn_;
    }
    // SYN already sent
    else
    {
      msg.seqno = Wrap32::wrap(next_seqno_,isn_); // Wrap32
    }
    
    // Get payload 
    uint64_t payload_size = min(TCPConfig::MAX_PAYLOAD_SIZE, window_size - sequence_numbers_in_flight() - msg.SYN);
    string payload;
    read(input_.reader(), payload_size, payload);
    msg.payload = payload;

    if (!fin_received && reader().is_finished() && msg.sequence_length() + sequence_numbers_in_flight() < window_size) // no more bytes being written
    { 
      msg.FIN = true;
      fin_received = true;
      cerr << "\nseq_length() + seqnos_in_flight = " << msg.sequence_length() + sequence_numbers_in_flight();
      cerr << "\nwindow_size = " << window_size;

    }

    // no payload or SYN or FIN flag set
    if (msg.sequence_length() == 0) {
      // cerr << "msg sequence length is 0";
      break;
    }

    if (outstanding_queue_.empty())
    {
      RTO_ms_ = initial_RTO_ms_;
      timer_ = 0;
    }

    next_seqno_ += msg.sequence_length();

    // add to queue
    outstanding_queue_.push(msg);
    // transmit
    transmit(msg);

    if (msg.FIN)
    {
      break;
    }
  }

}

TCPSenderMessage TCPSender::make_empty_message() const
{
  TCPSenderMessage empty_sender_msg;
  empty_sender_msg.seqno = Wrap32::wrap(next_seqno_, isn_);
  return empty_sender_msg;
}

void TCPSender::receive( const TCPReceiverMessage& msg )
{
  window_size_ = msg.window_size;
  if (msg.ackno.has_value()) 
  {
    uint64_t abs_seqno = msg.ackno.value().unwrap(isn_, next_seqno_);

    // Edge case in test 31
    if (abs_seqno > next_seqno_){
      return;
    }


    while (!outstanding_queue_.empty())
    {
      TCPSenderMessage front_msg = outstanding_queue_.front();
      uint64_t front_seqno = front_msg.seqno.unwrap(isn_, next_seqno_) + front_msg.sequence_length();

      // If your received seqno >= oldest seqno, oldest msg can be popped.
      if ( abs_seqno >= front_seqno)
      {
        outstanding_queue_.pop();
        // reset timer
        RTO_ms_ = initial_RTO_ms_;
        consecutive_retransmissions_ = 0;
        if (!outstanding_queue_.empty())
        {
          timer_ = 0;
        }
      }
      else 
      {
        break;
      }
    }
    
  }
}

void TCPSender::tick( uint64_t ms_since_last_tick, const TransmitFunction& transmit )
{
  timer_ += ms_since_last_tick;
  // alarm goes off once RTO has elapsed. Must be something in the queue.
  if (!outstanding_queue_.empty() && timer_ >= RTO_ms_)
  {
    //cerr << outstanding_queue_.front().payload;
    transmit(outstanding_queue_.front());
    // window size always greater than 0.
    if (window_size_ > 0)
    {
    RTO_ms_ *= 2;
    }
    timer_ = 0;
    consecutive_retransmissions_++;
  }
}
