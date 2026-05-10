#include <linux/ioctl.h>

// magic number
#define VCHAR_IOC_MAGIC 'k'
#define VCHAR_IOC_RESET _IO(VCHAR_IOC_MAGIC, 1)
#define VCHAR_IOC_SET_SIZE _IOW(VCHAR_IOC_MAGIC, 2, int)
#define VCHAR_IOC_GET_SIZE _IOR(VCHAR_IOC_MAGIC, 3, int)
#define VCHAR_IOC_MAXNR 3

