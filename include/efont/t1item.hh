#ifndef T1ITEM_HH
#define T1ITEM_HH
#include "t1cs.hh"
#include "vector.hh"
#include "permstr.hh"
class StringAccum;
class Type1Reader;
class Type1Writer;
class Type1Interp;
class Type1Font;
class Type1CopyItem;
class Type1Subr;
class Type1SubrGroupItem;
class Type1Definition;

class Type1Item { public:

    Type1Item()					{ }
    virtual ~Type1Item()			{ }
  
    virtual void gen(Type1Writer &) = 0;

    virtual Type1CopyItem *cast_copy()		{ return 0; }
    virtual Type1Subr *cast_subr()		{ return 0; }
    virtual Type1Definition *cast_definition()	{ return 0; }
    virtual Type1SubrGroupItem *cast_subr_group() { return 0; }
  
  private:
  
    Type1Item(const Type1Item &);
    Type1Item &operator=(const Type1Item &);
  
};

class Type1NullItem : public Type1Item {  public:
  
    Type1NullItem()			{ }
  
    void gen(Type1Writer &);

};

class Type1CopyItem : public Type1Item { public:

    Type1CopyItem(char *v, int l)	: _value(v), _length(l) { }
    ~Type1CopyItem()			{ delete[] _value; }

    char *value() const			{ return _value; }
    int length() const			{ return _length; }
  
    char *take_value();
    void set_value(char *, int);
  
    void gen(Type1Writer &);
  
    Type1CopyItem *cast_copy()		{ return this; }
  
  private:
  
    char *_value;
    int _length;
  
};

class Type1EexecItem : public Type1Item { public:

    Type1EexecItem(bool on)		: _eexec_on(on) { }
  
    void gen(Type1Writer &w);
  
  private:
  
    bool _eexec_on;
  
};

class Type1Definition : public Type1Item { public:
  
    typedef Vector<double> NumVector;
  
    Type1Definition(PermString, char *, PermString);
    ~Type1Definition();  
    static Type1Definition *make(StringAccum &, Type1Reader * = 0, bool force = false);
  
    PermString name() const		{ return _name; }
    const char *value() const		{ return _val; }
    PermString definer() const		{ return _definer; }
  
    bool value_bool(bool &) const;
    bool value_int(int &) const;
    bool value_num(double &) const;
    bool value_name(PermString &) const;
    bool value_numvec(NumVector &) const;
    bool value_numvec_vec(Vector<NumVector> &) const;
    bool value_normalize(Vector<NumVector> &in, Vector<NumVector> &out) const;
    bool value_namevec(Vector<PermString> &) const;
  
    void set_bool(bool);
    void set_int(int);
    void set_num(double);
    void set_name(PermString, bool name = true);
    void set_code(const char *s)	{ set_val_copy(s); }
    void set_numvec(const NumVector &, bool executable = false);
    void set_numvec_vec(const Vector<NumVector> &);
    void set_normalize(const Vector<NumVector> &, const Vector<NumVector> &);
    void set_namevec(const Vector<PermString> &, bool executable = true);
  
    void gen(Type1Writer &);
    void gen(StringAccum &);
  
    Type1Definition *cast_definition()	{ return this; }
  
  private:
  
    PermString _name;
    char *_val;
    PermString _definer;
  
    static int slurp_string(StringAccum &, int, Type1Reader *);
    static int slurp_proc(StringAccum &, int, Type1Reader *);
  
    inline void set_val(char *);
    inline void set_val(StringAccum &);
    void set_val_copy(const char *);
  
};

class Type1Encoding : public Type1Item { public:

    Type1Encoding();
    ~Type1Encoding();
  
    PermString operator[](int e) const	{ assert(e>=0&&e<256); return _v[e]; }
    PermString elt(int e) const		{ return (*this)[e]; }
    PermString operator[](unsigned char e) const { return _v[e]; }
    PermString elt(unsigned char e) const { return operator[](e); }
    void put(int e, PermString p)	{ assert(e>=0&&e<256); _v[e] = p; }
    
    static Type1Encoding *standard_encoding();

    void gen(Type1Writer &);

  private:
  
    PermString *_v;
    Type1Encoding *_copy_of;
    
    Type1Encoding(Type1Encoding *);
  
};

class Type1Subr : public Type1Item { public:

    static Type1Subr *make(char *, int, int cs_start, int cs_len, int lenIV);
    static Type1Subr *make_subr(int, PermString, const Type1Charstring &);
  
    bool is_subr() const		{ return !_name; }
    PermString name() const		{ return _name; }
    int subrno() const			{ return _subrno; }
    PermString definer() const		{ return _definer; }
  
    Type1Charstring &t1cs()		{ return _cs; }
    const Type1Charstring &t1cs() const	{ return _cs; }
    operator Type1Charstring &()	{ return _cs; }
    operator const Type1Charstring &() const { return _cs; }
  
    bool run(Type1Interp &t1i)		{ return _cs.run(t1i); }
  
    void gen(Type1Writer &);
  
    virtual Type1Subr *cast_subr()	{ return this; }
  
  private:
  
    PermString _name;
    int _subrno;
    PermString _definer;
  
    Type1Charstring _cs;
  
    static PermString cached_definer;
  
    Type1Subr(PermString, int, PermString, int, unsigned char *, int);
    Type1Subr(PermString, int, PermString, const Type1Charstring &);
  
};

class Type1SubrGroupItem : public Type1Item { public:

    Type1SubrGroupItem(Type1Font *, bool, char *, int);
    Type1SubrGroupItem(const Type1SubrGroupItem &, Type1Font *);
    ~Type1SubrGroupItem();

    void set_end_text(const char *);
    void add_end_text(const char *);

    bool is_subrs() const			{ return _is_subrs; }
    char *end_text() const			{ return _end_text; }
  
    void gen(Type1Writer &);
  
    Type1SubrGroupItem *cast_subr_group()	{ return this; }
  
  private:
    
    Type1Font *_font;
    bool _is_subrs;
    char *_value;
    int _length;
    char *_end_text;
    int _end_length;
  
};

class Type1IncludedFont : public Type1Item { public:

    Type1IncludedFont(Type1Font *, int);
    ~Type1IncludedFont();

    Type1Font *included_font() const	{ return _included_font; }
  
    void gen(Type1Writer &);

  private:

    Type1Font *_included_font;
    int _unique_id;

};

#endif
