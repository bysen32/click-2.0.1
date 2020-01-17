// label-route.click

// ------------------------- Configure --------------------------
define( 
	$DEV0 		ens38,			//192.168.2.128
	$DEV1 		ens39,			//192.168.3.128

	$MAC0 		00:0c:29:31:bb:fc,
	$MAC1 		00:0c:29:31:bb:06,

	$PSP0 		129.2.168.192.0.0.0.0,
	$PORT0		0,
	$DESTMAC0 	00:0c:29:5c:cf:96,

	$PSP1 		128.1.168.192.0.0.0.0,
	$PORT1		1,
	$DESTMAC1 	00:0c:29:47:f4:e6
)

// PSP Route
psprt :: LookupPSPRoute(
	$PSP0 0,
	$PSP1 1,
);
// Label Library
label :: LookupLIB(
	10 0 $PSP0 $PSP1 20 1
);
// ------------------- Common Code Bellow -----------------------
// 1.FromDevice
// data :: FromDevice($DEV0);

// 2.Classifier
c0 :: Classifier(12/0600, 12/0601, -);
FromDevice($DEV0) -> Paint(1) -> c0;
FromDevice($DEV1) -> Paint(2) -> c0;

// PSP Protocol Deal With -----------------------------------------
	// 3.PSP Package: C0[0]
	// 4. CheckPSPHeader
	// 5. DecPspTTL
	// 6. PSPLookup
	c0[0] 	-> Strip(14) -> Print("psp")
			-> chpsph :: CheckPSPHeader
			-> DecPspTTL
			-> psprt;

	// 7. PSPMacap && 8. Send Queue && 9. ToDevice
	out0 :: Queue(200) -> ToDevice($DEV0);
	out1 :: Queue(200) -> ToDevice($DEV1);

	// Dispatcher
	/*
	disp1 :: Dispatcher;
	sche1 :: Scheduler;
	sche1[0] -> ToDevice($DEV1);
	disp1[0] -> Queue(20) -> BandwidthShaper(2048B/s) -> [0]sche1;
	disp1[1] -> Queue(20) -> BandwidthShaper(4096B/s) -> [1]sche1;
	disp1[2] -> Queue(20) -> BandwidthShaper(8192B/s) -> [2]sche1;
	disp1[3] -> Queue(20) -> BandwidthShaper(16384B/s) -> [3]sche1;
	disp1[4] -> Queue(20) -> BandwidthShaper(32768B/s) -> [4]sche1;
	disp1[5] -> Queue(20) -> BandwidthShaper(65536B/s) -> [5]sche1;
	disp1[6] -> Queue(20) -> BandwidthShaper(131072B/s) -> [6]sche1;
	disp1[7] -> Queue(100)  -> [7]sche1;
	*/

	cp2 :: CheckPaint(1);
	cp2[0] -> Discard;
	cp2[1] -> EncapEth($MAC, $DESCMAC1) -> out0;

	psprt[0] -> DropBroadcasts -> cp2;
	psprt[1] -> PushLabel -> label;

// Label Protocol Deal With ------------------------------------
	c0[1] 	-> Strip(14)
			-> Print("label")
			-> label;

	label[0] -> cp0 :: CheckPaint(1);
	cp0[0] -> Discard;
	cp0[1] 	-> DecLabelTTL
			-> ExchangeLabel
			-> EncapEth($MAC0, $DESTMAC1)
			-> out0;

	label[1] -> cp1 :: CheckPaint(2);
	cp1[0] -> Discard;
	cp1[1] 	-> DecLabelTTL 
			-> ExchangeLabel 
			-> EncapEth($MAC0, $DESCMAC1) 
			-> out1;

	label[2] -> PopLabel -> chpsph;

c0[2]->Discard;
