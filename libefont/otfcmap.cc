// -*- related-file-name: "../include/efont/otfcmap.hh" -*-
#ifdef HAVE_CONFIG_H
# include <config.h>
#endif
#include <efont/otfcmap.hh>
#include <lcdf/error.hh>
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <netinet/in.h>		// for ntohl()

#define USHORT_AT(d)		(ntohs(*(const uint16_t *)(d)))
#define SHORT_AT(d)		((int16_t) ntohs(*(const uint16_t *)(d)))
#define ULONG_AT(d)		(ntohl(*(const uint32_t *)(d)))

namespace Efont {

OpenType_cmap::OpenType_cmap(const String &s, ErrorHandler *errh)
    : _str(s)
{
    _str.align(4);
    _error = parse_header(errh ? errh : ErrorHandler::silent_handler());
}

int
OpenType_cmap::parse_header(ErrorHandler *errh)
{
    // HEADER FORMAT:
    // USHORT	version
    // USHORT	numTables
    int len = _str.length();
    const uint8_t *data = _str.udata();
    if (HEADER_SIZE > len)
	return errh->error("OTF cmap too small for header"), -EFAULT;
    if (!(data[0] == '\000' && data[1] == '\000'))
	return errh->error("bad cmap version number"), -ERANGE;
    _ntables = USHORT_AT(data + 2);
    if (_ntables == 0)
	return errh->error("OTF cmap contains no tables"), -EINVAL;
    if (HEADER_SIZE + ENCODING_SIZE * _ntables > len)
	return errh->error("cmap directory out of range"), -EFAULT;

    // ENCODING ENTRY FORMAT:
    // USHORT	platformID
    // USHORT	encodingID
    // ULONG	offset
    int last_platform = -1;
    int last_encoding = -1;
    int last_version = -1;
    _first_unicode_table = -1;
     for (int i = 0; i < _ntables; i++) {
	int loc = HEADER_SIZE + ENCODING_SIZE * i;
	int platform = USHORT_AT(data + loc);
	int encoding = USHORT_AT(data + loc + 2);
	uint32_t offset = ULONG_AT(data + loc + 4);
	if (offset + 2 > (uint32_t) len)
	    return errh->error("encoding data for entry %d out of range", i);
	int version = USHORT_AT(data + offset);
	if (!(platform > last_platform
	      || (platform == last_platform
		  && (encoding > last_encoding
		      || (encoding == last_encoding
			  && version > last_version)))))
	    return errh->error("subtables out of order at entry %d", i);
	if ((platform == 0 || (platform == 3 && encoding == 1))
	    && _first_unicode_table < 0)
	    _first_unicode_table = i;
	last_platform = platform, last_encoding = encoding, last_version = version;
    }

    _table_error.assign(_ntables, -2);
    return 0;
}

int
OpenType_cmap::first_table(int platform, int encoding) const
{
    if (error() < 0)
	return -1;
    const uint8_t *data = _str.udata();
    data += HEADER_SIZE;
    for (int i = 0; i < _ntables; i++, data += ENCODING_SIZE) {
	int p = USHORT_AT(data), e = USHORT_AT(data + 2);
	if (platform == p && (encoding < 0 || encoding == e))
	    return i;
    }
    return -1;
}

int
OpenType_cmap::check_table(int t, ErrorHandler *errh) const
{
    if (!errh)
	errh = ErrorHandler::silent_handler();
    if (_error < 0 || t < 0 || t >= _ntables)
	return errh->error("no such table");
    if (_table_error[t] > -2)
	return _table_error[t];
    _table_error[t] = -1;
    
    const uint8_t *data = _str.udata();
    uint32_t offset = ULONG_AT(data + HEADER_SIZE + t * ENCODING_SIZE + 4);
    uint32_t left = _str.length() - offset;
    data += offset;
    int format = USHORT_AT(data);
    uint32_t length = 0;	// value not used
    
    switch (format) {
	
      case F_BYTE:
	if (left < 4
	    || (length = USHORT_AT(data + 2)) > left
	    || length != 259)
	    return errh->error("bad table %d length (format %d)", t, format);
	break;
	
      case F_HIBYTE:
	if (left < 4
	    || (length = USHORT_AT(data + 2)) > left
	    || length < 524)
	    return errh->error("bad table %d length (format %d)", t, format);
	for (int hi_byte = 0; hi_byte < 256; hi_byte++)
	    if (uint32_t subh_key = USHORT_AT(data + 6 + 2 * hi_byte)) {
		if ((subh_key & 7) || HIBYTE_SUBHEADERS + subh_key + 8 > length)
		    return errh->error("bad table %d subheader %d offset (format 2)", t, hi_byte);
		const uint8_t *subh = data + HIBYTE_SUBHEADERS + subh_key;
		int firstCode = USHORT_AT(subh);
		int entryCount = USHORT_AT(subh + 2);
		int idRangeOffset = USHORT_AT(subh + 6);
		if (firstCode + entryCount > 256 || entryCount == 0)
		    return errh->error("bad table %d subheader %d contents (format 2)", t, hi_byte);
		if ((HIBYTE_SUBHEADERS + subh_key + 6) // pos[idRangeOffset]
		    + idRangeOffset + entryCount * 2 > length)
		    return errh->error("bad table %d subheader %d length (format 2)", t, hi_byte);
	    }
	break;
	
      case F_SEGMENTED: {
	  if (left < 4
	      || (length = USHORT_AT(data + 2)) > left
	      || length < 16)
	      return errh->error("bad table %d length (format %d)", t, format);
	  int segCountX2 = USHORT_AT(data + 6);
	  int searchRange = USHORT_AT(data + 8);
	  int entrySelector = USHORT_AT(data + 10);
	  int rangeShift = USHORT_AT(data + 12);
	  if ((segCountX2 & 1)
	      || segCountX2 == 0
	      || (searchRange & (searchRange - 1)) /* not a power of 2? */
	      || searchRange <= segCountX2/2
	      || (searchRange>>1) > segCountX2/2
	      || 1 << (entrySelector + 1) != searchRange
	      || rangeShift != segCountX2 - searchRange)
	      return errh->error("bad table %d segment counts (format 4)", t);
	  uint32_t segCount = segCountX2 >> 1;
	  if (length < 16 + 8 * segCount)
	      return errh->error("bad table %d length (format %d)", t, format);
	  const uint8_t *endCodes = data + 14;
	  const uint8_t *startCodes = endCodes + 2 + segCountX2;
	  const uint8_t *idDeltas = startCodes + segCountX2;
	  const uint8_t *idRangeOffsets = idDeltas + segCountX2;
	  int last_end = 0;
	  for (int i = 0; i < segCountX2; i += 2) {
	      int endCode = USHORT_AT(endCodes + i);
	      int startCode = USHORT_AT(startCodes + i);
	      /* int idDelta = SHORT_AT(idDeltas + i); // no need to check */
	      int idRangeOffset = USHORT_AT(idRangeOffsets + i);
	      if (endCode < startCode || startCode < last_end)
		  return errh->error("bad table %d overlapping range %d (format %d)", t, i/2, format);
	      if (idRangeOffset
		  && idRangeOffsets + i + idRangeOffset + (endCode - startCode)*2 + 2 > data + length)
		  return errh->error("bad table %d range %d length (format 4)", t, i/2);
	      last_end = endCode + 1;
	  }
	  if (USHORT_AT(endCodes + segCountX2 - 2) != 0xFFFF)
	      return errh->error("bad table %d incorrect final endCode (format 4)", t);
	  break;
      }
	
      case F_TRIMMED: {
	  if (left < 4
	      || (length = USHORT_AT(data + 2)) > left
	      || length < 10)
	      return errh->error("bad table %d length (format %d)", t, format);
	  uint32_t entryCount = USHORT_AT(data + 8);
	  if (10 + entryCount * 2 > length)
	      return errh->error("bad table %d length (format %d)", t, format);
	  break;
      }

      case F_SEGMENTED32: {
	  if (left < 8
	      || (length = ULONG_AT(data + 4)) > left
	      || length < 16)
	      return errh->error("bad table %d length (format %d)", t, format);
	  uint32_t nGroups = ULONG_AT(data + 16);
	  if ((length - 16) / 12 < nGroups)
	      return errh->error("bad table %d length (format %d)", t, format);
	  uint32_t last_post_end = 0;
	  data += 16;
	  for (uint32_t i = 0; i < nGroups; i++, data += 12) {
	      uint32_t startCharCode = ULONG_AT(data);
	      uint32_t endCharCode = ULONG_AT(data + 4);
	      if (startCharCode < last_post_end || endCharCode < startCharCode)
		  return errh->error("bad table %d overlapping range %d (format %d)", t, i, format);
	      last_post_end = endCharCode + 1;
	  }
	  break;
      }

      case F_HIBYTE32:
      case F_TRIMMED32:
      default:
	return errh->error("bad table %d unsupported format %d", t, format);
	
    }

    _table_error[t] = 0;
    return 0;
}

OpenTypeGlyph
OpenType_cmap::map_table(int t, uint32_t uni, ErrorHandler *errh) const
{
    if (check_table(t, errh) < 0)
	return 0;
    
    const uint8_t *data = _str.udata();
    data += ULONG_AT(data + HEADER_SIZE + t * ENCODING_SIZE + 4);
    
    switch (USHORT_AT(data)) {
	
      case F_BYTE:
	if (uni < 256)
	    return data[6 + uni];
	else
	    return 0;
	
      case F_HIBYTE: {
	  if (uni >= 65536)
	      return 0;
	  int hi_byte = (uni >> 8) & 255;
	  int subh = USHORT_AT(data + 6 + hi_byte * 2);
	  if (subh == 0 && hi_byte)
	      return 0;
	  data += 524 + subh;
	  int firstCode = USHORT_AT(data);
	  int entryCount = USHORT_AT(data + 2);
	  int idDelta = SHORT_AT(data + 4);
	  int idRangeOffset = USHORT_AT(data + 6);
	  int lo_byte = uni & 255;
	  if (lo_byte < firstCode || lo_byte >= firstCode + entryCount)
	      return 0;
	  int answer = USHORT_AT(data + 6 + idRangeOffset + (lo_byte - firstCode) * 2);
	  if (answer == 0)
	      return 0;
	  return (answer + idDelta) & 65535;
      }
	
      case F_SEGMENTED: {
	  if (uni >= 65536)
	      return 0;
	  int segCount = USHORT_AT(data + 6) >> 1;
	  const uint8_t *endCounts = data + 14;
	  const uint8_t *startCounts = endCounts + (segCount << 1) + 2;
	  const uint8_t *idDeltas = startCounts + (segCount << 1);
	  const uint8_t *idRangeOffsets = idDeltas + (segCount << 1);
	  int l = 0, r = segCount - 1;
	  while (l <= r) {
	      int m = (l + r) / 2;
	      uint32_t endCount = USHORT_AT(endCounts + (m << 1));
	      uint32_t startCount = USHORT_AT(startCounts + (m << 1));
	      if (uni < startCount)
		  r = l - 1;
	      else if (uni <= endCount) {
		  int idDelta = SHORT_AT(idDeltas + (m << 1));
		  int idRangeOffset = USHORT_AT(idRangeOffsets + (m << 1));
		  if (idRangeOffset == 0)
		      return (idDelta + uni) & 65535;
		  int g = USHORT_AT(idRangeOffsets + (m << 1) + idRangeOffset + ((uni - startCount) << 1));
		  if (g == 0)
		      return 0;
		  return (idDelta + g) & 65535;
	      } else
		  l = r + 1;
	  }
	  return 0;
      }

      case F_TRIMMED: {
	  uint32_t firstCode = USHORT_AT(data + 6);
	  uint32_t entryCount = USHORT_AT(data + 8);
	  if (uni < firstCode || uni >= firstCode + entryCount)
	      return 0;
	  return USHORT_AT(data + 10 + ((uni - firstCode) << 1));
      }
	
      case F_SEGMENTED32: {
	  uint32_t nGroups = ULONG_AT(data + 12);
	  uint32_t l = 0, r = nGroups - 1;
	  const uint8_t *groups = data + 16;
	  while (l <= r) {
	      uint32_t m = (l + r) / 2;
	      uint32_t startCharCode = ULONG_AT(groups + m * 12);
	      uint32_t endCharCode = ULONG_AT(groups + m * 12 + 4);
	      if (uni < startCharCode)
		  r = l - 1;
	      else if (uni <= endCharCode)
		  return ULONG_AT(groups + m * 12 + 8) + uni - startCharCode;
	      else
		  l = r + 1;
	  }
	  return 0;
      }
	
      default:
	return 0;

    }
}

void
OpenType_cmap::dump_table(int t, Vector<uint32_t> &cs, Vector<uint32_t> &gs, ErrorHandler *errh) const
{
    if (check_table(t, errh) < 0)
	return;
    
    const uint8_t *data = _str.udata();
    data += ULONG_AT(data + HEADER_SIZE + t * ENCODING_SIZE + 4);
    
    switch (USHORT_AT(data)) {
	
      case F_BYTE:
	for (int c = 0; c < 256; c++)
	    if (int g = data[6 + c]) {
		cs.push_back(c);
		gs.push_back(g);
	    }
	break;
	
      case F_HIBYTE:
	assert(USHORT_AT(data + 6) == 0);
	for (int hi_byte = 0; hi_byte < 256; hi_byte++) {
	    int subh = USHORT_AT(data + 6 + hi_byte * 4);
	    if (subh == 0 && hi_byte > 0)
		continue;
	    const uint8_t *tdata = data + 524 + subh;
	    int firstCode = USHORT_AT(tdata);
	    int entryCount = USHORT_AT(tdata + 2);
	    int idDelta = SHORT_AT(tdata + 4);
	    int idRangeOffset = USHORT_AT(tdata + 6);
	    const uint8_t *gdata = tdata + 6 + idRangeOffset;
	    for (int i = 0; i < entryCount; i++)
		if (int g = USHORT_AT(gdata + (i << 1))) {
		    cs.push_back(hi_byte << 8 + firstCode + i);
		    gs.push_back((idDelta + g) & 65535);
		}
	}
	break;
	
      case F_SEGMENTED: {
	  int segCountX2 = USHORT_AT(data + 6);
	  const uint8_t *endCounts = data + 14;
	  const uint8_t *startCounts = endCounts + segCountX2 + 2;
	  const uint8_t *idDeltas = startCounts + segCountX2;
	  const uint8_t *idRangeOffsets = idDeltas + segCountX2;
	  for (int i = 0; i < segCountX2; i += 2) {
	      int endCount = USHORT_AT(endCounts + i);
	      int startCount = USHORT_AT(startCounts + i);
	      int idDelta = SHORT_AT(idDeltas + i);
	      int idRangeOffset = USHORT_AT(idRangeOffsets + i);
	      if (idRangeOffset == 0) {
		  for (int c = startCount; c <= endCount; c++) {
		      cs.push_back(c);
		      gs.push_back((c + idDelta) & 65535);
		  }
	      } else {
		  const uint8_t *gdata = idRangeOffsets + i + idRangeOffset;
		  for (int c = startCount; c <= endCount; c++, gdata += 2)
		      if (int g = USHORT_AT(gdata)) {
			  cs.push_back(c);
			  gs.push_back((g + idDelta) & 65535);
		      }
	      }
	  }
	  break;
      }

      case F_TRIMMED: {
	  int firstCode = USHORT_AT(data + 6);
	  int entryCount = USHORT_AT(data + 8);
	  for (int i = 0; i < entryCount; i++)
	      if (int g = USHORT_AT(data + 10 + (i << 1))) {
		  cs.push_back(firstCode + i);
		  gs.push_back(g);
	      }
	  break;
      }
	
      case F_SEGMENTED32: {
	  uint32_t nGroups = ULONG_AT(data + 12);
	  const uint8_t *groups = data + 16;
	  for (uint32_t i = 0; i < nGroups; i++, groups += 12) {
	      uint32_t startCharCode = ULONG_AT(groups);
	      uint32_t nCharCodes = ULONG_AT(groups + 4) - startCharCode;
	      uint32_t startGlyphID = ULONG_AT(groups + 8);
	      for (uint32_t i = 0; i <= nCharCodes; i++) {
		  cs.push_back(startCharCode + i);
		  gs.push_back(startGlyphID + i);
	      }
	  }
	  break;
      }
	
      default:
	break;

    }
}

int
OpenType_cmap::map_uni(const Vector<uint32_t> &vin, Vector<OpenTypeGlyph> &vout) const
{
    int t = first_unicode_table();
    if (check_table(t) < 0)
	return -1;
    vout.resize(vin.size(), 0);
    for (int i = 0; i < vin.size(); i++)
	vout[i] = map_table(t, vin[i]);
    return 0;
}

}
