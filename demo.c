#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "kernel.h"

// Test case 1
// Test create/destroy process.
// Totally 25 points.
int case1(){
  KERNEL_SPACE_SIZE = 8192;
  VIRTUAL_SPACE_SIZE = 512;
  PAGE_SIZE = 32;
  MAX_PROCESS_NUM = 8;

  printf("---------------- Test case 1 ----------------\n");
  printf("KERNEL_SPACE_SIZE=%d\nVIRTUAL_SPACE_SIZE=%d\nPAGE_SIZE=%d\nMAX_PROCESS_NUM=%d\n", KERNEL_SPACE_SIZE, VIRTUAL_SPACE_SIZE, PAGE_SIZE, MAX_PROCESS_NUM);
  printf("----------------------------------------------\n\n");

  int score = 0;

  struct Kernel * kernel = init_kernel();

  // Create process 0 with size VIRTUAL_SPACE_SIZE.
  int pid1 = proc_create_vm(kernel, VIRTUAL_SPACE_SIZE);
  if(pid1 == 0)
    score += 1;
    
  printf("-------------SCORE: %d-------------\n", score);
  // Try to create a process whose memory size larger than the limit.
  int pid = proc_create_vm(kernel, VIRTUAL_SPACE_SIZE + 1);
  if(pid == -1)
    score += 1;
  printf("-------------SCORE: %d-------------\n", score);
  int pid2 = proc_create_vm(kernel, VIRTUAL_SPACE_SIZE);
  if(pid2 == 1)
    score += 1;
  printf("-------------SCORE: %d-------------\n", score);
  int pid3 = proc_create_vm(kernel, VIRTUAL_SPACE_SIZE);
  if(pid3 == 2)
    score += 1;
  printf("-------------SCORE: %d-------------\n", score);
  int pid4 = proc_create_vm(kernel, VIRTUAL_SPACE_SIZE);
  if(pid4 == 3)
    score += 1;
  printf("-------------SCORE: %d-------------\n", score);
  int pid5 = proc_create_vm(kernel, VIRTUAL_SPACE_SIZE);
  if(pid5 == 4)
    score += 1;
  printf("-------------SCORE: %d-------------\n", score);
  int pid6 = proc_create_vm(kernel, VIRTUAL_SPACE_SIZE);
  if(pid6 == 5)
    score += 1;
  printf("-------------SCORE: %d-------------\n", score);
  int pid7 = proc_create_vm(kernel, VIRTUAL_SPACE_SIZE);
  if(pid7 == 6)
    score += 1;
  printf("-------------SCORE: %d-------------\n", score);
  int pid8 = proc_create_vm(kernel, VIRTUAL_SPACE_SIZE);
  if(pid8 == 7)
    score += 1;
  printf("-------------SCORE: %d-------------\n", score);
  pid = proc_create_vm(kernel, VIRTUAL_SPACE_SIZE);
  if(pid == -1)
    score += 1;
  printf("-------------SCORE: %d-------------\n", score);
  // Check if present is clear.
  int set = 1;
  for(int i = 0; i < 8; i ++) {
    for(int j = 0; j < kernel->mm[i].size/PAGE_SIZE; j++) {
      if(kernel->mm[i].page_table->ptes[j].present != 0){
        set = 0;
        break;
      }
    }
    if(set == 0) break;
  }
  if(set == 1)
    score += 2;
    
  printf("-------------SCORE: %d-------------\n", score);
  // Check if running is set.
  set = 1;
  for(int i = 0; i < 8; i ++){
    if(kernel->running[i] != 1){
      set = 0;
      break;
    }
  }
  printf("-------------SCORE: %d-------------\n", score);
  if(set == 1)
    score += 2;
  printf("-------------SCORE: %d-------------\n", score);
  if(proc_exit_vm(kernel, pid1) == 0)
    score += 1;
  printf("-------------SCORE: %d-------------\n", score);
  if(proc_exit_vm(kernel, pid2) == 0)
    score += 1;
  printf("-------------SCORE: %d-------------\n", score);
  if(proc_exit_vm(kernel, pid3) == 0)
    score += 1;
  printf("-------------SCORE: %d-------------\n", score);
  if(proc_exit_vm(kernel, pid4) == 0)
    score += 1;
  printf("-------------SCORE: %d-------------\n", score);
  if(proc_exit_vm(kernel, pid5) == 0)
    score += 1;
  printf("-------------SCORE: %d-------------\n", score);
  if(proc_exit_vm(kernel, pid6) == 0)
    score += 1;
  printf("-------------SCORE: %d-------------\n", score);
  if(proc_exit_vm(kernel, pid7) == 0)
    score += 1;
  printf("-------------SCORE: %d-------------\n", score);
  if(proc_exit_vm(kernel, pid8) == 0)
    score += 1;
  printf("-------------SCORE: %d-------------\n", score);
  // Check if occupied_pages is reset.
  set = 1;
  for(int i = 0; i < VIRTUAL_SPACE_SIZE/PAGE_SIZE*8; i ++){
    if(kernel->occupied_pages[i] != 0){
      set = 0;
      break;
    }
  }
  if(set == 1)
    score += 2;
  printf("-------------SCORE: %d-------------\n", score);
  // Check if running is reset.
  set = 1;
  for(int i = 0; i < 8; i ++){
    if(kernel->running[i] != 0){
      set = 0;
      break;
    }
  }
  if(set == 1)
    score += 1;
  printf("-------------SCORE: %d-------------\n", score);
  destroy_kernel(kernel);
  return score;
}

// Test case 2
// A simple test case for you to debug.
// totally 19 scores
int case2(){
  KERNEL_SPACE_SIZE = 8192;
  VIRTUAL_SPACE_SIZE = 512;
  PAGE_SIZE = 32;
  MAX_PROCESS_NUM = 8;

  printf("---------------- Test case 2 ----------------\n");
  printf("KERNEL_SPACE_SIZE=%d\nVIRTUAL_SPACE_SIZE=%d\nPAGE_SIZE=%d\nMAX_PROCESS_NUM=%d\n", KERNEL_SPACE_SIZE, VIRTUAL_SPACE_SIZE, PAGE_SIZE, MAX_PROCESS_NUM);
  printf("----------------------------------------------\n\n");

  int score = 0;

  struct Kernel * kernel = init_kernel();
  char * buf = (char *)malloc(128);
  char * temp_buf = (char *)malloc(512);

  // Try to create a process whose memory size larger than the limit.
  int pid_invalid = proc_create_vm(kernel, VIRTUAL_SPACE_SIZE + 1);
  if(pid_invalid == -1)
    score += 1;
  printf("-------------SCORE: %d-------------\n", score);
  // Create process 0 with size VIRTUAL_SPACE_SIZE.
  int pid0 = proc_create_vm(kernel, VIRTUAL_SPACE_SIZE);
  if(pid0 == 0)
    score += 1;

  printf("-------------SCORE: %d-------------\n", score);
  // Create process 1 with size VIRTUAL_SPACE_SIZE/2.
  int pid1 = proc_create_vm(kernel, VIRTUAL_SPACE_SIZE/2);
  if(pid1 == 1)
    score += 1;
  printf("-------------SCORE: %d-------------\n", score);
  // Print the memory mappings of process 1.
  printf("Before reading pages 0-7 of process %d\n", pid1);
  print_kernel_free_space(kernel);
  print_memory_mappings(kernel, pid1);

  // Check the free space after reading pages 0-7 for process 1.
  memset(buf, 0, 128);
  memset(temp_buf, 0, 512);
  vm_read(kernel, pid1, (char *)(0), 234, temp_buf);
  get_kernel_free_space_info(kernel, buf);
  if(strcmp(buf, "free space: (addr:256, size:7936)\n") == 0)
    score += 1;
  printf("-------------SCORE: %d-------------\n", score);
  // Print the memory mappings of process 1.
  printf("After reading pages 0-7 of process %d\n", pid1);
  print_kernel_free_space(kernel);
  print_memory_mappings(kernel, pid1);

  // Create process 2 with size VIRTUAL_SPACE_SIZE/4.
  int pid2 = proc_create_vm(kernel, VIRTUAL_SPACE_SIZE/4);
  if(pid2 == 2)
    score += 1;
  printf("-------------SCORE: %d-------------\n", score);
  // Print the memory mappings of process 2.
  printf("Before writting/reading page 1 of process %d\n", pid2);
  print_kernel_free_space(kernel);
  print_memory_mappings(kernel, pid2);

  // Verify writting/reading page 1 for process 2.
  memset(buf, 0, 128);
  memset(temp_buf, 0, 512);
  temp_buf[0] = 'a';
  if(vm_write(kernel, pid2, (char *)(42), 1, temp_buf) == 0)
    score += 1;
  printf("-------------SCORE: %d-------------\n", score);
  temp_buf[0] = '\0'; // Clean the first byte of temp_buf.
  if(vm_read(kernel, pid2, (char *)(42), 1, temp_buf) == 0)
    score += 1;
  printf("-------------SCORE: %d-------------\n", score);
  if(temp_buf[0] == 'a')
    score += 1;
  printf("-------------SCORE: %d-------------\n", score);
  get_kernel_free_space_info(kernel, buf);
  if(strcmp(buf, "free space: (addr:288, size:7904)\n") == 0)
    score += 1;
  printf("-------------SCORE: %d-------------\n", score);

  // Print the memory mappings of process 2.
  printf("After writting/reading page 1 of process %d\n", pid2);
  print_kernel_free_space(kernel);
  print_memory_mappings(kernel, pid2);

  // Create process 3 with size VIRTUAL_SPACE_SIZE/4.
  int pid3 = proc_create_vm(kernel, VIRTUAL_SPACE_SIZE/4);
  if(pid3 == 3)
    score += 1;
  printf("-------------SCORE: %d-------------\n", score);
  // Print the memory mappings of process 3.
  printf("Before writting pages 0-3 of process %d\n", pid3);
  print_kernel_free_space(kernel);
  print_memory_mappings(kernel, pid3);

  // Check the free space after writting pages 0-3 of process 3.
  memset(buf, 0, 128);
  memset(temp_buf, 0, 512);
  vm_write(kernel, pid3, (char *)(0), VIRTUAL_SPACE_SIZE/4, temp_buf);
  get_kernel_free_space_info(kernel, buf);
  if(strcmp(buf, "free space: (addr:416, size:7776)\n") == 0)
    score += 1;
  printf("-------------SCORE: %d-------------\n", score);
  // Print the memory mappings of process 3.
  printf("After writting pages 0-3 of process %d\n", pid3);
  print_kernel_free_space(kernel);
  print_memory_mappings(kernel, pid3);

  // Exit process 2 and check free space.
  if(proc_exit_vm(kernel, pid2) == 0)
    score += 1;
  printf("-------------SCORE: %d-------------\n", score);
  printf("After process 2 exits\n");
  print_kernel_free_space(kernel);
  get_kernel_free_space_info(kernel, buf);
  if(strcmp(buf, "free space: (addr:256, size:32) -> (addr:416, size:7776)\n") == 0)
    score += 1;
  printf("-------------SCORE: %d-------------\n", score);
  // Exit process 3 and check free space.
  if(proc_exit_vm(kernel, pid3) == 0)
    score += 1;
  printf("-------------SCORE: %d-------------\n", score);
  printf("After process 3 exits\n");
  print_kernel_free_space(kernel);
  get_kernel_free_space_info(kernel, buf);
  if(strcmp(buf, "free space: (addr:256, size:7936)\n") == 0)
    score += 1;
  printf("-------------SCORE: %d-------------\n", score);
  // Exit process 1 and check free space.
  if(proc_exit_vm(kernel, pid1) == 0)
    score += 1;
  printf("-------------SCORE: %d-------------\n", score);
  printf("After process 1 exits\n");
  print_kernel_free_space(kernel);
  get_kernel_free_space_info(kernel, buf);
  if(strcmp(buf, "free space: (addr:0, size:8192)\n") == 0)
    score += 1;
  printf("-------------SCORE: %d-------------\n", score);
  // Exit process 0 and check free space.
  if(proc_exit_vm(kernel, pid0) == 0)
    score += 1;
  printf("-------------SCORE: %d-------------\n", score);
  printf("After process 0 exits\n");
  print_kernel_free_space(kernel);
  get_kernel_free_space_info(kernel, buf);
  if(strcmp(buf, "free space: (addr:0, size:8192)\n") == 0)
    score += 1; 
  printf("-------------SCORE: %d-------------\n", score);
  destroy_kernel(kernel);
  free(buf);
  free(temp_buf);

  return score;
}

int main() {
  int score1 = 0;
  int score2 = 0;

  score1 = case1();
  printf("Test case 1 : Full Score=25, Your Score=%d\n", score1);

  score2 = case2();
  printf("Test case 2 : Full Score=19, Your Score=%d\n", score2);
}