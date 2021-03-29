#ifndef __IPP_PARSER_H
#define __IPP_PARSER_H

#include <string>
#include <unordered_map>

#include "ipp_constants.h"

using std::string;
using std::unordered_map;

class IppMainHeader {
 public:
  IppMainHeader() {
    ver_ = op_ = 0;
    request_id_ = 0;
  }

  unsigned short GetVersion() {
    return ver_;
  }

  unsigned short GetOp() {
    return op_;
  }

  unsigned int GetRequestId() {
    return request_id_;
  }

  void SetVersion(unsigned short ver) {
    ver_ = ver;  
  }

  void SetOp(unsigned short op) {
    op_ = op;
  }

  void SetRequestId(unsigned int request_id) {
    request_id_ = request_id;
  }

 private:
  unsigned short ver_;
  unsigned short op_;
  unsigned int request_id_;
};

class IppAttribute {
 public:
  IppAttribute() {
    type_ = IPP_TAG_CUPS_INVALID;
  }

  bool GetType(int* out) {
    if (type_ == IPP_TAG_CUPS_INVALID)
      return false;
    *out = type_;
    return true;
  }

  bool GetName(string* out) {
    switch (type_) {
      case IPP_TAG_CUPS_INVALID:
      case IPP_TAG_END:
        return false;
    }
    out->assign(name_);
    return true;
  }

  bool GetValue(string* out) {
    switch (type_) {
      case IPP_TAG_TEXTLANG:
      case IPP_TAG_NAMELANG:
      case IPP_TAG_TEXT:
      case IPP_TAG_NAME:
      case IPP_TAG_KEYWORD:
      case IPP_TAG_URI:
      case IPP_TAG_URISCHEME:
      case IPP_TAG_CHARSET:
      case IPP_TAG_LANGUAGE:
      case IPP_TAG_MIMETYPE:
      case IPP_TAG_DATE:
      case IPP_TAG_RESOLUTION:
        out->assign(value_string_);
        return true;
    }
    return false;
  }

  bool GetValue(int* out) {
    switch (type_) {
      case IPP_TAG_INTEGER:
      case IPP_TAG_ENUM:
        *out = value_numeric_;
        return true;
    }
    return false;
  }

  bool GetValue(bool* out) {
    if (type_ != IPP_TAG_BOOLEAN)
      return false;
    *out = value_bool_;
    return true;
  }

  bool GetValueRangeLower(int* out) {
    if (type_ != IPP_TAG_RANGE)
      return false;
    return value_range_[0];
  }

  bool GetValueRangeUpper(int* out) {
    if (type_ != IPP_TAG_RANGE)
      return false;
    return value_range_[1];
  }

  bool GetLanguage(string* out) {
    switch (type_) {
      case IPP_TAG_TEXTLANG:
      case IPP_TAG_NAMELANG:
        out->assign(value_language_);
        return true;
    }
    return false;
  }

  void SetType(int type) {
    type_ = type;
  }

  void SetName(const string& name) {
    name_ = name;
  }

  void SetLanguage(const string& language) {
    value_language_ = language;
  }

  void SetValueString(const string& value) {
    value_string_ = value;
  }

  void SetValueNumeric(int value) {
    value_numeric_ = value;
  }

  void SetValueBool(bool value) {
    value_bool_ = value;
  }

  void SetValueRange(int value_lower, int value_upper) {
    value_range_[0] = value_lower;
    value_range_[1] = value_upper;
  }

 private:
  int type_;
  string name_;
  string value_language_;
  string value_string_;
  int value_numeric_;
  bool value_bool_;
  int value_range_[2];
  unsigned char date[11];
};

class IppParser {
 public:
  IppParser(char* input, size_t len);
  ~IppParser();

  bool ParseResponse();
  bool HaveAttribute(const string& name);
  bool GetAttribute(const string& name, IppAttribute* out);

 private:
  bool InputLeft(size_t len);
  void SkipInput(size_t len);
  bool ReadUint8(unsigned char* out);
  bool ReadUint16BE(unsigned short* out);
  bool ReadUint32BE(unsigned int* out);
  bool ReadString(size_t len, string* out);
  bool ParseMainHeader();
  bool ParseTagType(int* out);
  bool ParseName(string* out);
  bool ParseName(IppAttribute* out);
  bool ParseValue(int type, IppAttribute* out);
  bool ParseAttribute(IppAttribute* out);
  bool ParseAttributes();

  const string input_;
  const char* ptr_;
  size_t left_;

  IppMainHeader header_;
  unordered_map<string, IppAttribute> attributes_;
};

#endif // __IPP_PARSER_H
