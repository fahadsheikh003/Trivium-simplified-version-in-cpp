#include <bitset>
#include <iostream>
#include <string>

using namespace std;

const int LengthOFA = 93;
const int LengthOFB = 84;
const int LengthOFC = 111;

string b2s(string in) {
	string out = "";
	for (int i = 0; i < in.length(); i += 8) {
		bitset<8> bits(in.substr(i, i + 8));
		out += char(bits.to_ulong());
	}
	return out;
}

string s2b(string in) {
	string out = "";
	for (int i = 0; i < in.length(); i++) {
		string temp = "";
		char ch = in[i];
		for (int j = 0; j < 8; j++)
		{
			temp += to_string((ch >> j) & 1);
		}
		string rev = "";
		for (int j = temp.length() - 1; j >= 0; j--) {
			rev += temp[j];
		}
		out += rev;
	}
	return out;
}

// func to generate key stream
bool cycle(bitset<LengthOFA>& A, bitset<LengthOFB>& B, bitset<LengthOFC>& C) {
	bool aout = A[65] ^ (A[90] && A[91]) ^ A[92];
	bool bout = B[68] ^ (B[81] && B[82]) ^ B[83];
	bool cout = C[65] ^ (C[108] && C[109]) ^ C[110];

	// doing left sheft hence index 0 will become free
	A = A << 1;
	B = B << 1;
	C = C << 1;

	bool ain = A[69] ^ cout;
	bool bin = B[78] ^ aout;
	bool cin = C[87] ^ bout;

	A[0] = ain;
	B[0] = bin;
	C[0] = cin;

	return aout ^ bout ^ cout;
}

string encryption_decryption(string key, string iv, string input) {
	bitset<LengthOFA> A(s2b(iv));
	bitset<LengthOFB> B(s2b(key));
	bitset<LengthOFC> C;

	for (int i = LengthOFC - 3; i < LengthOFC; i++) {
		C[i] = 1;
	}

	//warm up
	for (int i = 0; i < 1152; i++) {
		cycle(A, B, C);
	}

	string input_in_bits = s2b(input);
	string output_in_bits = "";

	// perform encryption/decryption
	for (int i = 0; i < input_in_bits.length(); i++) {
		output_in_bits += to_string((input_in_bits[i] - '0') ^ cycle(A, B, C));
	}

	return b2s(output_in_bits);
}

int main() {
	string iv = "1231231231";
	string key = "123123123a";
	string input = "Hello World";
	string ct = encryption_decryption(key, iv, input);
	string pt = encryption_decryption(key, iv, ct);

	cout << "KEY:    " << key << endl;
	cout << "IV:     " << iv << endl;
	cout << "INPUT:  " << input << endl;
	cout << "CIPHER: " << ct << endl;
	cout << "PLAIN : " << pt << endl;
}