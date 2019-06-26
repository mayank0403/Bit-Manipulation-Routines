#include <iostream>
#include <stdlib.h>

using namespace std;

void bit_manip_inside_byte(int bit /*0/1*/, int location, void* number){
	if(location >= 8){
		cout<<"Cannot set to farther than 8th bit"<<endl;
		return;
	}
	if(bit<0 || bit>1){
		cout<<"Bit cannot be anything other than 0/1"<<endl;
		return;
	}
	if(bit == 1){
		// setting the bit
		*((unsigned long*)number) |= 1UL << location;
	}
	else if(bit == 0){
		// clearing the bit
		*((unsigned long*)number) &= ~(1UL << location); //~ is bitwise NOT.
	}
}

//Print from LSB to MSB
void print_bits_inside_byte(void* number, bool new_line = false){
	for(int i=0; i<8; i++){
		int bit = ((*((unsigned long*)number)) >> i) & 1UL; 
		cout<<bit<<" ";
	}
	if(new_line)
		cout<<" | ";
	else
		cout<<endl;
}

//This will print bits of all 4 bytes at a mem location
void print_bits_long_type(void* number, bool is_float = false){
	if(is_float){
		if(__BYTE_ORDER == __LITTLE_ENDIAN){
			cout<<"Priting floats in little endian format"<<endl;
		}
		else{
			cout<<"TODO: Printing floats in big endian format"<<endl;
			return;
		}
		cout<<" | man (reverse of expected) => ";
		for(int i=0; i<23; i++){
			int bit = ((*((unsigned long*)number)) >> i) & 1UL; 
			cout<<bit<<" ";
		}
		cout<<" | exp (parsed normally, no rev)=> ";
		for(int i=0; i<8; i++){
			int bit = ((*((unsigned long*)number)) >> i) & 1UL; 
			cout<<bit<<" ";
		}
		cout<<" | sign => ";
		for(int i=0; i<1; i++){
			int bit = ((*((unsigned long*)number)) >> i) & 1UL; 
			cout<<bit<<" ";
		}
		cout<<" |"<<endl;

		return;
	}
	for(int i=0; i<4; i++){
		print_bits_inside_byte(number+i, true);
	}
	cout<<endl;
}

//Set/Unset nth bit (of 32)
void bit_manip_long_type(int bit, int location, void* number){
	int chunk_num = location/8;
	int relative_location = location%8;
	bit_manip_inside_byte(bit, relative_location, number+chunk_num);
}

//Returns int 0/1 as the bit.
int extract_bit_from_byte(int position, void* number){
	return ((*((unsigned long*)number)) >> position) & 1UL;  	
}

int extract_bit_from_long_type(int location, void* number){
	int chunk_num = location/8;
	int relative_location = location%8;
	return extract_bit_from_byte(relative_location, number+chunk_num);
}

//Starting from LSB, copy x no. of bits from <from> to <to>.
void set_x_bits_to(int x, int start_bit_pos /*bit offset to start from in <to>*/, void* from, void* to){
	for(int i=start_bit_pos; i<(x+start_bit_pos); i++){
		bit_manip_long_type(extract_bit_from_long_type(i-start_bit_pos, from), i, to);
	}
}

void set_x_bits_to_little_endian_float(int x, void* from, void* to){
	for(int i=x-1; i>=0; i--){
		bit_manip_long_type(extract_bit_from_long_type(x-1-i, from), i, to);
	}
}

//Generate a float value from integer representations of mantissa and exponent.
void gen_float_from_man_exp(int sign /*1 -ve*/, int exponent /*<2pow8-- actual exponent = exponent-127*/, int mantissa /*<2pow23*/, void* number){
	if(__BYTE_ORDER == __BIG_ENDIAN){
		cout<<"BIG ENDIAN"<<endl;
		bit_manip_long_type(sign, 0, number); //Set sign.
		set_x_bits_to(8, 1, &exponent, number);
		set_x_bits_to(23, 9, &mantissa, number);
	}
	else if(__BYTE_ORDER == LITTLE_ENDIAN){
		cout<<"LITTLE ENDIAN"<<endl;
		set_x_bits_to_little_endian_float(23, &mantissa, number); //parsed in little endian form.
		set_x_bits_to(8, 23, &exponent, number);
		bit_manip_long_type(sign, 31, number); //Set sign.
	}
}

void gen_random_valid_float(){

}

int main(){
	void* float_mem = malloc(4); //4 bytes -- size of float
	//print_bits_long_type(float_mem);
	//bit_manip_inside_byte(1, 0, float_mem);
	//bit_manip_inside_byte(1, 1, float_mem);
	//bit_manip_long_type(1, 31, float_mem);
	//print_bits_long_type(float_mem);
	//cout<<*((int*)float_mem)<<endl;
	gen_float_from_man_exp(0, 127, 3, float_mem);
	cout<<*((float*)float_mem)<<endl;
	print_bits_long_type(float_mem, true);
}
