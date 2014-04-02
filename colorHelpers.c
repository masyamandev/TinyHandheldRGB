// Color palette:
// 0b00BBGGRR - 2 bits / channel
// 0b01hhhBGR - 3 bits brightness (hhh), 1 bit / channel


const uint8_t CHANNEL23[4] = {0b000, 0b001, 0b011, 0b111};
inline uint8_t colorChannel2to3bits(uint8_t chan2) {
	return CHANNEL23[chan2];
}
//const uint8_t CHANNEL24[4] = {0b0000, 0b0001, 0b0011, 0b111};
//inline uint8_t colorChannel2to3bits(uint8_t chan2) {
//	return CHANNEL23[chan2];
//}

//const uint8_t RGB1[8] = {0b01000000, 0b01000000, 0b01000000, 0b01000000, 0b01000000, 0b01000000, 0b01000000, 0b01000000};
//inline uint8_t colorRgb1to3bits(uint8_t rgb1) {
//	return CHANNEL23[chan2];
//}
