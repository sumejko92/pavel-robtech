// Copyright (c) 2015-2016, The University of Texas at Austin
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
// 
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
// 
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
// 
//     * Neither the name of the copyright holder nor the names of its
//       contributors may be used to endorse or promote products derived from
//       this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

/** @file keyboard_reader.h
 * 
 *  @author karl.kruusamae(at)utexas.edu
 * 
 *  NOTE: If you get permission denied when starting this node. Use ' ls -l /dev/input/event* ' to learn which group can access the events.
 *        Then add your username to this group with ' sudo usermod -a -G group_name user_name '
 */

#include <linux/input.h>
#include <string.h>
#include <map>
#include <vector>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <glob.h>	// for counting files in a dir (needed for counting event files in /dev/input)
#include <sstream>

#ifndef MOUSE_READER_H
#define MOUSE_READER_H

class Mouse
{
public:
  // Constructor
  Mouse (std::string mouse_event_path)
  {
    if ( mouse_event_path.empty() )
    {
      // try to find a suitable event file from the files available on the system
      descriptor_ = findMouse();
    }
    else
    {
      //try to open user-specified event file as a keyboard
      descriptor_ = openMouse( mouse_event_path.c_str() );
    }
  };
  
  /** Goes through all the event files in /dev/input/ to locate a keyboard.
   *  @return file descriptor if all checks out; -1 otherwise.
   */
  int findMouse();

  /** Opens the input device and checks whether its meaningful name (ie, EVIOCGNAME in ioctl()) contains substrings specified in valid_substring.
   *  @param device_path file path of a linux event.
   *  @return file descriptor if open and other checks have been succesfully passed; -1 otherwise.
   */
  int openMouse(const char *device_path);

  /** Closes the device specificed by descriptor_. */
  void closeMouse();

  /** Checks if the keyboard event file has been succesfully opened.
   *  @return TRUE when descriptor_ is not negative, FALSE otherwise.
   */
  bool isReadable();

  /** Searches the input event for type EV_KEY and returns event code (ie, which key) and value (ie, pressed or depressed).
   *  @param ev input event.
   *  @return vector containing two unsigned integers: event code (based on linux/input.h) and event value (1 for pressed, 0 for depressed); returns {0, 0} if no EV_KEY.
   */
  std::vector <uint16_t> processEvent( struct input_event *ev );

  /** Reads event data, and returns relevant info only for EV_KEY events, and dismisses anything that is not a key being pressed or depressed.
   *  @param fd file descriptor for keyboard event file.
   *  @return vector containing two unsigned integers: event code (based on linux/input.h) and event value (1 for pressed, 0 for depressed).
   */
  std::vector <uint16_t> getKeyEvent();

  /** Return a string corresponding to a key code.
   *  @param key_code uint corresponding to key on a keyboard
   *  @return string corresponding to a key code
   */
  std::string getKeyName( uint16_t key_code );


private:
  /** File descriptor of the working mouse event file. */
  int descriptor_;

  /** A list of substrings that would indicate that a device is mouse. */
  std::vector<std::string> valid_substrings =
  {
    "mouse",
    "Mouse"
  };
  w
  /** Map key event code to key event string. */
  std::map<uint16_t, std::string> keymap_ =
  {

    {BTN_LEFT, "LeftButton"}, {BTN_RIGHT, "RightButton"},
    {BTN_MIDDLE, "MiddleButton"}, 
    {BTN_SIDE, "SideButton"}, {BTN_EXTRA, "ExtraButton"},
    {BTN_FORWARD, "ForwardButton"}, {BTN_BACK, "BackButton"},
    {BTN_TASK, "TaskButton"}, 

    {REL_X, "RelX"},
    {REL_Y, "RelY"},
    {REL_WHEEL, "RelWheel"},

    {MSC_SCAN, "MSCScan"},
  };

}; // end class Mouse

#endif
