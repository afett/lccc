/*
   Copyright (c) 2014, 2017 Andreas Fett
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

std::ostream & cc_base_class::initializer::print(std::ostream & os) const
{
	return os << name_ << "(" << init_ << ")";
}

cc_base_class::initializer::initializer(std::string const& name, std::string const& init)
:
	name_(name),
	init_(init)
{ }

std::ostream & cc_base_class::print(std::ostream & os) const
{
	return os << name_;
}

cc_base_class::ptr_t cc_base_class::make(std::string const& name)
{
	return ptr_t(new cc_base_class(name));
}

cc_base_class::initializer::ptr_t cc_base_class::make_initializer(std::string const& init)
{
	return initializer::ptr_t(new initializer(name_, init));
}

cc_base_class::cc_base_class(std::string const& name)
:
	name_(name)
{ }

}
