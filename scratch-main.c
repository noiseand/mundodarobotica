void makeScratchPacket(int NoCanal);
void sendScratchPacket(int NoCanal, long ValorSensor);

void makeScratchPacket(int NoCanal) {

   set_adc_channel(NoCanal);
   delay_us(channelSwitchDelay);
   long ValorSensor;
   ValorSensor=read_adc();
   sendScratchPacket(NoCanal, ValorSensor);

}

void sendScratchPacket(int NoCanal, long ValorSensor) {

   // 0="A", 1="B", 2="C", 3="D",
   // 4="Slider", 5="Light", 6="Sound", 7="Button"

   //long ValorSensor;

   output_high(RUN_LED);
   
   //printf(usb_cdc_putc,"%c", (0b10000000 | (0b0111 << 3) | (0b0011101010 >> 7))); //ok //B9
   printf(usb_cdc_putc,"%c", (0b10000000 | ((NoCanal & 0b1111) << 3) | (((ValorSensor & 0b1111111111) >> 7) & 0b111)));
   delay_us(400);
   //printf(usb_cdc_putc,"%c", (0b0011101010 & 0x7F)); //ok // 6A
   printf(usb_cdc_putc,"%c", ((ValorSensor & 0b1111111111) & 0x7F));

   output_low(RUN_LED);
   
   delay_us(400);

}

//switch (CMD_STATE) {
               case CMD_SCRATCH_DATA_REQUEST:
                  //sendScratchPacket();
                  //makeScratchPacket();
                  int s;
                  for(s=0;s<8;s++) makeScratchPacket(s);
                  break;
};
