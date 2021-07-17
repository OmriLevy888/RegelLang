#include <memory>

#include "lexer/Token.hpp"
#include "parser/Parser.hpp"
#include "parser/ast/constructs/BasicTypeNode.hpp"
#include "parser/ast/expressions/ConditionalNode.hpp"
#include "parser/ast/expressions/ForInLoopNode.hpp"
#include "parser/ast/expressions/ForLoopNode.hpp"
#include "parser/ast/expressions/IdentifierNode.hpp"
#include "parser/ast/expressions/VarDeclNode.hpp"
#include "parser/ast/expressions/literals/BooleanLiteralNode.hpp"
#include "parser/ast/expressions/literals/IntLiteralNode.hpp"
#include "parser/ast/expressions/ops/BinOpNode.hpp"
#include "parser/ast/expressions/ops/UnaryOpNode.hpp"
#include "parser/ast/statements/BreakNode.hpp"
#include "parser/ast/statements/ExpressionStatementNode.hpp"
#include "parser/ast/statements/YieldNode.hpp"
#include "tests/TestsCore.hpp"
#include "tests/parser/ParserTestsUtilities.hpp"

using namespace rgl;

TEST(Parser, emptyForLoop) {
  auto parser = makeParser({{TokenType::t_for},
                            {TokenType::t_semicolon},
                            {TokenType::t_semicolon},
                            {TokenType::t_open_bracket},
                            {TokenType::t_int32_literal, 0l},
                            {TokenType::t_semicolon},
                            {TokenType::t_close_bracket}});

  assertNode(
      parser->parseExpression(),
      std::make_unique<ForLoopNode>(
          nullptr, nullptr, nullptr,
          std::make_unique<ExpressionStatementNode>(
              std::make_unique<IntLiteralNode>(0, BasicTypeNode::t_int32()))));
}

TEST(Parser, fullForLoop) {
  auto parser = makeParser({{TokenType::t_for},
                            {TokenType::t_var},
                            {TokenType::t_identifier, "idx"s},
                            {TokenType::t_colon},
                            {TokenType::t_equal},
                            {TokenType::t_int32_literal, 0l},
                            {TokenType::t_semicolon},

                            {TokenType::t_identifier, "idx"s},
                            {TokenType::t_lesser_than},
                            {TokenType::t_int32_literal, 3l},
                            {TokenType::t_semicolon},

                            {TokenType::t_identifier, "idx"s},
                            {TokenType::t_plus_plus},

                            {TokenType::t_break},
                            {TokenType::t_semicolon}});

  assertNode(
      parser->parseExpression(),
      std::make_unique<ForLoopNode>(
          std::make_unique<VarDeclNode>(
              std::make_unique<IdentifierNode>("idx"s),
              BasicTypeNode::t_implicit()->getMutableType(),
              std::make_unique<IntLiteralNode>(0, BasicTypeNode::t_int32())),
          std::make_unique<BinOpNode>(
              BinOpType::b_lesser_than,
              std::make_unique<IdentifierNode>("idx"s),
              std::make_unique<IntLiteralNode>(3, BasicTypeNode::t_int32())),
          std::make_unique<UnaryOpNode>(
              UnaryOpType::u_post_plus_plus,
              std::make_unique<IdentifierNode>("idx"s)),
          std::make_unique<BreakNode>()));
}

TEST(Parser, forLoopNoInit) {
  auto parser = makeParser({{TokenType::t_for},
                            {TokenType::t_semicolon},

                            {TokenType::t_identifier, "idx"s},
                            {TokenType::t_lesser_than},
                            {TokenType::t_int32_literal, 3l},
                            {TokenType::t_semicolon},

                            {TokenType::t_identifier, "idx"s},
                            {TokenType::t_plus_plus},

                            {TokenType::t_break},
                            {TokenType::t_semicolon}});
  assertNode(
      parser->parseExpression(),
      std::make_unique<ForLoopNode>(
          nullptr,
          std::make_unique<BinOpNode>(
              BinOpType::b_lesser_than,
              std::make_unique<IdentifierNode>("idx"s),
              std::make_unique<IntLiteralNode>(3, BasicTypeNode::t_int32())),
          std::make_unique<UnaryOpNode>(
              UnaryOpType::u_post_plus_plus,
              std::make_unique<IdentifierNode>("idx"s)),
          std::make_unique<BreakNode>()));
}

TEST(Parser, forLoopNoCond) {
  auto parser = makeParser({{TokenType::t_for},
                            {TokenType::t_var},
                            {TokenType::t_identifier, "idx"s},
                            {TokenType::t_colon},
                            {TokenType::t_equal},
                            {TokenType::t_int32_literal, 0l},
                            {TokenType::t_semicolon},

                            {TokenType::t_semicolon},

                            {TokenType::t_identifier, "idx"s},
                            {TokenType::t_plus_plus},

                            {TokenType::t_break},
                            {TokenType::t_semicolon}});

  assertNode(
      parser->parseExpression(),
      std::make_unique<ForLoopNode>(
          std::make_unique<VarDeclNode>(
              std::make_unique<IdentifierNode>("idx"s),
              BasicTypeNode::t_implicit()->getMutableType(),
              std::make_unique<IntLiteralNode>(0, BasicTypeNode::t_int32())),
          nullptr,
          std::make_unique<UnaryOpNode>(
              UnaryOpType::u_post_plus_plus,
              std::make_unique<IdentifierNode>("idx"s)),
          std::make_unique<BreakNode>()));
}

TEST(Parser, forLoopNoAdvance) {
  auto parser = makeParser({{TokenType::t_for},
                            {TokenType::t_var},
                            {TokenType::t_identifier, "idx"s},
                            {TokenType::t_colon},
                            {TokenType::t_equal},
                            {TokenType::t_int32_literal, 0l},
                            {TokenType::t_semicolon},

                            {TokenType::t_identifier, "idx"s},
                            {TokenType::t_lesser_than},
                            {TokenType::t_int32_literal, 3l},
                            {TokenType::t_semicolon},

                            {TokenType::t_open_bracket},
                            {TokenType::t_break},
                            {TokenType::t_semicolon},
                            {TokenType::t_close_bracket}});

  assertNode(
      parser->parseExpression(),
      std::make_unique<ForLoopNode>(
          std::make_unique<VarDeclNode>(
              std::make_unique<IdentifierNode>("idx"s),
              BasicTypeNode::t_implicit()->getMutableType(),
              std::make_unique<IntLiteralNode>(0, BasicTypeNode::t_int32())),
          std::make_unique<BinOpNode>(
              BinOpType::b_lesser_than,
              std::make_unique<IdentifierNode>("idx"s),
              std::make_unique<IntLiteralNode>(3, BasicTypeNode::t_int32())),
          nullptr, std::make_unique<BreakNode>()));
}

TEST(Parser, forLoopNoInitNoAdvance) {
  auto parser = makeParser({{TokenType::t_for},
                            {TokenType::t_semicolon},

                            {TokenType::t_identifier, "idx"s},
                            {TokenType::t_lesser_than},
                            {TokenType::t_int32_literal, 3l},
                            {TokenType::t_semicolon},

                            {TokenType::t_open_bracket},
                            {TokenType::t_break},
                            {TokenType::t_semicolon},
                            {TokenType::t_close_bracket}});

  assertNode(
      parser->parseExpression(),
      std::make_unique<ForLoopNode>(
          nullptr,
          std::make_unique<BinOpNode>(
              BinOpType::b_lesser_than,
              std::make_unique<IdentifierNode>("idx"s),
              std::make_unique<IntLiteralNode>(3, BasicTypeNode::t_int32())),
          nullptr, std::make_unique<BreakNode>()));
}

TEST(Parser, forLoopNoInitNoCond) {
  auto parser = makeParser({{TokenType::t_for},
                            {TokenType::t_semicolon},

                            {TokenType::t_semicolon},

                            {TokenType::t_identifier, "idx"s},
                            {TokenType::t_plus_plus},

                            {TokenType::t_break},
                            {TokenType::t_semicolon}});

  assertNode(parser->parseExpression(),
             std::make_unique<ForLoopNode>(
                 nullptr, nullptr,
                 std::make_unique<UnaryOpNode>(
                     UnaryOpType::u_post_plus_plus,
                     std::make_unique<IdentifierNode>("idx"s)),
                 std::make_unique<BreakNode>()));
}

TEST(Parser, forLoopNoCondNoAdvance) {
  auto parser = makeParser({{TokenType::t_for},
                            {TokenType::t_var},
                            {TokenType::t_identifier, "idx"s},
                            {TokenType::t_colon},
                            {TokenType::t_equal},
                            {TokenType::t_int32_literal, 0l},
                            {TokenType::t_semicolon},

                            {TokenType::t_semicolon},

                            {TokenType::t_open_bracket},
                            {TokenType::t_break},
                            {TokenType::t_semicolon},
                            {TokenType::t_close_bracket}});

  assertNode(
      parser->parseExpression(),
      std::make_unique<ForLoopNode>(
          std::make_unique<VarDeclNode>(
              std::make_unique<IdentifierNode>("idx"s),
              BasicTypeNode::t_implicit()->getMutableType(),
              std::make_unique<IntLiteralNode>(0, BasicTypeNode::t_int32())),
          nullptr, nullptr, std::make_unique<BreakNode>()));
}

TEST(Parser, forLoopInOpInInit) {
  auto parser = makeParser({{TokenType::t_for},
                            {TokenType::t_identifier, "a"s},
                            {TokenType::t_in},
                            {TokenType::t_identifier, "b"s},
                            {TokenType::t_semicolon},
                            {TokenType::t_semicolon},
                            {TokenType::t_open_bracket},
                            {TokenType::t_yield},
                            {TokenType::t_identifier, "a"s},
                            {TokenType::t_semicolon},
                            {TokenType::t_close_bracket}});

  assertNode(
      parser->parseExpression(),
      std::make_unique<ForLoopNode>(
          std::make_unique<BinOpNode>(BinOpType::b_in,
                                      std::make_unique<IdentifierNode>("a"s),
                                      std::make_unique<IdentifierNode>("b"s)),
          nullptr, nullptr,
          std::make_unique<YieldNode>(std::make_unique<IdentifierNode>("a"s))));
}
