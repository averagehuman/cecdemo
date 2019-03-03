

#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <v4l-utils/cec.h>
#include <v4l-utils/cec-info.h>
#include <android_debug.h>

struct node {
    int fd;
    const char *device;
    unsigned caps;
    unsigned available_log_addrs;
    unsigned num_log_addrs;
    __u16 log_addr_mask;
    __u16 phys_addr;
    __u8 log_addr[CEC_MAX_LOG_ADDRS];
};

int cec_named_ioctl(int fd, const char *name, unsigned long int request, void *parm)
{
    int retval = ioctl(fd, request, parm);
    int e;

    e = retval == 0 ? 0 : errno;
    printf("\t\t%s returned %d (%s)\n", name, retval, strerror(e));

    return retval == -1 ? e : (retval ? -1 : 0);
}

#define doioctl(n, r, p) cec_named_ioctl((n)->fd, #r, r, p)

static unsigned parse_phys_addr(const char *value)
{
    unsigned p1, p2, p3, p4;

    if (!strchr(value, '.'))
        return strtoul(value, NULL, 0);
    if (sscanf(value, "%x.%x.%x.%x", &p1, &p2, &p3, &p4) != 4) {
        fprintf(stderr, "Expected a physical address of the form x.x.x.x\n");
        return 0;
    }
    if (p1 > 0xf || p2 > 0xf || p3 > 0xf || p4 > 0xf) {
        fprintf(stderr, "Physical address components should never be larger than 0xf\n");
        return 0;
    }
    return (p1 << 12) | (p2 << 8) | (p3 << 4) | p4;
}

void cec_info_driver_info()
{
    const char *device = "/dev/cec0";
    int fd = -1;
    struct node node = {};
    struct cec_caps caps = {};
    struct cec_log_addrs laddrs = {};
    __u16 phys_addr = 1;


    if ((fd = open(device, O_RDWR)) < 0) {
        LOGE("Failed to open %s: %s\n", device, strerror(errno));
    }
    node.fd = fd;
    node.device = device;
    doioctl(&node, CEC_ADAP_G_CAPS, &caps);
    node.caps = caps.capabilities;
    node.available_log_addrs = caps.available_log_addrs;

    doioctl(&node, CEC_ADAP_G_LOG_ADDRS, &laddrs);
    node.num_log_addrs = laddrs.num_log_addrs;
    node.log_addr_mask = laddrs.log_addr_mask;
    node.phys_addr = phys_addr;
}