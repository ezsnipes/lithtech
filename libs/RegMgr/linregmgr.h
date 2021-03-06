#ifndef RegMgr_H
#define RegMgr_H


// Includes...

#include "lith.h"
#include "rapidjson/document.h"
#include <string>

struct rootKeys;

typedef rootKeys* HKEY;

extern HKEY HKEY_LOCAL_MACHINE;
extern HKEY HKEY_CURRENT_USER;
extern HKEY HKEY_CLASSES_ROOT;
extern HKEY HKEY_USERS;
// Classes...

class CRegMgr {
public:

  // public functions
  CRegMgr() : m_Doc() { m_bInitialized = false; };
  ~CRegMgr() { Term(); };
  bool Init(const char* sCompany, const char* sApp, const char* sVersion, const char* sSubKey = nullptr, HKEY hRootKey = HKEY_LOCAL_MACHINE, const char* sRoot2 = nullptr);
  void Term();
  bool SetSubKey(const char* sSubKey);
  bool Set(const char* sKey, const char* sValue);
  bool Set(const char* sKey, void* pValue, int nLen);
  bool Set(const char* sKey, DWORD nValue);
  const char* Get(const char* sKey, char* sBuf, UINT32& nBufSize, const char* sDef = nullptr);
  DWORD Get(const char* sKey, DWORD nDef);
  void* Get(const char* sKey, void* pBuf, UINT32& nBufSize, void* pDef = nullptr, UINT32 nDefSize = 0);
  bool Delete(const char* sKey);
  bool DeleteApp();
  bool DeleteSubKey();
  bool DeleteUnderSubKey(const char* sKey);
  bool IsValid() { return (this != nullptr); };

private:
  bool m_bInitialized;
  char iobuffer[65536];
  std::string rootPath;
  rapidjson::Document m_Doc;
  rapidjson::Value m_hRootKey;
};            

#endif
