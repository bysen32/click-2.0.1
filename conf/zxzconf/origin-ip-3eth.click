// Generated by make-ip-conf.pl
// eth0 18.26.4.92 00:00:C0:3B:71:EF
// eth1 1.0.0.1 00:00:C0:CA:68:EF
// eth2 2.0.0.1 00:00:C0:8A:67:EF

// Shared IP input path and routing table
ip :: Strip(14)
    -> CheckIPHeader(INTERFACES 18.26.4.92/255.255.255.0 1.0.0.1/255.0.0.0 2.0.0.1/255.0.0.0)
    -> rt :: StaticIPLookup(
	18.26.4.92/32 0,
	18.26.4.255/32 0,
	18.26.4.0/32 0,
	1.0.0.1/32 0,
	1.255.255.255/32 0,
	1.0.0.0/32 0,
	2.0.0.1/32 0,
	2.255.255.255/32 0,
	2.0.0.0/32 0,
	18.26.4.0/255.255.255.0 1,
	1.0.0.0/255.0.0.0 2,
	2.0.0.0/255.0.0.0 3,
	255.255.255.255/32 0.0.0.0 0,
	0.0.0.0/32 0,
	0.0.0.0/0.0.0.0 18.26.4.1 1);

// ARP responses are copied to each ARPQuerier and the host.
arpt :: Tee(4);

// Input and output paths for eth0
c0 :: Classifier(12/0806 20/0001, 12/0806 20/0002, 12/0800, -);
PollDevice(eth0) -> c0;
out0 :: Queue(200) -> todevice0 :: ToDevice(eth0);
c0[0] -> ar0 :: ARPResponder(18.26.4.92 00:00:C0:3B:71:EF) -> out0;
arpq0 :: ARPQuerier(18.26.4.92, 00:00:C0:3B:71:EF) -> out0;
c0[1] -> arpt;
arpt[0] -> [1]arpq0;
c0[2] -> Paint(1) -> ip;
c0[3] -> Print("eth0 non-IP") -> Discard;

// Input and output paths for eth1
c1 :: Classifier(12/0806 20/0001, 12/0806 20/0002, 12/0800, -);
PollDevice(eth1) -> c1;
out1 :: Queue(200) -> todevice1 :: ToDevice(eth1);
c1[0] -> ar1 :: ARPResponder(1.0.0.1 00:00:C0:CA:68:EF) -> out1;
arpq1 :: ARPQuerier(1.0.0.1, 00:00:C0:CA:68:EF) -> out1;
c1[1] -> arpt;
arpt[1] -> [1]arpq1;
c1[2] -> Paint(2) -> ip;
c1[3] -> Print("eth1 non-IP") -> Discard;

// Input and output paths for eth2
c2 :: Classifier(12/0806 20/0001, 12/0806 20/0002, 12/0800, -);
PollDevice(eth2) -> c2;
out2 :: Queue(200) -> todevice2 :: ToDevice(eth2);
c2[0] -> ar2 :: ARPResponder(2.0.0.1 00:00:C0:8A:67:EF) -> out2;
arpq2 :: ARPQuerier(2.0.0.1, 00:00:C0:8A:67:EF) -> out2;
c2[1] -> arpt;
arpt[2] -> [1]arpq2;
c2[2] -> Paint(3) -> ip;
c2[3] -> Print("eth2 non-IP") -> Discard;

// Local delivery
toh :: ToHost;
arpt[3] -> toh;
rt[0] -> EtherEncap(0x0800, 1:1:1:1:1:1, 2:2:2:2:2:2) -> toh;

// Forwarding path for eth0
rt[1] -> DropBroadcasts
    -> cp0 :: PaintTee(1)
    -> gio0 :: IPGWOptions(18.26.4.92)
    -> FixIPSrc(18.26.4.92)
    -> dt0 :: DecIPTTL
    -> fr0 :: IPFragmenter(1500)
    -> [0]arpq0;
dt0[1] -> ICMPError(18.26.4.92, timeexceeded) -> rt;
fr0[1] -> ICMPError(18.26.4.92, unreachable, needfrag) -> rt;
gio0[1] -> ICMPError(18.26.4.92, parameterproblem) -> rt;
cp0[1] -> ICMPError(18.26.4.92, redirect, host) -> rt;

// Forwarding path for eth1
rt[2] -> DropBroadcasts
    -> cp1 :: PaintTee(2)
    -> gio1 :: IPGWOptions(1.0.0.1)
    -> FixIPSrc(1.0.0.1)
    -> dt1 :: DecIPTTL
    -> fr1 :: IPFragmenter(1500)
    -> [0]arpq1;
dt1[1] -> ICMPError(1.0.0.1, timeexceeded) -> rt;
fr1[1] -> ICMPError(1.0.0.1, unreachable, needfrag) -> rt;
gio1[1] -> ICMPError(1.0.0.1, parameterproblem) -> rt;
cp1[1] -> ICMPError(1.0.0.1, redirect, host) -> rt;

// Forwarding path for eth2
rt[3] -> DropBroadcasts
    -> cp2 :: PaintTee(3)
    -> gio2 :: IPGWOptions(2.0.0.1)
    -> FixIPSrc(2.0.0.1)
    -> dt2 :: DecIPTTL
    -> fr2 :: IPFragmenter(1500)
    -> [0]arpq2;
dt2[1] -> ICMPError(2.0.0.1, timeexceeded) -> rt;
fr2[1] -> ICMPError(2.0.0.1, unreachable, needfrag) -> rt;
gio2[1] -> ICMPError(2.0.0.1, parameterproblem) -> rt;
cp2[1] -> ICMPError(2.0.0.1, redirect, host) -> rt;
