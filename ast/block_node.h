#ifndef __PWN_BLOCKNODE_H__
#define __PWN_BLOCKNODE_H__

#include <cdk/ast/basic_node.h>
#include <cdk/ast/sequence_node.h>

namespace pwn {

  class block_node: public cdk::basic_node {
    cdk::sequence_node *_declarations, *_instructions;
  public:
    /**
     * Simple constructor.
     *
     * @param lineno the source code line number corresponding to
     * the node
     */
    inline block_node(int lineno,cdk::sequence_node *declarations, cdk::sequence_node *instructions) :
        basic_node(lineno), _declarations(declarations), _instructions(instructions) {
    }

    inline cdk::sequence_node *declarations() {
      return _declarations;
    }

    inline cdk::sequence_node *instructions() {
      return _instructions;
    }

    /**
     * @param sp semantic processor visitor
     * @param level syntactic tree level
     */
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_block_node(this, level);
    }

  };

} 

#endif