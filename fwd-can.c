#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <ctype.h>
#include <libgen.h>
#include <time.h>
#include <errno.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/uio.h>
#include <net/if.h>

#include <linux/can.h>
#include <linux/can/raw.h>


void * PassThread(void* args){
  fd_set fds;
  int status;
  struct msghdr msg;
  struct canfd_frame frame;
  int in_socket = (int) args[0];
  int out_socket = (int) args[1];

  FD_ZERO(&fds);
  FD_SET(in_socket,&fds);

  while(1){
    status = pselect(in_socket + 1, &fds, NULL, NULL, NULL, NULL);
    


  }
}


int main(int argc, char **argv){
  int can0 = 0;
  int can1 = 0;

  char* interface0 = "can0";
  char* interface1 = "can1";
  struct sockaddr_can addr0;
  struct sockaddr_can addr1;
  
  struct ifreq ifr0;
  struct ifreq ifr1;

  if(can0 = socket(PF_CAN, SOCK_RAW, CAN_RAW) < 0){
    perror("CAN0");
    return 1;
  }

  addr0.can_family = AF_CAN;

  strcpy(ifr0.ifr_name, interface0);
  if(ioctl(can0, SIOCGIFINDEX, &ifr0) < 0){
    perror("SIOCGIFINDEXCAN0");
    return 1;
  }
  addr0.can_ifindex = ifr0.ifr_ifindex;

  if (can1 = socket(PF_CAN, SOCK_RAW, CAN_RAW) < 0){
    perror("CAN1");
    return 1;
  }

  strcpy(ifr1.ifr_name, interface1);
  if(ioctl(can1, SIOCGIFINDEX, &ifr1) < 0){
    perror("SIOCGIFINDEXCAN1");
    return 1;
  }
  addr1.can_ifindex = ifr1.ifr_ifindex;

  if(bind(can0,(struct sockaddr*)&addr0, sizeof(addr0)) < 0){
    perror("bind0");
    return 1;
  }

  if(bind(can1,(struct sockaddr*)&addr1, sizeof(addr1)) < 0){
    perror("bind1");
    return 1;
  }




}
