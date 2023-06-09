#include "SP/ProgramParser/ExprParser.h"
#include "SP/ProgramParser/Parser.h"
#include "SP/Tokenizer/Tokenizer.h"
#include "catch.hpp"

#include <vector>

TEST_CASE("Parser parses program to statements") {
    VariableNode var1("west"), var2("y"), var3("z"), var4("west");
    Operator op("+");
    ExpressionNode expr(&op);
    AssignStatementNode stmt1(&var1, &expr, 1), stmt2(&var2, &expr, 2),
        stmt3(&var3, &expr, 3), stmt4(&var4, &expr, 4);
    std::vector<StatementNode *> stmt_list = {&stmt1, &stmt2, &stmt3, &stmt4};
    ProcedureNode procedure1("Bedok", stmt_list);
    std::vector<ProcedureNode *> proc_list = {&procedure1};
    ProgramNode expected = ProgramNode(proc_list);

    std::string sourceProgram =
        "procedure Bedok {\nwest = 9 + east;\ny = east - 4;\nz = west + "
        "2;\nwest = 9 + east + west;\n}";
    std::vector<Token *> tokens = Tokenizer(sourceProgram).tokenize();
    ProgramNode *program = ProgramParser(0, tokens).parse();
    REQUIRE(program->equals(&expected));
}

TEST_CASE("keyword as assignment LHS") {
    VariableNode var1("west"), var2("a"), var3("b"), var4("read"),
        var5("print"), var6("call");
    Operator op("+");
    ExpressionNode expr(&op);
    AssignStatementNode stmt1(&var1, &expr, 1);
    ReadStatementNode stmt2(&var2, 2);
    PrintStatementNode stmt3(&var3, 3);
    AssignStatementNode stmt4(&var4, &expr, 4);
    AssignStatementNode stmt5(&var5, &expr, 5);
    AssignStatementNode stmt6(&var6, &expr, 6);
    std::vector<StatementNode *> stmt_list = {&stmt1, &stmt2, &stmt3,
                                              &stmt4, &stmt5, &stmt6};
    ProcedureNode procedure1("Bedok", stmt_list);
    std::vector<ProcedureNode *> proc_list = {&procedure1};
    ProgramNode expected = ProgramNode(proc_list);

    std::string sourceProgram =
        "procedure Bedok {\nwest = 9 + east;\nread a;\nprint b;\nread = "
        "c;\nprint = d;\ncall = e;\n}";
    std::vector<Token *> tokens = Tokenizer(sourceProgram).tokenize();
    ProgramNode *program = ProgramParser(0, tokens).parse();
    REQUIRE(program->equals(&expected));
}

TEST_CASE("read read; print print") {
    VariableNode var1("read"), var2("print"), var3("call");
    Operator op("+");
    ExpressionNode expr(&op);
    ReadStatementNode stmt1(&var1, 1);
    PrintStatementNode stmt2(&var2, 2);
    ReadStatementNode stmt3(&var3, 3);
    PrintStatementNode stmt4(&var3, 4);
    std::vector<StatementNode *> stmt_list = {&stmt1, &stmt2, &stmt3, &stmt4};
    ProcedureNode procedure1("Bedok", stmt_list);
    std::vector<ProcedureNode *> proc_list = {&procedure1};
    ProgramNode expected = ProgramNode(proc_list);

    std::string sourceProgram = "procedure Bedok {\nread read;\nprint "
                                "print;\nread call;\nprint call;\n}";
    std::vector<Token *> tokens = Tokenizer(sourceProgram).tokenize();
    ProgramNode *program = ProgramParser(0, tokens).parse();
    REQUIRE(program->equals(&expected));
}

TEST_CASE("Test Expression Parser") {
    std::string statement = "((x+3)*8+2)*(9/(y+a)+2);";

    VariableNode var1("x"), var2("a"), var3("y");
    ConstantNode c1("3"), c2("8"), c3("2"), c4("9");
    Operator op1("+"), op2("*"), op3("/");

    ExpressionNode plus1(&op1);
    plus1.setLeft(new ExpressionNode(&var1));
    plus1.setRight(new ExpressionNode(&c1));

    ExpressionNode mul(&op2);
    mul.setLeft(&plus1);
    mul.setRight(new ExpressionNode(&c2));

    ExpressionNode plus2(&op1);
    plus2.setLeft(&mul);
    plus2.setRight(new ExpressionNode(&c3));

    ExpressionNode plus3(&op1);
    plus3.setLeft(new ExpressionNode(&var3));
    plus3.setRight(new ExpressionNode(&var2));

    ExpressionNode div(&op3);
    div.setLeft(new ExpressionNode(&c4));
    div.setRight(&plus3);

    ExpressionNode plus4(&op1);
    plus4.setLeft(&div);
    plus4.setRight(new ExpressionNode(&c3));

    ExpressionNode expected(&op2);
    expected.setLeft(&plus2);
    expected.setRight(&plus4);

    std::vector<Token *> tokens = Tokenizer(statement).tokenize();
    ExprParser parser(0, tokens, false);
    ExpressionNode *result = parser.parse();

    // REQUIRE(result->equals(&expected));
}

TEST_CASE("Test Conditional Parser") {
    VariableNode var1("a"), var2("b");
    ConstantNode c1("2"), c2("3"), c3("5");
    Operator op1("!"), op2("+"), op3("*"), op4(">="), op5("&&"), op6("!=");

    ExpressionNode plus(&op2);
    plus.setLeft(new ExpressionNode(&c1));
    plus.setRight(new ExpressionNode(&c2));

    ExpressionNode mul(&op3);
    mul.setLeft(&plus);
    mul.setRight(new ExpressionNode(&c1));

    ExpressionNode gte(&op4);
    gte.setLeft(&mul);
    gte.setRight(new ExpressionNode(&c3));

    ExpressionNode inverse(&op1);
    inverse.setLeft(&gte);

    ExpressionNode mul2(&op3);
    mul2.setLeft(new ExpressionNode(&var1));
    mul2.setRight(new ExpressionNode(&var2));

    ExpressionNode nte(&op6);
    nte.setLeft(&mul2);
    nte.setRight(new ExpressionNode(&c2));

    ExpressionNode andNode(&op5);
    andNode.setLeft(&inverse);
    andNode.setRight(&nte);

    ExpressionNode *expected = &andNode;

    std::string statement = "((!((2+3)*2 >= 5)) && (a*b != 3))";
    std::vector<Token *> tokens = Tokenizer(statement).tokenize();
    CondParser parser = CondParser(1, tokens);
    ExpressionNode *cond = parser.parse();

    REQUIRE(cond->equals(expected));
}

TEST_CASE("Invalid expression statement") {
    std::string statement = "(x+3) > 2";
    std::vector<Token *> tokens = Tokenizer(statement).tokenize();
    ExprParser parser = ExprParser(0, tokens, false);
    REQUIRE_THROWS(parser.parse(), "invalid expression");
}

TEST_CASE("Invalid conditional statements") {
    std::string statement = "((x+3)&&(y-2))";
    std::vector<Token *> tokens = Tokenizer(statement).tokenize();
    CondParser parser = CondParser(1, tokens);
    REQUIRE_THROWS(parser.parse(), "invalid cond expression");

    statement = "(((a<b) && (a==b)) > x)";
    tokens = Tokenizer(statement).tokenize();
    parser = CondParser(1, tokens);
    REQUIRE_THROWS(parser.parse(), "invalid cond expression");
}

TEST_CASE("While Statement Parser") {
    VariableNode var1("a"), var2("b");
    ConstantNode c1("2"), c2("3"), c3("5");
    Operator op1("!"), op2("+"), op3("*"), op4(">="), op5("&&"), op6("!=");
    std::vector<StatementNode *> stmtList = {};

    ExpressionNode mul2(&op3);
    mul2.setLeft(new ExpressionNode(&var1));
    mul2.setRight(new ExpressionNode(&var2));

    ExpressionNode nte(&op6);
    nte.setLeft(&mul2);
    nte.setRight(new ExpressionNode(&c2));

    AssignStatementNode a(&var1, new ExpressionNode(&c2), 2);
    ReadStatementNode r(&var2, 3);
    std::vector<StatementNode *> stmtList2 = {&a, &r};

    WhileStatementNode expected(stmtList2, &nte, 1);

    std::string statement = "while(a*b != 3) {a = 3; read b;}";
    std::vector<Token *> tokens = Tokenizer(statement).tokenize();
    WhileStatementNode *result = WhileStmParser(0, tokens, 1).parse();

    REQUIRE(result->equals(&expected));
}

TEST_CASE("If Statement Parser") {
    VariableNode var1("a"), var2("b");
    ConstantNode c1("2"), c2("3"), c3("5");
    Operator op1("!"), op2("+"), op3("*"), op4(">="), op5("&&"), op6("!=");

    ExpressionNode mul2(&op3);
    mul2.setLeft(new ExpressionNode(&var1));
    mul2.setRight(new ExpressionNode(&var2));

    ExpressionNode nte(&op6);
    nte.setLeft(&mul2);
    nte.setRight(new ExpressionNode(&c2));

    AssignStatementNode a1(&var1, new ExpressionNode(&c2), 2);
    AssignStatementNode a2(&var1, new ExpressionNode(&c1), 4);
    ReadStatementNode r(&var2, 4);
    std::vector<StatementNode *> stmt1 = {&a2};
    WhileStatementNode w1(stmt1, &nte, 3);
    std::vector<StatementNode *> stmt2 = {&a1, &w1};
    std::vector<StatementNode *> stmt3 = {&r};

    IfStatementNode expected(stmt2, stmt3, &nte, 1);

    std::string statement =
        "if(a*b!=3) then {a=3; while(a*b!=3){a=2;}} else{read b;}";
    std::vector<Token *> tokens = Tokenizer(statement).tokenize();
    IfStatementNode *result = IfStmParser(0, tokens, 1).parse();

    REQUIRE(result->equals(&expected));
}

TEST_CASE("Test getAllCalls") {
    std::string sourceProgram =
        "procedure a{ if(x == 3) then { if (y == 3) then {while(1<x){call b;}} "
        "else{call c;}} else{ while(9<=x){call d;} }}"
        "procedure b {while(y < 3) {call c;} call d;}"
        "procedure c { print y; }"
        "procedure d { print x; }";

    std::vector<std::string> expecteda{"b", "c", "d"};
    std::vector<std::string> expectedb{"c", "d"};
    std::vector<std::string> expectedc{};
    std::vector<std::string> expectedd{};

    std::vector<Token *> tokens = Tokenizer(sourceProgram).tokenize();
    ProgramNode *program = ProgramParser(0, tokens).parse();

    std::vector<ProcedureNode *> procList = program->getProcList();
    std::vector<std::string> resulta = procList[0]->getAllCalls();
    std::vector<std::string> resultb = procList[1]->getAllCalls();
    std::vector<std::string> resultc = procList[2]->getAllCalls();
    std::vector<std::string> resultd = procList[3]->getAllCalls();

    REQUIRE(expecteda == resulta);
    REQUIRE(expectedb == resultb);
    REQUIRE(expectedc == resultc);
    REQUIRE(expectedd == resultd);
}

TEST_CASE("recursive call is not allowed") {
    std::string sourceProgram = "procedure Bedok {\ncall Bedok;\n}";
    std::vector<Token *> tokens = Tokenizer(sourceProgram).tokenize();
    REQUIRE_THROWS(ProgramParser(0, tokens).parse(),
                   "recursive call is not allowed");
}

TEST_CASE("container with wrong cond expression") {
    std::string statement = "while((a+b)*3){read x;}";
    std::vector<Token *> tokens = Tokenizer(statement).tokenize();
    WhileStmParser parser(0, tokens, 1);
    REQUIRE_THROWS(parser.parse(), "invalid cond expression");
}

TEST_CASE("procedure of same name is not allowed") {
    std::string sourceProgram =
        "procedure Bedok {\nread a;\n}\n\nprocedure Bedok{\nprint b;\n}";
    std::vector<Token *> tokens = Tokenizer(sourceProgram).tokenize();
    REQUIRE_THROWS(ProgramParser(0, tokens).parse(),
                   "procedure of same name is not allowed");
}

TEST_CASE("calling undeclared procedure is not allowed") {
    std::string sourceProgram = "procedure Bedok {\ncall a;\n}";
    std::vector<Token *> tokens = Tokenizer(sourceProgram).tokenize();
    REQUIRE_THROWS(ProgramParser(0, tokens).parse(),
                   "calling undeclared procedure is not allowed");
}

TEST_CASE("cyclic calling is not allowed") {
    std::string sourceProgram = "procedure a{call b;} procedure b{call c; call "
                                "d;} procedure c{call d;} procedure d{call a;}";
    std::vector<Token *> tokens = Tokenizer(sourceProgram).tokenize();
    REQUIRE_THROWS(ProgramParser(0, tokens).parse(),
                   "cyclic calling is not allowed");
}

TEST_CASE("Invalid source program, bracket not in pair") {
    std::string sourceProgram = "procedure a { b = x+8);}";
    std::vector<Token *> tokens = Tokenizer(sourceProgram).tokenize();
    REQUIRE_THROWS(ProgramParser(0, tokens).parse(),
                   "assignment statement wrong syntax");
}
