#include "parser/ast/expressions/BasicIdentifierNode.hpp"
#include "parser/ast/expressions/ops/BinOpNode.hpp"
#include "parser/ast/expressions/ops/ParenthesesNode.hpp"
#include "parser/ast/expressions/ops/UnaryOpNode.hpp"
#include "tests/TestsCore.hpp"
#include "tests/parser/ParserTestsUtilities.hpp"

using namespace rgl;

TEST(Parser, preUnaryOp) {
  auto parser = makeParser({{TokenType::t_plus_plus},
                            {TokenType::t_identifier, "a"s},
                            {TokenType::t_asterisk},
                            {TokenType::t_open_paren},
                            {TokenType::t_identifier, "b"s},
                            {TokenType::t_plus},
                            {TokenType::t_exclamation},
                            {TokenType::t_identifier, "c"s},
                            {TokenType::t_close_paren}});

  assertNode(
      parser->parseExpression(),
      std::make_unique<BinOpNode>(
          BinOpType::b_asterisk,
          std::make_unique<UnaryOpNode>(
              UnaryOpType::u_pre_plus_plus,
              std::make_unique<BasicIdentifierNode>("a")),
          std::make_unique<ParenthesesNode>(std::make_unique<BinOpNode>(
              BinOpType::b_plus, std::make_unique<BasicIdentifierNode>("b"),
              std::make_unique<UnaryOpNode>(
                  UnaryOpType::u_pre_exclamation,
                  std::make_unique<BasicIdentifierNode>("c"))))));
}

TEST(Parser, postUnaryOp) {
  auto parser = makeParser({{TokenType::t_identifier, "a"s},
                            {TokenType::t_minus_minus},
                            {TokenType::t_forward_slash},
                            {TokenType::t_identifier, "b"s}});

  assertNode(parser->parseExpression(),
             std::make_unique<BinOpNode>(
                 BinOpType::b_forward_slash,
                 std::make_unique<UnaryOpNode>(
                     UnaryOpType::u_post_minus_minus,
                     std::make_unique<BasicIdentifierNode>("a")),
                 std::make_unique<BasicIdentifierNode>("b")));
}

TEST(Parser, preAmpersnad) {
  auto parser =
      makeParser({{TokenType::t_ampersand}, {TokenType::t_identifier, "a"s}});

  assertNode(parser->parseExpression(),
             std::make_unique<UnaryOpNode>(
                 UnaryOpType::u_pre_ampersand,
                 std::make_unique<BasicIdentifierNode>("a")));
}

TEST(Parser, preColonDontIncludeBinOp) {
  auto parser = makeParser({{TokenType::t_colon},
                            {TokenType::t_identifier, "a"s},
                            {TokenType::t_plus},
                            {TokenType::t_identifier, "b"s}});

  assertNode(parser->parseExpression(),
             std::make_unique<BinOpNode>(
                 BinOpType::b_plus,
                 std::make_unique<UnaryOpNode>(
                     UnaryOpType::u_pre_colon,
                     std::make_unique<BasicIdentifierNode>("a")),
                 std::make_unique<BasicIdentifierNode>("b")));
}

TEST(Parser, preAtWithParentheses) {
  auto parser = makeParser({
      {TokenType::t_at},
      {TokenType::t_open_paren},
      {TokenType::t_identifier, "a"s},
      {TokenType::t_forward_slash},
      {TokenType::t_identifier, "b"s},
      {TokenType::t_close_paren},
  });

  assertNode(parser->parseExpression(),
             std::make_unique<UnaryOpNode>(
                 UnaryOpType::u_pre_at,
                 std::make_unique<ParenthesesNode>(std::make_unique<BinOpNode>(
                     BinOpType::b_forward_slash,
                     std::make_unique<BasicIdentifierNode>("a"),
                     std::make_unique<BasicIdentifierNode>("b")))));
}
