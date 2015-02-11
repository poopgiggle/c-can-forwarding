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


struct addr_struct {
  int in_socket;
  int out_socket;
};

void * PassThread(void* addrs){
  fd_set fds;
  int status;
  int nbytes;
  struct can_frame frame;
  struct addr_struct *sockets = (struct addr_struct *) addrs;
  int in_socket = sockets -> in_socket;
  int out_socket = sockets -> out_socket;


  FD_ZERO(&fds);
  FD_SET(in_socket,&fds);

  while(1){
    status = pselect(in_socket + 1, &fds, NULL, NULL, NULL, NULL);
    if(status > 0){
      nbytes = read(in_socket, &frame, sizeof(struct can_frame));
      if(nbytes < 0){
	perror("can read error");
      }else if(nbytes < sizeof(struct can_frame)){
	perror("incomplete frame read");
      }else{
	nbytes = write(out_socket,&frame,nbytes);
	if(nbytes < 0){
	  perror("can write error");
	}
      }

    }else{
      FD_ZERO(&fds);
      FD_SET(in_socket,&fds);
    }
  }
}


int main(int argc, char **argv){
  void* status;
  int i;
  pthread_t threads[2];
  struct addr_struct addrs1;
  struct addr_struct addrs2;

  int can0 = 0;
  int can1 = 0;

  char* interface0 = "can0";
  char* interface1 = "can1";
  struct sockaddr_can addr0;
  struct sockaddr_can addr1;
  
  struct ifreq ifr0;
  struct ifreq ifr1;

  int rcr;
  int rcw;

  if(can0 = socket(PF_CAN, SOCK_RAW, CAN_RAW) < 0){
    perror("CAN0");
    return 1;
  }


  strcpy(ifr0.ifr_name, "can0");
  if(ioctl(can0, SIOCGIFINDEX, &ifr0) < 0){
    perror("SIOCGIFINDEXCAN0");
    return 1;
  }
  addr0.can_family = AF_CAN;
  addr0.can_ifindex = ifr0.ifr_ifindex;

  if (can1 = socket(PF_CAN, SOCK_RAW, CAN_RAW) < 0){
    perror("CAN1");
    return 1;
  }

  strcpy(ifr1.ifr_name, "can1");
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

  addrs1.in_socket = can0;
  addrs1.out_socket = can1;
  addrs2.in_socket = can1;
  addrs2.out_socket = can0;
  
  rcr = pthread_create(&threads[0], NULL, PassThread, (void *) &addrs1);
  rcw = pthread_create(&threads[1], NULL, PassThread, (void *) &addrs2);
  
  for(i=0;i<2;i++){
    pthread_join(threads[i],&status);
  }

  pthread_exit(NULL);

}
