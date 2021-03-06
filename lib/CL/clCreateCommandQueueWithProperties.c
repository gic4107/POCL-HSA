/* OpenCL runtime library: clCreateCommandQueue()

   Copyright (c) 2011 Universidad Rey Juan Carlos
   
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

CL_API_ENTRY cl_command_queue CL_API_CALL
POname(clCreateCommandQueueWithProperties)(cl_context context,
                     cl_device_id device, 
                     cl_command_queue_properties properties,
                     cl_int *errcode_ret) CL_API_SUFFIX__VERSION_2_0
{
  int i;
  int errcode;
  cl_bool found;

  /* validate flags */
  if (properties > (1<<2)-1)
  {
    errcode = CL_INVALID_VALUE;
    goto ERROR;
  }

  /* we don't handle out-of-order queues yet */
  if (properties & CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE) 
  {
    errcode = CL_INVALID_QUEUE_PROPERTIES;
    goto ERROR;
  }

  for (i=0; i<context->num_devices; i++)
    if (context->devices[i] == device)
      found = CL_TRUE;

  if (found == CL_FALSE)
  {
    errcode = CL_INVALID_DEVICE; 
  }

  cl_command_queue command_queue = (cl_command_queue) malloc(sizeof(struct _cl_command_queue));
  if (command_queue == NULL)
  {
    errcode = CL_OUT_OF_HOST_MEMORY;
    goto ERROR;
  }
  
  POCL_INIT_OBJECT(command_queue);

  command_queue->context = context;
  command_queue->device = device;
  command_queue->properties = properties;
  command_queue->root = NULL;
#if defined HSA_RUNTIME
  if(device->type&CL_DEVICE_TYPE_GPU){
	  hsa_status_t err;
	  /*
	   * Query the maximum size of the queue.
	   */
	  uint32_t queue_size = 0;
	  err = hsa_agent_get_info(device->agent_id, HSA_AGENT_INFO_QUEUE_MAX_SIZE, &queue_size);
#ifndef DEBUG
	  if(err != HSA_STATUS_SUCCESS)
#endif
	  {
		  check(Querying the device maximum queue size, err);
	  	  printf("The maximum queue size is %u.\n", (unsigned int) queue_size);
	  }
	  err = hsa_queue_create(device->agent_id, queue_size, HSA_QUEUE_TYPE_MULTI, NULL, NULL, &command_queue->queue);
#ifndef DEBUG
	  if(err != HSA_STATUS_SUCCESS)
#endif
	  check(Creating the queue, err);
  }
#endif
  if (errcode_ret != NULL)
    *errcode_ret = CL_SUCCESS;
  return command_queue;

ERROR:
    if(errcode_ret)
    {
        *errcode_ret = errcode;
    }
    return NULL;
}
POsym(clCreateCommandQueueWithProperties)
