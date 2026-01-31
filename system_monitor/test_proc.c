#include "proc_status.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
//~ void print_detailed_info(const struct proc_info *info){ //const pointer to struct , can modify the data, but cannot change the pointer points to.
	//~ printf("┌─ Process Details ──────────────────────────┐\n");
	//~ printf("│ %-40s │\n",info->name);
	//~ printf("├────────────────────────────────────────────┤\n");
	//~ printf("│ PID:  %-34d │\n",info->pid);
	//~ printf("│ PPID: %-34d │\n",info->ppid);
	//~ printf("│ State:%-34c │\n",info->state);
	//~ printf("│ Threads:%-32d │\n",info->num_threads);
	//~ printf("│ Memory:%-33ld KB │\n",info->vmrss);
	//~ printf("│ Nice Value: %-29d │\n",info->nice_value);
	//~ printf("│ Priority: %-31ld │\n",info->priority);
	//~ printf("│ utime: %-34ld │\n",info->utime);
	//~ printf("│ stime: %-34ld │\n",info->stime);
	//~ printf("│ cutime: %-33ld │\n",info->cutime);
	//~ printf("│ cstime: %-33ld │\n",info->cstime);
	//~ printf("│ starttime: %-30ld │\n",info->starttime);
	//~ printf("└────────────────────────────────────────────┘\n");
	
	//~ //interpret state 
	
	//~ const char* state_desc;
	//~ switch(info->state){
		//~ case 'R': state_desc = "Running";break;
		//~ case 'S': state_desc = "Sleeping";break;
		//~ case 'D': state_desc = "Disk Sleeping";break;
		//~ case 'Z' : state_desc = "Zombie";break;
		//~ case 'T': state_desc = "stoppped";break;
		//~ default: state_desc = "Unknown";
		//~ }
	
	//~ printf("state: '%c = %s'\n",info->state,state_desc);
	//~ DEBUG_PRINT("Debug parsed state: %c = %s\n",info->state,state_desc);
	
	//~ }


void test_current_process(){
	struct proc_info info;
	
	assert(read_proc_self_full_info(&info) == 0);
	assert(info.pid > 0);
	assert(info.pid > 0);
	assert(info.name[0] != '\0');
	assert(info.num_threads >= 1);
	assert(info.state == 'S' || info.state == 'R');
	assert(info.vmrss > 0);
	assert(info.utime >= 0);
	assert(info.stime >= 0);
	assert(info.cutime >= 0);
	assert(info.starttime > 0);
	printf("Current process test passed\n");
	}

void test_pid1(){
	struct proc_info info;
	
	int result = read_proc_full_info(1,&info);
	if(result == 0){
		assert(info.pid == 1);
		assert(info.ppid == 0);
		assert(strcmp(info.name,"systemd") == 0);
		printf("PID 1 test passed (running as root)\n");
	}
	else{
		printf("Test failed for pid 1 (not running as root)");
		}
	}
void test_error_handling(){
	struct proc_info info;
	assert(read_proc_self_full_info(NULL) != 0);
	assert(read_proc_full_info(-1,NULL) != 0);
	assert(read_proc_full_info(99999,&info) != 0);
	assert(read_proc_full_info(1,NULL) != 0);
	
	printf("Error handling testy passed\n");
	}


int main(){
	printf("Running process ProcMOn tests...\n\n");
	
	test_current_process();
	test_pid1();
	test_error_handling();
	printf("\n All tests passed!\n");
	return 0;
}		
