#define         MASKBITS                0x3F
#define         MASKBYTE                0x80
#define         MASK2BYTES              0xC0
#define         MASK3BYTES              0xE0
#define         MASK4BYTES              0xF0
#define         MASK5BYTES              0xF8
#define         MASK6BYTES              0xFC

typedef unsigned char byte;

//http://www.codeguru.com/cpp/misc/misc/multi-lingualsupport/article.php/c10451
void UTF8Encode4BytesUnicode(std::vector< unsigned > input,
                             std::vector< byte >& output)
{
   for(unsigned int i=0; i < input.size(); i++)
   {
      // 0xxxxxxx
      if(input[i] < 0x80)
      {
         output.push_back((byte)input[i]);
      }
      // 110xxxxx 10xxxxxx
      else if(input[i] < 0x800)
      {
         output.push_back((byte)(MASK2BYTES | input[i] >> 6));
         output.push_back((byte)(MASKBYTE | (input[i] & MASKBITS)));
      }
      // 1110xxxx 10xxxxxx 10xxxxxx
      else if(input[i] < 0x10000)
      {
         output.push_back((byte)(MASK3BYTES | input[i] >> 12));
         output.push_back((byte)(MASKBYTE | ((input[i] >> 6) & MASKBITS)));
         output.push_back((byte)(MASKBYTE | (input[i] & MASKBITS)));
      }
      // 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
      else if(input[i] < 0x200000)
      {
         output.push_back((byte)(MASK4BYTES | input[i] >> 18));
         output.push_back((byte)(MASKBYTE | ((input[i] >> 12) & MASKBITS)));
         output.push_back((byte)(MASKBYTE | ((input[i] >> 6) & MASKBITS)));
         output.push_back((byte)(MASKBYTE | (input[i] & MASKBITS)));
      }
      // 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
      else if(input[i] < 0x4000000)
      {
         output.push_back((byte)(MASK5BYTES | input[i] >> 24));
         output.push_back((byte)(MASKBYTE | ((input[i] >> 18) & MASKBITS)));
         output.push_back((byte)(MASKBYTE | ((input[i] >> 12) & MASKBITS)));
         output.push_back((byte)(MASKBYTE | ((input[i] >> 6) & MASKBITS)));
         output.push_back((byte)(MASKBYTE | (input[i] & MASKBITS)));
      }
      // 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
      else if(input[i] < 0x8000000)
      {
         output.push_back((byte)(MASK6BYTES | input[i] >> 30));
         output.push_back((byte)(MASKBYTE | ((input[i] >> 24) & MASKBITS)));
         output.push_back((byte)(MASKBYTE | ((input[i] >> 18) & MASKBITS)));
         output.push_back((byte)(MASKBYTE | ((input[i] >> 12) & MASKBITS)));
         output.push_back((byte)(MASKBYTE | ((input[i] >> 6) & MASKBITS)));
         output.push_back((byte)(MASKBYTE | (input[i] & MASKBITS)));
      }
   }
}

string ftw(std::vector< unsigned > input)
{
    string res;
    std::vector< byte > output;
    UTF8Encode4BytesUnicode(input, output);
    for(unsigned int i=0; i<output.size(); i++)
	res.append(1, output[i]);
    return res;
}
