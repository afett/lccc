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
#include <lccc/cpp.h>

namespace lccc {

src::ptr_t
cpp_condition::add(src::ptr_t const& src)
{
	content_.push_back(src);
	return src;
}

std::ostream & cpp_condition::print(std::ostream & os) const
{
	os << symbol_ << " " << cond_ << "\n";
	print_content(os);
	os << "#endif\n";
	return os;
}

cpp_condition::cpp_condition(std::string const& symbol, std::string const& cond)
:
	symbol_(symbol),
	cond_(cond)
{ }

cpp_ifdef::ptr_t cpp_ifdef::make(std::string const& cond)
{
	return ptr_t(new cpp_ifdef(cond));
}

cpp_ifdef::cpp_ifdef(std::string const& cond)
:
	cpp_condition("#ifdef", cond)
{ }

cpp_ifndef::ptr_t cpp_ifndef::make(std::string const& cond)
{
	return ptr_t(new cpp_ifndef(cond));
}

cpp_ifndef::cpp_ifndef(std::string const& cond)
:
	cpp_condition("#ifndef", cond)
{ }

}
