/*
Reference https://help.twitter.com/en/managing-your-account/twitter-username-rules
*/
					// _    CAPS		 not caps 	numbers 0-9
#define RADIX 63	// 1 + (1+'Z'-'A') + (1+'z' - 'a') + 10;

#include <string>
#include <iostream>
#include <algorithm>

using namespace std;

// Maximum number of character 15. 63 is the range. so 63^15 <= 64^15 <= (2^6)^15 <= 2^90
// It will use a substring so it can fit into 64 bits. So how many characters? (2^6)^x => 6*x <=64 => x => 10.6
unsigned long long twitter_string_encode(string &s) {
	unsigned long long e = 0;
	unsigned char c;

	for (int i=0; i<min(int(s.size()),10); i++) {
		if (s[i]<='9' && s[i]>='0')
			c = s[i] - '0';
		else if (s[i]<='Z' && s[i]>='A')
			c = s[i] - 'A' + 10;
		else if (s[i]=='_')
			c = s[i] - '_' + 10 + 'Z'-'A' + 1;
		else if (s[i]<='z' && s[i]>='a')
			c = s[i] - 'a' + 10 + 'Z'-'A' + 1 + 1;
		else {
			cout << "Character is not valid: " << s[i] << endl;
			return -1;
		}

		e = c + RADIX*e;
	}

	return e;
}