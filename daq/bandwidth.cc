// -*- C++ -*-
//
// Bandwidth Test for ITSDAQ NexysV board 
//
// Original Author:  SHI Xin <shixin@ihep.ac.cn>
//         Created:  [2017-03-30 Thu 10:37] 
//

// Borrowed code from Matt Warren and Liejian Chen 
//  
// To use this code: 
// [].L macros/bandwidth.cc 
// [] bandwidthtest()

// 
// Log 
// 2017.04.19 Bruce Added runWithBurstManager  

// Tests 

void bandwidthtest(){
    //.L macros/BandTestChen.cpp 
    Matt_DataGenTest(3,0,0,28,28,183,1,3000);
    //while(!abc130_ReceivePacket())
    streamConfigRead();
    e->DAQStatus();
}

int abc130_DropPacket() {
  uint16_t opcode=0xd008;
  uint16_t seqnum;
  uint16_t length;
  uint16_t *recv_data;

  e->HsioReceiveOpcode(opcode, seqnum, length, recv_data);

  if(opcode == 0) { printf("Timeout, no more data\n"); return 1; }
  return 0;
}

// Functions 
void Matt_DataGenTest(unsigned int dg_mode = 3,
        unsigned int dg_src =  0,
        unsigned int strmmode = 0x1,
        unsigned int first_stream = 0,
        unsigned int n_streams = 4,
        unsigned int trigs=0,
        unsigned int freqid=100,
        unsigned int bursts=0){

    if (dg_src < 4) {
        e->ConfigureVariable(10036+dg_src, (dg_mode*0x10)+strmmode); 
        Matt_ConfStream( (strmmode*0x100)+((dg_src+4)*2) + 1, first_stream,  n_streams, 0);
        printf("DG Reg %d set to 0x%x\n", 36+dg_src, (dg_mode*0x10)+strmmode);

    } else {
        printf("DG source too large!!!!\n");
    }

  int drop_count = 0;

    //Internal Trigger
    if (trigs > 0) {
        Matt_TrigBurst(freqid, trigs, bursts);
// while(!abc130_DropPacket())
   // drop_count++;
        e->Sleep(freqid*trigs*(bursts+1)/100);
    }
    else {
        //External trigger
        e->ConfigureVariable(10000, 0x0001);     // PMOD-TTC trig input enable (TRIG0) 
        e-> Sleep(1);
        e->ConfigureVariable(10000, 0x0000);     // PMOD-TTC trig input disable (TRIG0) 
    }
    Matt_ConfStream( (strmmode*0x100) + ((dg_src+4)*2) + 0, first_stream,  n_streams, 0);

  printf("Finished, dropping %d packets\n", drop_count);
}

void Matt_ConfStream( unsigned int strmconf = 0x11, 
        unsigned int first_strmid = 0, 
        unsigned int n_strms = 72, 
        unsigned int n_trigs = 1){
    uint16_t opdata[300];
    opdata[0]=0xffff;

    printf("Configuring streams %d to %d (inclusive) with 0x%x\n", first_strmid, first_strmid+n_strms-1, strmconf);
    for(int ii=0; ii<n_strms; ii++) 	{ 
        opdata[ii*2+1]=ii+first_strmid; 
        opdata[ii*2+2]=strmconf; 
    }
    e->HsioSendOpcode(0x0050, 0x1234, n_strms*2+1, opdata);

    if (n_trigs > 0) {
        e->ConfigureVariable(10023, 0x880); // set trig = capt start

        for(int n=0; n<n_trigs; n++) 	{ 
            printf("ConfStream: Sending trig command %d\r", n);
            Matt_SendCommand(1);
        }
        printf("\n");  
    }
}

//----------------------------------------------------------------------------
void Matt_SendCommand(unsigned int commands = 0, unsigned int resets = 0 ) {

    uint16_t ocd[2];
    ocd[ 0]=commands;
    ocd[ 1]=resets;
    e->HsioSendOpcode(0x0030, 0xc0c0, 2, ocd);
}


//----------------------------------------------------------------------------
void Matt_TrigBurst( unsigned int freqid = 10, 
		     unsigned int ntrigs = 10, 
		     unsigned int bursts = 0)
{

  printf("Fixed freq burst with %d trigs, %d bursts (= %d trigs), freqid %d\n", 
	 ntrigs, bursts, ntrigs*(bursts+1), freqid);

  Matt_WriteReg(24,  ntrigs-1); //R_TB_TRIGS
  Matt_WriteReg(25,  bursts); //R_TB_BURSTS
  Matt_WriteReg(26,   freqid); //R_TB_PMIN
  Matt_WriteReg(27,   freqid); //R_TB_PMAX
  Matt_WriteReg(28,  0); //R_TB_PDEAD
  
  Matt_SendCommand(0x200);  //CMD_TB_RST
  Matt_SendCommand(0x100);  //CMD_TB_START

}

//----------------------------------------------------------------------------
void Matt_WriteReg(unsigned int reg = 0, unsigned int dat = 0) {

  uint16_t ocd[2];
  ocd[ 0]=reg;
  ocd[ 1]=dat;
  e->HsioSendOpcode(0x0010, 0xc0c0, 2, ocd);

}

int abc130_ReceivePacket(unsigned int displevel = 10, bool tmu = false) {
  uint16_t opcode=0xd008;
  uint16_t seqnum;
  uint16_t length;
  uint16_t *recv_data;

  e->HsioReceiveOpcode(opcode, seqnum, length, recv_data);

  if (displevel > 9) printf("op: %x seq: %d len: %d\n", opcode, seqnum, length);
    
  if(opcode == 0) { printf("Timeout\n"); return 1; }
  
  if (displevel > 9) printf("  Stream: %x\n", recv_data[0]);
  if (displevel > 9) printf("  Format: %x\n", recv_data[1]);
  if (displevel > 9) printf("  DatSrc: %x\n", recv_data[2]);

  if(0) {
    // Print undecoded opcode
    for(int i=6; i<length; i++) {
      if(((i-6) % 8) == 0) {
        printf("  %3d:", i);
      }
      printf(" %04x", recv_data[i]);
      if(((i-6) % 8) == 7) {
        printf("\n");
      }
    }
    printf("\n");
  }

  if (displevel > 9) printf(" [No. : chip  type   L0:BC StpBt :  StripData (as per type)      ]\n");

  for(int offset=3; offset<length; offset+=4) {
    unsigned long lodata = 0;
    unsigned long hidata = 0;
    hidata = ((recv_data[offset + 1] & 0xff) << 8) + ((recv_data[offset + 1] & 0xff00) >> 8);
    hidata |= ((recv_data[offset] & 0xff00) << 8UL) + ((recv_data[offset] & 0xff) << 24UL);
    lodata = ((recv_data[offset + 3] & 0xff) << 8) + ((recv_data[offset + 3] & 0xff00) >> 8);
    lodata |= ((recv_data[offset+2] & 0xff00) << 8UL) + ((recv_data[offset+2] & 0xff) << 24UL);

    if(lodata == 0 && hidata == 0) continue;
    // Skip over timestamps
    if((hidata & 0xf0000000) == 0xf0000000) continue;
//    printf(" %03d: %04x %04x %04x %04x\n", offset, recv_data[offset], recv_data[offset + 1], recv_data[offset + 2], recv_data[offset + 3]);
//    printf(" %03d: %08x %08x\n", offset, hidata, lodata);

    //printf(" %03d: %16b %16b %16b %16b\n", offset, recv_data[offset], recv_data[offset + 1], recv_data[offset + 2], recv_data[offset + 3]);
    //printf(" %03d: %32b %32b\n", offset, hidata, lodata);
    if (displevel > 9) printf("  %03d : ", ((offset-3)/4));
    abc130_DecodePacket(lodata, hidata, displevel, tmu);
  }

  if (displevel > 9) printf("\n");

  return 0;
}


void abc130_DecodePacket(unsigned long lodata, unsigned long hidata, unsigned int displevel, bool tmu) {
    const char *types[] = {
        "none", "Revd", "R3  ", "noR3",
        "1BC ", "HCCH", "1BC>", "1BC_",
        "Reg ", "Stat", "DCS ", "Undf",
        "3BC ", "HCCL", "3BC>", "3BC_",
    };

    // start bit   5,1
    int startBit = (hidata >> 27) & 1;
    // chipid   6,5
    int chipid = (hidata >> 22) & 0x1f;
    // typ  11,4
    int typ = (hidata >> 18) & 0xf;
    // l0id  15, 8
    int l0id = (hidata >> 10) & 0xff;
    // bcid  23, 8
    int bcid = (hidata >> 2) & 0xff;

    if((!tmu)&&(e->HccPresent())) {  
        chipid = (hidata >> 21) & 0x1f;
        typ = (hidata >> 17) & 0xf;
        l0id = (hidata >> 9) & 0xff; 
    }

    // stop bit    64,1  -- done now as payload has multi formats
    int stopBit = (lodata >> 0) & 1;

    if (displevel > 9) {
        printf("%2d  %2d:%s  %02x:%02x   %d   :",
                chipid, typ, types[typ], l0id, bcid, stopBit);
    }

    // payload   31, 33
    int payloadhi = (hidata >> 1) & 1;
    // 1 bit from hi | 31 bits from lo
    unsigned int payload = ((hidata << 31) & 0x80000000) | ((lodata >> 1) & 0x7fffffff);
    if((!tmu)&&(e->HccPresent())) {  
        payloadhi = hidata & 1;
        payload = (lodata >> 0) & 0xffffffff;
    }
    //printf("\n hidata: \t %32b \t lodata:  %32b \n", hidata, lodata);
    //printf("Payload: %32b \n", payload);

    switch(typ) {
        case 4: case 6: { // 1BC
                            // l1-strip0    P1,8
                            int strip0 = (payloadhi << 7) | (payload >> 25) & 0x7f;
                            // map0     P9,3
                            int map0 = (payload >> 22) & 0x7;
                            // l1-strip1    P12,8
                            int strip1 = (payload >> 14) & 0xff;
                            // map1    P20,3
                            int map1 = (payload >> 11) & 0x7;
                            // l1-strip2    P23,8
                            int strip2 = (payload >> 3) & 0xff;
                            // map2    P31,3
                            int map2 = (payload >> 0) & 0x7;

                            if(strip0 == strip1) {
                                printf("%3d %d            ",
                                        strip0, map0);
                            } else  if(strip1 == strip2) {
                                printf("%3d %d %3d %d      ",
                                        strip0, map0, strip1, map1);
                            } else {
                                printf("%3d %d %3d %d %3d %d",
                                        strip0, map0, strip1, map1, strip2, map2);
                            }
                        } break;
        case 2: { // R3
                    int strip0 = (payloadhi << 7) | (payload >> 25) & 0x7f;
                    int strip1 = (payload >> 17) & 0xff;
                    int strip2 = (payload >> 9) & 0xff;
                    int strip3 = (payload >> 1) & 0xff;
                    int ovf = payload & 1;
                    printf("%3d %3d %3d %3d %c",
                            strip0, strip1, strip2, strip3, (ovf?'!':'_'));
                } break;
        case 12: case 14: {   // 3BC
                              int address = (payloadhi << 7) | (payload >> 25) & 0x7f;
                              int hit0 = (payload >> 22) & 0x7;
                              int hit1 = (payload >> 19) & 0x7;
                              int hit2 = (payload >> 16) & 0x7;
                              int hit3 = (payload >> 13) & 0x7;
                              printf("%3d  %d  %d  %d  %d  ", address, hit0, hit1, hit2, hit3);
                          } break;
        case 5:  // HCC hi priority reg
        case 13: // HCC Lo priority reg
        case 8: {    // Reg
                    unsigned int reg = payload >> 1;
                    if(payloadhi) reg |= (unsigned int)0x80000000;
                    if (displevel > 9)     	printf("    %08x     ", reg);
                    else if (displevel == 2) printf("0x%02x: 0x%08x\n", l0id, reg);
                } break;
        default:
                printf("  %01x  %04x  %04x  ",
                        payloadhi, (payload>>16) & 0xffff, payload&0xffff);
                break;
    } // end switch

    if (displevel > 9) printf("\n");
}

void streamConfigRead(uint16_t mask0 = 0xffff, uint16_t mask1 = 0xffff, uint16_t mask2 = 0xffff,
        uint16_t mask3 = 0xffff, uint16_t mask4 = 0xffff, uint16_t mask5 = 0xffff,
        uint16_t mask6 = 0xffff, uint16_t mask7 = 0xffff, uint16_t mask8 = 0xffff){
    uint16_t opData[] = {mask0, mask1, mask2, mask3, mask4, mask5, mask6, mask7, mask8};

    // Readback register block
    uint16_t recv_buff[2];
    uint16_t length = e->HsioSendReceiveOpcode(0x51, 0x1234, 9, opData, 2, recv_buff);

    if(length > 0xf000) { printf("No response\n"); return; }

    printf("ack: [");
    for(int i=0; i<length; i++) {
        printf("%x, ", recv_buff[i]);
    }
    printf("]\n");

    while(1) {
        uint16_t seqnum;
        uint16_t opcode = 0xd051;
        uint16_t *recv_data;
        e->HsioReceiveOpcode(opcode, seqnum, length, recv_data);
        if(opcode == 0) break;
        for(int i=0; i<length; i++) {
            uint16_t swapped = ((recv_data[i]&0x00ff)<<8) | ((recv_data[i]&0xff00)>>8);
            if(i == 0) {
                printf(" Stream %3d: ", swapped);
            } else {
                printf(" %04x", swapped);
            }
        }
        printf("\n");
    }
}


void runWithBurstManager() {
    unsigned int dg_mode = 3;
    unsigned int dg_src =  0;
    unsigned int strmmode = 0x1;

    e->ConfigureVariable(10036+dg_src, (dg_mode*0x10)+strmmode); 
    int first_stream = 0;
    int n_streams = 144;  // max streams in firmware 
    Matt_ConfStream( (strmmode*0x100)+((dg_src+4)*2), first_stream,  n_streams, 0);
    printf("DG Reg %d set to 0x%x\n", 36+dg_src, (dg_mode*0x10)+strmmode);

    // Run burster in firmware
    // Count of executions of the burster
    e->burst_ntrigs=1;
    e->burst_maxtrigs=10000;

    // Just run outer loop at 2.5kHz
    //uint16_t nt = 183; // Triggers per "burst" (-1)
    //uint16_t nb = 3000;   // Bursts of triggers (-1)

    uint16_t nt = 10; // Triggers per "burst" (-1)
    uint16_t nb = 3;   // Bursts of triggers (-1)
    uint16_t rmin = 1; // Min randomiser (400ns steps)
    uint16_t rmax = 1; // Max randomiser   "    "
    uint16_t inter = 0; // Between bursts "    "

    // Min readout time for 5 chips is about 4us
    //  (5 * 60bits ... at 80Mbit?)

    // Max (100% occ) is 250us? -> 4kHz

    e->ConfigureVariable(10000 + 0x18, nt);
    e->ConfigureVariable(10000 + 0x19, nb);
    e->ConfigureVariable(10000 + 0x1a, rmin);
    e->ConfigureVariable(10000 + 0x1b, rmax);
    e->ConfigureVariable(10000 + 0x1c, inter);

    e->burst_trtype = 100;
    e->L1A_per_loop = (nt+1) * (nb+1);

    // For ABC130, send L0 and L1
    e->ConfigureVariable(10019, 0x400);

    // Data gen doesn't make the right L0, so nothing to check
    e->burst.pull_event_by_l0 = false;

    e->burst_type = 100;
    e->ExecuteBurst();
}
