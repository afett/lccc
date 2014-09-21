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

cc_method::ptr_t cc_method::make(std::string const& rtype, std::string const& name)
{
	return ptr_t(new cc_method(rtype, name));
}

cc_method::cc_method(std::string const& rtype, std::string const& name)
:
	cc_method_base(name),
	rtype_(rtype),
	virtual_(false),
	abstract_(false)
{ }

void cc_method::make_virtual()
{
	virtual_ = true;
}

void cc_method::make_abstract()
{
	virtual_ = true;
	abstract_ = true;
}

std::ostream & cc_method::print(std::ostream & os) const
{
	os << (virtual_ ? "virtual " : "");
	os << rtype_ << " ";
	os << name_;
	os << "(" << args() << ")";
	if (abstract_) {
		os << " = 0;";
	} else if (src_) {
		os << "\n";
		src_->print(os);
	} else {
		os << ";";
	}
	os << "\n";
	return os;
}

}
