// mytest-ip-route.click
// eth0 202.197.12.4 00:0c:29:cb:72:14    
// eth1 11.1.12.4 00:0c:29:cb:72:1e    

define($DEV0 ens38, $DEV1 ens39, $DADDR 202.197.12.190, $GW 202.197.12.1)

psp :: Print("Forward PSP Pkt") -> rt :: PSPLookup(
          11.1.12.4.0.0.0.0      0,
          202.197.12.4.0.0.0.0   0,
          129.2.168.192.0.0.0.0  1,
          128.1.168.192.0.0.0.0  2
);

c0 :: Classifier(12/0600, -);
out0 :: Queue(200) -> ToDevice($DEV0);

c1 :: Classifier(12/0600, -);
out1 :: Queue(200) -> ToDevice($DEV1);

FromDevice($DEV0) -> c0;
c0[0]->psp;
c0[1]->Discard;

FromDevice($DEV1) -> c1;
c1[0]->psp;
c1[1]->Discard;

rt[0] -> Discard;

rt[1] -> PSPMacap(00:0c:29:5c:cf:8c) -> Print("rt1\n") -> out0;

// Forwarding path for eth1
rt[2] -> PSPMacap(00:0c:29:5c:cf:8c) -> out1;
