// $Id: symbol.h,v 1.1 2015/02/21 20:27:35 ist13500 Exp $ -*- c++ -*-
#ifndef __PWN_SEMANTICS_FUNC_SYMBOL_H__
#define __PWN_SEMANTICS_FUNC_SYMBOL_H__

#include <string>
#include <targets/symbol.h>

namespace pwn {

  class func_symbol : public symbol{

    bool _isDefined;

    public:
      inline func_symbol(const basic_type *type, const std::string &name, long value, bool isDefined) :
          symbol(type, name, value), _isDefined(isDefined) {
      }

      virtual ~func_symbol() {
        
      }

      inline bool isDefined(){
        return _isDefined;
      }

      void isDefined(bool isDefined) {
        _isDefined = isDefined;
      }
    };
} // pwn

#endif
