/* OpenCL runtime library: clReleaseCommandQueue()

   Copyright (c) 2011-2012 Universidad Rey Juan Carlos and Pekka Jääskeläinen
   
   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:
   
   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.
   
   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   THE SOFTWARE.
*/

#include "pocl_cl.h"

CL_API_ENTRY cl_int CL_API_CALL
POname(clReleaseCommandQueue)(cl_command_queue command_queue) CL_API_SUFFIX__VERSION_1_0
{
  int new_refcount;
  POname(clFlush)(command_queue);
  POCL_RELEASE_OBJECT(command_queue, new_refcount);
  if (new_refcount == 0)
    {
#if defined HSA_RUNTIME
	  if(command_queue->device->type&CL_DEVICE_TYPE_GPU){
		  if(command_queue->queue != NULL){
		  hsa_status_t err=hsa_queue_destroy(command_queue->queue);
#ifndef DEBUG
		  if(err != HSA_STATUS_SUCCESS)
#endif
			  check(Destroying the queue, err);
		  }
	  }
#endif
	  free (command_queue);
      /* TODO: should clReleaseContext()? */
    }
  return CL_SUCCESS;
}
POsym(clReleaseCommandQueue)
