#ifndef EFONT_CFF_HH
#define EFONT_CFF_HH
#include "hashmap.hh"
#include "t1cs.hh"
class ErrorHandler;
class Type1Encoding;

class EfontCFF { public:

    EfontCFF(const String &, ErrorHandler * = 0);
    ~EfontCFF();

    bool ok() const			{ return _error >= 0; }
    int error() const			{ return _error; }

    const String &data_string() const	{ return _data_string; }
    const unsigned char *data() const	{ return _data; }
    int length() const			{ return _len; }
    
    int nfonts() const			{ return _name_index.size(); }
    PermString font_name(int i) const	{ return _name_index[i]; }
    int font_offset(int, int &, int &) const;
    int font_offset(PermString, int &, int &) const;

    enum { NSTANDARD_STRINGS = 391, MAX_SID = 64999 };
    int max_sid() const			{ return NSTANDARD_STRINGS - 1 + _strings.size(); }
    int sid(PermString);
    PermString sid_permstring(int sid) const;

    int ngsubrs() const			{ return _gsubrs_index.nitems(); }
    EfontCharstring *gsubr(int i);
    
    enum DictOperator {
	oVersion = 0, oNotice = 1, oFullName = 2, oFamilyName = 3,
	oWeight = 4, oFontBBox = 5, oBlueValues = 6, oOtherBlues = 7,
	oFamilyBlues = 8, oFamilyOtherBlues = 9, oStdHW = 10, oStdVW = 11,
	oUniqueID = 13, oXUID = 14, oCharset = 15, oEncoding = 16,
	oCharStrings = 17, oPrivate = 18, oSubrs = 19, oDefaultWidthX = 20,
	oNominalWidthX = 21,
	oCopyright = 32 + 0, oIsFixedPitch = 32 + 1, oItalicAngle = 32 + 2,
	oUnderlinePosition = 32 + 3, oUnderlineThickness = 32 + 4,
	oPaintType = 32 + 5, oCharstringType = 32 + 6, oFontMatrix = 32 + 7,
	oStrokeWidth = 32 + 8, oBlueScale = 32 + 9, oBlueShift = 32 + 10,
	oBlueFuzz = 32 + 11, oStemSnapH = 32 + 12, oStemSnapV = 32 + 13,
	oForceBold = 32 + 14, oLanguageGroup = 32 + 17,
	oExpansionFactor = 32 + 18, oInitialRandomSeed = 32 + 19,
	oSyntheticBase = 32 + 20, oPostScript = 32 + 21,
	oBaseFontName = 32 + 22, oBaseFontBlend = 32 + 23,
	oROS = 32 + 30, oCIDFontVersion = 32 + 31, oCIDFontRevision = 32 + 32,
	oCIDFontType = 32 + 33, oCIDCount = 32 + 34, oUIDBase = 32 + 35,
	oFDArray = 32 + 36, oFDSelect = 32 + 37, oFontName = 32 + 38,
	oLastOperator = oFontName
    };

    enum DictType {
	tNone = 0, tSID, tFontNumber, tBoolean, tNumber, tOffset, tLocalOffset,
	tArray, tArray2, tArray3, tArray4, tArray5, tArray6, tPrivateType,
	tTypeMask = 0x7F, tPrivate = 0x80, tP = tPrivate
    };

    class Dict;
    class IndexIterator;
    class Charset;
    class Font;

    static const char * const operator_names[];
    static const int operator_types[];


    class EfontCFF::IndexIterator { public:

	IndexIterator()		: _offset(0), _last_offset(0), _offsize(-1) { }
	IndexIterator(const unsigned char *, int, int, ErrorHandler * = 0, const char *index_name = "INDEX");

	int error() const	{ return (_offsize < 0 ? _offsize : 0); }
    
	bool live() const	{ return _offset < _last_offset; }
	operator bool() const	{ return live(); }
	int nitems() const;

	const unsigned char *operator*() const;
	const unsigned char *operator[](int) const;
	const unsigned char *index_end() const;

	void operator++()	{ _offset += _offsize; }
	void operator++(int)	{ ++(*this); }
	
      private:
    
	const unsigned char *_contents;
	const unsigned char *_offset;
	const unsigned char *_last_offset;
	int _offsize;

	unsigned offset_at(const unsigned char *) const;
    
    };
    
  private:

    String _data_string;
    const unsigned char *_data;
    int _len;

    int _error;
    
    Vector<PermString> _name_index;

    IndexIterator _top_dict_index;

    IndexIterator _strings_index;
    mutable Vector<PermString> _strings;
    mutable HashMap<PermString, int> _strings_map;

    IndexIterator _gsubrs_index;
    Vector<EfontCharstring *> _gsubrs_cs;

    int parse_header(ErrorHandler *);

    enum { HEADER_SIZE = 4 };
    
};


class EfontCFF::Dict { public:

    Dict(EfontCFF *, int pos, int dict_len, ErrorHandler * = 0, const char *dict_name = "DICT");

    bool ok() const			{ return _error >= 0; }
    int error() const			{ return _error; }

    int check(bool is_private, ErrorHandler * = 0, const char *dict_name = "DICT") const;

    bool has(DictOperator) const;
    bool value(DictOperator, Vector<double> &) const;
    bool value(DictOperator, int, int *) const;
    bool value(DictOperator, double, double *) const;

    void unparse(ErrorHandler *, const char *) const;

  private:

    EfontCFF *_cff;
    int _pos;
    Vector<int> _operators;
    Vector<int> _pointers;
    Vector<double> _operands;
    int _error;

};

class EfontCFF::Charset { public:

    Charset()				: _error(-1) { }
    Charset(const EfontCFF *, int pos, int nglyphs, ErrorHandler * = 0);
    void assign(const EfontCFF *, int pos, int nglyphs, ErrorHandler * = 0);

    int error() const			{ return _error; }
    
    int nglyphs() const			{ return _sids.size(); }
    int nsids() const			{ return _gids.size(); }
    
    int gid_to_sid(int gid) const;
    int sid_to_gid(int sid) const;
    
  private:

    Vector<int> _sids;
    Vector<int> _gids;
    int _error;

    void assign(const int *, int, int);
    int parse(const EfontCFF *, int pos, int nglyphs, ErrorHandler *);
    
};


class EfontCFF::Font : public EfontProgram { public:

    Font(EfontCFF *, PermString = PermString(), ErrorHandler * = 0);
    ~Font();

    bool ok() const			{ return _error >= 0; }
    int error() const			{ return _error; }

    PermString font_name() const	{ return _font_name; }
    
    int nsubrs() const			{ return _subrs_index.nitems(); }
    EfontCharstring *subr(int) const;
    int ngsubrs() const			{ return _cff->ngsubrs(); }
    EfontCharstring *gsubr(int) const;

    int nglyphs() const			{ return _charstrings_index.nitems(); }
    void glyph_names(Vector<PermString> &) const;
    PermString glyph_name(int) const;
    EfontCharstring *glyph(int) const;
    EfontCharstring *glyph(PermString) const;

    Type1Encoding *type1_encoding() const;

    double global_width_x(bool) const;
    
  private:

    EfontCFF *_cff;
    PermString _font_name;
    int _charstring_type;

    EfontCFF::Charset _charset;

    IndexIterator _charstrings_index;
    mutable Vector<EfontCharstring *> _charstrings_cs;

    IndexIterator _subrs_index;
    mutable Vector<EfontCharstring *> _subrs_cs;

    int _encoding[256];
    mutable Type1Encoding *_t1encoding;

    double _default_width_x;
    double _nominal_width_x;

    int _error;

    int parse_encoding(int pos, ErrorHandler *);
    int assign_standard_encoding(const int *standard_encoding);
    EfontCharstring *charstring(const IndexIterator &, int) const;
    
};


inline unsigned
EfontCFF::IndexIterator::offset_at(const unsigned char *x) const
{
    switch (_offsize) {
      case 0:
	return 0;
      case 1:
	return x[0];
      case 2:
	return (x[0] << 8) | x[1];
      case 3:
	return (x[0] << 16) | (x[1] << 8) | x[2];
      default:
	return (x[0] << 24) | (x[1] << 16) | (x[2] << 8) | x[3];
    }
}

inline const unsigned char *
EfontCFF::IndexIterator::operator*() const
{
    assert(live());
    return _contents + offset_at(_offset);
}

inline const unsigned char *
EfontCFF::IndexIterator::operator[](int which) const
{
    assert(live() && _offset + which * _offsize <= _last_offset);
    return _contents + offset_at(_offset + which * _offsize);
}

inline int
EfontCFF::Charset::gid_to_sid(int gid) const
{
    if (gid >= 0 && gid < _sids.size())
	return _sids[gid];
    else
	return -1;
}

inline int
EfontCFF::Charset::sid_to_gid(int sid) const
{
    if (sid >= 0 && sid < _gids.size())
	return _gids[sid];
    else
	return -1;
}

#endif