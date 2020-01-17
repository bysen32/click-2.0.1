define(
    $PktPerSec      1,
    $MaxPktCount    20000,
    $PktLen         128,
    $SRCMAC         00:0c:29:5c:cf:96,
    $DSTMAC         00:0c:29:31:bb:fc,
    $SRCIP          0xC0A80281, 		//192.168.2.129
    $DSTIP          0xC0A80180, 		//192.168.1.128
    $SRCPORT        2345,
    $DSTPORT        2346,
    $DEV0           ens38,
    $DEV1           ens39
)

FastPSPSource($PktPerSec, $MaxPktCount, $PktLen, $SRCMAC, $SRCIP, $SRCPORT, $DSTMAC, $DSTIP, $DSTPORT) -> PSPPrint() -> ToDevice($DEV1);
