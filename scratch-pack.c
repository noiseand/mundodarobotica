oid sendScratchPacket() {

   // 0="A", 1="B", 2="C", 3="D",
   // 4="Slider", 5="Light", 6="Sound", 7="Button"

   output_high(RUN_LED);

   printf(usb_cdc_putc,"%c", (0b10000000 | (0b0111 << 3) | (0b0011101010 >> 7))); //ok //B9
   delay_us(400);
   printf(usb_cdc_putc,"%c", (0b0011101010 & 0x7F)); //ok // 6A

   output_low(RUN_LED);

}
