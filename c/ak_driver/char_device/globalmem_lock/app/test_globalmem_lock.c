
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

main(int argc ,char * argv[])
{
  int testdev ;
  int i ;
  char buf[10] ;
  int pos = 0 ;
 
  testdev = open(argv[1],O_RDWR) ;
  if(testdev==-1) 
  { 
    printf("can not open file %s\n",argv[1]) ;
    exit(0) ;
  }
  printf("open device %s ok \n",argv[1]) ;
   
   pos = lseek(testdev,0,SEEK_SET);
  read(testdev,buf,10) ;
  printf("_______________the buff is  ________\n") ; 
  for(i = 0 ;i <10 ;i++) 
   printf("%c",buf[i]) ;
  printf("\n") ;
  printf("sizeof(buf)=%d\n",sizeof(buf));
  memset(buf,0,sizeof(buf)) ;
  pos = lseek(testdev,0,SEEK_SET);
     write(testdev ,"123456" ,6) ;
  pos = lseek(testdev,0,SEEK_SET);
  read(testdev,buf,7) ;
  printf("_______________the  NEW buff is  as follows________\n") ; 
  for(i = 0 ;i <10 ;i++) 
   printf("%c",buf[i]) ;
  printf("\n") ;
  close(testdev) ;
}


