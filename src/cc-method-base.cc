/*
   Copyright (c) 2014, Andreas Fett
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:

   * Redistributions of source code must retain the above copyright notice, this
     list of conditions and the following disclaimer.

   * Redistributions in binary form must reproduce the above copyright notice,
     this list of conditions and the following disclaimer in the documentation
     and/or other materials provided with the distribution.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
   DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
   FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
   DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
   SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
   CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
   OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
   OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include <lccc/cc.h>

namespace lccc {

void cc_method_base::add_arg(std::string const& type, std::string name)
{
	args_.push_back(argument(type, name));
}

cc_block::ptr_t
cc_method_base::define(cc_block::ptr_t const& src)
{
	src_ = src;
	return src;
}

cc_method_base::argument::argument(std::string const& type_, std::string const& name_)
:
	type(type_),
	name(name_)
{ }

cc_method_base::cc_method_base(std::string const& name)
:
	name_(name)
{ }

std::string cc_method_base::args() const
{
	std::string sep;
	std::stringstream res;
	for (auto arg: args_) {
		res << sep << arg.type;
		sep = ", ";
	}

	return res.str();
}

std::string cc_method_base::named_args() const
{
	std::string sep;
	std::stringstream res;
	for (auto arg: args_) {
		res << sep << arg.type;
		if (!arg.name.empty()) {
			res << " " << arg.name;
		}
		sep = ", ";
	}

	return res.str();
}

std::string cc_method_base::name() const
{
	return name_;
}

}
