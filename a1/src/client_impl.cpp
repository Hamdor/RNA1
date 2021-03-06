/******************************************************************************
 *                     ____    __  __      ______     _                       *
 *                    /\  _`\ /\ \/\ \    /\  _  \  /' \                      *
 *                    \ \ \L\ \ \ `\\ \   \ \ \L\ \/\_, \                     *
 *                     \ \ ,  /\ \ , ` \   \ \  __ \/_/\ \                    *
 *                      \ \ \\ \\ \ \`\ \   \ \ \/\ \ \ \ \                   *
 *                       \ \_\ \_\ \_\ \_\   \ \_\ \_\ \ \_\                  *
 *                        \/_/\/ /\/_/\/_/    \/_/\/_/  \/_/                  *
 *                                                                            *
 * Copyright (C) 2014                                                         *
 * Marian Triebe  <marian.triebe  (at) haw-hamburg.de>                        *
 * Katja Kirstein <katja.kirstein (at) haw-hamburg.de>                        *
 *                                                                            *
 ******************************************************************************/

#include "client_impl.hpp"

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "ring_buffer.hpp"

using namespace rna1;

namespace {

const char*  window_name   = "RNA1 - Client";

const int image_height     = 320;
const int image_width      = 240;
const int image_depth      = IPL_DEPTH_8U;
const int image_channel    = 3;

const size_t delay         = 5;
const size_t unkown_and    = 255; // what is that
const size_t unkown_value  = 27;  // what is that

} // namespace <anonymous>

void* client_impl::exec(void* arg) {
  cvNamedWindow(window_name);
  IplImage* image = cvCreateImage(cvSize(image_height, image_width),
                                  image_depth,
                                  image_channel);
  ring_buffer* buffer = ring_buffer::get_instance();
  size_t current_pos  = buffer->get_current_pos();
  while(true) {
    image->imageData = buffer->get_picture(current_pos).m_data;
    cvShowImage(window_name, image);
    if ((cvWaitKey(delay) & unkown_and) == unkown_value) {
      break;
    }
    buffer->wait_on_picture(current_pos);
    current_pos = (current_pos + 1) % buffer->size;
  }
  cvReleaseImage(&image);
  cvDestroyAllWindows();
  return NULL;
}
