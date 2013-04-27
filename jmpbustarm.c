//lolgplv3 or some shit

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <elf.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

#define NORMAL "\033[0m"
#define BOLD           "\033[1m"
#define GREEN  "\033[92m"
#define YELLOW "\033[01;93m"

typedef unsigned short int u16;
typedef unsigned long int u32;

int main(int argc, char**argv)
{
    char* file;
    unsigned char* code;
    int i, fd, text_offset, code_size;

    u32 buff; //bx is a 32 bits 
    u32 addr;
    Elf32_Ehdr elfhdr;
    Elf32_Phdr p_hdr;
    Elf32_Shdr s_hdr;

    printf("** JMP buster - Tool for searching registers BX inside an executable file\n"
            "by BlackLight, released under GNU GPL license v.3, 2009\n"
            "ARM haxx by Jeffrey Crowell\n");
    if(!argv[1])
    {
        printf("Usage: %s <executable_file>\n", argv[1]);
        exit(1);
    }

    file = argv[1];

    if((fd=open(file, O_RDONLY)) < 0)
    {
        error(errno, 2, argv[0]);
    }

    read(fd, &elfhdr, sizeof(Elf32_Ehdr));
    code_size = elfhdr.e_shoff;

    lseek(fd, elfhdr.e_phoff, SEEK_SET);
    addr = elfhdr.e_entry;
    text_offset = addr - 0x00008000;

    lseek(fd, text_offset, SEEK_SET);
    
    code = (char*) malloc(code_size);
    memset(code, 0x0, code_size);
    read(fd, code, code_size);

    /*
     *
  14:   e12fff10        bx      r0
  18:   e12fff11        bx      r1
  1c:   e12fff12        bx      r2
  20:   e12fff13        bx      r3
  24:   e12fff14        bx      r4
  28:   e12fff15        bx      r5
  2c:   e12fff16        bx      r6
  30:   e12fff17        bx      r7
  34:   e12fff18        bx      r8
  38:   e12fff19        bx      r9
  3c:   e12fff1a        bx      sl
  40:   e12fff1b        bx      fp
  44:   e12fff1c        bx      ip
  48:   e12fff1d        bx      sp
  4c:   e12fff1e        bx      lr
  50:   e12fff1f        bx      pc
*/

    for(i=0; i<code_size; i++, addr = elfhdr.e_entry+i)
    {
        memcpy(&buff, &(code[i]), sizeof(buff));

        switch(buff)
        {

            case 0xe12fff10:
                printf("%s-> [bx r0] found at addr %s0x%.8x%s\n", NORMAL, GREEN, addr, NORMAL);
                break;
            
            case 0xe12fff11:
                printf("%s-> [bx r1] found at addr %x0x%.8x%s\n", NORMAL, GREEN, addr, NORMAL);
                break;
                
            case 0xe12fff12:
                printf("%s-> [bx r2] found at addr %s0x%.8x%s\n", NORMAL, GREEN, addr, NORMAL);
                break;
                
            case 0xe12fff13:
                printf("%s-> [bx r3] found at addr %s0x%.8x%s\n", NORMAL, GREEN, addr, NORMAL);
                break;
        
            case 0xe12fff14:
                printf("%s-> [bx r4] found at addr %s0x%.8x%s\n", NORMAL, GREEN, addr, NORMAL);
                break;
        
            case 0xe12fff15:
                printf("%s-> [bx r5] found at addr %s0x%.8x%s\n", NORMAL, GREEN, addr, NORMAL);
                break;

            case 0xe12fff16:
                printf("%s-> [bx r6] found at addr %s0x%.8x%s\n", NORMAL, GREEN, addr, NORMAL);
                break;

            case 0xe12fff17:
                printf("%s-> [bx r7] found at addr %s0x%.8x%s\n", NORMAL, GREEN, addr, NORMAL);
                break;

            case 0xe12fff18:
                printf("%s-> [bx r8] found at addr %s0x%.8x%s\n", NORMAL, GREEN, addr, NORMAL);
                break;

            case 0xe12fff19:
                printf("%s-> [bx r9] found at addr %s0x%.8x%s\n", NORMAL, GREEN, addr, NORMAL);
                break;

            case 0xe12fff1a:
                printf("%s-> [bx sl] found at addr %s0x%.8x%s\n", NORMAL, GREEN, addr, NORMAL);
                break;

            case 0xe12fff1b:
                printf("%s-> [bx fp] found at addr %s0x%.8x%s\n", NORMAL, GREEN, addr, NORMAL);
                break;

            case 0xe12fff1c:
                printf("%s-> [bx ip] found at addr %s0x%.8x%s\n", NORMAL, GREEN, addr, NORMAL);
                break;

            case 0xe12fff1d:
                printf("%s-> [%sbx sp%s] found at addr %s0x%.8x%s\n", BOLD, YELLOW, NORMAL, GREEN, addr, NORMAL);
                break;

            case 0xe12fff1e:
                printf("%s-> [bx lr] found at addr %s0x%.8x%s\n", NORMAL, GREEN, addr, NORMAL);
                break;

            case 0xe12fff1f:
                printf("%s-> [bx pc] found at addr %s0x%.8x%s\n", NORMAL, GREEN, addr, NORMAL);
                break;

        }
    }
    close(fd);
    return 0;
}

