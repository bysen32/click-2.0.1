define(
    $PktPerSec      1,
    $MaxPktCount    20000,
    $PktLen         128,
    $SRCMAC         00:0c:29:5c:cf:96,
    $DSTMAC         00:0c:29:31:bb:fc,
    $SRCPSP         0xC0A80181,         //192.168.1.129
    $DSTPSP         0xC0A80480,         //192.168.4.128
    $SRCPORT        2345,
    $DSTPORT        2346,
    $DEV0           ens38,
    $DEV1           ens39
)

FastPSPSource($PktPerSec, $MaxPktCount, $PktLen, $SRCMAC, $SRCPSP, $SRCPORT, $DSTMAC, $DSTPSP, $DSTPORT) -> PSPPrint() -> ToDevice($DEV1);
