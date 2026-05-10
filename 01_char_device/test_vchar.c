#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ioctl.h>

/*test ioctl function */
#define VCHAR_IOC_MAGIC 'k'
#define VCHAR_IOC_RESET _IO(VCHAR_IOC_MAGIC, 1)
#define VCHAR_IOC_SET_SIZE _IOW(VCHAR_IOC_MAGIC, 2, int)
#define VCHAR_IOC_GET_SIZE _IOR(VCHAR_IOC_MAGIC, 3, int)
#define VCHAR_IOC_MAXNR 3

int readMsg(int fd) {
    char buf[100];
    int n = read(fd, buf, sizeof(buf) - 1);
    if (n > 0) {
        buf[n] = '\0';
        printf("read from driver msg: %s \n", buf);
    }
    return n;
}

int writeMsg(int fd, char* message) {
    int ret = write(fd, message, strlen(message));
    printf("Writing to driver: %s len %d\n", message, strlen(message));
    return ret;
}

void testIoctl(int fd) {
    int size_val;
    if (ioctl(fd, VCHAR_IOC_GET_SIZE, &size_val) < 0) {
        perror("ioctl get_size failed \n");
    } else {
        printf("current size %d \n", size_val);
    }

    size_val = 512;
    if (ioctl(fd, VCHAR_IOC_SET_SIZE, &size_val) < 0) {
        perror("ioctl set size failed \n");
    } else {
        printf("set sucessfully \n");
    }

    if (ioctl(fd, VCHAR_IOC_GET_SIZE, &size_val) < 0) {
        perror("ioctl get size failed \n");
    } else {
        printf("current size %d \n", size_val);
    }

    if (ioctl(fd, VCHAR_IOC_RESET) < 0) {
        perror("ioctl resetting failed \n");
    } else {
        printf("resetting sucessfully \n");
    }

}

int main() {
    int fd = open("/dev/vchar", O_RDWR);
    if (fd < 0) { 
        perror(" open failed");
        return -1;
    }

    int ret = readMsg(fd);
    if (ret < 0) {
        printf("read failed \n");
        return -1;
    }

    // write
    char* msg = "write to kernel";
    ret = writeMsg(fd, msg);

    if (ret < 0) {
         printf("Write Logic Failed. Error Code: %d fd %d\n", errno, fd);
        return -1;
    }


    lseek(fd, 0, SEEK_SET);
    ret = readMsg(fd);
    if (ret < 0) {
        printf("read message failed Error code: %d \n", errno);
        return -1;
    }
    printf("read message finished. Error code: %d fd %d\n", errno, fd);

    testIoctl(fd);
    close(fd);
    return 0;
}