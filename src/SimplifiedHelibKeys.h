//
// Created by gil on 17.8.2020.
//

#ifndef SECURE_REPORT_SIMPLIFIEDHELIBKEYS_H
#define SECURE_REPORT_SIMPLIFIEDHELIBKEYS_H

#include <NTL/ZZ.h>
#include <FHE.h>
#include <EncryptedArray.h>

class SimplifiedHelibKeys {
private:
    FHEPubKey *_publicKey;
    FHESecKey *_secretKey;
    EncryptedArray *_ea;
    FHEcontext *_context;

    long _p;
    long _r;
    int _word_length;

public:
    SimplifiedHelibKeys() : _publicKey(NULL), _secretKey(NULL), _ea(NULL), _context(NULL), _p(0), _r(0) {}
    SimplifiedHelibKeys(const SimplifiedHelibKeys &h) : _publicKey(h._publicKey), _secretKey(h._secretKey), _ea(h._ea), _context(h._context), _p(h._p), _r(h._r) {}
    SimplifiedHelibKeys(FHEPubKey *pub, FHESecKey *sec, EncryptedArray *ea, FHEcontext *ctx) : _p(0), _r(0) { setKeys(pub, sec, ea, ctx); }

    void initKeys(long s, long R, long p, long r, long d, long c, long k, long w, long L, long m, const Vec<long> &gens, const Vec<long> &ords);
    void initKeys_with_recrypt(long s, long p, long r, long d, long c, long k, long w, long L, long m);
    void setKeys(FHEPubKey *pub, FHESecKey *sec, EncryptedArray *ea, FHEcontext *ctx)
    { _publicKey = pub; _secretKey = sec; _ea = ea; _context = ctx; }

    long &p() { return _p; }
    long &r() { return _r; }

    int nslots() const { return _ea->size(); }
    int simd_factor() const { return nslots(); }

    FHEPubKey &publicKey() { return *_publicKey; }
    EncryptedArray &ea() { return *_ea; }
    Ctxt encrypt(int b);
    void encrypt(Ctxt &c, int b);

    Ctxt encrypt(const std::vector<long> &b);
    Ctxt encrypt(const std::vector<int> &b);
    void encrypt(Ctxt &c, const std::vector<long> &b);
    void encrypt(Ctxt &c, const std::vector<int> &b);
    long decrypt(const Ctxt &b);
    void decrypt(std::vector<long> &out, const Ctxt &b);

    void write_to_file(std::ostream& s);
    void read_from_file(std::istream& s);

    void print(const Ctxt &b);

    void rotate(Ctxt &a, int step) { _ea->rotate(a, step); }
};


#endif //SECURE_REPORT_SIMPLIFIEDHELIBKEYS_H
