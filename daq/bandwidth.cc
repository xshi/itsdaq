// -*- C++ -*-
//
// Bandwidth Test for ITSDAQ NexysV board 
//
// Original Author:  SHI Xin <shixin@ihep.ac.cn>
//         Created:  [2017-03-30 Thu 10:37] 
//
// Borrowed code from Matt Warren 
//  


// Functions 
void Matt_DataGenTest(unsigned int dg_mode = 3,
                      unsigned int dg_src =  0,
                      unsigned int strmmode = 0x1,
                      unsigned int first_stream = 0,
                      unsigned int n_streams = 4,
                      unsigned int trigs=0,
                      unsigned int freqid=100,
                      unsigned int bursts=0);

void Matt_DataGenTest(unsigned int dg_mode,
                      unsigned int dg_src,
                      unsigned int strmmode,
                      unsigned int first_stream,
                      unsigned int n_streams,
                      unsigned int trigs,
                      unsigned int freqid,
                      unsigned int bursts)
{
  if (dg_src < 4) {
    e->ConfigureVariable(10036+dg_src, (dg_mode*0x10)+strmmode); 
    Matt_ConfStream( (strmmode*0x100)+((dg_src+4)*2) + 1, first_stream,  n_streams, 0);
    printf("DG Reg %d set to 0x%x\n", 36+dg_src, (dg_mode*0x10)+strmmode);

  } else {
    printf("DG source too large!!!!\n");
  }
  
  //Internal Trigger
  if (trigs > 0) {
    Matt_TrigBurst(freqid, trigs, bursts);
    e->Sleep(freqid*trigs*(bursts+1)/100);
  }
  else {
    //External trigger
    e->ConfigureVariable(10000, 0x0001);     // PMOD-TTC trig input enable (TRIG0) 
    e-> Sleep(1);
    e->ConfigureVariable(10000, 0x0000);     // PMOD-TTC trig input disable (TRIG0) 
  }
  Matt_ConfStream( (strmmode*0x100) + ((dg_src+4)*2) + 0, first_stream,  n_streams, 0);

}
