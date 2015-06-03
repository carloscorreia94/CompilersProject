// $Id: factory.h,v 1.2 2015/03/23 16:13:37 ist176512 Exp $ -*- c++ -*-
#ifndef __PWN_FACTORY_H__
#define __PWN_FACTORY_H__

#include <memory>
#include <cdk/yy_factory.h>
#include "pwn_scanner.h"

namespace pwn {

  /**
   * This class implements the compiler factory for the pwn compiler.
   */
  class factory: public cdk::yy_factory<pwn_scanner> {
    /**
     * This object is automatically registered by the constructor in the
     * superclass' language registry.
     */
    static factory _self;

  protected:
    /**
     * @param language name of the language handled by this factory (see .cpp file)
     */
    factory(const std::string &language = "pwn") :
        cdk::yy_factory<pwn_scanner>(language) {
    }

  };

} // pwn

#endif
