#ifndef SimpleConnectInternal_h
#define SimpleConnectInternal_h

// Reserved paths that SimpleConnect handles internally
static const char *RESERVED_PATHS[] = {
    "/wifi/settings",
    "/wifi/api/status",
    "/wifi/api/scan",
    "/wifi/api/connect",
    "/wifi/api/forget",
    "/generate_204",
    "/fwlink",
    "/hotspot-detect.html",
    "/ncsi.txt",
    "/connecttest.txt"};

#endif