// $Id: symbol.h,v 1.1 2015/02/21 20:27:35 ist13500 Exp $ -*- c++ -*-
#ifndef __PWN_SEMANTICS_VAR_SYMBOL_H__
#define __PWN_SEMANTICS_VAR_SYMBOL_H__

#include <string>
#include <targets/symbol.h>

namespace pwn {

  class var_symbol : public symbol{

    bool _isGlobal;

    public:
      inline var_symbol(const basic_type *type, const std::string &name, long value, bool isGlobal) :
          symbol(type, name, value), _isGlobal(isGlobal) {
      }

      virtual ~var_symbol() {
        
      }

      inline bool is_global(){
        return _isGlobal;
      }
    };
} // pwn

#endif
