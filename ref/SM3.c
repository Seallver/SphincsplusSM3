#include"SM3.h"

array<uint32_t, 8> LV = { 0x7380166f,0x4914b2b9,0x172442d7,0xda8a0600,0xa96f30bc,0x163138aa,0xe38dee4d,0xb0fb0e4e };

array<uint32_t, 2> const_T = { 0x79cc4519 ,0x7a879d8a };

uint32_t SM3::shift_left(uint32_t value, int shift) {
	return (value << shift) | (value >> (32 - shift));
}

uint32_t SM3::P(uint32_t value) {
	return value ^ shift_left(value, 15) ^ shift_left(value, 23);
}

uint32_t SM3::FF(uint32_t value1, uint32_t value2, uint32_t value3, size_t index) {
	if (index < 16)
		return value1 ^ value2 ^ value3;
	else
		return (value1 & value2) | (value1 & value3) | (value2 & value3);
}

uint32_t SM3::GG(uint32_t value1, uint32_t value2, uint32_t value3, size_t index) {
	if (index < 16)
		return value1 ^ value2 ^ value3;
	else
		return (value1 & value2) | ((~value1) & value3);
}

uint32_t SM3::P_(uint32_t value) {
	return value ^ shift_left(value, 9) ^ shift_left(value, 17);
}

void SM3::CF() {
	array<uint32_t, 68> W;
	for (size_t i = 0;i < 16;i++)
		W[i] = Mess[i];
	for (size_t i = 16;i < 68;i++)
		W[i] = P(W[i - 16] ^ W[i - 9] ^ shift_left(W[i - 3], 15)) ^ shift_left(W[i - 13], 7) ^ W[i - 6];

	array<uint32_t, 64> W_;
	for (size_t i = 0;i < 64;i++)
		W_[i] = W[i] ^ W[i + 4];

	uint32_t SS, SS_, TT, TT_;
	for (size_t i = 0;i < 64;i++) {
		SS = shift_left((shift_left(Hash[0], 12) + Hash[4] + shift_left(const_T[i >= 16], i)), 7);
		SS_ = SS ^ shift_left(Hash[0], 12);
		TT = FF(Hash[0], Hash[1], Hash[2], i) + Hash[3] + SS_ + W_[i];
		TT_ = GG(Hash[4], Hash[5], Hash[6], i) + Hash[7] + SS + W[i];
		Hash[3] = Hash[2];
		Hash[2] = shift_left(Hash[1], 9);
		Hash[1] = Hash[0];
		Hash[0] = TT;
		Hash[7] = shift_left(Hash[6], 19);
		Hash[6] = Hash[5];
		Hash[5] = P_(TT_);
	}
}

SM3::SM3() {
	for (size_t i = 0;i < 16;i++)
		Mess[i] = 0;
	for (size_t i = 0;i < 8;i++)
		Hash[i] = LV[i];
};

SM3::SM3(array<uint32_t, 12> mess) {
	uint32_t padding[4] = { 0x80000000, 0x00000000, 0x00000000, 0x180 };
	for (size_t i = 0;i < 12;i++) {
		Mess[i] = mess[i];
	}
	for (size_t i = 12;i < 16;i++) {
		Mess[i] = padding[i - 12];
	}
	for (size_t i = 0;i < 8;i++)
		Hash[i] = LV[i];
};

array<uint32_t, 8> SM3::retHash() {
	CF();
	return Hash;
}

void SM3::reset(array<uint32_t, 12> Mess_) {
	for (size_t i = 0;i < 12;i++)
		Mess[i] = Mess_[i];
	for (size_t i = 0;i < 8;i++)
		Hash[i] = LV[i];

}