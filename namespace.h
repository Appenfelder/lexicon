#ifndef __NAMESPACE_H__
#define __NAMESPACE_H__
namespace Lane {
enum Tabs {
  Create_Tab          = 0x00000001,
  Switch_Tab          = 0x00000002,
};
 enum SearchType {
  Root                = 0x00000004,
  Word                = 0x00000008,
  Entry               = 0x00000010
 };
 enum Search {
   Ignore_Diacritics =  0x00000020,
   Whole_Word        =  0x00000040,
   Buckwalter        =  0x00000080,
   Arabic            =  0x00000100,
   Normal            =  0x00000200,
   Regex             =  0x00000400 };
}
#endif