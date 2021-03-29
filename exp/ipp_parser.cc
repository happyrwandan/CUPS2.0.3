#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <string>
#include <unordered_map>

#include "ipp_constants.h"

#include "ipp_parser.h"

IppParser::IppParser(char* input, size_t len)
    : input_(input, len) {
  ptr_ = input_.c_str();
  left_ = len;
}

IppParser::~IppParser() {
}

bool IppParser::InputLeft(size_t len) {
  return (left_ >= len);
}

void IppParser::SkipInput(size_t len) {
  assert(left_ >= len);
  left_ -= len;
  ptr_ += len;
}

bool IppParser::ReadUint8(unsigned char* out) {
  if (!InputLeft(1))
    return false;
  if (out)
    *out = *ptr_;
  SkipInput(1);
  return true;
}

bool IppParser::ReadUint16BE(unsigned short* out) {
  if (!InputLeft(2))
    return false;
  if (out) {
    *out = ((ptr_[0] & 0xff) << 8) | (ptr_[1] & 0xff);
  }
  SkipInput(2);
  return true;
}

bool IppParser::ReadUint32BE(unsigned int* out) {
  if (!InputLeft(4))
    return false;
  if (out) {
    *out = ((ptr_[0] & 0xff) << 24) | ((ptr_[1] & 0xff) << 16) | ((ptr_[2] & 0xff) << 8) | (ptr_[3] & 0xff);
  }
  SkipInput(4);
  return true;
}

bool IppParser::ReadString(size_t len, string* out) {
  if (!InputLeft(len))
    return false;
  if (out) {
    out->assign(ptr_, len);
  }
  SkipInput(len);
  return true;
}

bool IppParser::ParseMainHeader() {
  unsigned short ver, op;
  unsigned int request_id;
  if (!ReadUint16BE(&ver))
    return false;
  header_.SetVersion(ver);

  if (!ReadUint16BE(&op))
    return false;
  header_.SetOp(op);

  if (!ReadUint32BE(&request_id))
    return false;
  header_.SetRequestId(request_id);

  return true;
}

bool IppParser::ParseTagType(int* out) {
  int tag = 0;

  if (!ReadUint8(reinterpret_cast<unsigned char*>(&tag)))
    return false;

  if (tag == IPP_TAG_EXTENSION) {
    if (!ReadUint32BE(reinterpret_cast<unsigned int*>(&tag)))
      return false;
  }

  if (out)
    *out = tag;

  return true;
}

bool IppParser::ParseName(string* out) {
  unsigned short len;
  if (!ReadUint16BE(&len))
    return false;

  if (len == 0)
    return true;

  if (!InputLeft(len))
    return false;

  if (out)
    out->assign(ptr_, len);

  SkipInput(len);
  return true;
}

bool IppParser::ParseName(IppAttribute* out) {
  string name;
  if (!ParseName(&name))
    return false;
  if (out)
    out->SetName(name);
  return true;
}

bool IppParser::ParseValue(int type, IppAttribute* out) {
  unsigned short len;
  if (!ReadUint16BE(&len))
    return false;

  if (!InputLeft(len))
    return false;

  switch (type) {
    case IPP_TAG_INTEGER:
    case IPP_TAG_ENUM:
      {
        if (len != 4)
          return false;
        unsigned int value;
        if (!ReadUint32BE(&value))
          return false;
        if (out)
          out->SetValueNumeric(value);
        return true;
      }
    case IPP_TAG_BOOLEAN:
      {
        if (len != 1)
          return false;
        unsigned char x;
        if (!ReadUint8(&x))
          return false;
        if (out)
          out->SetValueBool(x != 0);
        return true;
      }
    case IPP_TAG_TEXT:
    case IPP_TAG_NAME:
    case IPP_TAG_KEYWORD:
    case IPP_TAG_URI:
    case IPP_TAG_URISCHEME:
    case IPP_TAG_CHARSET:
    case IPP_TAG_LANGUAGE:
    case IPP_TAG_MIMETYPE:
      {
        string value;
        if (!ReadString(len, &value))
          return false;
        if (out)
          out->SetValueString(value);
        return true;
      }
    case IPP_TAG_TEXTLANG:
    case IPP_TAG_NAMELANG:
      {
        string language;
        if (!ReadString(len, &language))
          return false;
        if (out)
          out->SetLanguage(language);
        if (!ReadUint16BE(&len))
          return false;
        string value;
        if (!ReadString(len, &value))
          return false;
        if (out)
          out->SetValueString(value);
        return true;
      }
    case IPP_TAG_RANGE:
      {
        if (len != 8)
          return false;
        int lower, upper;
        if (!ReadUint32BE(reinterpret_cast<unsigned int *>(&lower)) ||
            !ReadUint32BE(reinterpret_cast<unsigned int *>(&upper)))
          return false;
        if (out)
          out->SetValueRange(lower, upper);
        return true;
      }
    case IPP_TAG_DATE:
      {
        if (len != 11)
          return false;

        SkipInput(len);
        return true;
      }
  }

  SkipInput(len);
  return true;
}

bool IppParser::ParseAttribute(IppAttribute* out) {
 int tag;

 if (!ParseTagType(&tag))
   return false;

 if (out)
   out->SetType(tag);

 if (tag < IPP_TAG_UNSUPPORTED_VALUE)
   return true;

 if (!ParseName(out))
   return false;

 if (!ParseValue(tag, out))
   return false;

 return true;
}

bool IppParser::ParseAttributes() {
  while (1) {
    IppAttribute attr;

    if (!ParseAttribute(&attr))
      return false;

    int type;
    if (!attr.GetType(&type))
      return false;

    if (type == IPP_TAG_END)
      break;

    if (type < IPP_TAG_UNSUPPORTED_VALUE)
      continue;

    string name;
    if (!attr.GetName(&name))
      return false;

    attributes_[name] = attr;
  }
  return true;
}

bool IppParser::ParseResponse() {
  if (!ParseMainHeader())
    return false;

  if (header_.GetVersion() != kIppVersion2)
    return false;

  if (header_.GetOp() != 0)
    return false;

  if (!ParseAttributes())
    return false;
  return true;
}

bool IppParser::HaveAttribute(const string& name) {
  if (attributes_.find(name) != attributes_.end())
    return true;
  return false;
}

bool IppParser::GetAttribute(const string& name, IppAttribute* out) {
  auto it = attributes_.find(name);
  if (it == attributes_.end())
    return false;
  if (out)
    *out = it->second;
  return true;
}
