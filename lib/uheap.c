#include <inc/lib.h>

// malloc()
//	This function use FIRST FIT strategy to allocate space in heap
//  with the given size and return void pointer to the start of the allocated space

//	To do this, we need to switch to the kernel, allocate the required space
//	in Page File then switch back to the user again.
//
//	We can use sys_allocateMem(uint32 virtual_address, uint32 size); which
//		switches to the kernel mode, calls allocateMem(struct Env* e, uint32 virtual_address, uint32 size) in
//		"memory_manager.c", then switch back to the user mode here
//	the allocateMem function is empty, make sure to implement it.

//==================================================================================//
//============================ REQUIRED FUNCTIONS ==================================//
//==================================================================================//

int freeZ = 0 ;
int start_address=USER_HEAP_START;
int stAd=USER_HEAP_START ;

////////////////////////////

struct allocatedFrames{
	int address;
	int size;
	int num;
}allocatd_frames[41500];

///////////////////////////////

int size_of_frees=0;
int size_of_alloc = 0;

///////////////////////////////
int currentBlock = 0 ;
int heapFrames = (USER_HEAP_MAX-USER_HEAP_START)/PAGE_SIZE ;
int user_heap_frames[(USER_HEAP_MAX-USER_HEAP_START)/PAGE_SIZE]= {0};

void* malloc(uint32 size)
{
		uint32 mini=4294967294;
		int ans=-1;
		int found=0;
		size=ROUNDUP(size,PAGE_SIZE);
		int numOfBlocks = ROUNDUP(size,PAGE_SIZE) / PAGE_SIZE;
		int cnt =0;
		int flag=0;
		int fr=0;
		int ans2=0,ans3=0;
		for (int i = USER_HEAP_START; i < USER_HEAP_MAX; i+=PAGE_SIZE)
		{
			if (user_heap_frames[fr] == 0)
			{
				if(!cnt)
					stAd = i,ans2=fr;
				cnt++;
			}
			else if (user_heap_frames[fr] == 1 )
			{
				flag=1;
				int freeSpace = cnt * PAGE_SIZE ;
				cnt = 0 ;
				if (freeSpace >= size && freeSpace-size<mini)
				{
					ans = stAd;
					ans3=ans2;
					mini = freeSpace-size;
					found=1;
				}
			}
			fr++;
		}
				if (cnt)
				{
					int freeSpace = cnt * PAGE_SIZE ;
					if (freeSpace >= size && freeSpace-size<=mini)
					{
						ans = stAd;
						ans3=ans2;
						mini = freeSpace;
						found=1;
					}
				}
		/*if (ans==-1){
			ans=start_address;
		}*/






	if(!found)
		return NULL;


		//int add = fr;
		for (int i = 0 ; i < numOfBlocks ; i++  )
		{

			user_heap_frames[ans3+i] = 1 ;

		}
		sys_allocateMem(ans,size);
		allocatd_frames[size_of_alloc].address=ans;
		allocatd_frames[size_of_alloc].size=size;
		allocatd_frames[size_of_alloc].num=ans3;
		size_of_alloc++;
		return (void*)ans;


}
// free():
//	This function frees the allocation of the given virtual_address
//	To do this, we need to switch to the kernel, free the pages AND "EMPTY" PAGE TABLES
//	from page file and main memory then switch back to the user again.
//
//	We can use sys_freeMem(uint32 virtual_address, uint32 size); which
//		switches to the kernel mode, calls freeMem(struct Env* e, uint32 virtual_address, uint32 size) in
//		"memory_manager.c", then switch back to the user mode here
//	the freeMem function is empty, make sure to implement it.

void free(void* virtual_address)
{
	//TODO: [PROJECT 2021 - [2] User Heap] free() [User Side]
	// Write your code here, remove the panic and write your code
	//panic("free() is not implemented yet...!!");
	//you should get the size of the given allocation using its address
	//refer to the project presentation and documentation for details
	int index=-1;
	for(int i=0;i<size_of_alloc;i++){
		if (allocatd_frames[i].address==(uint32)virtual_address){
			index=i;
			sys_freeMem((uint32)virtual_address,allocatd_frames[i].size);
			break;
		}
	}
	//int add = (uint32)virtual_address - USER_HEAP_START ;

	int numOfBlocks = allocatd_frames[index].size / PAGE_SIZE;
	for (int i = 0 ; i < numOfBlocks ; i++  )
			{

				user_heap_frames[allocatd_frames[index].num+i] = 0;

			}
	allocatd_frames[index].address=-1;
	allocatd_frames[index].size=-1;

}

//==================================================================================//
//================================ OTHER FUNCTIONS =================================//
//==================================================================================//

void* smalloc(char *sharedVarName, uint32 size, uint8 isWritable)
{
	panic("this function is not required...!!");
	return 0;
}

void* sget(int32 ownerEnvID, char *sharedVarName)
{
	panic("this function is not required...!!");
	return 0;
}

void sfree(void* virtual_address)
{
	panic("this function is not required...!!");
}

void *realloc(void *virtual_address, uint32 new_size)
{
	panic("this function is not required...!!");
	return 0;
}

void expand(uint32 newSize)
{
	panic("this function is not required...!!");
}
void shrink(uint32 newSize)
{
	panic("this function is not required...!!");
}

void freeHeap(void* virtual_address)
{
	panic("this function is not required...!!");
}
