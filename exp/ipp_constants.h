#ifndef __IPP_CONSTANTS_H
#define __IPP_CONSTANTS_H

const unsigned short kIppVersion2 = 0x0200;

#define IPP_PRINTER_IDLE 3

enum {
  IPP_TAG_CUPS_INVALID = -1,            /* Invalid tag name for @link ippTagValue@ */
  IPP_TAG_ZERO = 0x00,                  /* Zero tag - used for separators */
  IPP_TAG_OPERATION,                    /* Operation group */
  IPP_TAG_JOB,                          /* Job group */
  IPP_TAG_END,                          /* End-of-attributes */
  IPP_TAG_PRINTER,                      /* Printer group */
  IPP_TAG_UNSUPPORTED_GROUP,            /* Unsupported attributes group */
  IPP_TAG_SUBSCRIPTION,                 /* Subscription group */
  IPP_TAG_EVENT_NOTIFICATION,           /* Event group */
  IPP_TAG_RESOURCE,                     /* Resource group @private@ */
  IPP_TAG_DOCUMENT,                     /* Document group */
  IPP_TAG_UNSUPPORTED_VALUE = 0x10,     /* Unsupported value */
  IPP_TAG_DEFAULT,                      /* Default value */
  IPP_TAG_UNKNOWN,                      /* Unknown value */
  IPP_TAG_NOVALUE,                      /* No-value value */
  IPP_TAG_NOTSETTABLE = 0x15,           /* Not-settable value */
  IPP_TAG_DELETEATTR,                   /* Delete-attribute value */
  IPP_TAG_ADMINDEFINE,                  /* Admin-defined value */
  IPP_TAG_INTEGER = 0x21,               /* Integer value */
  IPP_TAG_BOOLEAN,                      /* Boolean value */
  IPP_TAG_ENUM,                         /* Enumeration value */
  IPP_TAG_STRING = 0x30,                /* Octet string value */
  IPP_TAG_DATE,                         /* Date/time value */
  IPP_TAG_RESOLUTION,                   /* Resolution value */
  IPP_TAG_RANGE,                        /* Range value */
  IPP_TAG_BEGIN_COLLECTION,             /* Beginning of collection value */
  IPP_TAG_TEXTLANG,                     /* Text-with-language value */
  IPP_TAG_NAMELANG,                     /* Name-with-language value */
  IPP_TAG_END_COLLECTION,               /* End of collection value */
  IPP_TAG_TEXT = 0x41,                  /* Text value */
  IPP_TAG_NAME,                         /* Name value */
  IPP_TAG_RESERVED_STRING,              /* Reserved for future string value @private@ */
  IPP_TAG_KEYWORD,                      /* Keyword value */
  IPP_TAG_URI,                          /* URI value */
  IPP_TAG_URISCHEME,                    /* URI scheme value */
  IPP_TAG_CHARSET,                      /* Character set value */
  IPP_TAG_LANGUAGE,                     /* Language value */
  IPP_TAG_MIMETYPE,                     /* MIME media type value */
  IPP_TAG_MEMBERNAME,                   /* Collection member name value */
  IPP_TAG_EXTENSION = 0x7f,             /* Extension point for 32-bit tags */
  IPP_TAG_MASK = 0x7fffffff,            /* Mask for copied attribute values @private@ */
  /* The following expression is used to avoid compiler warnings with +/-0x80000000 */
  IPP_TAG_COPY = -0x7fffffff-1          /* Bitflag for copied attribute values @private@ */
};

#endif // __IPP_CONSTANTS_H
