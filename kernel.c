
#include "kernel.h"
#include <assert.h>
#include <stdint.h>

int find_empty_page_number(struct Kernel* kernel);
int out_of_bounds(struct Kernel* kernel, int pid, char* addr, int size, char* buf);
int find_free_pid(struct Kernel* kernel);
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

  //check if free process exists -> iterate through running
  //1.1. Check if a free process slot exists (check the running, the slot will be the pid returned)
  int pid = find_free_pid(kernel);


  //1.2. Check if there's enough free space (check allocated_pages)
  int pages_needed = (PAGE_SIZE-1 + size)/PAGE_SIZE;


  //Return if None exist
  if (pid == -1 || size > VIRTUAL_SPACE_SIZE || KERNEL_SPACE_SIZE < (PAGE_SIZE * (kernel->allocated_pages + pages_needed))) {
    return -1;
  }


  //1.3 Allocate the space for page_table (the size of it depends on the pages you needed. e.g. if size=33 and PAGE_SIZE=32, then you need 2 pages) and update allocated_pages
  kernel->allocated_pages += pages_needed;

  kernel->running[pid] = 1;

  //1.4. The mapping to kernel-managed memory is not built up, all the PFN should be set to -1 and present byte to 0 (PTE) and set the corresponding element in running to 1.
  struct PTE* page_table_entries = (struct PTE*) malloc(pages_needed * sizeof(struct PTE));

  for (int i = 0; i < pages_needed; i++) {
    page_table_entries[i].present = 0;
    page_table_entries[i].PFN = -1;
    //TODO: is MMstruct size, should it be the actual size or page size?
  }

  struct PageTable* page_table = (struct PageTable*) malloc(sizeof(struct PageTable));
  page_table->ptes = page_table_entries;

  kernel->mm[pid].page_table = page_table;
  kernel->mm[pid].size = size;


  return pid;
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
  if (out_of_bounds(kernel, pid, addr, size, buf)) return -1;
  
  for (int i = 0;i < size; i++) {
    int page_number = ((uintptr_t) addr + i)/PAGE_SIZE;
    if(kernel->mm[pid].page_table->ptes[page_number].present == 0) {
      kernel->mm[pid].page_table->ptes[page_number].present = 1;
      kernel->mm[pid].page_table->ptes[page_number].PFN = find_empty_page_number(kernel);
    }

    buf[i] = kernel->space[kernel->mm[pid].page_table->ptes[page_number].PFN*PAGE_SIZE + i % PAGE_SIZE];
  }


  return 0;
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
  if(out_of_bounds(kernel, pid, addr, size, buf)) return -1;

  for (int i = 0;i < size; i++) {
    int page_number = ((uintptr_t) addr + i)/PAGE_SIZE;
    if(kernel->mm[pid].page_table->ptes[page_number].present == 0) {
      kernel->mm[pid].page_table->ptes[page_number].present = 1;
      kernel->mm[pid].page_table->ptes[page_number].PFN = find_empty_page_number(kernel);
    }
    kernel->space[kernel->mm[pid].page_table->ptes[page_number].PFN*PAGE_SIZE + i % PAGE_SIZE] = buf[i];
  }

  return 0;
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
  if (kernel->running[pid] == 0) return -1;

  int totalPages = (kernel->mm[pid].size + PAGE_SIZE - 1) / PAGE_SIZE;

  for (int i = 0; i < totalPages; i++) {
      struct PTE *pte = &kernel->mm[pid].page_table->ptes[i];
      if (pte->present) {
          memset(kernel->space + (pte->PFN * PAGE_SIZE), 0, PAGE_SIZE);
          kernel->occupied_pages[pte->PFN] = 0;
          pte->present = 0;
          pte->PFN = -1;
      }
  }

  kernel->allocated_pages -= totalPages;
  kernel->mm[pid].size = 0;

  free(kernel->mm[pid].page_table->ptes);
  free(kernel->mm[pid].page_table);
  kernel->mm[pid].page_table = NULL;
  kernel->mm[pid].size = 0;
  kernel->running[pid] = 0;

  return 0;
}


int find_empty_page_number(struct Kernel* kernel) {
  int pages = KERNEL_SPACE_SIZE/PAGE_SIZE;

  for (int i = 0; i < pages; i++) {
    if (kernel->occupied_pages[i] == 0) {
      kernel->occupied_pages[i] = 1;
      return i;
    }
  }

  printf("ERROR Occured in find_empty_page_number, NO PAGE FOUND!");
  assert(0 != 1);
  return -1;
}


int find_free_pid(struct Kernel* kernel) {
  for (int i = 0; i < MAX_PROCESS_NUM; i++) {
    if (kernel->running[i] == 0) {
      return i;
    }
  }
  return -1;
}


int out_of_bounds(struct Kernel* kernel, int pid, char* addr, int size, char* buf) {
  //TODO: Check extra stuff
  if (kernel->mm[pid].size < ((uintptr_t) addr) + size) return 1;
  return 0;
}