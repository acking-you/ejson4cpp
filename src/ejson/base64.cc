#include "base64.h"

#include <string>
#include <vector>

const ejson::string_view BASE64_CHARS =
  "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

std::string ejson::base64_encode(ejson::string_view const &input)
{
   std::string           output;
   std::vector<uint32_t> bytes;
   for (char c : input) { bytes.push_back(static_cast<uint8_t>(c)); }

   uint8_t padding = bytes.size() % 3;
   if (padding > 0)
   {
      for (int i = 0; i < 3 - padding; i++) { bytes.push_back(0); }
   }

   for (int i = 0; i < bytes.size(); i += 3)
   {
      uint32_t triplet = (bytes[i] << 16) | (bytes[i + 1] << 8) | bytes[i + 2];
      output += BASE64_CHARS[(triplet >> 18) & 0x3F];
      output += BASE64_CHARS[(triplet >> 12) & 0x3F];
      output += BASE64_CHARS[(triplet >> 6) & 0x3F];
      output += BASE64_CHARS[triplet & 0x3F];
   }

   if (padding > 0)
   {
      output[output.size() - 1] = '=';
      if (padding == 1) { output[output.size() - 2] = '='; }
   }

   return output;
}

std::string ejson::base64_decode(ejson::string_view const &input)
{
   if (input.size() % 4 != 0) return "";

   std::string           output;
   std::vector<uint32_t> bytes;
   uint8_t               padding = 0;
   for (char c : input)
   {
      if (padding > 2) { return ""; }

      if (c == '=')
      {
         ++padding;
         bytes.push_back(0);
         continue;
      }

      size_t idx = BASE64_CHARS.find(c);
      if (idx == std::string::npos) { return ""; }
      bytes.push_back(static_cast<uint32_t>(idx));
   }

   for (int i = 0; i < bytes.size(); i += 4)
   {
      uint32_t triplet = (bytes[i] << 18) | (bytes[i + 1] << 12) |
                         (bytes[i + 2] << 6) | bytes[i + 3];
      output += static_cast<char>((triplet >> 16) & 0xFF);
      output += static_cast<char>((triplet >> 8) & 0xFF);
      output += static_cast<char>(triplet & 0xFF);
   }

   if (padding > 0) { output.erase(output.size() - padding); }

   return output;
}
