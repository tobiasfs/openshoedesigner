///////////////////////////////////////////////////////////////////////////////
// Name               : MathParser.cpp
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

#include "MathParser.h"

#include <cmath>
#include <sstream>
#include <stdexcept>

MathParser::Value::Value(const double value) :
		value(value) {
}

MathParser::Value::Value(const double value, const Unit &unit) :
		unit(unit) {
	this->value = unit.ToSI(value);
}

MathParser::Value::Value(const Unit &unit) :
		unit(unit) {
	value = unit.ToSI(1.0);
	pureUnit = true;
}

double MathParser::Value::operator ()() const {
	return value;
}

double& MathParser::Value::operator ()() {
	return value;
}

double MathParser::Value::ToDouble() const {
	return value;
}

bool MathParser::Value::ToBool() const {
	return value > 0.5;
}

void MathParser::Value::SetUnit(const Unit &unit, bool pureUnit) {
	this->unit = unit;
	this->pureUnit = pureUnit;
	if (pureUnit)
		value = 1.0;
	value = unit.ToSI(value);
}

const Unit MathParser::Value::GetUnit() const {
	return unit;
}

Unit& MathParser::Value::GetUnit() {
	return unit;
}

void MathParser::Value::ClearUnit() {
	unit = Unit();
}

bool MathParser::Value::HasUnitOf(const Value &other) const {
	return this->unit == other.unit;
}

void MathParser::Value::MultiplyUnit(const Value &other) {
	this->unit *= other.unit;
}

void MathParser::Value::DivideUnit(const Value &other) {
	this->unit /= other.unit;
}

void MathParser::Value::PowerUnit(double exponent) {
	this->unit.Power(exponent);
}

bool MathParser::Value::IsUnitEmpty() const {
	return this->unit.NoUnit();
}

std::ostream& operator<<(std::ostream &out, const MathParser::Value &value) {
//	if (!value.pureUnit)
	out << value.value;
//	if (!value.pureUnit && !value.unit.NoUnit())
	if (!value.unit.NoUnit())
		out << ' ';
	if (!value.unit.NoUnit())
		out << value.unit;
	return out;
}

MathParser::Variable::Variable(const std::string &name) :
		name(name) {
}

void MathParser::ClearVariables() {
	variables.clear();
	externalvariables.clear();
}

size_t MathParser::SetVariable(const std::string &variablename,
		const Value &value) {
	const auto idx = SetupIdentifier(variablename);
	variables[idx]() = value();
	variables[idx].GetUnit() = value.GetUnit();
	return idx;
}

size_t MathParser::SetVariable(const size_t idx, const Value &value) {
	variables[idx]() = value();
	variables[idx].GetUnit() = value.GetUnit();
	return idx;
}

size_t MathParser::GetVariableIndex(const std::string &variablename) const {
	const size_t idx = LocateIdentifier(variablename);
	return idx;
}

const MathParser::Variable& MathParser::GetVariable(
		const std::string &variablename) const {
	const size_t idx = LocateIdentifier(variablename);
	if (idx == (size_t) -1)
		throw std::runtime_error(
				"The variable " + variablename + " does not exist.");
	return variables[idx];
}

const MathParser::Variable& MathParser::GetVariable(const size_t idx) const {
	return variables[idx];
}

bool MathParser::Exists(const std::string &variablename) const {
	return IdentifierExists(variablename);
}

size_t MathParser::StackSize() const {
	return stack.size();
}

const MathParser::Value& MathParser::GetStack(size_t idx) {
	return stack[idx];
}

double MathParser::SIFromString(const std::string &expression) {
	ParseExpression(expression);
	InitMachine();
	Run();
	if (StackSize() == 0)
		throw std::runtime_error(
				"The expression \"" + expression + "\" returned nothing.");
	return GetStack(0).ToDouble();
}

// ----------------------------------------------------------------------------
// Virtual machine

void MathParser::InitMachine() {
	stack.clear();
	instructionpointer = 0;
}

void MathParser::Run() {
	while (instructionpointer < instructions.size()
			&& instructions[instructionpointer].opcode != OpCode::STOP) {
		StepOpCode();
	}
}

bool MathParser::HasRun() const {
	// Either the instructionpointer points somewhere outside of the array.
	// (for example if the instructions array was cleared) or points to a
	// Stop opcode.
	return (!instructions.empty())
			&& (instructionpointer >= instructions.size()
					|| instructions[instructionpointer].opcode == OpCode::STOP);
}

void MathParser::StepExpression() {
	if (instructionpointer < instructions.size()
			&& instructions[instructionpointer].opcode != OpCode::STOP)
		StepOpCode();
	while (instructionpointer < instructions.size()
			&& instructions[instructionpointer].opcode != OpCode::STOP
			&& !instructions[instructionpointer].expression) {
		StepOpCode();
	}
}

void MathParser::StepOpCode() {
	switch (instructions[instructionpointer].opcode) {

	case OpCode::NOP:
	case OpCode::STOP:
		return;
	case OpCode::PUSH_I: {
		stack.push_back(instructions[instructionpointer].value);
		break;
	}
	case OpCode::POP: {
		stack.pop_back();
		break;
	}

	case OpCode::FETCH_I: {
		const size_t idx = instructions[instructionpointer].idx;
		stack.push_back(variables[idx]);
		break;
	}

	case OpCode::STORE_I: {
		const size_t idx = instructions[instructionpointer].idx;
		variables[idx]() = stack.back()();
		variables[idx].GetUnit() = stack.back().GetUnit();

		break;
	}

	case OpCode::FETCH_EXT_I: {
		const size_t idx = instructions[instructionpointer].idx;
		auto ext = externalvariables.at(idx).lock();
		stack.push_back(*ext);
		break;
	}

	case OpCode::STORE_EXT_I: {
		const size_t idx = instructions[instructionpointer].idx;
		auto ext = externalvariables.at(idx).lock();
		(*ext)() = stack.back()();
		ext->GetUnit() = stack.back().GetUnit();
		break;
	}
	case OpCode::SWAP: {
		const Value a = *(stack.end() - 1);
		*(stack.end() - 1) = *(stack.end() - 2);
		*(stack.end() - 2) = a;
		break;
	}

	case OpCode::DUP: {
		stack.push_back(stack.back());
		break;
	}

	case OpCode::JMP: {
		instructionpointer += instructions[instructionpointer].idx;
		break;
	}
	case OpCode::JMPR: {
		instructionpointer -= instructions[instructionpointer].idx;
		break;
	}

	case OpCode::JMP_Z: {
		if (fabs(stack.back()()) <= epsilon)
			instructionpointer += instructions[instructionpointer].idx;
		stack.pop_back();
		break;
	}

	case OpCode::JMP_NZ: {
		if (fabs(stack.back()()) > epsilon)
			instructionpointer += instructions[instructionpointer].idx;
		stack.pop_back();
		break;
	}
	case OpCode::JMPR_Z: {
		if (fabs(stack.back()()) <= epsilon)
			instructionpointer -= instructions[instructionpointer].idx;
		stack.pop_back();

		break;
	}

	case OpCode::JMPR_NZ: {
		if (fabs(stack.back()()) > epsilon)
			instructionpointer -= instructions[instructionpointer].idx;
		stack.pop_back();
		break;
	}

	case OpCode::ADD: {
		const Value a = stack.back();
		stack.pop_back();
		TestUnits(a, stack.back(), instructions[instructionpointer]);
		stack.back()() += a();
		break;
	}

	case OpCode::SUB: {
		const Value a = stack.back();
		stack.pop_back();
		TestUnits(a, stack.back(), instructions[instructionpointer]);
		stack.back()() -= a();
		break;
	}

	case OpCode::MULT: {
		const Value a = stack.back();
		stack.pop_back();
		stack.back().MultiplyUnit(a);
		stack.back()() *= a();
		break;
	}

	case OpCode::DIV: {
		const Value a = stack.back();
		stack.pop_back();
		stack.back().DivideUnit(a);
		stack.back()() /= a();
		break;
	}

	case OpCode::MOD: {
		const Value b = stack.back();
		stack.pop_back();
		const Value a = stack.back();
		TestUnits(a, b, instructions[instructionpointer]);
		stack.back()() = std::fmod(a(), b());
		break;
	}

	case OpCode::POW: {
		const Value b = stack.back();
		TestUnitEmpty(b, instructions[instructionpointer]);
		stack.pop_back();
		const Value a = stack.back();
		stack.back().PowerUnit(b());
		stack.back()() = std::pow(a(), b());
		break;
	}

	case OpCode::AND: {
		const Value b = stack.back();
		TestUnitEmpty(b, instructions[instructionpointer]);
		stack.pop_back();
		const Value a = stack.back();
		TestUnitEmpty(a, instructions[instructionpointer]);
		stack.back()() = (a() >= 0.5 && b() >= 0.5) ? 1.0 : 0.0;
		break;
	}

	case OpCode::OR: {
		const Value b = stack.back();
		TestUnitEmpty(b, instructions[instructionpointer]);
		stack.pop_back();
		const Value a = stack.back();
		TestUnitEmpty(a, instructions[instructionpointer]);
		stack.back()() = (a() >= 0.5 || b() >= 0.5) ? 1.0 : 0.0;
		break;
	}

	case OpCode::LT: {
		const Value b = stack.back();
		stack.pop_back();
		const Value a = stack.back();
		TestUnits(a, b, instructions[instructionpointer]);
		stack.back()() = (a() < b() - epsilon) ? 1.0 : 0.0;
		stack.back().ClearUnit();
		break;
	}

	case OpCode::LE: {
		const Value b = stack.back();
		stack.pop_back();
		const Value a = stack.back();
		TestUnits(a, b, instructions[instructionpointer]);
		stack.back()() = (a() <= b() + epsilon) ? 1.0 : 0.0;
		stack.back().ClearUnit();
		break;
	}

	case OpCode::GT: {
		const Value b = stack.back();
		stack.pop_back();
		const Value a = stack.back();
		TestUnits(a, b, instructions[instructionpointer]);
		stack.back()() = (a() > b() + epsilon) ? 1.0 : 0.0;
		stack.back().ClearUnit();
		break;
	}

	case OpCode::GE: {
		const Value b = stack.back();
		stack.pop_back();
		const Value a = stack.back();
		TestUnits(a, b, instructions[instructionpointer]);
		stack.back()() = (a() >= b() - epsilon) ? 1.0 : 0.0;
		stack.back().ClearUnit();
		break;
	}

	case OpCode::EQ: {
		const Value b = stack.back();
		stack.pop_back();
		const Value a = stack.back();
		TestUnits(a, b, instructions[instructionpointer]);
		stack.back()() = (fabs(a() - b()) <= epsilon) ? 1.0 : 0.0;
		stack.back().ClearUnit();
		break;
	}

	case OpCode::NEQ: {
		const Value b = stack.back();
		stack.pop_back();
		const Value a = stack.back();
		TestUnits(a, b, instructions[instructionpointer]);
		stack.back()() = (fabs(a() - b()) > epsilon) ? 1.0 : 0.0;
		stack.back().ClearUnit();
		break;
	}

	case OpCode::NEG: {
		const Value a = stack.back();
		stack.back()() = -a();
		break;
	}

	case OpCode::F_ABS: {
		const Value a = stack.back();
		stack.back()() = std::fabs(a());
		break;
	}

	case OpCode::F_EXP: {
		const Value a = stack.back();
		TestUnitEmpty(a, instructions[instructionpointer]);
		stack.back()() = std::exp(a());
		stack.back().ClearUnit();
		break;
	}

	case OpCode::F_EXP2: {
		const Value a = stack.back();
		TestUnitEmpty(a, instructions[instructionpointer]);
		stack.back()() = std::pow(2.0, a());
		stack.back().ClearUnit();
		break;
	}

	case OpCode::F_LOG: {
		const Value a = stack.back();
		TestUnitEmpty(a, instructions[instructionpointer]);
		stack.back()() = std::log(a());
		stack.back().ClearUnit();
		break;
	}

	case OpCode::F_LOG2: {
		const Value a = stack.back();
		TestUnitEmpty(a, instructions[instructionpointer]);
		stack.back()() = std::log(a()) / M_LN2;
		stack.back().ClearUnit();
		break;
	}

	case OpCode::F_LOG10: {
		const Value a = stack.back();
		TestUnitEmpty(a, instructions[instructionpointer]);
		stack.back()() = std::log10(a());
		stack.back().ClearUnit();
		break;
	}

	case OpCode::F_MAX: {
		const Value b = stack.back();
		stack.pop_back();
		const Value a = stack.back();
		TestUnits(a, b, instructions[instructionpointer]);
		stack.back()() = (a() > b()) ? a() : b();
		break;
	}

	case OpCode::F_MIN: {
		const Value b = stack.back();
		stack.pop_back();
		const Value a = stack.back();
		TestUnits(a, b, instructions[instructionpointer]);
		stack.back()() = (a() > b()) ? b() : a();
		break;
	}

	case OpCode::F_SIN: {
		const Value a = stack.back();
		TestUnitEmpty(a, instructions[instructionpointer]);
		stack.back()() = std::sin(a());
		stack.back().ClearUnit();
		break;
	}

	case OpCode::F_COS: {
		const Value a = stack.back();
		TestUnitEmpty(a, instructions[instructionpointer]);
		stack.back()() = std::cos(a());
		stack.back().ClearUnit();
		break;
	}

	case OpCode::F_TAN: {
		const Value a = stack.back();
		TestUnitEmpty(a, instructions[instructionpointer]);
		stack.back()() = std::tan(a());
		stack.back().ClearUnit();
		break;
	}

	case OpCode::F_ASIN: {
		const Value a = stack.back();
		TestUnitEmpty(a, instructions[instructionpointer]);
		stack.back()() = std::asin(a());
		stack.back().ClearUnit();
		break;
	}

	case OpCode::F_ACOS: {
		const Value a = stack.back();
		TestUnitEmpty(a, instructions[instructionpointer]);
		stack.back()() = std::acos(a());
		stack.back().ClearUnit();
		break;
	}

	case OpCode::F_ATAN: {
		const Value a = stack.back();
		TestUnitEmpty(a, instructions[instructionpointer]);
		stack.back()() = std::atan(a());
		stack.back().ClearUnit();
		break;
	}

	case OpCode::F_ATAN2: {
		const Value b = stack.back();
		stack.pop_back();
		const Value a = stack.back();
		TestUnits(a, b, instructions[instructionpointer]);
		stack.back()() = std::atan2(a(), b());
		stack.back().ClearUnit();
		break;
	}

	case OpCode::F_CBRT: {
		const Value a = stack.back();
		stack.back()() = std::cbrt(a());
		stack.back().PowerUnit(1.0 / 3.0);
		break;
	}

	case OpCode::F_SQRT: {
		const Value a = stack.back();
		stack.back()() = std::sqrt(a());
		stack.back().PowerUnit(1.0 / 2.0);
		break;
	}

	case OpCode::F_CEIL: {
		const Value a = stack.back();
		stack.back()() = std::ceil(a());
		break;
	}

	case OpCode::F_FLOOR: {
		const Value a = stack.back();
		stack.back()() = std::floor(a());
		break;
	}

	case OpCode::F_ROUND: {
		const Value a = stack.back();
		stack.back()() = std::round(a());
		break;
	}
	}
	++instructionpointer;
}

void MathParser::TestUnits(const Value &lval, const Value &rval,
		const Instruction &instr) {
// If the units are compatible everything is ok.
	if (lval.HasUnitOf(rval))
		return;

	std::ostringstream err;
	err << "During evaluation of row ";
	err << instr.row << " column " << instr.col << " the units of ";
	err << lval << " and " << rval << " are not compatible.";
	err << " (Operation: " << instr.ToString(true) << " written as "
			<< code.substr(instr.pos, instr.length) << ")";
	throw std::runtime_error(err.str());
}

void MathParser::TestUnitEmpty(const Value &val, const Instruction &instr) {
	if (val.IsUnitEmpty())
		return;

	std::ostringstream err;
	err << "During evalutation of row ";
	err << instr.row << " column " << instr.col << " the unit";
	err << " should be empty, but it is a value with unit: " << val << ".";
	err << " (Operation: " << code.substr(instr.pos, instr.length) << ")";
	throw std::runtime_error(err.str());
}

// ----------------------------------------------------------------------------
// Recursive Descent Parser & Codegenerator for the virtual machine.

void MathParser::ParseCode(const std::string &code) {
	this->code = code;
	instructions.clear();
	InitLexer();
	ReadNextToken();
	while (token != TokenType::EndOfInput)
		ParseStatement();
}

void MathParser::ParseExpression(const std::string &expression) {
	this->code = expression;
	instructions.clear();
	InitLexer();
	do {
		ReadNextToken();
		ParseExpression();
	} while (token == TokenType::Semi || token == TokenType::Comma);
	if (token != TokenType::EndOfInput)
		ErrorBefore("Expression",
				"The simple expression does not evaluate completely.");
}

void MathParser::ParseStatement() {
	switch (token) {
	case TokenType::LeftBrace:
		ParseCompoundStatement();
		break;
	case TokenType::If:
		ParseSelectionStatement();
		break;
	case TokenType::For:
	case TokenType::Do:
	case TokenType::While:
		ParseIterationStatement();
		break;
	case TokenType::Plus:
	case TokenType::PlusPlus:
	case TokenType::Minus:
	case TokenType::MinusMinus:
	case TokenType::Not:
	case TokenType::Tilde:
	case TokenType::Identifier:
	case TokenType::Constant:
	case TokenType::LeftParen:
	case TokenType::_True:
	case TokenType::_False:
	case TokenType::Pi:
	case TokenType::Euler_E:
	case TokenType::Func_abs:
	case TokenType::Func_acos:
	case TokenType::Func_asin:
	case TokenType::Func_atan:
	case TokenType::Func_atan2:
	case TokenType::Func_cbrt:
	case TokenType::Func_ceil:
	case TokenType::Func_cos:
	case TokenType::Func_exp:
	case TokenType::Func_exp2:
	case TokenType::Func_floor:
	case TokenType::Func_limit:
	case TokenType::Func_log:
	case TokenType::Func_log10:
	case TokenType::Func_log2:
	case TokenType::Func_max:
	case TokenType::Func_min:
	case TokenType::Func_mod:
	case TokenType::Func_pow:
	case TokenType::Func_round:
	case TokenType::Func_sin:
	case TokenType::Func_sqrt:
	case TokenType::Func_tan:
	case TokenType::Continue:
	case TokenType::Break:
	case TokenType::Semi:
		ParseExpressionStatement();
		break;
	case TokenType::Return:
		ParseJumpStatement();
		break;
	default:
		ErrorAt("Statement", "Unknown token.");
	}
}

void MathParser::ParseCompoundStatement() {
	if (token != TokenType::LeftBrace)
		ErrorAt("CompoundStatement", "Expected an left brace {.");
	ReadNextToken();
	while (token != TokenType::RightBrace) {
		ParseStatement();
	}
	if (token != TokenType::RightBrace)
		ErrorAt("CompoundStatement", "Expected a right brace }.");
	ReadNextToken();
}

void MathParser::ParseSelectionStatement() {
	if (token != TokenType::If)
		ErrorAt("SelectionStatement", "'if' keyword missing.");
	ReadNextToken();
	if (token != TokenType::LeftParen)
		ErrorAt("SelectionStatement",
				"if condition is missing the opening parenthesis '('.");
	ReadNextToken();
	size_t stacksize = ParseExpression();
	if (stacksize != 1)
		ErrorAt("SelectionStatement",
				"The if-condition needs to return one value.");
	if (token != TokenType::RightParen)
		ErrorAt("SelectionStatement",
				"if condition is missing the closing parenthesis '('.");
	ReadNextToken();
	AddInstruction(OpCode::JMP_Z);
	const size_t idx_then = instructions.size() - 1;
	ParseStatement();

	if (token == TokenType::Else) {
		ReadNextToken();
		AddInstruction(OpCode::JMP);
		const size_t idx_else = instructions.size() - 1;
		ParseStatement();
		const size_t idx_next = instructions.size();
		instructions[idx_then].idx = idx_else - idx_then;
		instructions[idx_else].idx = idx_next - idx_else - 1;
	} else {
		const size_t idx_next = instructions.size();
		instructions[idx_then].idx = idx_next - idx_then - 1;
	}
}

void MathParser::ParseIterationStatement() {

	//TODO Write test for exactly this function.

	size_t idx_loop_body = 0;
	size_t idx_iteration = 0;
	size_t idx_loop_end = 0;

	switch (token) {
	case TokenType::For: {
		ReadNextToken();
		if (token != TokenType::LeftParen)
			ErrorAt("IterationStatement",
					"missing opening parenthesis '(' after 'for'.");
		ReadNextToken();

		ParseExpressionStatement(); // for-loop initialization

		const size_t idx_condition = instructions.size();

		const size_t stackcountcondition = ParseExpression(); // for-loop condition
		if (stackcountcondition != 1)
			ErrorAt("IterationStatement",
					"The for-loop condition needs to return one value.");

		if (token != TokenType::Semi)
			ErrorAt("IterationStatement",
					"missing semicolon ';' after for-loop condition.");
		ReadNextToken();

		// Testing if the for-loop should continue
		AddInstruction(OpCode::JMP_Z);
		const size_t idx_jmp_end = instructions.size() - 1;
		AddInstruction(OpCode::JMP);
		const size_t idx_jmp_start = instructions.size() - 1;
		idx_iteration = instructions.size();

		if (token != TokenType::RightParen) {
			const size_t stackcountiterator = ParseExpression();
			for (size_t n = 0; n < stackcountiterator; ++n)
				AddInstruction(OpCode::POP);
		}
		AddInstruction(OpCode::JMPR, Value(0.0),
				instructions.size() - idx_condition + 1);
		instructions[idx_jmp_start].idx = instructions.size() - idx_jmp_start
				- 1;

		if (token != TokenType::RightParen)
			ErrorAt("IterationStatement",
					"missing closing parenthesis ')' after for-loop-iterator.");
		ReadNextToken();

		idx_loop_body = instructions.size();

		ParseStatement();

		AddInstruction(OpCode::JMPR, Value(0.0),
				instructions.size() - idx_iteration + 1);
		idx_loop_end = instructions.size();
		instructions[idx_jmp_end].idx = idx_loop_end - idx_jmp_end - 1;

	}
		break;
	case TokenType::Do: {
		idx_loop_body = instructions.size();
		ReadNextToken();
		ParseStatement();
		if (token != TokenType::While)
			ErrorAt("IterationStatement",
					"Missing 'while' for 'do' statement.");
		ReadNextToken();
		if (token != TokenType::LeftParen)
			ErrorAt("IterationStatement",
					"missing opening parenthesis '(' after 'while'.");
		ReadNextToken();
		idx_iteration = instructions.size();
		size_t stacksize = ParseExpression();
		if (stacksize != 1)
			ErrorAt("IterationStatement",
					"The while-condition needs to return one value.");
		if (token != TokenType::RightParen)
			ErrorAt("IterationStatement",
					"missing closing parenthesis '(' in 'while' condition.");
		ReadNextToken();
		AddInstruction(OpCode::JMPR_NZ, Value(0.0),
				instructions.size() - idx_loop_body + 1);
		if (token != TokenType::Semi)
			ErrorAt("IterationStatement",
					"missing semicolon ';' after 'do'-'while' condition.");
		ReadNextToken();
		idx_loop_end = instructions.size();
	}
		break;
	case TokenType::While: {
		const size_t idx_iteration = instructions.size();
		ReadNextToken();
		if (token != TokenType::LeftParen)
			ErrorAt("IterationStatement",
					"if condition is missing the opening parenthesis '('.");
		ReadNextToken();
		size_t stacksize = ParseExpression();
		if (stacksize != 1)
			ErrorAt("IterationStatement",
					"The if-condition needs to return one value.");
		if (token != TokenType::RightParen)
			ErrorAt("IterationStatement",
					"if-condition is missing the closing parenthesis '('.");
		ReadNextToken();

		AddInstruction(OpCode::JMP_Z);
		const size_t idx_loop_body = instructions.size() - 1;
		ParseStatement();
		AddInstruction(OpCode::JMPR, Value(0.0),
				instructions.size() - idx_iteration + 1);
		idx_loop_end = instructions.size();
		instructions[idx_loop_body].idx = idx_loop_end - idx_loop_body - 1;
	}
		break;

	default:
		ErrorAt("IterationStatement", "Looptype not implemented.");
	}

	for (size_t n = idx_loop_body; n < idx_loop_end; ++n) {
		if (instructions[n].opcode == OpCode::NOP && instructions[n].idx == 0) {
			instructions[n].opcode = OpCode::JMP;
			instructions[n].idx = idx_loop_end - n - 1;
		}
		if (instructions[n].opcode == OpCode::NOP && instructions[n].idx == 1) {
			if (n < idx_iteration) {
				instructions[n].opcode = OpCode::JMP;
				instructions[n].idx = idx_iteration - n - 1;
			} else {
				instructions[n].opcode = OpCode::JMPR;
				instructions[n].idx = n - idx_iteration + 1;
			}
		}
	}
}

void MathParser::ParseJumpStatement() {
	if (token != TokenType::Return)
		ErrorAt("JumpStatement", "'return' keyword missing.");
	ReadNextToken();
	if (token != TokenType::Semi) {
		ParseExpression();
		if (token != TokenType::Semi)
			ErrorAt("ExpressionStatement", "Missing ';' to close statement.");
	}
	AddInstruction(OpCode::STOP);
	if (!instructions.empty())
		instructions.back().expression = true;
	ReadNextToken();
}

size_t MathParser::ParseExpressionStatement() {
	if (token == TokenType::Semi) {
		if (!instructions.empty())
			instructions.back().expression = true;
		ReadNextToken();
	} else {
		const size_t stackcount = ParseExpression();
		if (token != TokenType::Semi)
			ErrorAt("ExpressionStatement", "Missing ';' to close statement.");
		for (size_t n = 0; n < stackcount; ++n)
			AddInstruction(OpCode::POP);
		if (!instructions.empty())
			instructions.back().expression = true;
		ReadNextToken();
	}
	return 0;
}

size_t MathParser::ParseExpression() {
	if (token == TokenType::Continue) {
		AddInstruction(OpCode::NOP, Value(0.0), 1);
		ReadNextToken();
		return 0;
	}
	if (token == TokenType::Break) {
		AddInstruction(OpCode::NOP, Value(0.0), 0);
		ReadNextToken();
		return 0;
	}
	size_t stacksize = ParseAssignmentExpression();
	if (token == TokenType::Comma) {
		if (stacksize == 0)
			ErrorBefore("Expression",
					"The expression left of the comma returned no value.");
		if (stacksize > 1)
			ErrorBefore("Expression",
					"The expression left of the comma returned more than one value.");
	}
	while (token == TokenType::Comma) {
		ReadNextToken();
		size_t stacksize2 = ParseAssignmentExpression();
		if (stacksize2 == 0)
			ErrorBefore("Expression",
					"This expression in this comma-separated list returned nothing.");
		if (stacksize2 > 1)
			ErrorBefore("Expression",
					"This expression in this comma-separated list returned more than one value.");

		stacksize += stacksize2;
	}
	return stacksize;
}

size_t MathParser::ParseAssignmentExpression() {

	if (token == TokenType::Identifier) {
		// Lookahead of 1 token needed.
		const auto oldpos = pos;
		const auto oldrow = row;
		const auto oldcol = col;
		const auto oldtoken = token;
		const auto oldvalue = value;
		const auto oldid = id;
		ReadNextToken();
		if (token == TokenType::Assign || token == TokenType::StarAssign
				|| token == TokenType::DivAssign
				|| token == TokenType::ModAssign
				|| token == TokenType::PlusAssign
				|| token == TokenType::MinusAssign
				|| token == TokenType::LeftShiftAssign
				|| token == TokenType::RightShiftAssign) {

			const size_t variableindex = SetupIdentifier(oldid);
			const auto comparetype = token;
			ReadNextToken();
			const size_t stacksize = ParseAssignmentExpression();
			if (stacksize != 1)
				ErrorAt("AssignmentExpression",
						"The '?' operator needs one single value on its left side as input.");
			switch (comparetype) {
			case TokenType::Assign:
				break;
			case TokenType::StarAssign:
				AddInstruction(OpCode::FETCH_I, Value(0.0), variableindex);
				AddInstruction(OpCode::MULT);
				break;
			case TokenType::DivAssign:
				AddInstruction(OpCode::FETCH_I, Value(0.0), variableindex);
				AddInstruction(OpCode::SWAP);
				AddInstruction(OpCode::DIV);
				break;
			case TokenType::ModAssign:
				AddInstruction(OpCode::FETCH_I, Value(0.0), variableindex);
				AddInstruction(OpCode::SWAP);
				AddInstruction(OpCode::MOD);
				break;
			case TokenType::PlusAssign:
				AddInstruction(OpCode::FETCH_I, Value(0.0), variableindex);
				AddInstruction(OpCode::ADD);
				break;
			case TokenType::MinusAssign:
				AddInstruction(OpCode::FETCH_I, Value(0.0), variableindex);
				AddInstruction(OpCode::SWAP);
				AddInstruction(OpCode::SUB);
				break;
			case TokenType::LeftShiftAssign:
				AddInstruction(OpCode::FETCH_I, Value(0.0), variableindex);
				AddInstruction(OpCode::SWAP);
				AddInstruction(OpCode::PUSH_I, Value(2.0));
				AddInstruction(OpCode::SWAP);
				AddInstruction(OpCode::POW);
				AddInstruction(OpCode::MULT);
				break;
			case TokenType::RightShiftAssign:
				AddInstruction(OpCode::FETCH_I, Value(0.0), variableindex);
				AddInstruction(OpCode::SWAP);
				AddInstruction(OpCode::NEG);
				AddInstruction(OpCode::PUSH_I, Value(2.0));
				AddInstruction(OpCode::SWAP);
				AddInstruction(OpCode::POW);
				AddInstruction(OpCode::MULT);
				break;
			default:
				ErrorAt("AssignmentExpression", "Operator not implemented.");
			}
			AddInstruction(OpCode::STORE_I, Value(0.0), variableindex);
			variables[variableindex].isoutput = true;
			return stacksize;
		}
		pos = oldpos;
		row = oldrow;
		col = oldcol;
		token = oldtoken;
		value = oldvalue;
		id = oldid;
	}

	return ParseConditionalExpression();
}

size_t MathParser::ParseConditionalExpression() {
	size_t stacksize = ParseLogicalOrExpression();
	if (token == TokenType::Question) {
		if (stacksize != 1)
			ErrorAt("ConditionalExpression",
					"The ? operator needs 1 value as input.");
		ReadNextToken();
		AddInstruction(OpCode::JMP_Z);
		const size_t idx_then = instructions.size() - 1;
		ParseExpression();
		if (token != TokenType::Colon)
			ErrorAt("ConditionalExpression",
					"Missing colon ':' for the else clause.");
		ReadNextToken();
		AddInstruction(OpCode::JMP);
		const size_t idx_else = instructions.size() - 1;
		ParseConditionalExpression();
		const size_t idx_next = instructions.size();
		instructions[idx_then].idx = idx_else - idx_then;
		instructions[idx_else].idx = idx_next - idx_else - 1;
	}
	return stacksize;
}

size_t MathParser::ParseLogicalOrExpression() {
	const size_t stacksize = ParseLogicalAndExpression();
	while (token == TokenType::Or) {
//		const auto type = token;
		if (stacksize != 1)
			ErrorAt("LogicalOrExpression",
					"The last instruction left of the and-operation did not return a singular value.");
		ReadNextToken();
		const size_t stacksize2 = ParseLogicalAndExpression();
		if (stacksize2 != 1)
			ErrorAt("LogicalOrExpression",
					"The last instruction did not return a singular value.");
		AddInstruction(OpCode::OR);
	}
	return stacksize;
}

size_t MathParser::ParseLogicalAndExpression() {
	const size_t stacksize = ParseEqualityExpression();
	while (token == TokenType::And) {
//		const auto type = token;
		if (stacksize != 1)
			ErrorAt("LogicalAndExpression",
					"The last instruction left of the and-operation did not return a singular value.");
		ReadNextToken();
		const size_t stacksize2 = ParseEqualityExpression();
		if (stacksize2 != 1)
			ErrorAt("LogicalAndExpression",
					"The last instruction did not return a singular value.");
		AddInstruction(OpCode::AND);
	}
	return stacksize;
}

size_t MathParser::ParseEqualityExpression() {
	const size_t stacksize = ParseRelationalExpression();
	while (token == TokenType::Equal || token == TokenType::NotEqual) {
		const auto type = token;
		if (stacksize != 1)
			ErrorAt("EqualityExpression",
					"The last instruction on the comparison operation did not return a singular value.");
		ReadNextToken();
		const size_t stacksize2 = ParseRelationalExpression();
		if (stacksize2 != 1)
			ErrorAt("EqualityExpression",
					"The last instruction did not return a singular value.");

		if (type == TokenType::Equal) {
			AddInstruction(OpCode::EQ);
		}
		if (type == TokenType::NotEqual) {
			AddInstruction(OpCode::NEQ);
		}
	}
	return stacksize;
}

size_t MathParser::ParseRelationalExpression() {
	const size_t stacksize = ParseShiftExpression();
	while (token == TokenType::Less || token == TokenType::LessEqual
			|| token == TokenType::Greater || token == TokenType::GreaterEqual) {
		const auto type = token;
		if (stacksize != 1)
			ErrorAt("RelationalExpression",
					"The last instruction on the comparison operation did not return a singular value.");
		ReadNextToken();
		const size_t stacksize2 = ParseShiftExpression();
		if (stacksize2 != 1)
			ErrorAt("RelationalExpression",
					"The last instruction did not return a singular value.");
		if (type == TokenType::Less) {
			AddInstruction(OpCode::LT);
		}
		if (type == TokenType::LessEqual) {
			AddInstruction(OpCode::LE);
		}
		if (type == TokenType::Greater) {
			AddInstruction(OpCode::GT);
		}
		if (type == TokenType::GreaterEqual) {
			AddInstruction(OpCode::GE);
		}

	}
	return stacksize;
}

size_t MathParser::ParseShiftExpression() {
	const size_t stacksize = ParseAdditiveExpression();
	while (token == TokenType::LeftShift || token == TokenType::RightShift) {
		const auto type = token;
		if (stacksize != 1)
			ErrorAt("ShiftExpression",
					"The last instruction on the left of the shift operation did not return a singular value.");
		ReadNextToken();
		const size_t stacksize2 = ParseAdditiveExpression();
		if (stacksize2 != 1)
			ErrorAt("ShiftExpression",
					"The last instruction did not return a singular value.");
		if (type == TokenType::LeftShift) {
			AddInstruction(OpCode::PUSH_I, Value(2.0));
			AddInstruction(OpCode::SWAP);
			AddInstruction(OpCode::POW);
			AddInstruction(OpCode::MULT);
		}
		if (type == TokenType::RightShift) {
			AddInstruction(OpCode::NEG);
			AddInstruction(OpCode::PUSH_I, Value(2.0));
			AddInstruction(OpCode::SWAP);
			AddInstruction(OpCode::POW);
			AddInstruction(OpCode::MULT);
		}
	}
	return stacksize;
}

size_t MathParser::ParseAdditiveExpression() {

// This function processes the addition and subtraction of values. It also
// accepts the implicit addition when a chaing of values is found.
// For example 3m 15cm or 8"7'

	const size_t stacksize = ParseMultiplicativeExpression();

	while (token == TokenType::Plus || token == TokenType::Minus
			|| (token == TokenType::Constant && implicitAddition)) {
		const auto type = token;
		if (stacksize != 1)
			ErrorAt("AdditiveExpression",
					"The last instruction left of the modulo / power did not return a singular value.");

		if (token == TokenType::Constant && implicitAddition) {
			const size_t stacksize2 = ParseMultiplicativeExpression();
			if (stacksize2 != 1)
				ErrorAt("AdditiveExpression",
						"The last instruction did not return a singular value.");
			AddInstruction(OpCode::ADD);
			continue;
		}

		ReadNextToken();
		const size_t stacksize2 = ParseMultiplicativeExpression();
		if (stacksize2 != 1)
			ErrorAt("AdditiveExpression",
					"The last instruction did not return a singular value.");
		if (type == TokenType::Plus) {
			AddInstruction(OpCode::ADD);
		}
		if (type == TokenType::Minus) {
			AddInstruction(OpCode::SUB);
		}
	}
	return stacksize;
}

size_t MathParser::ParseMultiplicativeExpression() {
	const size_t stacksize = ParsePowExpression();

	while (token == TokenType::Star || token == TokenType::Div
			|| ((token == TokenType::Identifier || token == TokenType::Func_min)
					&& implicitMultiplication)) {
		const auto type = token;
		if (stacksize != 1)
			ErrorAt("MultiplicativeExpression",
					"The last instruction left of the multiplication / division did not return a singular value.");
		if ((token == TokenType::Identifier || token == TokenType::Func_min)
				&& implicitMultiplication) {
			Value value(1.0);
			value.SetUnit(Unit(id), true);
			AddInstruction(OpCode::PUSH_I, value);
			AddInstruction(OpCode::MULT);
			ReadNextToken();
			continue;
		}
		ReadNextToken();
		const size_t stacksize2 = ParsePowExpression();
		if (stacksize2 != 1)
			ErrorBefore("MultiplicativeExpression",
					"The last instruction did not return a singular value.");
		if (type == TokenType::Star) {
			AddInstruction(OpCode::MULT);
		}
		if (type == TokenType::Div) {
			AddInstruction(OpCode::DIV);
		}
	}
	return stacksize;
}

size_t MathParser::ParsePowExpression() {
	const size_t stacksize = ParseUnaryExpression();
	while (token == TokenType::Modulo || token == TokenType::Caret) {
		const auto type = token;
		if (stacksize != 1)
			ErrorBefore("PowExpression",
					"The last instruction left of the modulo / power did not return a singular value.");
		ReadNextToken();
		const size_t stacksize2 = ParseUnaryExpression();
		if (stacksize2 != 1)
			ErrorBefore("PowExpression",
					"The last instruction did not return a singular value.");
		if (type == TokenType::Modulo) {
			AddInstruction(OpCode::MOD);
		}
		if (type == TokenType::Caret) {
			AddInstruction(OpCode::POW);
		}
	}
	return stacksize;
}

size_t MathParser::ParseUnaryExpression() {

	if (token != TokenType::Plus && token != TokenType::PlusPlus
			&& token != TokenType::Minus && token != TokenType::MinusMinus
			&& token != TokenType::Not && token != TokenType::Tilde) {
		return ParsePostfixExpression();
	}

	const auto unarytype = token;
	ReadNextToken();
	const size_t stackcount = ParseUnaryExpression();
	switch (unarytype) {
	case TokenType::PlusPlus: {
		if (instructions.back().opcode != OpCode::FETCH_I
				&& instructions.back().opcode != OpCode::STORE_I)
			ErrorAt("UnaryExpression",
					"The expression after ++ is not an modifiable variable.");
		const size_t idx = instructions.back().idx;
		AddInstruction(OpCode::PUSH_I, Value(1.0));
		AddInstruction(OpCode::ADD);
		AddInstruction(OpCode::STORE_I, Value(0.0), idx);
	}
		break;
	case TokenType::MinusMinus: {
		if (instructions.back().opcode != OpCode::FETCH_I
				&& instructions.back().opcode != OpCode::STORE_I)
			ErrorAt("UnaryExpression",
					"The expression after -- is not an modifiable variable.");
		const size_t idx = instructions.back().idx;
		AddInstruction(OpCode::PUSH_I, Value(1.0));
		AddInstruction(OpCode::SUB);
		AddInstruction(OpCode::STORE_I, Value(0.0), idx);
	}
		break;
	case TokenType::Plus:
		break;
	case TokenType::Minus:
		AddInstruction(OpCode::NEG);
		break;
	case TokenType::Not:
		AddInstruction(OpCode::JMP_Z, Value(0.0), 2);
		AddInstruction(OpCode::PUSH_I, Value(0.0));
		AddInstruction(OpCode::JMP, Value(0.0), 1);
		AddInstruction(OpCode::PUSH_I, Value(1.0));
		break;
	case TokenType::Tilde:
		AddInstruction(OpCode::PUSH_I, Value(0.5));
		AddInstruction(OpCode::LT);
		AddInstruction(OpCode::JMP_NZ, Value(0.0), 2);
		AddInstruction(OpCode::PUSH_I, Value(-1.0));
		AddInstruction(OpCode::JMP, Value(0.0), 1);
		AddInstruction(OpCode::PUSH_I, Value(1.0));
		break;
	default:
		ErrorAt("ParseUnaryExpression",
				"Programming error. This should not happen.");
		break;
	}
	return stackcount;
}

size_t MathParser::ParseArgumentExpressionList() {
	size_t stackcount = 0;
	while (token != TokenType::RightParen) {
		size_t s = ParseAssignmentExpression();
		if (s != 1)
			ErrorAt("ExpressionList",
					"The argument should reduce to/be one value.");
		++stackcount;
		if (token != TokenType::Comma && token != TokenType::RightParen)
			ErrorAt("ExpressionList",
					"Missing comma ',' or closing parenthesis ')'.");
		if (token == TokenType::Comma)
			ReadNextToken();
	}
	return stackcount;
}

size_t MathParser::ParseFunction() {
	const TokenType func_id = token;
	const std::string str_id = id;
	ReadNextToken();
	if (token != TokenType::LeftParen) {
		if (Unit::IsUnit(str_id)) {
			AddInstruction(OpCode::PUSH_I);
			instructions.back().value = Value(Unit(str_id));
			return 1;
		}
		ErrorAt("Function",
				"Missing left parenthesis '(' after " + ToString(func_id)
						+ "-function.");
	}
	ReadNextToken();
	const size_t stackcount = ParseArgumentExpressionList();
	if (token != TokenType::RightParen)
		ErrorAt("Function",
				"Missing right parenthesis ')' after function-argument-list for "
						+ ToString(func_id) + "-function.");
	ReadNextToken();
	switch (func_id) {
	case TokenType::Func_atan2:
	case TokenType::Func_pow:
	case TokenType::Func_mod:
		if (stackcount != 2)
			ErrorBefore("Function",
					"The " + ToString(func_id)
							+ "-function expects two input arguments.");
		break;

	case TokenType::Func_max:
	case TokenType::Func_min:
		// Nothing to do. These functions expect an arbitrary number of inputs.
		break;
	case TokenType::Func_log:
		if (stackcount == 0 || stackcount > 2)
			ErrorBefore("Function",
					"The log function expects one or two arguments: log(value) or log(value, basis).");
		break;

	case TokenType::Func_limit:
		if (stackcount != 3)
			ErrorBefore("Function",
					"The limit-function expects three input arguments (min, value, max).");
		break;

	default:
		if (stackcount != 1)
			ErrorBefore("Function",
					"The " + ToString(func_id)
							+ "-function expects one input argument.");
		break;
	}
	switch (func_id) {
	case TokenType::Func_abs:
		AddInstruction(OpCode::F_ABS);
		break;
	case TokenType::Func_acos:
		AddInstruction(OpCode::F_ACOS);
		break;
	case TokenType::Func_asin:
		AddInstruction(OpCode::F_ASIN);
		break;
	case TokenType::Func_atan:
		AddInstruction(OpCode::F_ATAN);
		break;
	case TokenType::Func_atan2:
		AddInstruction(OpCode::F_ATAN2);
		break;
	case TokenType::Func_cbrt:
		AddInstruction(OpCode::F_CBRT);
		break;
	case TokenType::Func_ceil:
		AddInstruction(OpCode::F_CEIL);
		break;
	case TokenType::Func_cos:
		AddInstruction(OpCode::F_COS);
		break;
	case TokenType::Func_exp:
		AddInstruction(OpCode::F_EXP);
		break;
	case TokenType::Func_exp2:
		AddInstruction(OpCode::F_EXP2);
		break;
	case TokenType::Func_floor:
		AddInstruction(OpCode::F_FLOOR);
		break;
	case TokenType::Func_limit:
		AddInstruction(OpCode::F_MIN);
		AddInstruction(OpCode::F_MAX);
		break;
	case TokenType::Func_log:
		if (stackcount == 2) {
			AddInstruction(OpCode::F_LOG);
			AddInstruction(OpCode::SWAP);
			AddInstruction(OpCode::F_LOG);
			AddInstruction(OpCode::SWAP);
			AddInstruction(OpCode::DIV);
		} else
			AddInstruction(OpCode::F_LOG);
		break;
	case TokenType::Func_log10:
		AddInstruction(OpCode::F_LOG10);
		break;
	case TokenType::Func_log2:
		AddInstruction(OpCode::F_LOG2);
		break;
	case TokenType::Func_max: {
		for (size_t n = 1; n < stackcount; ++n)
			AddInstruction(OpCode::F_MAX);
	}
		break;
	case TokenType::Func_min: {
		for (size_t n = 1; n < stackcount; ++n)
			AddInstruction(OpCode::F_MIN);
	}
		break;
	case TokenType::Func_mod:
		AddInstruction(OpCode::MOD);
		break;
	case TokenType::Func_pow:
		AddInstruction(OpCode::POW);
		break;
	case TokenType::Func_round:
		AddInstruction(OpCode::F_ROUND);
		break;
	case TokenType::Func_sin:
		AddInstruction(OpCode::F_SIN);
		break;
	case TokenType::Func_sqrt:
		AddInstruction(OpCode::F_SQRT);
		break;
	case TokenType::Func_tan:
		AddInstruction(OpCode::F_TAN);
		break;
	default:
		ErrorAt("Function", ToString(token) + " is not implemented.");
	}
	return 1;
}

size_t MathParser::ParsePostfixExpression() {
	size_t stackcount;
	switch (token) {
	case TokenType::Func_abs:
	case TokenType::Func_acos:
	case TokenType::Func_asin:
	case TokenType::Func_atan:
	case TokenType::Func_atan2:
	case TokenType::Func_cbrt:
	case TokenType::Func_ceil:
	case TokenType::Func_cos:
	case TokenType::Func_exp:
	case TokenType::Func_exp2:
	case TokenType::Func_floor:
	case TokenType::Func_limit:
	case TokenType::Func_log:
	case TokenType::Func_log10:
	case TokenType::Func_log2:
	case TokenType::Func_max:
	case TokenType::Func_min:
	case TokenType::Func_mod:
	case TokenType::Func_pow:
	case TokenType::Func_round:
	case TokenType::Func_sin:
	case TokenType::Func_sqrt:
	case TokenType::Func_tan:
		stackcount = ParseFunction();
		break;
	default:
		stackcount = ParsePrimaryExpression();
		break;
	}
	while (token == TokenType::PlusPlus || token == TokenType::MinusMinus) {
		if (instructions.back().opcode != OpCode::FETCH_I)
			ErrorAt("PostfixExpression",
					"lvalue is not an modifiable variable.");
		const size_t idx = instructions.back().idx;
		if (token == TokenType::PlusPlus) {

			AddInstruction(OpCode::DUP);
			AddInstruction(OpCode::PUSH_I, Value(1.0));
			AddInstruction(OpCode::ADD);
			AddInstruction(OpCode::STORE_I, Value(0.0), idx);
			AddInstruction(OpCode::POP);
		}
		if (token == TokenType::MinusMinus) {
			AddInstruction(OpCode::DUP);
			AddInstruction(OpCode::PUSH_I, Value(1.0));
			AddInstruction(OpCode::SUB);
			AddInstruction(OpCode::STORE_I, Value(0.0), idx);
			AddInstruction(OpCode::POP);
		}
		ReadNextToken();
	}
	return stackcount;
}

size_t MathParser::ParsePrimaryExpression() {
	switch (token) {
	case TokenType::Identifier: {
		if (Unit::IsUnit(id) && !IdentifierExists(id)) {
			AddInstruction(OpCode::PUSH_I);
			instructions.back().value = Value(Unit(id));
		} else {
			const size_t variableindex = SetupIdentifier(id);
			variables[variableindex].isinput = true;
			AddInstruction(OpCode::FETCH_I, Value(0.0), variableindex);
		}
		break;
	}
	case TokenType::Constant: {
		AddInstruction(OpCode::PUSH_I, Value(value));
		break;
	}
	case TokenType::_True: {
		AddInstruction(OpCode::PUSH_I, Value(1.0));
		break;
	}
	case TokenType::_False: {
		AddInstruction(OpCode::PUSH_I, Value(0.0));
		break;
	}
	case TokenType::Pi: {
		AddInstruction(OpCode::PUSH_I, Value(M_PI));
		break;
	}
	case TokenType::Euler_E: {
		AddInstruction(OpCode::PUSH_I, Value(M_E));
		break;
	}
	case TokenType::LeftParen: {
		ReadNextToken();
		const size_t stackcount = ParseExpression();
		if (token != TokenType::RightParen)
			ErrorAt("PrimaryExpression", "Expected a closing parenthesis ')'.");
		ReadNextToken();
		return stackcount;
	}
	case TokenType::EndOfInput:
		return 0;
	default:
		ErrorAt("PrimaryExpression",
				"Expected an identifier, a constant or an expression in parenthesis. Found "
						+ ToString(token) + " instead.");
	}
	const auto oldToken = token;
	ReadNextToken();
	if (glueUnitToNumber || doubleGlueUnitToNumber) {
		if (oldToken == TokenType::Identifier
				&& (token == TokenType::Identifier
						|| token == TokenType::Func_min))
			ErrorAt("PrimaryExpression",
					"A variable cannot be assigned a unit directly, because the variable might already contain an unit. Use an explicit multiplication here.");
		if (oldToken == TokenType::Constant
				&& (token == TokenType::Identifier
						|| token == TokenType::Func_min)) {
			// Unit after constant
			// Modify the last PUSH_I opcode and add the unit.
			instructions.back().value.SetUnit(Unit(id));
			ReadNextToken();

			if (doubleGlueUnitToNumber) {
				if (token == TokenType::Constant) {
					AddInstruction(OpCode::PUSH_I, Value(value));
					ReadNextToken();
					if (token == TokenType::Identifier
							|| token == TokenType::Func_min) {
						// Modify the last PUSH_I opcode and add the unit.
						instructions.back().value.SetUnit(Unit(id));
						ReadNextToken();
					}
					AddInstruction(OpCode::ADD);
				}
			}
		}
	}
	return 1;
}

void MathParser::AddInstruction(OpCode opcode, const Value &value, size_t idx) {
	Instruction instr;
	instr.opcode = opcode;
	instr.value = value;
	instr.idx = idx;
	instr.pos = pos0;
	instr.length = pos - pos0 + 1;
	instr.row = row;
	instr.col = col;
	instructions.push_back(instr);

//TODO Add Expression optimizer here.
}

bool MathParser::IdentifierExists(const std::string &name) const {
	for (size_t n = 0; n < variables.size(); ++n)
		if (variables[n].name.compare(name) == 0)
			return true;
	return false;
}

size_t MathParser::LocateIdentifier(const std::string &name) const {
	for (size_t n = 0; n < variables.size(); ++n)
		if (variables[n].name.compare(name) == 0)
			return n;
	return (size_t) -1;
}

size_t MathParser::SetupIdentifier(const std::string &name) {
	const size_t idx = LocateIdentifier(name);
	if (idx == (size_t) -1) {
		variables.push_back(Variable(name));
		return variables.size() - 1;
	}
	return idx;
}

// ----------------------------------------------------------------------------
// Lexer

void MathParser::InitLexer() {
	pos = 0;
	pos0 = 0;
	row = 1;
	col = 0;
}

void MathParser::ReadNextToken() {
	pos0 = pos;
	if (pos >= code.size()) {
		token = TokenType::EndOfInput;
		return;
	}

	size_t state = 0;
	token = TokenType::Unknown;
	double fraction = 0.1;
	bool negative = false;
	int exponent = 0;
	bool skip = false;
	value = 0.0;
	id.clear();

	while (pos <= code.size()) {
		const uint8_t c = (pos < code.size()) ? code[pos] : 0;

		const size_t nextstate = transitionTable[state * charWidth + c];
		const ActionType action = actionTable[state * charWidth + c];

		if (nextstate == 0) {
			token = tokenTable[state];
			if (skip) {
				// Small speedup by knowing that NewLines are always skipped.
				if (token == TokenType::Newline) {
					++row;
					col = 0;
				}
				// If no further tokens are in the text, set the token to the
				// end of the input.
				token = TokenType::EndOfInput;
				state = 0;
				skip = false;
				continue;
			}
			if (token == TokenType::Unknown) {

				std::ostringstream err;
				err << "The character ";
				if (c <= 32 || c >= 127)
					err << "char(" << (unsigned int) c << ')';
				else
					err << "'" << c << "'";
				err << " was unexpected.";
				ErrorAt("ReadNextToken", err.str());
			}
			if (token == TokenType::Constant) {
				if (negative)
					exponent = -exponent;
				if (exponent != 0)
					value *= std::pow(10.0, (double) exponent);
			}
			return;
		}

		switch (action) {
		case ActionType::NoAction:
			break;
		case ActionType::skip:
			skip = true;
			break;
		case ActionType::AppendChar:
			id.push_back(c);
			break;
		case ActionType::AppendNumber:
			value = value * 10 + (double) (c - '0');
			break;
		case ActionType::AppendFraction:
			value += fraction * (double) (c - '0');
			fraction /= 10.0;
			break;
		case ActionType::InvertExponentSign:
			negative = true;
			break;
		case ActionType::AppendExponent:
			exponent = exponent * 10 + (c - '0');
			break;
		}
		state = nextstate;

		++pos;
		if (c == '\t') {
			col += 4 - (col % 4); // Tab stops with the width of 4 chars
		} else {
			++col;
		}
	}
}

void MathParser::ErrorBefore(const std::string &functionname,
		const std::string &errortext) {
	throw MathParser::Exception(
			"In " + functionname + " before " + PositionInText() + " : "
					+ errortext);
}

void MathParser::ErrorAt(const std::string &functionname,
		const std::string &errortext) {
	throw MathParser::Exception(
			"In " + functionname + " at " + PositionInText() + " : "
					+ errortext);
}

std::string MathParser::PositionInText() const {
	std::ostringstream positiontext;
	positiontext << "row " << row << ", column " << col;
	return positiontext.str();
}

std::string MathParser::ToString(MathParser::TokenType token) {
	std::ostringstream buffer;
	switch (token) {
	case TokenType::EndOfInput:
		buffer << "EndOfInput";
		break;
	case TokenType::Func_abs:
		buffer << "abs";
		break;
	case TokenType::Func_acos:
		buffer << "acos";
		break;
	case TokenType::Func_asin:
		buffer << "asin";
		break;
	case TokenType::Func_atan:
		buffer << "atan";
		break;
	case TokenType::Func_atan2:
		buffer << "atan2";
		break;
	case TokenType::Func_cbrt:
		buffer << "cbrt";
		break;
	case TokenType::Func_ceil:
		buffer << "ceil";
		break;
	case TokenType::Func_cos:
		buffer << "cos";
		break;
	case TokenType::Func_exp:
		buffer << "exp";
		break;
	case TokenType::Func_exp2:
		buffer << "exp2";
		break;
	case TokenType::Func_floor:
		buffer << "floor";
		break;
	case TokenType::Func_limit:
		buffer << "limit";
		break;
	case TokenType::Func_log:
		buffer << "log";
		break;
	case TokenType::Func_log10:
		buffer << "log10";
		break;
	case TokenType::Func_log2:
		buffer << "log2";
		break;
	case TokenType::Func_max:
		buffer << "max";
		break;
	case TokenType::Func_min:
		buffer << "min";
		break;
	case TokenType::Func_mod:
		buffer << "mod";
		break;
	case TokenType::Func_pow:
		buffer << "pow";
		break;
	case TokenType::Func_round:
		buffer << "round";
		break;
	case TokenType::Func_sin:
		buffer << "sin";
		break;
	case TokenType::Func_sqrt:
		buffer << "sqrt";
		break;
	case TokenType::Func_tan:
		buffer << "tan";
		break;
	default:
		buffer << "<?>";
	}
	return buffer.str();
}

std::string MathParser::Instruction::ToString(bool withParam) const {
	std::ostringstream buffer;
	switch (opcode) {
	case OpCode::NOP:
	case OpCode::STOP:
		buffer << "STOP";
		break;
	case OpCode::PUSH_I:
		buffer << "PUSH_I";
		if (withParam)
			buffer << '(' << value << ')';
		break;
	case OpCode::POP:
		buffer << "POP";
		break;
	case OpCode::SWAP:
		buffer << "SWAP";
		break;
	case OpCode::DUP:
		buffer << "DUP";
		break;
	case OpCode::FETCH_I:
		buffer << "FETCH_I";
		if (withParam)
			buffer << '[' << idx << ']';
		break;
	case OpCode::STORE_I:
		buffer << "STORE_I";
		if (withParam)
			buffer << '[' << idx << ']';
		break;
	case OpCode::FETCH_EXT_I:
		buffer << "FETCH_EXT_I";
		if (withParam)
			buffer << '[' << idx << ']';
		break;
	case OpCode::STORE_EXT_I:
		buffer << "STORE_EXT_I";
		if (withParam)
			buffer << '[' << idx << ']';
		break;
	case OpCode::JMP:
		buffer << "JMP";
		if (withParam)
			buffer << '+' << idx;
		break;
	case OpCode::JMPR:
		buffer << "JMPR";
		if (withParam)
			buffer << "-" << idx;
		break;
	case OpCode::JMP_Z:
		buffer << "JMP_Z";
		if (withParam)
			buffer << '+' << idx;
		break;
	case OpCode::JMP_NZ:
		buffer << "JMP_NZ";
		if (withParam)
			buffer << '+' << idx;
		break;
	case OpCode::JMPR_Z:
		buffer << "JMPR_Z";
		if (withParam)
			buffer << '-' << idx;
		break;
	case OpCode::JMPR_NZ:
		buffer << "JMPR_NZ";
		if (withParam)
			buffer << '-' << idx;
		break;
	case OpCode::ADD:
		buffer << "ADD";
		break;
	case OpCode::SUB:
		buffer << "SUB";
		break;
	case OpCode::MULT:
		buffer << "MULT";
		break;
	case OpCode::DIV:
		buffer << "DIV";
		break;
	case OpCode::MOD:
		buffer << "MOD";
		break;
	case OpCode::POW:
		buffer << "POW";
		break;
	case OpCode::AND:
		buffer << "AND";
		break;
	case OpCode::OR:
		buffer << "OR";
		break;
	case OpCode::LT:
		buffer << "LT";
		break;
	case OpCode::LE:
		buffer << "LE";
		break;
	case OpCode::GT:
		buffer << "GT";
		break;
	case OpCode::GE:
		buffer << "GE";
		break;
	case OpCode::EQ:
		buffer << "EQ";
		break;
	case OpCode::NEQ:
		buffer << "NEQ";
		break;
	case OpCode::NEG:
		buffer << "NEG";
		break;
	case OpCode::F_ABS:
		buffer << "F_ABS";
		break;
	case OpCode::F_EXP:
		buffer << "F_EXP";
		break;
	case OpCode::F_EXP2:
		buffer << "F_EXP2";
		break;
	case OpCode::F_LOG:
		buffer << "F_LOG";
		break;
	case OpCode::F_LOG2:
		buffer << "F_LOG2";
		break;
	case OpCode::F_LOG10:
		buffer << "F_LOG10";
		break;
	case OpCode::F_MAX:
		buffer << "F_MAX";
		break;
	case OpCode::F_MIN:
		buffer << "F_MIN";
		break;
	case OpCode::F_SIN:
		buffer << "F_SIN";
		break;
	case OpCode::F_COS:
		buffer << "F_COS";
		break;
	case OpCode::F_TAN:
		buffer << "F_TAN";
		break;
	case OpCode::F_ASIN:
		buffer << "F_ASIN";
		break;
	case OpCode::F_ACOS:
		buffer << "F_ACOS";
		break;
	case OpCode::F_ATAN:
		buffer << "F_ATAN";
		break;
	case OpCode::F_ATAN2:
		buffer << "F_ATAN2";
		break;
	case OpCode::F_CBRT:
		buffer << "F_CBRT";
		break;
	case OpCode::F_SQRT:
		buffer << "F_SQRT";
		break;
	case OpCode::F_CEIL:
		buffer << "F_CEIL";
		break;
	case OpCode::F_FLOOR:
		buffer << "F_FLOOR";
		break;
	case OpCode::F_ROUND:
		buffer << "F_ROUND";
		break;
	}
	return std::string(buffer.str());
}

// ----------------------------------------------------------------------------
//  Tables for Turbolexer:

// Note: No constexpr here as this is supposed to compile for C++11 for now
// and std::array is not constexpr yet. The setup of the tables takes place
// once right before the main() function is run.

std::array<size_t, MathParser::stateCount * MathParser::charWidth> MathParser::transitionTable =
		MathParser::InitStateTransitionTable();
std::array<MathParser::TokenType, MathParser::stateCount> MathParser::tokenTable =
		MathParser::InitTokenTypeTable();
std::array<MathParser::ActionType,
		MathParser::stateCount * MathParser::charWidth> MathParser::actionTable =
		MathParser::InitActionTable();

std::array<size_t, MathParser::stateCount * MathParser::charWidth> MathParser::InitStateTransitionTable() {
	std::array<size_t, MathParser::stateCount * MathParser::charWidth> table { };

	for (size_t state = 0; state < MathParser::stateCount; ++state) {
		for (uint16_t c = 0; c < MathParser::charWidth; ++c) {
			size_t value = 0;
			if (state == 0) {
				if (c == 10)
					value = 2;
				if (c == '"' || c == '\'')
					value = 3;
				if (c == '(')
					value = 4;
				if (c == ')')
					value = 5;
				if (c == ',')
					value = 6;
				if (c == 0)
					value = 7;
				if (c == ':')
					value = 8;
				if (c == ';')
					value = 9;
				if (c == '?')
					value = 10;
				if (c == '[')
					value = 11;
				if (c == ']')
					value = 12;
				if (c == '^')
					value = 13;
				if (c == '{')
					value = 14;
				if (c == '}')
					value = 15;
				if (c == '~')
					value = 16;
				if (c == '|')
					value = 33;
				if (c == '%')
					value = 34;
				if (c == '=')
					value = 35;
				if (c == '!')
					value = 36;
				if (c == '*')
					value = 39;
				if (c == '-')
					value = 40;
				if (c == '&')
					value = 41;
				if (c == 13)
					value = 42;
				if (c == '<')
					value = 43;
				if (c == '>')
					value = 44;
				if (c == '+')
					value = 45;
				if (c == '/')
					value = 46;
				if (c == 9 || c == ' ')
					value = 47;
				if (c == '.')
					value = 48;
				if ((c >= 194 && c <= 223))
					value = 63;
				if ((c >= 224 && c <= 239))
					value = 72;
				if ((c >= 240 && c <= 244))
					value = 73;
				if (c == 'M')
					value = 75;
				if ((c >= 'A' && c <= 'E') || (c >= 'G' && c <= 'L')
						|| (c >= 'N' && c <= 'O') || (c >= 'Q' && c <= 'S')
						|| (c >= 'U' && c <= 'Z') || c == '_'
						|| (c >= 'g' && c <= 'h') || (c >= 'j' && c <= 'k')
						|| (c >= 'n' && c <= 'o') || c == 'q'
						|| (c >= 'u' && c <= 'v') || (c >= 'x' && c <= 'z'))
					value = 79;
				if (c == 'i')
					value = 110;
				if (c == 'w')
					value = 111;
				if (c == 'b')
					value = 112;
				if (c == 'P')
					value = 123;
				if (c == 'p')
					value = 124;
				if (c == 'd')
					value = 127;
				if (c == 's')
					value = 161;
				if (c == 'e')
					value = 164;
				if (c == 'r')
					value = 178;
				if (c == 'l')
					value = 179;
				if (c == 'm')
					value = 180;
				if (c == 'F')
					value = 181;
				if (c == 'f')
					value = 182;
				if (c == 't')
					value = 183;
				if (c == 'T')
					value = 184;
				if (c == 'c')
					value = 185;
				if (c == 'a')
					value = 186;
			}
			if (state == 1) {
				if (c == '.')
					value = 50;
				if (c == 'E')
					value = 53;
				if (c == 'e')
					value = 54;
			}
			if (state == 40) {
				if (c == '-')
					value = 23;
				if (c == '=')
					value = 24;
			}
			if (state == 43) {
				if (c == '=')
					value = 26;
				if (c == '<')
					value = 37;
			}
			if (state == 44) {
				if (c == '=')
					value = 28;
				if (c == '>')
					value = 38;
			}
			if (state == 45) {
				if (c == '+')
					value = 21;
				if (c == '=')
					value = 22;
			}
			if (state == 46) {
				if (c == '=')
					value = 25;
				if (c == '/')
					value = 188;
				if (c == '*')
					value = 189;
			}
			if (state == 47) {
				if (c == 9 || c == ' ')
					value = 47;
			}
			if ((state >= 49 && state <= 50)) {
				if (c == 'E')
					value = 59;
				if (c == 'e')
					value = 60;
			}
			if (state == 53) {
				if (c == '+' || c == '-')
					value = 51;
			}
			if (state == 54) {
				if (c == '+' || c == '-')
					value = 55;
			}
			if (state == 59) {
				if (c == '+' || c == '-')
					value = 57;
			}
			if (state == 60) {
				if (c == '+' || c == '-')
					value = 61;
			}
			if ((state >= 75 && state <= 186)) {
				if ((c >= 194 && c <= 223))
					value = 66;
				if ((c >= 224 && c <= 239))
					value = 69;
				if ((c >= 240 && c <= 244))
					value = 70;
			}
			if (state == 110) {
				if (c == 'f')
					value = 78;
				if ((c >= 'g' && c <= 'z'))
					value = 80;
			}
			if (state == 111) {
				if ((c >= 'i' && c <= 'z'))
					value = 80;
				if (c == 'h')
					value = 158;
			}
			if (state == 121) {
				if ((c >= 'A' && c <= 'R'))
					value = 80;
				if (c == 'S' || c == 's')
					value = 174;
			}
			if (state == 153) {
				if ((c >= 'e' && c <= 'o'))
					value = 80;
				if (c == 'p')
					value = 137;
			}
			if (state == 154) {
				if ((c >= 'l' && c <= 'z'))
					value = 80;
				if (c == 'k')
					value = 102;
			}
			if (state == 161) {
				if (c == 'q')
					value = 117;
				if (c == 'i')
					value = 134;
			}
			if (state == 162) {
				if ((c >= 'i' && c <= 'z'))
					value = 80;
				if (c == 'g')
					value = 176;
			}
			if (state == 163) {
				if ((c >= 'g' && c <= 'w'))
					value = 80;
				if (c == 'x')
					value = 85;
			}
			if (state == 164) {
				if (c == 'l')
					value = 120;
				if (c == 'x')
					value = 153;
			}
			if (state == 167) {
				if (c == 'L' || c == 'l')
					value = 121;
			}
			if (state == 171) {
				if ((c >= 'd' && c <= 'v'))
					value = 80;
				if (c == 'w')
					value = 88;
			}
			if (state == 172) {
				if ((c >= 'd' && c <= 't'))
					value = 80;
				if (c == 'U' || c == 'u')
					value = 173;
			}
			if ((state >= 172 && state <= 186)) {
				if ((c >= 'V' && c <= 'Z') || (c >= 'v' && c <= 'z'))
					value = 80;
			}
			if (state == 173) {
				if (c == 'E' || c == 'e')
					value = 91;
			}
			if (state == 174) {
				if (c == 'E' || c == 'e')
					value = 100;
			}
			if (state == 175) {
				if (c == 'E')
					value = 81;
				if (c == 'P')
					value = 122;
			}
			if (state == 176) {
				if (c == '2')
					value = 98;
				if (c == '1')
					value = 168;
			}
			if (state == 177) {
				if ((c >= 'e' && c <= 'm') || (c >= 'o' && c <= 'r'))
					value = 80;
				if (c == 's')
					value = 83;
				if (c == 'n')
					value = 152;
			}
			if (state == 178) {
				if ((c >= 'f' && c <= 'n'))
					value = 80;
				if (c == 'o')
					value = 146;
				if (c == 'e')
					value = 151;
			}
			if (state == 179) {
				if (c == 'i')
					value = 155;
				if (c == 'o')
					value = 162;
			}
			if (state == 180) {
				if (c == 'i')
					value = 132;
				if (c == 'a')
					value = 163;
				if (c == 'o')
					value = 170;
			}
			if ((state >= 181 && state <= 182)) {
				if ((c >= 'B' && c <= 'Z'))
					value = 80;
				if (c == 'A' || c == 'a')
					value = 167;
			}
			if (state == 182) {
				if (c == 'o')
					value = 114;
				if (c == 'l')
					value = 128;
			}
			if ((state >= 182 && state <= 186)) {
				if ((c >= 'i' && c <= 'k') || (c >= 'm' && c <= 'n'))
					value = 80;
			}
			if ((state >= 183 && state <= 184)) {
				if ((c >= 'l' && c <= 'q'))
					value = 80;
				if (c == 'R' || c == 'r')
					value = 172;
			}
			if ((state >= 183 && state <= 186)) {
				if ((c >= 'A' && c <= 'Q') || (c >= 'S' && c <= 'Z'))
					value = 80;
			}
			if (state == 185) {
				if ((c >= 'c' && c <= 'd') || (c >= 'p' && c <= 'z'))
					value = 80;
				if (c == 'b')
					value = 113;
				if (c == 'e')
					value = 160;
				if (c == 'o')
					value = 177;
			}
			if ((state >= 185 && state <= 186)) {
				if ((c >= 'R' && c <= 'Z') || c == 'a'
						|| (c >= 'f' && c <= 'n'))
					value = 80;
			}
			if (state == 186) {
				if ((c >= 'e' && c <= 'r') || (c >= 'u' && c <= 'z'))
					value = 80;
				if (c == 'b')
					value = 118;
				if (c == 'c')
					value = 125;
				if (c == 't')
					value = 139;
				if (c == 's')
					value = 157;
			}
			if ((state >= 187 && state <= 188)) {
				if (c <= 9 || (c >= 11 && c <= 12) || (c >= 14 && c <= 255))
					value = 187;
			}
			if ((state >= 189 && state <= 190)) {
				if ((c >= '+' && c <= 255))
					value = 190;
				if (c == '*')
					value = 191;
			}
			if (state == 191) {
				if (c == '/')
					value = 32;
				if ((c >= '*' && c <= '.') || (c >= '0' && c <= 255))
					value = 190;
			}
			if (c == '0') {
				if ((state >= 176 && state <= 186))
					value = 80;
				if (state == 168)
					value = 105;
			}
			if ((c >= '0' && c <= '9')) {
				if (state <= 1)
					value = 1;
				if ((state >= 48 && state <= 50))
					value = 49;
				if ((state >= 51 && state <= 53))
					value = 52;
				if ((state >= 54 && state <= 56))
					value = 56;
				if ((state >= 57 && state <= 59))
					value = 58;
				if ((state >= 60 && state <= 62))
					value = 62;
				if ((state >= 75 && state <= 136)
						|| (state >= 169 && state <= 175))
					value = 80;
			}
			if ((c >= '1' && c <= '9')) {
				if ((state >= 168 && state <= 175)
						|| (state >= 177 && state <= 186))
					value = 80;
			}
			if (c == '2') {
				if (state == 137)
					value = 97;
				if (state == 138)
					value = 101;
			}
			if (c == '=') {
				if (state == 36)
					value = 17;
				if (state == 34)
					value = 18;
				if (state == 39)
					value = 20;
				if (state == 35)
					value = 27;
				if (state == 37)
					value = 30;
				if (state == 38)
					value = 31;
			}
			if (c == '_') {
				if ((state >= 76 && state <= 186))
					value = 80;
				if (state == 75)
					value = 175;
			}
			if (c == 'a') {
				if (state == 139)
					value = 130;
				if (state == 183)
					value = 135;
				if (state == 140)
					value = 154;
			}
			if (c == 'd') {
				if ((state >= 173 && state <= 186))
					value = 80;
				if (state == 170)
					value = 87;
				if (state == 169)
					value = 106;
			}
			if (c == 'e') {
				if (state == 143)
					value = 96;
				if (state == 142)
					value = 107;
				if (state == 144)
					value = 109;
				if (state == 141)
					value = 140;
			}
			if ((c >= 'e' && c <= 'h')) {
				if ((state >= 156 && state <= 161)
						|| (state >= 179 && state <= 184))
					value = 80;
			}
			if (c == 'i') {
				if ((state >= 123 && state <= 124))
					value = 76;
				if (state == 157)
					value = 129;
				if (state == 156)
					value = 133;
				if (state == 159)
					value = 149;
				if (state == 158)
					value = 165;
				if (state == 160)
					value = 166;
			}
			if ((c >= 'j' && c <= 'n')) {
				if ((state >= 124 && state <= 128)
						|| (state >= 179 && state <= 181))
					value = 80;
			}
			if ((c >= 'j' && c <= 'z')) {
				if (state == 123 || (state >= 156 && state <= 160))
					value = 80;
			}
			if (c == 'l') {
				if (state == 166)
					value = 95;
				if (state == 165)
					value = 142;
			}
			if (c == 'n') {
				if (state == 132)
					value = 86;
				if (state == 134)
					value = 89;
				if (state == 135)
					value = 90;
				if (state == 129)
					value = 93;
				if (state == 136)
					value = 108;
				if (state == 130)
					value = 138;
				if (state == 133)
					value = 147;
				if (state == 131)
					value = 169;
			}
			if ((c >= 'n' && c <= 'z')) {
				if ((state >= 137 && state <= 144)
						|| (state >= 155 && state <= 160))
					value = 80;
			}
			if (c == 'o') {
				if (state == 127)
					value = 77;
				if (state == 126)
					value = 115;
				if (state == 125)
					value = 119;
				if (state == 128)
					value = 126;
				if (state == 124)
					value = 171;
			}
			if ((c >= 'p' && c <= 'z')) {
				if ((state >= 124 && state <= 144)
						|| (state >= 178 && state <= 182))
					value = 80;
			}
			if (c == 'r') {
				if ((state >= 118 && state <= 182))
					value = 80;
				if (state == 114)
					value = 84;
				if (state == 115)
					value = 103;
				if (state == 116)
					value = 136;
				if (state == 112)
					value = 141;
				if (state == 117)
					value = 148;
				if (state == 113)
					value = 150;
			}
			if (c == 's') {
				if (state == 118)
					value = 82;
				if (state == 119)
					value = 92;
				if (state == 120)
					value = 143;
			}
			if ((c >= 's' && c <= 'z')) {
				if ((state >= 112 && state <= 117)
						|| (state >= 122 && state <= 144)
						|| (state >= 161 && state <= 162)
						|| (state >= 183 && state <= 185))
					value = 80;
			}
			if (c == 't') {
				if (state == 150)
					value = 94;
				if (state == 148)
					value = 99;
				if (state == 149)
					value = 104;
				if (state == 151)
					value = 145;
				if (state == 152)
					value = 156;
			}
			if ((c >= 't' && c <= 'z')) {
				if ((state >= 118 && state <= 144)
						|| (state >= 177 && state <= 185))
					value = 80;
			}
			if (c == 'u') {
				if (state == 145)
					value = 116;
				if (state == 146)
					value = 131;
				if (state == 147)
					value = 144;
			}
			if ((c >= 128 && c <= 191)) {
				if (state == 72)
					value = 64;
				if (state == 71)
					value = 65;
				if (state == 69)
					value = 67;
				if (state == 74)
					value = 68;
				if (state == 73)
					value = 71;
				if (state == 70)
					value = 74;
				if ((state >= 63 && state <= 65))
					value = 79;
				if ((state >= 66 && state <= 68))
					value = 80;
			}
			if (state == 33 && c == '|')
				value = 29;
			if (state == 41 && c == '&')
				value = 19;
			if (state == 42 && c == 10)
				value = 2;
			if ((state >= 75 && state <= 109) && (c >= 'a' && c <= 'z'))
				value = 80;
			if ((state >= 75 && state <= 120) && (c >= 'A' && c <= 'Z'))
				value = 80;
			if ((state >= 110 && state <= 138) && (c >= 'a' && c <= 'e'))
				value = 80;
			if ((state >= 111 && state <= 161) && (c >= 'f' && c <= 'g'))
				value = 80;
			if ((state >= 112 && state <= 122) && (c >= 'h' && c <= 'q'))
				value = 80;
			if ((state >= 121 && state <= 171) && (c >= 'T' && c <= 'Z'))
				value = 80;
			if ((state >= 122 && state <= 124) && c == 'I')
				value = 76;
			if ((state >= 122 && state <= 166) && (c >= 'J' && c <= 'Z'))
				value = 80;
			if ((state >= 122 && state <= 172) && (c >= 'A' && c <= 'H'))
				value = 80;
			if ((state >= 123 && state <= 186) && c == 'h')
				value = 80;
			if ((state >= 125 && state <= 128) && (c >= 'i' && c <= 'n'))
				value = 80;
			if ((state >= 125 && state <= 166) && (c >= 'I' && c <= 'Z'))
				value = 80;
			if ((state >= 129 && state <= 144) && (c >= 'o' && c <= 'z'))
				value = 80;
			if ((state >= 129 && state <= 153) && (c >= 'i' && c <= 'm'))
				value = 80;
			if ((state >= 137 && state <= 167) && (c >= '0' && c <= '1'))
				value = 80;
			if ((state >= 137 && state <= 186) && (c >= '3' && c <= '9'))
				value = 80;
			if ((state >= 139 && state <= 140) && (c >= 'b' && c <= 'z'))
				value = 80;
			if ((state >= 139 && state <= 175) && (c >= '2' && c <= '9'))
				value = 80;
			if ((state >= 141 && state <= 168) && (c >= 'a' && c <= 'd'))
				value = 80;
			if ((state >= 145 && state <= 147) && (c >= 'e' && c <= 't'))
				value = 80;
			if ((state >= 145 && state <= 162) && (c >= 'v' && c <= 'z'))
				value = 80;
			if ((state >= 148 && state <= 152) && (c >= 'e' && c <= 's'))
				value = 80;
			if ((state >= 148 && state <= 162) && (c >= 'u' && c <= 'z'))
				value = 80;
			if ((state >= 153 && state <= 160) && (c >= 'q' && c <= 'z'))
				value = 80;
			if ((state >= 154 && state <= 155) && (c >= 'e' && c <= 'j'))
				value = 80;
			if (state == 155 && c == 'm')
				value = 159;
			if ((state >= 155 && state <= 163) && (c >= 'k' && c <= 'l'))
				value = 80;
			if ((state >= 161 && state <= 163) && (c >= 'j' && c <= 'p'))
				value = 80;
			if ((state >= 162 && state <= 172) && (c >= 'e' && c <= 'f'))
				value = 80;
			if ((state >= 163 && state <= 186) && (c >= 'y' && c <= 'z'))
				value = 80;
			if ((state >= 164 && state <= 170) && (c >= 'm' && c <= 'w'))
				value = 80;
			if ((state >= 164 && state <= 178) && (c >= 'g' && c <= 'k'))
				value = 80;
			if ((state >= 165 && state <= 186) && (c >= 'x' && c <= 'z'))
				value = 80;
			if ((state >= 167 && state <= 171) && (c >= 'M' && c <= 'Z'))
				value = 80;
			if ((state >= 167 && state <= 186) && (c >= 'I' && c <= 'K'))
				value = 80;
			if ((state >= 168 && state <= 170) && (c >= 'l' && c <= 'z'))
				value = 80;
			if ((state >= 168 && state <= 171) && (c >= 'L' && c <= 'Z'))
				value = 80;
			if ((state >= 169 && state <= 179) && (c >= 'a' && c <= 'c'))
				value = 80;
			if ((state >= 172 && state <= 174) && (c >= 'L' && c <= 'T'))
				value = 80;
			if ((state >= 173 && state <= 174) && (c >= 'F' && c <= 'Z'))
				value = 80;
			if ((state >= 173 && state <= 176) && (c >= 'f' && c <= 'z'))
				value = 80;
			if ((state >= 173 && state <= 180) && (c >= 'A' && c <= 'D'))
				value = 80;
			if ((state >= 175 && state <= 176) && (c >= 'e' && c <= 'z'))
				value = 80;
			if ((state >= 175 && state <= 182) && (c >= 'Q' && c <= 'Z'))
				value = 80;
			if ((state >= 175 && state <= 186) && (c >= 'F' && c <= 'O'))
				value = 80;
			if ((state >= 176 && state <= 182) && (c >= 'E' && c <= 'Z'))
				value = 80;
			if ((state >= 180 && state <= 184) && (c >= 'b' && c <= 'h'))
				value = 80;
			if (state == 181 && (c >= 'i' && c <= 'z'))
				value = 80;
			if (state == 184 && (c >= 'a' && c <= 'q'))
				value = 80;
			if ((state >= 189 && state <= 191) && c <= ')')
				value = 190;
			table[state * MathParser::charWidth + c] = value;
		}
	}

	return table;
}

std::array<MathParser::TokenType, MathParser::stateCount> MathParser::InitTokenTypeTable() {
	std::array<MathParser::TokenType, MathParser::stateCount> table { };
	for (size_t state = 0; state < MathParser::stateCount; ++state) {
		MathParser::TokenType value = MathParser::TokenType::Unknown;

		if (state == 7)
			value = MathParser::TokenType::EndOfInput;
		if (state == 47)
			value = MathParser::TokenType::Whitespace;
		if (state == 2 || state == 42)
			value = MathParser::TokenType::Newline;
		if (state == 36)
			value = MathParser::TokenType::Not;
		if (state == 3 || state == 75 || (state >= 79 && state <= 80)
				|| (state >= 110 && state <= 136)
				|| (state >= 139 && state <= 175)
				|| (state >= 177 && state <= 186))
			value = MathParser::TokenType::Identifier;
		if (state == 34)
			value = MathParser::TokenType::Modulo;
		if (state == 41)
			value = MathParser::TokenType::And;
		if (state == 4)
			value = MathParser::TokenType::LeftParen;
		if (state == 5)
			value = MathParser::TokenType::RightParen;
		if (state == 39)
			value = MathParser::TokenType::Star;
		if (state == 45)
			value = MathParser::TokenType::Plus;
		if (state == 6)
			value = MathParser::TokenType::Comma;
		if (state == 40)
			value = MathParser::TokenType::Minus;
		if (state == 46)
			value = MathParser::TokenType::Div;
		if (state == 1 || (state >= 49 && state <= 50) || state == 52
				|| state == 56 || state == 58 || state == 62)
			value = MathParser::TokenType::Constant;
		if (state == 8)
			value = MathParser::TokenType::Colon;
		if (state == 9)
			value = MathParser::TokenType::Semi;
		if (state == 43)
			value = MathParser::TokenType::Less;
		if (state == 35)
			value = MathParser::TokenType::Assign;
		if (state == 44)
			value = MathParser::TokenType::Greater;
		if (state == 10)
			value = MathParser::TokenType::Question;
		if (state == 11)
			value = MathParser::TokenType::LeftBracket;
		if (state == 12)
			value = MathParser::TokenType::RightBracket;
		if (state == 13)
			value = MathParser::TokenType::Caret;
		if (state == 14)
			value = MathParser::TokenType::LeftBrace;
		if (state == 33)
			value = MathParser::TokenType::Or;
		if (state == 15)
			value = MathParser::TokenType::RightBrace;
		if (state == 16)
			value = MathParser::TokenType::Tilde;
		if (state == 17)
			value = MathParser::TokenType::NotEqual;
		if (state == 18)
			value = MathParser::TokenType::ModAssign;
		if (state == 19)
			value = MathParser::TokenType::AndAnd;
		if (state == 20)
			value = MathParser::TokenType::StarAssign;
		if (state == 21)
			value = MathParser::TokenType::PlusPlus;
		if (state == 22)
			value = MathParser::TokenType::PlusAssign;
		if (state == 23)
			value = MathParser::TokenType::MinusMinus;
		if (state == 24)
			value = MathParser::TokenType::MinusAssign;
		if ((state >= 187 && state <= 188))
			value = MathParser::TokenType::LineComment;
		if (state == 25)
			value = MathParser::TokenType::DivAssign;
		if (state == 37)
			value = MathParser::TokenType::LeftShift;
		if (state == 26)
			value = MathParser::TokenType::LessEqual;
		if (state == 27)
			value = MathParser::TokenType::Equal;
		if (state == 28)
			value = MathParser::TokenType::GreaterEqual;
		if (state == 38)
			value = MathParser::TokenType::RightShift;
		if (state == 76)
			value = MathParser::TokenType::Pi;
		if (state == 77)
			value = MathParser::TokenType::Do;
		if (state == 78)
			value = MathParser::TokenType::If;
		if (state == 29)
			value = MathParser::TokenType::OrOr;
		if (state == 30)
			value = MathParser::TokenType::LeftShiftAssign;
		if (state == 31)
			value = MathParser::TokenType::RightShiftAssign;
		if (state == 81)
			value = MathParser::TokenType::Euler_E;
		if (state == 82)
			value = MathParser::TokenType::Func_abs;
		if (state == 83)
			value = MathParser::TokenType::Func_cos;
		if (state == 137)
			value = MathParser::TokenType::Func_exp;
		if (state == 84)
			value = MathParser::TokenType::For;
		if (state == 176)
			value = MathParser::TokenType::Func_log;
		if (state == 85)
			value = MathParser::TokenType::Func_max;
		if (state == 86)
			value = MathParser::TokenType::Func_min;
		if (state == 87)
			value = MathParser::TokenType::Func_mod;
		if (state == 88)
			value = MathParser::TokenType::Func_pow;
		if (state == 89)
			value = MathParser::TokenType::Func_sin;
		if (state == 90)
			value = MathParser::TokenType::Func_tan;
		if (state == 32)
			value = MathParser::TokenType::BlockComment;
		if (state == 91)
			value = MathParser::TokenType::_True;
		if (state == 92)
			value = MathParser::TokenType::Func_acos;
		if (state == 93)
			value = MathParser::TokenType::Func_asin;
		if (state == 138)
			value = MathParser::TokenType::Func_atan;
		if (state == 94)
			value = MathParser::TokenType::Func_cbrt;
		if (state == 95)
			value = MathParser::TokenType::Func_ceil;
		if (state == 96)
			value = MathParser::TokenType::Else;
		if (state == 97)
			value = MathParser::TokenType::Func_exp2;
		if (state == 98)
			value = MathParser::TokenType::Func_log2;
		if (state == 99)
			value = MathParser::TokenType::Func_sqrt;
		if (state == 100)
			value = MathParser::TokenType::_False;
		if (state == 101)
			value = MathParser::TokenType::Func_atan2;
		if (state == 102)
			value = MathParser::TokenType::Break;
		if (state == 103)
			value = MathParser::TokenType::Func_floor;
		if (state == 104)
			value = MathParser::TokenType::Func_limit;
		if (state == 105)
			value = MathParser::TokenType::Func_log10;
		if (state == 106)
			value = MathParser::TokenType::Func_round;
		if (state == 107)
			value = MathParser::TokenType::While;
		if (state == 108)
			value = MathParser::TokenType::Return;
		if (state == 109)
			value = MathParser::TokenType::Continue;

		table[state] = value;
	}
	return table;
}

std::array<MathParser::ActionType,
		MathParser::stateCount * MathParser::charWidth> MathParser::InitActionTable() {
	std::array<MathParser::ActionType,
			MathParser::stateCount * MathParser::charWidth> table { };
	for (size_t state = 0; state < MathParser::stateCount; ++state) {
		for (uint16_t c = 0; c < MathParser::charWidth; ++c) {

			MathParser::ActionType value = MathParser::ActionType::NoAction;
			if (state == 0) {
				if ((c >= 9 && c <= 10) || c == 13 || c == ' ')
					value = MathParser::ActionType::skip;
				if (c == '"' || c == '\'' || (c >= 'A' && c <= 'Z') || c == '_'
						|| (c >= 'a' && c <= 'z') || (c >= 194 && c <= 244))
					value = MathParser::ActionType::AppendChar;
			}
			if (state == 47) {
				if (c == 9 || c == ' ')
					value = MathParser::ActionType::skip;
			}
			if ((state >= 75 && state <= 186)) {
				if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || c == '_'
						|| (c >= 'a' && c <= 'z') || (c >= 194 && c <= 244))
					value = MathParser::ActionType::AppendChar;
			}
			if ((state >= 187 && state <= 188)) {
				if (c <= 9 || (c >= 11 && c <= 12) || (c >= 14 && c <= 255))
					value = MathParser::ActionType::skip;
			}
			if (c == '-') {
				if ((state >= 53 && state <= 54)
						|| (state >= 59 && state <= 60))
					value = MathParser::ActionType::InvertExponentSign;
			}
			if (c == '/') {
				if (state == 46 || state == 191)
					value = MathParser::ActionType::skip;
			}
			if ((c >= '0' && c <= '9')) {
				if (state <= 1)
					value = MathParser::ActionType::AppendNumber;
				if ((state >= 48 && state <= 50))
					value = MathParser::ActionType::AppendFraction;
				if ((state >= 51 && state <= 62))
					value = MathParser::ActionType::AppendExponent;
			}
			if (state == 42 && c == 10)
				value = MathParser::ActionType::skip;
			if ((state >= 63 && state <= 74) && (c >= 128 && c <= 191))
				value = MathParser::ActionType::AppendChar;

			table[state * MathParser::charWidth + c] = value;
		}
	}
	return table;
}

