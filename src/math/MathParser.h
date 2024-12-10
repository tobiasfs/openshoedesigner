///////////////////////////////////////////////////////////////////////////////
// Name               : MathParser.h
// Purpose            : 
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 19.06.2021
// Copyright          : (C) 2021 Tobias Schaefer <tobiassch@users.sourceforge.net>
// Licence            : GNU General Public License version 3.0 (GPLv3)
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef MATH_MATHPARSER_H
#define MATH_MATHPARSER_H

/*!\class MathParser
 * \brief Lexer/Parser/Evaluator with Unit%s
 *
 * Parses a mathematical expression into a number with an Unit.
 *
 * A single expression (using ParseExpression()) or a code snippet (using
 * ParseCode()) are compiled in the code for a simple virtual machine and
 * executed.
 *
 * All Value%s are double-typed and have an Unit.
 *
 *
 * Supported functions are:
 *  - + - * / ^ ( ) ++ -- >> << %
 *  - cos sin tan acos asin atan atan2
 *  - pow exp exp2 log log10 log2
 *  - sqrt cbrt
 *  - ceil floor round limit min max
 *  - abs mod
 *
 * The functions atan2, pow and mod need two parameter, the function log can
 * have a second parameter with the base of the logarithm.
 *
 * The function limit needs three parameter lower-limit, value and upper-limit.
 *
 * The min- and the max-function can have one or more parameter.
 *
 * Predefined values are:
 *  - Eulers number as M_E
 *  - Pi number as M_PI, pi, PI, Pi or pI
 *
 * Rounding is always done with respect to the SI base units,
 * e.g. ceil(1 cm) == 1 m
 *
 * Unit%s are processed by the unit parser. All SI units and some non SI
 * units are accepted.
 *
 * Accepted additional units are e.g.:
 *  - in, " for inches
 *  - ft, ' for feet
 *  - mil for 1/1000 inch
 */

#include "Unit.h"

#include <array>
#include <memory>
#include <vector>
#include <string>
#include <stdexcept>

class MathParser {
public:
	class Exception: public std::runtime_error {
	public:
		explicit Exception(const std::string &message) throw () :
				std::runtime_error(message.c_str()) {
		}
		virtual char const* what() const throw () {
			return std::runtime_error::what();
		}
	};

	class Value {
	public:
		Value() = default;
		explicit Value(const double value);
		Value(const double value, const Unit &unit);
		explicit Value(const Unit &unit);

		double operator()() const;
		double& operator()();
		double ToDouble() const;
		bool ToBool() const;

		Unit& GetUnit();
		const Unit GetUnit() const;
		void SetUnit(const Unit &unit, bool pureUnit = false);
		void ClearUnit();
		bool IsUnitEmpty() const;
		bool HasUnitOf(const Value &other) const;
		void MultiplyUnit(const Value &other);
		void DivideUnit(const Value &other);
		void PowerUnit(double exponent);

		friend std::ostream& operator<<(std::ostream &out, const Value &value);

	protected:
		double value = 0.0;
		bool pureUnit = false;
		Unit unit;
	};

	class Variable: public Value {
	public:
		Variable() = default;
		virtual ~Variable() = default;
		explicit Variable(const std::string &name);
		std::string name; ///< Name of the variable
		bool isinput = false; ///< Indicator if used as an input
		bool isoutput = false; ///< Indicator, if used as an output
	};

	/**\name Variable manipulation
	 *
	 *	Manipulate the variable list. Variables are created while parsing the
	 *	code. The can later (or even earlier) be created and accessed to add
	 *	values to the variables for the code to manipulate.
	 *
	 *	External variables are a mechanism, that is used by the
	 *	ParameterEvaluator for handling acyclic directed graphs of multiple
	 *	evaluators connected with each other.
	 * \{
	 */
	void ClearVariables();
	size_t SetVariable(const std::string &variablename, const Value &value);
	size_t SetVariable(const size_t idx, const Value &value);
	size_t GetVariableIndex(const std::string &variablename) const;
	const MathParser::Variable& GetVariable(
			const std::string &variablename) const;
	const MathParser::Variable& GetVariable(const size_t idx) const;
	bool Exists(const std::string &variablename) const;
	/**\}
	 */

	double SIFromString(const std::string &expression);

	/**\name Virtual Machine
	 * \{
	 */

public:
	/*! OpCodes for the virtual machine.
	 *
	 * Note that for logical operation or comparison with equality the epsilon
	 * member variable of this class defines what is considered equal. For
	 * example the JMP_Z command jumps, if the top value on the stack is zero.
	 * This is true, if the absolute value if this number is smaller than
	 * epsilon.
	 *
	 * Most commands manipulate the top of the stack by popping some values
	 * off of it, executions some kind of operation and pushing the result
	 * back onto it.
	 *
	 * Operations that pop two values and push one result back:
	 *  * ADD, SUB, MULT, DIV
	 *  * POW, MOD
	 *  * AND, OR
	 *  * LT, LE, GT, GE, EQ, NEQ
	 *  * F_MIN, F_MAX, F_ATAN2
	 *
	 * The conditional jump commands pop one value off of the stack.
	 *
	 * The jump commands are split into a forward and a backward direction,
	 * because the jump distance is given by a unsigned integer number.
	 * Jumping forward is one more than the integer value and jumping backwards
	 * is one less, because after the shifting of the instruction pointer, the
	 * instruction pointer is incremented by one.
	 *
	 * The _Z and _NZ is done with an epsilon comparison.
	 *  * JMP_Z, JMPR_Z
	 *  * JMP_NZ, JMPR_NZ
	 *
	 * For AND and OR operation the level of 0.5 is used to determine if a value
	 * is true or false. True is everything above 0.5, false everything below.
	 *
	 * The results are stored as 0 and 1. And thus can be used with the
	 * conditional jumps.
	 *
	 */
	enum class OpCode {
		NOP, ///< No operation, do nothing for one cycle.
		STOP, ///< Stop execution, the VM can only be restarted by calling InitMachine();
		PUSH_I, ///< Push one value (with unit) directly onto the stack.
		POP, ///< Pop one value from the stack and discard of it.
		SWAP, ///< Swap the top two values on the stack.
		DUP, ///< Duplicate the top-most value on the stack.
		FETCH_I, ///< Fetch the value of a variable from the variable-vector and push it onto the stack.
		STORE_I, ///< Store the top-most value into the variable-vector. The value is left on the stack.
		FETCH_EXT_I, ///< Fetch the value of an external variable from the variable-vector and push it onto the stack.
		STORE_EXT_I, ///< Store the top-most value into an external variable. The value is left on the stack.
		JMP, ///< Do a relative forward jump.
		JMPR, ///< Do a relative backwards jump.
		JMP_Z, ///< Do a relative forward jump, if the top value on the stack is zero.
		JMP_NZ, ///< Do a relative forward jump, if the top value on the stack is not zero.
		JMPR_Z, ///< Do a relative backward jump, if the top value on the stack is zero.
		JMPR_NZ, ///< Do a relative backward jump, if the top value on the stack is not zero.
		ADD, ///< Add the top two values together. The units of the values have to be of the same type (for example meter 'm' and inch 'in').
		SUB, ///< Subtract the top two values. The units of the values have to be of the same type (for example meter 'm' and inch 'in').
		MULT, ///< Multiply the top two values. The units of the values do not have to be of the same type (e.g. 5V * 3A = 15W).
		DIV, ///< Divide the two to values together. The units of the values do not have to be of the same type (e.g. 6V / 3A = 2Ohm).
		MOD, ///< Calculate the modulo of the first top value with the second. The units of the values have to be of the same type.
		POW, ///< Take the power of the first top value by the second. The second values has to be unitless.
		AND, ///< AND operation on the top two values. Both values have to be unitless.
		OR, ///< OR operation on the top two values. Both values have to be unitless.
		LT, ///< Less than. The value pushed last has to be smaller than the other value minus epsilon.
		LE, ///< Less or equal. The value pushed last has to be smaller than the other value plus epsilon.
		GT, ///< Greater than. The value pushed last has to be greater than the other value plus epsilon.
		GE, ///< Greater or equal. The value pushed last has to be greater than the other value minus epsilon.
		EQ, ///< Equality. The absolute difference between both values has to be smaller than epsilon.
		NEQ, ///< Not equal. The absolute difference between both values has to be greater than epsilon.
		NEG, ///< Invert the top value on the stack, i.e. -15m are changed to 15m.
		F_ABS, ///< Calculate the absolute value. The sign is made positive.
		F_EXP, ///< Calculate e to the power of the top value.
		F_EXP2, ///< Calculate 2 to the power of the top value.
		F_LOG, ///< Calculate the logarithm to the base of e.
		F_LOG2, ///< Calculate the logarithm to the base of 2.
		F_LOG10, ///< Calculate the logarithm to the base of 10.
		F_MAX, ///< Find the maximum of the top two stack values.
		F_MIN, ///< Find the minimum of the top two stack values.
		F_SIN, ///< Calculate the sinus from radians.
		F_COS, ///< Calculate the cosinus from radians.
		F_TAN, ///< Calculate the tangens from radians.
		F_ASIN, ///< Calculate the inverse sinus.
		F_ACOS, ///< Calculate the inverse cosinus.
		F_ATAN, ///< Calculate the inverse tangens.
		F_ATAN2, ///< Calculate the angle in radians from two values (same units).
		F_CBRT, ///< Calculate the cubic root.
		F_SQRT, ///< Calculate the square root.
		F_CEIL, ///< Round the top value up.
		F_FLOOR, ///< Round the top value down.
		F_ROUND ///< Round the top value.
	};

	class Instruction {
	public:
		Instruction() = default;
		OpCode opcode = OpCode::NOP;
		Value value = Value(0.0);
		size_t idx = 0;
		bool expression = false;

		size_t pos = 0;
		size_t row = 0;
		size_t col = 0;
		size_t length = 0;

		/**
		 *
		 */
		std::string ToString(bool withParam = false) const; ///< Convert OpCodes to a string
	};

	size_t instructionpointer = 0;
	std::vector<Instruction> instructions;
	std::vector<Variable> variables;
	std::vector<std::weak_ptr<Variable>> externalvariables;
	std::vector<Value> stack;

	bool ignoreCase = false; ///< Ignore upper-/lowercase when looking up variables.

	double epsilon = 1e-12;

	/** \brief Resets the virtual machine to the beginning of a compiled program.
	 */
	void InitMachine();
	/** \brief Run the program until it finishes.
	 */
	void Run();
	/** \brief Checks if a program was successfully evaluated.
	 *
	 * An empty parser (i.e. where no string of code or expression has been
	 * parsed) is evaluated as not having run, even if InitMachine() and Run()
	 * have been called.
	 */
	bool HasRun() const;
	/** \brief Execute a single opcode.
	 */
	void StepOpCode();
	/** \brief Execute all opcodes, that make up a single expression.
	 */
	void StepExpression();

	size_t StackSize() const;
	const MathParser::Value& GetStack(size_t idx);

private:
	void TestUnits(const Value &lval, const Value &rval,
			const Instruction &instr);
	void TestUnitEmpty(const Value &val, const Instruction &instr);

	/**\}
	 */
	/**\name Parser
	 * \{
	 */

public:
	void ParseCode(const std::string &code);
	void ParseExpression(const std::string &expression);

	/** \brief Flag: An expression consisting of a number and an unit is joined while parsing.
	 *
	 * If true: 1/2s = 0.5 Hz.
	 * If false and implicitMultiplication is true: 1/2s = 0.5 s.
	 * If false and implicitMultiplication is false, this results in a parse error.
	 */
	bool glueUnitToNumber = true;

	/** \brief Flag: An expression consisting of twice a number and an unit is joined while parsing.
	 *
	 * If true: 1 m 3 cm = 1.03 m.
	 * If false and implicitMultiplication is true: 1 m 3 cm = 0.03 m^2.
	 * If false and implicitMultiplication is false, this results in a parse error.
	 */
	bool doubleGlueUnitToNumber = true;

	/** \brief Flag: An expression followed by a unit is multiplied by that unit.
	 *
	 * If true: 1 A s = 1C.
	 * 			But 1 As = 0, because As is per se not a unit but a variable
	 * 			that will probably be zero.
	 *
	 * If false: 1 A s results in a parse error.
	 * 			 It would have to be written as 1 A*s if glueUnitToNumber is
	 * 			 true or 1*A*s if it is false.
	 */
	bool implicitMultiplication = true;

	/** \brief Flag: Two numbers following without a separating sign are added.
	 *
	 * This is used to write expressions like 3"5' = 3 ft + 5 in
	 */
	bool implicitAddition = false;

private:
	void ParseStatement();
	void ParseCompoundStatement();
	void ParseSelectionStatement();
	void ParseIterationStatement();
	void ParseJumpStatement();
	size_t ParseExpressionStatement();
	size_t ParseExpression();
	size_t ParseAssignmentExpression();
	size_t ParseConditionalExpression();

	size_t ParseLogicalOrExpression();
	size_t ParseLogicalAndExpression();
	size_t ParseEqualityExpression();

	size_t ParseRelationalExpression();
	size_t ParseShiftExpression();
	size_t ParseAdditiveExpression();
	size_t ParseMultiplicativeExpression();
	size_t ParsePowExpression();
	size_t ParseUnaryExpression();
	size_t ParseArgumentExpressionList();
	size_t ParseFunction();
	size_t ParsePostfixExpression();
	size_t ParsePrimaryExpression();

	void AddInstruction(OpCode opcode, const Value &value = Value(0.0),
			size_t idx = 0);

	bool IdentifierExists(const std::string &name) const;
	size_t LocateIdentifier(const std::string &name) const;
	size_t SetupIdentifier(const std::string &name);

	/**\}
	 */
	/**\name Lexer
	 * \{
	 */

private:
	void InitLexer();
	void ReadNextToken();

	// Note: False and True have an extra _ at the beginning because of
	//       OpenGL using these as global symbols.
	enum class TokenType {
		Unknown = 0,
		And,
		AndAnd,
		Assign,
		BlockComment,
		Break,
		Caret,
		Colon,
		Comma,
		Constant,
		Continue,
		Div,
		DivAssign,
		Do,
		Else,
		EndOfInput,
		Equal,
		Euler_E,
		_False,
		For,
		Func_abs,
		Func_acos,
		Func_asin,
		Func_atan,
		Func_atan2,
		Func_cbrt,
		Func_ceil,
		Func_cos,
		Func_exp,
		Func_exp2,
		Func_floor,
		Func_limit,
		Func_log,
		Func_log10,
		Func_log2,
		Func_max,
		Func_min,
		Func_mod,
		Func_pow,
		Func_round,
		Func_sin,
		Func_sqrt,
		Func_tan,
		Greater,
		GreaterEqual,
		Identifier,
		If,
		LeftBrace,
		LeftBracket,
		LeftParen,
		LeftShift,
		LeftShiftAssign,
		Less,
		LessEqual,
		LineComment,
		Minus,
		MinusAssign,
		MinusMinus,
		ModAssign,
		Modulo,
		Newline,
		Not,
		NotEqual,
		Or,
		OrOr,
		Pi,
		Plus,
		PlusAssign,
		PlusPlus,
		Question,
		Return,
		RightBrace,
		RightBracket,
		RightParen,
		RightShift,
		RightShiftAssign,
		Semi,
		Star,
		StarAssign,
		Tilde,
		_True,
		While,
		Whitespace
	};

	enum class ActionType {
		NoAction = 0,
		AppendChar,
		skip,
		AppendNumber,
		AppendFraction,
		InvertExponentSign,
		AppendExponent
	};

	std::string code;

	size_t pos0 = 0;
	size_t pos = 0;
	size_t row = 0;
	size_t col = 0;
	TokenType token = TokenType::Unknown;
	double value = 0.0;
	std::string id;

	static std::string ToString(TokenType token); ///< Convert a function-name into a string
	std::string PositionInText() const;
	void ErrorAt(const std::string &functionname, const std::string &errortext);
	void ErrorBefore(const std::string &functionname,
			const std::string &errortext);

	// Lexertables

private:
	static const size_t stateCount = 192;
	static const size_t charWidth = 256; // Normal 8-bit bytes

	static std::array<size_t, stateCount * charWidth> InitStateTransitionTable();
	static std::array<TokenType, stateCount> InitTokenTypeTable();
	static std::array<ActionType, stateCount * charWidth> InitActionTable();

	static std::array<size_t, stateCount * charWidth> transitionTable;
	static std::array<TokenType, stateCount> tokenTable;
	static std::array<ActionType, stateCount * charWidth> actionTable;
	/** \}
	 */
};

#endif /* MATH_MATHPARSER_H */

