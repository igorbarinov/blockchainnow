
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <stdio.h>

#include <v8.h>

#include <node.h>
#include <node_version.h>
#include <node_buffer.h>

#include <openssl/bn.h>
#include <openssl/buffer.h>

#include "common.h"

using namespace std;
using namespace v8;
using namespace node;


static const char* BASE58_ALPHABET = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";

// input: Buffer, output: string
static Handle<Value>
base58_encode (const Arguments& args)
{
  HandleScope scope;
  
  if (args.Length() != 1) {
    return VException("One argument expected: a Buffer");
  }
  if (!Buffer::HasInstance(args[0])) {
    return VException("One argument expected: a Buffer");
  }
  v8::Handle<v8::Object> buf = args[0]->ToObject();
  
  unsigned char *buf_data = (unsigned char *) Buffer::Data(buf);
  int buf_length = Buffer::Length(buf);
  
  BN_CTX *ctx = BN_CTX_new();
  
  BIGNUM *bn = BN_bin2bn(buf_data, buf_length, NULL);
  
  BIGNUM *bn58 = BN_new();
  BN_set_word(bn58, 58);
  
  BIGNUM *bn0 = BN_new();
  BN_set_word(bn0, 0);
  
  BIGNUM *dv = BN_new();
  BIGNUM *rem = BN_new();
  
  // FIXME! compute safe length.
  char *str = new char[100];
  unsigned int c;
  int i, j, j2;
  
  i = 0;
  while (BN_cmp(bn, bn0) > 0) {
    if (!BN_div(dv, rem, bn, bn58, ctx)) {
      BN_free(bn);
      BN_free(bn58);
      BN_free(bn0);
      if (bn != dv)
        BN_free(dv);
      BN_free(rem);
      BN_CTX_free(ctx);
  
      delete [] str;

      return VException("BN_div failed");
    }

    if (bn != dv) {
      BN_free(bn);
      bn = dv;
    }
    c = BN_get_word(rem);
    str[i] = BASE58_ALPHABET[c];
    i++;
  }
  
  // Leading zeros
  for (j = 0; j < buf_length; j++) {
    if (buf_data[j] != 0) {
      break;
    }
    str[i] = BASE58_ALPHABET[0];
    i++;
  }
  
  // Terminator
  str[i] = 0;
  
  // Reverse string
  int numSwaps = (i / 2);
  char tmp;
  for (j = 0; j < numSwaps; j++) {
    j2 = i - 1 - j;
    tmp = str[j];
    str[j] = str[j2];
    str[j2] = tmp;
  }
  
  BN_free(bn);
  BN_free(bn58);
  BN_free(bn0);
  BN_free(rem);
  BN_CTX_free(ctx);
  
  Local<String> ret = String::New(str);
  delete [] str;
  return scope.Close(ret);
}


// input: string, output: Buffer
static Handle<Value>
base58_decode (const Arguments& args)
{
  HandleScope scope;
  const char *errmsg = NULL;
  int nLeadingZeros = 0;
  Buffer *buf = NULL;
  char* data = NULL;
  unsigned int tmpLen = 0;
  unsigned char *tmp = NULL;
  
  if (args.Length() != 1) {
    return VException("One argument expected: a String");
  }
  if (!args[0]->IsString()) {
    return VException("One argument expected: a String");
  }
  
  BN_CTX *ctx = BN_CTX_new();
  
  BIGNUM *bn58 = BN_new();
  BN_set_word(bn58, 58);
  
  BIGNUM *bn = BN_new();
  BN_set_word(bn, 0);

  BIGNUM *bnChar = BN_new();

  String::Utf8Value str(args[0]->ToString());
  char *psz = *str;
  
  while (isspace(*psz))
    psz++;
  
  // Convert big endian string to bignum
  for (const char* p = psz; *p; p++) {
    const char* p1 = strchr(BASE58_ALPHABET, *p);
    if (p1 == NULL) {
      while (isspace(*p))
        p++;
      if (*p != '\0') {
	errmsg = "Error";
	goto err_out;
      }
      break;
    }
    BN_set_word(bnChar, p1 - BASE58_ALPHABET);
    if (!BN_mul(bn, bn, bn58, ctx)) {
      errmsg = "BN_mul failed";
      goto err_out;
    }
    if (!BN_add(bn, bn, bnChar)) {
      errmsg = "BN_add failed";
      goto err_out;
    }
  }

  // Get bignum as little endian data
  tmpLen = BN_num_bytes(bn);
  tmp = (unsigned char *)malloc(tmpLen);
  BN_bn2bin(bn, tmp);

  // Trim off sign byte if present
  if (tmpLen >= 2 && tmp[tmpLen-1] == 0 && tmp[tmpLen-2] >= 0x80)
    tmpLen--;
  
  // Restore leading zeros
  for (const char* p = psz; *p == BASE58_ALPHABET[0]; p++)
    nLeadingZeros++;

  // Allocate buffer and zero it
  buf = Buffer::New(nLeadingZeros + tmpLen);
  data = Buffer::Data(buf);
  memset(data, 0, nLeadingZeros + tmpLen);
  memcpy(data+nLeadingZeros, tmp, tmpLen);

  BN_free(bn58);
  BN_free(bn);
  BN_free(bnChar);
  BN_CTX_free(ctx);
  free(tmp);

  return scope.Close(buf->handle_);

err_out:
  BN_free(bn58);
  BN_free(bn);
  BN_free(bnChar);
  BN_CTX_free(ctx);
  return VException(errmsg);
}


extern "C" void
init (Handle<Object> target)
{
  HandleScope scope;
  target->Set(String::New("base58_encode"), FunctionTemplate::New(base58_encode)->GetFunction());
  target->Set(String::New("base58_decode"), FunctionTemplate::New(base58_decode)->GetFunction());
}

NODE_MODULE(base58, init)
