#include "stdafx.h"
#include <mmoloader.h>
#include <interpreter.h>
#include <cost_graph.h>
#include <jit.h>

#pragma warning (disable : 4996)

using namespace MmixLlvm;
using namespace MmixLlvm::cg;

using Cost = MmixLlvm::cg::CostGraph::Cost;
using NodeType = MmixLlvm::cg::CostGraph::NodeType;

namespace {
	template<typename T> struct _2double {
		static double calc(T arg);
	};

	template<typename T> double _2double<T>::calc(T arg) {
		return double(arg);
	}

	template<> double _2double<Cost>::calc(Cost arg) {
		using ddur = std::chrono::duration<double>;
		return std::chrono::duration_cast<ddur>(arg).count();
	}

	template<typename T> int getWidth(T val, T maxVal) {
		double c1 = _2double<T>::calc(val);
		double c2 = _2double<T>::calc(maxVal);
		return static_cast<int>(1. + 29. * std::pow(c1 / c2, 2.));
	}

	std::string formatFunctionCap(NodeType* pNode) {
		std::stringstream ss;
		if (!pNode->symbol().isUndef()) { //known function
			ss << pNode->symbol().value();
		}
		else {
			ss << std::hex << '#' << pNode->addr();
		}
		return ss.str();
	}

	std::string formatNodeText(NodeType* pNode) {
		std::stringstream ss;
		ss << '\"';
		if (pNode->addr() == pNode->fnAddr()) { // function entry
			ss << "Entry ";
			if (!pNode->symbol().isUndef()) { //known function
				ss << pNode->symbol().value();
			}
			else {
				ss << std::hex << '#' << pNode->addr();
			}
		}
		else {
			if (!pNode->symbol().isUndef()) { //known branch
				ss << pNode->symbol().value();
			}
			else {
				ss << std::hex << '#' << pNode->addr();
			}
		}
		ss << '\"';
		return ss.str();
	}
}

void testCostGraph(MmixLlvm::ECtx& ectx) {

}