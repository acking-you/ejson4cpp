#pragma once
#include <string>
inline bool isValidUTF8(const std::string& str)
{
   int bytes_to_check = 0;

   for (char c : str)
   {
      if (bytes_to_check == 0)
      {
         if ((c & 0b10000000) == 0)
         {
            // Single byte character
            continue;
         }
         else if ((c & 0b11100000) == 0b11000000)
         {
            // Two-byte character
            bytes_to_check = 1;
         }
         else if ((c & 0b11110000) == 0b11100000)
         {
            // Three-byte character
            bytes_to_check = 2;
         }
         else if ((c & 0b11111000) == 0b11110000)
         {
            // Four-byte character
            bytes_to_check = 3;
         }
         else
         {
            return false;   // Invalid UTF-8 sequence
         }
      }
      else
      {
         if ((c & 0b11000000) != 0b10000000)
         {
            return false;   // Invalid UTF-8 sequence
         }
         bytes_to_check--;
      }
   }

   return bytes_to_check == 0;
}