Checkpoint 0 Writeup
====================

My name: Emilio Melgoza

My SUNet ID: emilio18

I collaborated with: patwei

I would like to credit/thank these classmates for their help: pjovel, 

This lab took me about 9 hours to do. I did not attend the lab session.

My secret code from section 2.1 was: 293736

I was surprised by or edified to learn that: a bitstream could be represented 
through a string. 

Describe ByteStream implementation. [Describe data structures and
approach taken. Describe alternative designs considered or tested.
Describe benefits and weaknesses of your design compared with
alternatives -- perhaps in terms of simplicity/complexity, risk of
bugs, asymptotic performance, empirical performance, required
implementation time and difficulty, and other factors. Include any
measurements if applicable.]

I thought about using a queue to implement my ByteStream implementation. It 
felt like it made sense to queue data by pushing to the back and popping from
the front. However, my peers suggested a simpler structure and suggested I look
at the data type of data. Data is of type string, so it made sense to me just 
to append data to a string of my own. It is simple to get the size of a string,
append to it, and popping from it using the erase method. 

- Optional: I had unexpected difficulty with: [describe]

- Optional: I think you could make this lab better by: [describe]

- Optional: I'm not sure about: [describe]

- Optional: I contributed a new test case that catches a plausible bug
  not otherwise caught: [provide Pull Request URL]