#include "kernel.h"

/*
  1. Check if a free process slot exists and if the there's enough free space (check allocated_pages).
  2. Alloc space for page_table (the size of it depends on how many pages you need) and update allocated_pages.
  3. The mapping to kernel-managed memory is not built up, all the PFN should be set to -1 and present byte to 0.
  4. After creating the process, update global state: allocated_pages
  5. Return a pid (the index in MMStruct array) which is >= 0 when success, -1 when failure in any above step.
  @param[in]  kernel    kernel simulator
  @param[in]  size      number of needed bytes
  @return     pid       process id when success, -1 when failure
*/
int proc_create_vm(struct Kernel* kernel, int size) {
  /* Fill Your Code Below */
}

/*
  This function will read the range [addr, addr+size) from user space of a specific process to the buf (buf should be >= size).
  1. Check if the reading range is out-of-bounds.
  2. If the pages in the range [addr, addr+size) of the user space of that process are not present,
     this leads to page fault.
     You should firstly map them to the free kernel-managed memory pages (first fit policy).
     You may use PFN, present, occupied_pages.
  3. Return 0 when success, -1 when failure (out of bounds).
  @param[in]  kernel    kernel simulator
  @param[in]  pid       process id
  @param[in]  addr      read content from [addr, addr+size)
  @param[in]  size      number of needed bytes
  @param[out] buf       buffer that should be filled with data
  @return               0 when success, -1 when failure
*/
int vm_read(struct Kernel* kernel, int pid, char* addr, int size, char* buf) {
  /* Fill Your Code Below */
}

/*
  This function will write the content of buf to user space [addr, addr+size) (buf should be >= size).
  1. Check if the writing range is out-of-bounds.
  2. If the pages in the range [addr, addr+size) of the user space of that process are not present,
     this leads to page fault.
     You should firstly map them to the free kernel-managed memory pages (first fit policy).
     You may use PFN, present, occupied_pages.
  3. Return 0 when success, -1 when failure (out of bounds).
  @param[in]  kernel    kernel simulator
  @param[in]  pid       process id
  @param[in]  addr      read content from [addr, addr+size)
  @param[in]  size      number of needed bytes
  @param[in]  buf       data that should be written
  @return               0 when success, -1 when failure
*/
int vm_write(struct Kernel* kernel, int pid, char* addr, int size, char* buf) {
  /* Fill Your Code Below */
}

/*
  This function will free the space of a process.
  1. Reset the corresponding pages in occupied_pages to 0, update allocated_pages
  2. Release the page_table in the corresponding MMStruct and set to NULL.
  3. Release the pid, reset running
  4. Return 0 when success, -1 when failure.
  @param[in]  kernel    kernel simulator
  @param[in]  pid       process id
  @return               0 when success, -1 when failure
*/
int proc_exit_vm(struct Kernel* kernel, int pid) {
  /* Fill Your Code Below */
}